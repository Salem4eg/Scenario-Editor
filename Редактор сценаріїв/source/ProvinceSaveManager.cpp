#include "ProvinceSaveManager.h"
#include <QRegularExpression>

ProvinceSaveManager::ProvinceSaveManager(QString save_path, QString game_directory, QObject *parent)
	: m_save_path(save_path)
{
	QDir dir(game_directory);
	QString provinces_path(dir.filePath("history/provinces"));
	m_provinces_directory = provinces_path;
	m_default_map = dir.filePath("map/default.map");
}

ProvinceSaveManager::~ProvinceSaveManager()
{
	saveFile();
}

void ProvinceSaveManager::changeProvincesOwner(QList<int> provinces, QString country_tag)
{
	for (int provinceID : provinces)
	{
		changeProvinceOwner(provinceID, country_tag);
	}
}

void ProvinceSaveManager::addCoreToProvinces(QList<int> provinces, QString country_tag)
{
	for (int provinceID : provinces)
	{
		addCoreToProvince(provinceID, country_tag);
	}
}

void ProvinceSaveManager::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{
	for (int provinceID : provinces)
	{
		removeCoreFromProvince(provinceID, country_tag);
	}
}

ProvinceInfo ProvinceSaveManager::getProvinceInfo(int provinceID)
{
	const QRegularExpression paradoxRegex("^\\s*(owner|core|name)\\s*=\\s*\"?([^\"]+)\"?\\s*$");
	
	ProvinceInfo provinceInfo;

	int currentLine = m_province_line_numbers[provinceID];

	moveCurrentLineIntoProvinceBlock(provinceID, currentLine);

	int openBraces = 1;
	int closeBraces = 0;

	int insertLine = currentLine + 1;


	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;

		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');


		auto match = paradoxRegex.match(line);

		if (match.hasMatch())
		{
			QString key = match.captured(1);
			QString value = match.captured(2);

			if (key == "owner")
			{
				provinceInfo.owner = value;
			}
			else if (key == "core")
			{
				provinceInfo.cores.push_back(value);
			}
			else if (key == "name")
			{
				provinceInfo.name = value;
			}
		}
	}

	// temporary check
	provinceInfo.name.replace(QChar(0xFFFD), QString("?"));

	return provinceInfo;
}

void ProvinceSaveManager::loadProvinces(ParadoxGameData& game_data)
{
	loadSaveFile();

	for (auto [provinceId, lineNumber] : m_province_line_numbers.asKeyValueRange())
	{
		loadProvinceInfo(provinceId, lineNumber, game_data);
	}

	loadChosableProvinces(game_data);
}

void ProvinceSaveManager::loadSaveFile()
{
	FileReader reader(m_save_path);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open save file: " + m_save_path.toStdString());

	// save lines and find provinces block
	// save lines and save province lines numbers
	// save lines without any checking
	QString line = parseHeader(reader);
	parseProvincesBlock(reader, line);
	parseFooter(reader);
}

void ProvinceSaveManager::loadProvinceInfo(int provinceID, int lineNumber, ParadoxGameData& game_data)
{
	int currentLine = lineNumber;

	moveCurrentLineIntoProvinceBlock(provinceID, currentLine);

	int openBraces = 1;
	int closeBraces = 0;
	bool hasOwner = false;

	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;
		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');

		const QRegularExpression ownerRegex("^\\s*(owner)\\s*=\\s*\"?(\\w+)\"?\\s*$");

		auto match = ownerRegex.match(line);

		if (match.hasMatch())
		{
			game_data.countries_provinces[match.captured(2)].push_back(provinceID);
			hasOwner = true;
			continue;
		}
	}

	if (!hasOwner)
	{
		game_data.countries_provinces["NO_OWNER"].push_back(provinceID);
	}
}

void ProvinceSaveManager::loadChosableProvinces(ParadoxGameData& game_data)
{
	QDirIterator it(m_provinces_directory, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		QString filepath = it.next();
		int provinceID = getProvinceIDFromFilepath(filepath);

		game_data.choosable_provinces.push_back(provinceID);
	}

}

int ProvinceSaveManager::getProvinceIDFromFilepath(const QString& filepath)
{
	auto words = filepath.split("/");
	QString document = words.last();

	auto items = document.split("-");

	int provinceID = items.first().toInt();

	return provinceID;
}

void ProvinceSaveManager::changeProvinceOwner(int provinceID, QString country_tag)
{
	const QRegularExpression ownerRegex("^\\s*(owner)\\s*=\\s*\"?(\\w+)\"?\\s*$");
	int currentLine = m_province_line_numbers[provinceID];

	moveCurrentLineIntoProvinceBlock(provinceID, currentLine);

	int openBraces = 1;
	int closeBraces = 0;

	// If no owner found, adding owner line at the start of the province block
	int insertLine = currentLine + 1;

	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;

		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');

		// If province has owner to change
		auto match = ownerRegex.match(line);
		if (match.hasMatch())
		{
			if (match.captured(2) != country_tag)
				m_savefile[currentLine] = QString("\towner=\"%1\"").arg(country_tag);
			return;
		}
	}

	m_savefile.insert(insertLine, QString("\towner=\"%1\"").arg(country_tag));
	updateProvincesLineNumber(insertLine, true);
}

