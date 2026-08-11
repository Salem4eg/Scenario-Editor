#include "ParadoxParser.h"

ParadoxParser::ParadoxParser(QString directory, QObject* parent)
	: QObject(parent)
{
	directory_path = directory;
	QDir dir(directory);

	provinces_definition = dir.filePath("map/definition.csv");
	countries_filepath = dir.filePath("common/countries.txt");
	provinces_directory_path = dir.filePath("history/provinces");
}

ParadoxParser::~ParadoxParser()
{}

void ParadoxParser::loadProvincesDefinition(ParadoxGameData& game_data)
{
	FileReader file(provinces_definition);

	if (!file.isOpen())
		throw std::runtime_error("Cannot open definition csv");

	while (!file.atEnd())
	{
		QString line = file.readLine();

		QStringList items = line.split(';');

		// Skip provinces without IDs, such as lakes
		if (items[0].isEmpty() || items.size() < 4)
			continue;

		int provinceID = getOnlyDigits(items[0]);
		int red = getOnlyDigits(items[1]);
		int green = getOnlyDigits(items[2]);
		int blue = getOnlyDigits(items[3]);

		auto rgb = qRgb(red, green, blue);

		if (provinceID == 1825)
			qDebug() << "1825: " << rgb << " colors: " << red << " " << green << " " << blue;

		game_data.color_to_province.insert(rgb, provinceID);
	}
}

int ParadoxParser::getOnlyDigits(const QString& text)
{
	QString numbers;
	numbers.reserve(text.size());

	for (QChar ch : text)
	{
		if (ch.isDigit())
			numbers += ch;
	}

	return numbers.toInt();
}

void ParadoxParser::loadCountriesColors(ParadoxGameData& game_data)
{
	auto countrie_rows = readCountriesFile();
	auto countries_filepaths = parseCountriesFilePaths(countrie_rows);
	extractCountriesColors(countries_filepaths, game_data);
}

QList<DemographicCategory> ParadoxParser::loadIdeologies()
{
	return getDemographicCategories(directory_path + "/common/ideologies.txt");
}

QList<DemographicCategory> ParadoxParser::loadCultures()
{
	return getDemographicCategories(directory_path + "/common/cultures.txt");
}

QList<DemographicCategory> ParadoxParser::loadReligions()
{
	return getDemographicCategories(directory_path + "/common/religions.txt");
}

QList<DemographicCategory> ParadoxParser::loadPopTypes()
{
	QDir dir(directory_path + "/poptypes");

	dir.setNameFilters({ "*.txt" });
	QFileInfoList fileList = dir.entryInfoList();

	QList<DemographicCategory> popTypes;

	for (const auto& file : fileList)
	{
		DemographicCategory category;

		category.type = file.baseName();
		category.color = getPopTypeColor(file.filePath());

		popTypes.push_back(category);
	}

	return popTypes;
}

QList<QString> ParadoxParser::readCountriesFile()
{
	QList<QString> countries_rows;

	FileReader countries_file(countries_filepath);

	if (!countries_file.isOpen())
		throw std::runtime_error("Cannot open countries.txt");

	while (!countries_file.atEnd())
	{
		QString row = countries_file.readLine();

		int commentIndex = row.indexOf("#");

		if (commentIndex != -1)
			row = row.left(commentIndex);

		row = row.trimmed();

		if (!row.isEmpty())
			countries_rows.push_back(row);
	}

	return countries_rows;
}

QList<QPair<QString, QString>> ParadoxParser::parseCountriesFilePaths(QList<QString>& countries_rows)
{
	QList<QPair<QString, QString>> countries_filepaths;

	for (QString& row : countries_rows)
	{
		QStringList parts = row.split('=');

		if (parts.size() != 2)
			continue;

		QString tag = parts[0].trimmed();
		QString path = parts[1].trimmed();

		path = path.mid(1, path.length() - 2);

		if (path.endsWith(".txt", Qt::CaseInsensitive))
			countries_filepaths.emplace_back(tag, path);
	}

	return countries_filepaths;
}

void ParadoxParser::extractCountriesColors(QList<QPair<QString, QString>>& countries_filepaths, ParadoxGameData& game_data)
{
	QDir dir(directory_path);

	for (auto& country : countries_filepaths)
	{
		// first = tag
		// second = filepath
		QString full_filepath = dir.filePath("common/" + country.second);
		FileReader countryFile(full_filepath);

		if (!countryFile.isOpen())
			throw std::runtime_error("Couldn't open country's filepath");

		while (!countryFile.atEnd())
		{
			QString line = countryFile.readLine();

			if (line.contains("color"))
			{
				QString numbers = line.mid(line.indexOf('{') + 1);
				numbers = numbers.left(numbers.indexOf('}'));

				auto items = numbers.split(' ', Qt::SkipEmptyParts);

				int red = items[0].toInt();
				int green = items[1].toInt();
				int blue = items[2].toInt();

				game_data.countries_color.insert(country.first, qRgb(red, green, blue));

				break;
			}
		}
	}
	game_data.countries_color.insert("NO_OWNER", qRgb(50, 50, 50));
}

