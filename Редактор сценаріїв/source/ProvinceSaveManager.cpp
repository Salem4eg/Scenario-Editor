#include "ProvinceSaveManager.h"
#include <QRegularExpression>

ProvinceSaveManager::ProvinceSaveManager(QString save_path, QString game_directory, QObject *parent)
	: m_save_path(save_path), ProvinceManager(parent)
{
	QDir dir(game_directory);
	QString provinces_path(dir.filePath("history/provinces"));
	m_provinces_directory = provinces_path;
	m_default_map = dir.filePath("map/default.map");
}

ProvinceSaveManager::~ProvinceSaveManager()
{
	qDebug() << "~ProvinceSaveManager()";
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

Province ProvinceSaveManager::getProvinceInfo(int provinceID)
{	
	return m_provinces[provinceID];
}

void ProvinceSaveManager::loadProvinces(ParadoxGameData& game_data)
{
	loadChosableProvinces(game_data);

	loadSaveFile();

	for (int provinceID : game_data.choosable_provinces)
	{
		loadProvinceInfo(provinceID, game_data);
	}

}

void ProvinceSaveManager::setTypes(const QStringList& popTypes)
{
	m_popTypes = popTypes;
}

void ProvinceSaveManager::setProvincePopData(int provinceID, const QList<PopData>& population)
{
	m_provinces[provinceID].isModified = true;
	m_provinces[provinceID].population = population;
}

void ProvinceSaveManager::loadSaveFile()
{
	QFile file(m_save_path);

	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Failed to open save file:" << m_save_path;
		return;
	}

	auto memoryData = file.map(0, file.size());

	FileStreamScanner scanner(memoryData, file.size());
	int firstProvinceId = parseHeader(&scanner);

	if (firstProvinceId == -1) 
	{
		file.unmap(memoryData);
		return;
	}

	parseProvincesBlock(&scanner, firstProvinceId);


	file.unmap(memoryData);
}

void ProvinceSaveManager::loadProvinceInfo(int provinceID, ParadoxGameData& game_data)
{
	if (m_provinces[provinceID].owner.isEmpty())
		game_data.countries_provinces["NO_OWNER"].push_back(provinceID);
	else
		game_data.countries_provinces[m_provinces[provinceID].owner].push_back(provinceID);
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
	m_provinces[provinceID].isModified = true;
	m_provinces[provinceID].owner = country_tag;
	m_provinces[provinceID].controller = country_tag;
}

void ProvinceSaveManager::addCoreToProvince(int provinceID, QString country_tag)
{
	m_provinces[provinceID].isModified = true;
	m_provinces[provinceID].cores.push_back(country_tag);
}

void ProvinceSaveManager::removeCoreFromProvince(int provinceID, QString country_tag)
{
	m_provinces[provinceID].isModified = true;
	m_provinces[provinceID].cores.removeAll(country_tag);
}

void ProvinceSaveManager::saveFile()
{
	QFile file(m_save_path);

	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Failed to open save file for reading:" << m_save_path;
		return;
	}

	char* mappedData = reinterpret_cast<char*>(file.map(0, file.size()));

	QSaveFile saveFile(m_save_path);
	if (!saveFile.open(QIODevice::WriteOnly))
	{
		qWarning() << "Failed to open save file for writing:" << m_save_path;
		file.unmap(reinterpret_cast<uchar *>(mappedData));
		return;
	}

	QList<Province> modifiedProvinces;
	for (const Province& province : m_provinces)
	{
		if (province.isModified)
		{
			modifiedProvinces.append(province);
		}
	}

	std::sort(modifiedProvinces.begin(), modifiedProvinces.end(), [](const Province& a, const Province& b)
	{
		return a.startOffSet < b.startOffSet;
	});

	qint64 currentPosition = 0;

	for (const Province& province : modifiedProvinces)
	{
		qint64 bytesToCopy = province.startOffSet - currentPosition;

		if (bytesToCopy != 0)
		{
			saveFile.write(mappedData + currentPosition, bytesToCopy);
		}

		QByteArray modifiedProvinceData = serializeProvince(province);
		saveFile.write(modifiedProvinceData);

		currentPosition = province.endOffSet;
	}
	
	if (currentPosition < file.size())
	{
		qint64 bytesToCopy = file.size() - currentPosition;
		saveFile.write(mappedData + currentPosition, bytesToCopy);
	}

	file.unmap(reinterpret_cast<uchar *>(mappedData));
	file.close();
	bool isSaved = saveFile.commit();

	if (!isSaved)
	{
		qWarning() << "Failed to save modified provinces to save file:" << m_save_path << " reason: " << saveFile.errorString();
	}
}

