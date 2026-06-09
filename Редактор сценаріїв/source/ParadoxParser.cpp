#include "ParadoxParser.h"

ParadoxParser::ParadoxParser(QString directory, QString save_file, QObject *parent)
	: QObject(parent)
{
	directory_path = directory;
	save_file_path = save_file;
	save_file_exists = save_file_path.isEmpty() ? true : false;
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


void ParadoxParser::loadCountriesProvinces(ParadoxGameData& game_data)
{
	if (!save_file_exists)
	{
		QDirIterator it(provinces_directory_path, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

		while (it.hasNext())
		{
			parseProvinceFile(it.next(), game_data);
		}
	}
	else
	{
		loadProvincesFromSavefile(game_data);
	}
}

void ParadoxParser::parseProvinceFile(const QString& filepath, ParadoxGameData& game_data)
{
	FileReader reader(filepath);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open province file: " + filepath.toStdString());

	int provinceID = getProvinceIDFromFilepath(filepath);

	QString province_owner = Province_manager::getOwnerFromProvince(filepath);

	// choosable_provinces для уникання морських провінцій
	game_data.choosable_provinces.push_back(provinceID);
	game_data.countries_provinces[province_owner].push_back(provinceID);
}

int ParadoxParser::getProvinceIDFromFilepath(const QString& filepath)
{
	auto words = filepath.split("/");
	QString document = words.last();

	auto items = document.split("-");

	int provinceID = items.first().toInt();

	return provinceID;
}

void ParadoxParser::loadProvincesFromSavefile(ParadoxGameData& game_data)
{
	FileReader reader(save_file_path);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open save file: " + save_file_path.toStdString());

	static const QRegularExpression provKeyRegex("^(\\d+)=\\s*$");

	int currentProvinceId = -1;
	int braceDepth = 0;
	bool insideProvince = false;

	while (!reader.atEnd()) 
	{
		QString line = reader.readLine().trimmed();
		if (line.isEmpty()) continue;

		int opens = line.count('{');
		int closes = line.count('}');

		if (!insideProvince && braceDepth == 0) 
		{
			auto match = provKeyRegex.match(line);
			if (match.hasMatch()) 
			{
				currentProvinceId = match.captured(1).toInt();
				insideProvince = true;
				braceDepth += opens - closes;
				continue;
			}
		}
		else if (insideProvince) 
		{
			if (line.startsWith("owner=")) 
			{
				QString owner = line.section('"', 1, 1);

				game_data.choosable_provinces.push_back(currentProvinceId);
				game_data.countries_provinces[owner].push_back(currentProvinceId);

			}

			braceDepth += opens - closes;

			if (braceDepth <= 0) 
			{
				insideProvince = false;
				braceDepth = 0;
				currentProvinceId = -1;
			}
		}
		else 
		{
			braceDepth += opens - closes;
		}
	}
}