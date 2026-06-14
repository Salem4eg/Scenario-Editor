#include "ProvinceSaveManager.h"
#include <QRegularExpression>

ProvinceSaveManager::ProvinceSaveManager(QString save_directory, QString provinces_directory, QObject *parent)
	: QObject(parent), m_save_directory(save_directory), m_provinces_directory(provinces_directory)
{}

ProvinceSaveManager::~ProvinceSaveManager()
{}

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

ProvinceInfo ProvinceSaveManager::getProvinceInfo(int province)
{
	return ProvinceInfo();
}

void ProvinceSaveManager::loadProvincesFromSavefile(ParadoxGameData& game_data)
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
	FileReader reader(m_save_directory);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open save file: " + m_save_directory.toStdString());

	while (!reader.atEnd())
	{
		QString line = reader.readLine();

		const QRegularExpression provKeyRegex("^(\\d+)=\\s*$");

		auto match = provKeyRegex.match(line.trimmed());

		if (match.hasMatch())
		{
			int provinceId = match.captured(1).toInt();
			m_province_line_numbers[provinceId] = m_savefile.size();
		}

		m_savefile.push_back(line);
	}
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

		const QRegularExpression ownerRegex("^\\s*owner\\s*=\\s*(\\w+)\\s*$");

		auto ownerMatch = ownerRegex.match(line);

		if (ownerMatch.hasMatch())
		{
			game_data.countries_provinces[ownerMatch.captured(1)].push_back(provinceID);
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
		int provinceID = getProvinceIDFromFilepath(it.next());

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

		const QRegularExpression ownerRegex("^\\s*owner\\s*=\\s*(\\w+)\\s*$");
		auto ownerMatch = ownerRegex.match(line);

		if (ownerMatch.hasMatch())
		{
			insertLine = currentLine + 1;
			return;
		}
	}

	// No owner found, add it at the start of the province block
	m_savefile.insert(insertLine, QString("owner=%1").arg(country_tag));
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

		const QRegularExpression ownerRegex("^\\s*owner\\s*=\\s*(\\w+)\\s*$");
		auto ownerMatch = ownerRegex.match(line);

		if (ownerMatch.hasMatch())
		{
			insertLine = currentLine + 1;
			continue;
		}

		// check if the core already exists with regular expression
		const QRegularExpression coreRegex("^\\s*core\\s*=\\s*(\\w+)\\s*$");
		auto coreMatch = coreRegex.match(line);

		if (coreMatch.hasMatch())
			return;
	}

	QString lineToAdd = QString("owner=%1").arg(country_tag);
	m_savefile.insert(insertLine, lineToAdd);
	updateProvincesLineNumber(insertLine, true);
}

void ProvinceSaveManager::removeCoreFromProvince(int provinceID, QString country_tag)
{
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

		const QRegularExpression coreRegex("^\\s*core\\s*=\\s*(\\w+)\\s*$");
		auto coreMatch = coreRegex.match(line);

		if (coreMatch.hasMatch())
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