int ProvinceSaveManager::parseHeader(FileStreamScanner* scanner)
{
	int firstProvinceId = -1;
	TokenType token;
	QString value;
	int depth = 0; // Depth counter to track nested blocks

	while ((token = scanner->nextToken(value)) != TokenType::EndOfFile) 
	{
		if (token == TokenType::OpenBrace) 
		{
			depth++;
		} 
		else if (token == TokenType::CloseBrace) 
		{
			depth--;
		} 
		else if (token == TokenType::Identifier && depth == 0)
		{
			bool isInt = false;
			int id = value.toInt(&isInt);
			if (isInt)
			{
				if (scanner->nextToken(value) == TokenType::Equals &&
					scanner->nextToken(value) == TokenType::OpenBrace)
				{
					firstProvinceId = id;
					break;
				}
			}
		}
	}

	return firstProvinceId;
}

void ProvinceSaveManager::parseProvincesBlock(FileStreamScanner* scanner, int firstProvinceId)
{
	int currentProvinceId = firstProvinceId;
	TokenType token;
	QString value;
	int depth = 0;

	// Parse the first province block
	Province province;
	province.id = currentProvinceId;
	province.startOffSet = scanner->currentOffset();

	parseProvinceBlock(scanner, province);

	province.endOffSet = scanner->currentOffset();

	m_provinces.insert(province.id, province);


	// Parse the remaining province blocks
	while ((token = scanner->nextToken(value)) != TokenType::EndOfFile) 
	{
		if (token == TokenType::OpenBrace)
		{
			depth++;
		}
		else if (token == TokenType::CloseBrace)
		{
			depth--;
		}
		else if (token == TokenType::Identifier && depth == 0)
		{
			bool isInt = false;
			currentProvinceId = value.toInt(&isInt);
			bool isBlockStart = (scanner->nextToken(value) == TokenType::Equals &&
				scanner->nextToken(value) == TokenType::OpenBrace);

			// Province block found
			if (isInt && isBlockStart)
			{
				Province province;
				province.id = currentProvinceId;
				province.startOffSet = scanner->currentOffset();

				parseProvinceBlock(scanner, province);

				province.endOffSet = scanner->currentOffset();

				m_provinces.insert(province.id, province);
			}

			// Check if it is the end of the provinces block (next country TAG)
			if (!isInt || !isBlockStart)
			{
				break;
			}
		}
	}
}

void ProvinceSaveManager::parseProvinceBlock(FileStreamScanner* scanner, Province& province)
{
	int depth = 1;
	TokenType token;
	QString value;
	bool hasPopsStarted = false;

	qint64 lastReadPosition = scanner->currentOffset();

	while (depth > 0 && (token = scanner->nextToken(value)) != TokenType::EndOfFile)
	{
		if (token == TokenType::OpenBrace)
		{
			depth++;
		}
		else if (token == TokenType::CloseBrace)
		{
			depth--;
		}
		else if (depth == 1 && token == TokenType::Identifier)
		{

			bool isTargetFields = (value == "owner") || (value == "controller") || (value == "name") || (value == "core");

			if (!isTargetFields)
			{
				if (m_popTypes.contains(value))
				{

					qint64 beforeTokenPos = scanner->currentOffset() - value.length();
					if (beforeTokenPos > lastReadPosition)
					{
						QString text = scanner->getTextBetween(lastReadPosition, beforeTokenPos);
						if (!text.trimmed().isEmpty())
						{
							if (hasPopsStarted)
								province.rawLinesAfterPops.append(text);
							else
								province.rawLinesBeforePops.append(text);
						}
					}

					hasPopsStarted = true;
					parsePopInProvinceBlock(scanner, province, value);
					lastReadPosition = scanner->currentOffset();
				}
				continue;
			}

			// Для цільових полів (owner, controller, name, core)
			qint64 beforeTokenPos = scanner->currentOffset() - value.length();
			if (beforeTokenPos > lastReadPosition)
			{
				QString text = scanner->getTextBetween(lastReadPosition, beforeTokenPos);
				if (!text.trimmed().isEmpty())
				{
					if (hasPopsStarted)
						province.rawLinesAfterPops.append(text);
					else
						province.rawLinesBeforePops.append(text);
				}
			}

			QString identifier = value;
			token = scanner->nextToken(value); // Expecting '='
			token = scanner->nextToken(value); // Expecting a tag

			if (identifier == "owner")
				province.owner = value;
			else if (identifier == "controller")
				province.controller = value;
			else if (identifier == "name")
				province.name = value;
			else if (identifier == "core")
				province.cores.push_back(value);

			lastReadPosition = scanner->currentOffset();
		}
	}

	// Забираємо все, що залишилося до кінця провінції
	if (scanner->currentOffset() > lastReadPosition)
	{
		QString text = scanner->getTextBetween(lastReadPosition, scanner->currentOffset());
		if (!text.trimmed().isEmpty())
		{
			if (hasPopsStarted)
				province.rawLinesAfterPops.append(text);
			else
				province.rawLinesBeforePops.append(text);
		}
	}
}