QColor ParadoxParser::getPopTypeColor(QString filepath)
{
	QColor color;

	QFile popTypeFile(filepath);
	uchar* mappedData = getMappedData(popTypeFile);

	QList<int> values; // RGB

	FileStreamScanner scanner(mappedData, popTypeFile.size());
	QString outValue;
	TokenType token;
	int depth = 0;

	while ((token = scanner.nextToken(outValue)) != TokenType::EndOfFile)
	{
		if (token == TokenType::OpenBrace)
		{
			depth++;
		}
		else if (token == TokenType::CloseBrace)
		{
			depth--;
		}
		else if (token == TokenType::Identifier)
		{
			// line must follow "color = { n n n }" format to be correct, where n - int number between 0 - 255
			if (outValue != "color")
				continue;

			if (scanner.nextToken(outValue) != TokenType::Equals)
				continue;

			if (scanner.nextToken(outValue) != TokenType::OpenBrace)
				continue;

			while (token != TokenType::CloseBrace)
			{
				token = scanner.nextToken(outValue);

				if (token == TokenType::Identifier)
				{
					values.push_back(outValue.toInt());
				}
			}

			if (values.size() == 3)
				break;
		}
	}
	
	if (values.size() == 3)
		color.setRgb(values[0], values[1], values[2]);
	else
		throw std::runtime_error("Coudn't get color in the file: " + filepath.toStdString());

	return color;
}

uchar* ParadoxParser::getMappedData(QFile& file)
{
	if (!file.open(QFile::ReadOnly | QFile::Text))
		throw std::runtime_error("Coudn't open file: " + file.fileName().toStdString());

	if (file.size() == 0)
		throw std::runtime_error("File is empty! file: " + file.fileName().toStdString());

	uchar* mappedData = file.map(0, file.size());

	if (!mappedData)
		throw std::runtime_error("Memory mapping error! file: " + file.fileName().toStdString());
	
	return mappedData;
}

QList<DemographicCategory> ParadoxParser::getDemographicCategories(QString filepath, bool isFloat)
{
	QList<DemographicCategory> categories;

	QFile file(filepath);
	uchar* mappedData = getMappedData(file);

	FileStreamScanner scanner(mappedData, file.size());
	TokenType token;
	QString outValue;
	int depth = 0;

	while ((token = scanner.nextToken(outValue)) != TokenType::EndOfFile)
	{
		if (token == TokenType::OpenBrace)
		{
			depth++;
			continue;
		}
		if (token == TokenType::CloseBrace)
		{
			depth--;
			continue;
		}

		if (token != TokenType::Identifier || depth != 1)
			continue;

		QString type = outValue;

		// must follow "type = {" 

		if (scanner.nextToken(outValue) != TokenType::Equals)
			continue;
		if (scanner.nextToken(outValue) != TokenType::OpenBrace)
			continue;
		depth++;

		while (depth == 2 && (token = scanner.nextToken(outValue)) != TokenType::EndOfFile)
		{
			if (token == TokenType::OpenBrace)
			{
				depth++;
				continue;
			}
			if (token == TokenType::CloseBrace)
			{
				depth--;
				continue;
			}
			if (token != TokenType::Identifier)
				continue;

			if (outValue != "color")
				continue;

			// color = { r g b }

			if (scanner.nextToken(outValue) != TokenType::Equals)
				continue;
			if (scanner.nextToken(outValue) != TokenType::OpenBrace)
				continue;

			// Religion has colors in float (0-1) for some reason
			if (isFloat)
			{
				QList<float> color_values;

				while (scanner.nextToken(outValue) == TokenType::Identifier)
				{
					color_values.push_back(outValue.toFloat());
				}

				if (color_values.size() == 3)
				{
					QColor color = QColor::fromRgbF(color_values[0], color_values[1], color_values[2]);

					categories.push_back({ .type = type, .color = color });
					break;
				}
			}
			else
			{
				QList<int> color_values;

				while (scanner.nextToken(outValue) == TokenType::Identifier)
				{
					color_values.push_back(outValue.toInt());
				}

				if (color_values.size() == 3)
				{
					QColor color(color_values[0], color_values[1], color_values[2]);

					categories.push_back({ .type = type, .color = color });
					break;
				}
			}			
		}
	}

	return categories;
}