void ProvinceSaveManager::addCoreToProvince(int provinceID, QString country_tag)
{
	int currentLine = m_province_line_numbers[provinceID];

	moveCurrentLineIntoProvinceBlock(provinceID, currentLine);

	int openBraces = 1;
	int closeBraces = 0;

	int insertLine = currentLine + 1;

	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;

		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');

		const QRegularExpression paradoxRegex("^\\s*(owner|core)\\s*=\\s*\"?(\\w+)\"?\\s*$");

		auto match = paradoxRegex.match(line);

		if (match.hasMatch())
		{
			QString key = match.captured(1);
			QString value = match.captured(2);

			if (key == "owner")
			{
				insertLine = currentLine + 1;
				continue;
			}
			else if (key == "core" && value == country_tag)
			{
				return; // Core already exists, no need to add
			}
		}
	}

	QString lineToAdd = QString("\tcore=\"%1\"").arg(country_tag);
	m_savefile.insert(insertLine, lineToAdd);
	updateProvincesLineNumber(insertLine, true);
}

void ProvinceSaveManager::removeCoreFromProvince(int provinceID, QString country_tag)
{
	const QRegularExpression coreRegex("^\\s*(core)\\s*=\\s*\"?(\\w+)\"?\\s*$");
	int currentLine = m_province_line_numbers[provinceID];

	moveCurrentLineIntoProvinceBlock(provinceID, currentLine);

	int openBraces = 1;
	int closeBraces = 0;

	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;

		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');

		
		auto match = coreRegex.match(line);

		if (match.hasMatch() && match.captured(2) == country_tag)
		{
			m_savefile.removeAt(currentLine);
			updateProvincesLineNumber(currentLine, false);
			return;
		}
	}
}

void ProvinceSaveManager::moveCurrentLineIntoProvinceBlock(int provinceID, int& currentLine)
{
	const QRegularExpression provKeyRegex("^(\\d+)=\\s*$");

	auto match = provKeyRegex.match(m_savefile[currentLine].trimmed());

	if (!match.hasMatch() || match.captured(1).toInt() != provinceID)
		throw std::runtime_error(std::format("Expected province {} at line {}", provinceID, currentLine));

	bool inProvinceBlock = m_savefile[currentLine].contains('{');

	while (!inProvinceBlock && currentLine < m_savefile.size())
	{
		currentLine++;
		inProvinceBlock = m_savefile[currentLine].contains('{');
	}

	if (!inProvinceBlock)
		throw std::runtime_error(std::format("Could not find province block for province {} at line {}", provinceID, currentLine));
}

void ProvinceSaveManager::updateProvincesLineNumber(int lineNumber, bool newLineInserted)
{
	for (auto [provinceID, provinceLineNumber] : m_province_line_numbers.asKeyValueRange())
	{
		if (provinceLineNumber > lineNumber)
		{
			if (newLineInserted)
				m_province_line_numbers[provinceID]++;
			else
				m_province_line_numbers[provinceID]--;
		}
	}
}

void ProvinceSaveManager::saveFile()
{
	QFile file(m_save_path);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Could not open save file for writing: " << m_save_path;
	}

	QTextStream stream(&file);
	for (const auto& line : m_savefile)
	{
		stream << line << '\n';
	}
	file.close();
}

QString ProvinceSaveManager::parseHeader(FileReader& reader)
{
	const QRegularExpression provKeyRegex("^(\\d{1,4})=\\s*\\{?\\s*$");
	
	while (!reader.atEnd())
	{
		QString line = reader.readLine();
		
		// Check if it is province block, if yes quit
		auto match = provKeyRegex.match(line.trimmed());

		if (match.hasMatch())
			return line;
		
		m_savefile.push_back(line);

		// Do not check with regex while it is in { block }
		if (line.contains("{"))
		{
			consumeBlock(reader, line);
		}
	}

	return QString();
}

void ProvinceSaveManager::parseProvincesBlock(FileReader& reader, QString& province_line)
{
	const QRegularExpression provKeyRegex("^(\\d{1,4})=\\s*\\{?\\s*$");
	const QRegularExpression countryKeyRegex("^[A-Z0-9]{3}=\\s*$");
	auto match = provKeyRegex.match(province_line.trimmed());

	if (match.hasMatch())
	{
		int provinceId = match.captured(1).toInt();
		m_province_line_numbers[provinceId] = m_savefile.size();
	}

	m_savefile.push_back(province_line);

	while (!reader.atEnd())
	{
		QString line = reader.readLine();

		auto match = provKeyRegex.match(line.trimmed());

		if (match.hasMatch())
		{
			int provinceId = match.captured(1).toInt();
			m_province_line_numbers[provinceId] = m_savefile.size();

			m_savefile.push_back(line);
			
			if (line.contains("{"))
				consumeBlock(reader, line);

			continue;
		}

		// Watch for TAG= format, it indicates end of provinces
		auto tagMatch = countryKeyRegex.match(line.trimmed());
		if (tagMatch.hasMatch())
		{
			m_savefile.push_back(line);
			return;
		}

		if (line.contains("{"))
		{
			m_savefile.push_back(line);
			consumeBlock(reader, line);
		}
	}

}

void ProvinceSaveManager::parseFooter(FileReader& reader)
{
	while (!reader.atEnd())
	{
		m_savefile.push_back(reader.readLine());
	}
}

void ProvinceSaveManager::consumeBlock(FileReader& reader, QString& province_line)
{
	// just save lines until end of a block
	int leftBrackets = province_line.count("{");
	int rightBrackets = province_line.count("}");

	while (leftBrackets > rightBrackets)
	{
		QString line = reader.readLine();


		leftBrackets += line.count("{");
		rightBrackets += line.count("}");

		m_savefile.push_back(line);
	}
}