void ProvinceSaveManager::parsePopInProvinceBlock(FileStreamScanner* scanner, Province& province, const QString& popType)
{
	int depth = 1;
	TokenType token;
	QString value;

	if ((token = scanner->nextToken(value)) != TokenType::Equals)
		return;
	if ((token = scanner->nextToken(value)) != TokenType::OpenBrace)
		return;
	depth++;

	PopData pop;
	pop.type = popType;
	static const QStringList targetFields { "id", "size", "money", "con", "mil", "literacy" };
	qint64 lastReadPosition = scanner->currentOffset();

	while (depth >= 2 && (token = scanner->nextToken(value)) != TokenType::EndOfFile)
	{
		if (depth == 2 && token == TokenType::Identifier)
		{
			QString varName = value;

			if (targetFields.contains(varName))
			{
				if (scanner->nextToken(value) != TokenType::Equals) 
					continue;
				if (scanner->nextToken(value) != TokenType::Identifier) 
					continue;

				if (varName == "id")
					pop.id = value.toInt();
				else if (varName == "size")
					pop.size = value.toInt();
				else if (varName == "money")
					pop.money = value.toFloat();
				else if (varName == "con")
					pop.consciousness = value.toFloat();
				else if (varName == "mil")
					pop.militancy = value.toFloat();
				else if (varName == "literacy")
					pop.literacy = value.toFloat();

				lastReadPosition = scanner->currentOffset();
			}
			else if (varName == "ideology")
			{
				if (scanner->nextToken(value) != TokenType::Equals)
					continue;
				if (scanner->nextToken(value) != TokenType::OpenBrace)
					continue;

				QList<Ideology> ideologies;
				while ((token = scanner->nextToken(value)) != TokenType::EndOfFile)
				{
					if (token == TokenType::CloseBrace) 
						break;
					if (token == TokenType::Identifier)
					{
						int id = value.toInt();
						if (scanner->nextToken(value) == TokenType::Equals &&
							scanner->nextToken(value) == TokenType::Identifier)
						{
							ideologies.push_back(Ideology { .id = id, .percentage = value.toFloat() });
						}
					}
				}
				pop.ideologies = ideologies;
				lastReadPosition = scanner->currentOffset();
			}
			else if (varName == "issues")
			{
				if (scanner->nextToken(value) != TokenType::Equals)
					continue;
				if (scanner->nextToken(value) != TokenType::OpenBrace)
					continue;

				QList<QPair<int, float>> issues;
				while ((token = scanner->nextToken(value)) != TokenType::EndOfFile)
				{
					if (token == TokenType::CloseBrace)
						break;
					if (token == TokenType::Identifier)
					{
						int id = value.toInt();
						if (scanner->nextToken(value) == TokenType::Equals &&
							scanner->nextToken(value) == TokenType::Identifier)
						{
							issues.push_back(QPair<int, float> { id, value.toFloat() });
						}
					}
				}
				pop.issues = issues;
				lastReadPosition = scanner->currentOffset();
			}
			else if (pop.culture.isEmpty())
			{
				qint64 startTokenPos = scanner->currentOffset() - varName.length();

				if (scanner->nextToken(value) == TokenType::Equals)
				{
					token = scanner->nextToken(value);
					if (token == TokenType::Identifier)
					{
						pop.culture = varName;
						pop.religion = value;
						lastReadPosition = scanner->currentOffset();
						continue;
					}
				}

				// if not culture-religion pair, treat it as a raw field
				if (token == TokenType::OpenBrace)
				{
					int innerDepth = 1;
					while (innerDepth > 0 && (token = scanner->nextToken(value)) != TokenType::EndOfFile)
					{
						if (token == TokenType::OpenBrace) innerDepth++;
						else if (token == TokenType::CloseBrace) innerDepth--;
					}
				}

				qint64 endTokenPos = scanner->currentOffset();
				QString rawChunk = scanner->getTextBetween(startTokenPos, endTokenPos);
				if (!rawChunk.trimmed().isEmpty())
					pop.rawLines.append(rawChunk);

				lastReadPosition = endTokenPos;
			}
			else
			{
				qint64 startTokenPos = scanner->currentOffset() - varName.length();

				token = scanner->nextToken(value);
				if (token == TokenType::Equals)
				{
					token = scanner->nextToken(value);
					if (token == TokenType::OpenBrace)
					{
						int innerDepth = 1;
						while (innerDepth > 0 && (token = scanner->nextToken(value)) != TokenType::EndOfFile)
						{
							if (token == TokenType::OpenBrace) innerDepth++;
							else if (token == TokenType::CloseBrace) innerDepth--;
						}
					}
				}

				qint64 endTokenPos = scanner->currentOffset();
				QString rawChunk = scanner->getTextBetween(startTokenPos, endTokenPos);
				if (!rawChunk.trimmed().isEmpty())
					pop.rawLines.append(rawChunk);

				lastReadPosition = endTokenPos;
			}
			continue;
		}

		if (token == TokenType::OpenBrace)
		{
			depth++;
		}
		else if (token == TokenType::CloseBrace)
		{
			depth--;
		}
	}

	province.population.push_back(pop);
}

QByteArray ProvinceSaveManager::serializeProvince(const Province& province)
{
	QByteArray buffer;

	QTextStream stream(&buffer, QIODevice::WriteOnly);

	//stream << province.id << " = {\n";

	stream << "\n\tname=\"" << province.name.toUtf8() << "\"\n";

	if (!province.owner.isEmpty())
		stream << "\towner=\"" << province.owner.toUtf8() << "\"\n";

	if (!province.controller.isEmpty())
		stream << "\tcontroller=\"" << province.controller.toUtf8() << "\"";

	if (!province.cores.isEmpty())
	{
		for (const QString& core : province.cores)
		{
			stream << "\n\tcore=\"" << core.toUtf8() << "\"";
		}
	}

	for (const QString& line : province.rawLinesBeforePops)
	{
		stream << line << "\n";
	}

	for (const PopData& pop : province.population)
	{
		stream << "\n\t" << pop.type.toUtf8() << "= \n\t{\n";
		stream << "\t\tid=" << pop.id << "\n";
		stream << "\t\tsize=" << pop.size << "\n";
		stream << "\t\t" << pop.culture.toUtf8() << "=" << pop.religion.toUtf8() << "\n";
		stream << "\t\tmoney=" << pop.money << "\n";

		if (!pop.ideologies.isEmpty())
		{
			stream << "\t\tideology=\n\t{\n";
			for (const Ideology& ideology : pop.ideologies)
			{
				stream << QString("%1=%2").arg(ideology.id).arg(ideology.percentage);
			}
			stream << "\t\t}\n";
		}

		if (!pop.issues.isEmpty())
		{
			stream << "\t\tissues=\n\t{\n";
			for (const QPair<int, float>& issue : pop.issues)
			{
				stream << QString("%1=%2").arg(issue.first).arg(issue.second);
			}
			stream << "\t\t}\n";
		}


		stream << "\t\tcon=" << pop.consciousness << "\n";
		stream << "\t\tmil=" << pop.militancy << "\n";
		stream << "\t\tliteracy=" << pop.literacy << "\n";


		for (const QString& line : pop.rawLines)
		{
			stream << line;
		}
		stream << "\t}\n";
	}

	for (const QString& line : province.rawLinesAfterPops)
	{
		stream << line << "\n";
	}

	return buffer;
}
