#include "ProvinceBaseManager.h"
#include <QDirIterator>

ProvinceBaseManager::ProvinceBaseManager(QString provinces_directory, QObject *parent)
	: QObject(parent), m_provinces_directory(provinces_directory)
{
	getProvincesFilepath(m_provinces_directory);
}

ProvinceBaseManager::~ProvinceBaseManager()
{}

void ProvinceBaseManager::changeProvincesOwner(QList<int> provinces, QString country_tag)
{
	for (int province : provinces)
		changeProvinceOwner(province, country_tag);
}

void ProvinceBaseManager::addCoreToProvinces(QList<int> provinces, QString country_tag)
{
	for (int province : provinces)
		addCoreToProvince(province, country_tag);
}

void ProvinceBaseManager::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{
	for (int province : provinces)
		removeCoreFromProvince(province, country_tag);
}

ProvinceInfo ProvinceBaseManager::getProvinceInfo(int province)
{
	FileReader province_file;

	if (!openProvinceFile(province_file, province))
		return {};

	ProvinceInfo info;

	int depth = 0;
	while (!province_file.atEnd())
	{
		QString line = province_file.readLine();

		int comment_pos = line.indexOf("#");

		if (comment_pos != -1)
			line = line.left(comment_pos);

		if (line.contains("{"))
			depth++;

		if (line.contains("owner =") && depth == 0)
		{
			info.owner = line.remove("owner =").trimmed();

			if (info.owner.size() != 3)
				qDebug() << "Province: " << province << " info.owner = " << info.owner;
		}

		if (line.contains("add_core =") && depth == 0)
		{
			info.cores.push_back(line.remove("add_core =").trimmed());

			if (info.cores.last().size() != 3)
				qDebug() << "Province: " << province << " info.cores.last = " << info.cores.last();
		}

		if (line.contains("}"))
			depth--;
	}

	info.name = getProvinceName(province);

	return info;
}

void ProvinceBaseManager::loadProvincesFromBaseGame(ParadoxGameData& game_data)
{
	QDirIterator it(m_provinces_directory, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		parseProvinceFile(it.next(), game_data);
	}
}


void ProvinceBaseManager::getProvincesFilepath(QString provinces_directory)
{
	QDirIterator it(provinces_directory, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		QString filepath = it.next();

		int province = getProvinceFromFilepath(filepath);

		m_provinces_filepath.insert(province, filepath);
	}
}

int ProvinceBaseManager::getProvinceFromFilepath(QString filepath)
{
	QString filename = filepath.split('/').last();

	int province = filename.split('-').first().trimmed().toInt();

	return province;
}

void ProvinceBaseManager::changeProvinceOwner(int province, QString country_tag)
{
	FileReader province_file;

	if (!openProvinceFile(province_file, province))
		return;

	QStringList text;

	text.append("owner = " + country_tag);
	text.append("controller = " + country_tag);
	int depth = 0;

	while (!province_file.atEnd())
	{
		QString line = province_file.readLine();

		if (line.contains("#"))
		{
			int comment_pos = line.indexOf("#");
			line = line.left(comment_pos);
		}

		if (line.contains("{"))
			depth++;

		if ((!line.contains("owner") && !line.contains("controller")) || depth != 0)
			text.append(line);

		if (line.contains("}"))
			depth--;

	}

	writeToFile(province, text);
}

void ProvinceBaseManager::addCoreToProvince(int province, QString country_tag)
{
	FileReader province_file;

	if (!openProvinceFile(province_file, province))
		return;

	QStringList text;

	bool has_core = false;
	int insert_line_index = 0;
	int depth = 0;

	QString line_to_add("add_core = " + country_tag);

	while (!province_file.atEnd())
	{
		QString line = province_file.readLine();

		if (line.contains("#"))
		{
			int comment_pos = line.indexOf("#");
			line = line.left(comment_pos);
		}

		if (line.contains("{"))
			depth++;


		if (line.contains(line_to_add) && depth == 0)
		{
			has_core = true;
			break;
		}

		if ((line.contains("owner") || line.contains("controller")) && depth == 0)
			insert_line_index = text.size() + 1;


		if (line.contains("}"))
			depth--;

		text.append(line);
	}

	if (has_core)
		return;

	text.insert(insert_line_index, line_to_add);

	writeToFile(province, text);
}

void ProvinceBaseManager::removeCoreFromProvince(int province, QString country_tag)
{
	FileReader province_file;

	if (!openProvinceFile(province_file, province))
		return;

	QStringList text;

	bool removed = false;
	int depth = 0;

	while (!province_file.atEnd())
	{
		QString line = province_file.readLine();

		QString code_part;
		QString commented_part;

		int comment_pos = line.indexOf("#");

		if (comment_pos != -1)
		{
			code_part = line.left(comment_pos);
			commented_part = line.mid(comment_pos);
		}
		else
			code_part = std::move(line);

		if (code_part.contains("{"))
		{
			depth++;
		}

		if (!code_part.contains("add_core = " + country_tag) || depth != 0)
			text.append(code_part + commented_part);
		else
			removed = true;

		if (code_part.contains("}"))
		{
			depth--;
		}
	}

	if (!removed)
		return;

	writeToFile(province, text);
}

void ProvinceBaseManager::writeToFile(int province, QStringList& text)
{
	QString filepath = m_provinces_filepath.value(province);

	QFile province_file(filepath);
	province_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

	QTextStream write(&province_file);

	for (const auto& line : text)
		write << line << "\n";
}

bool ProvinceBaseManager::openProvinceFile(FileReader& province_file, int province)
{
	QString filepath = m_provinces_filepath.value(province, "");

	if (filepath.isEmpty())
	{
		qDebug() << "Couldn't find filepath for province: " << province;
		return false;
	}

	if (!province_file.open(filepath))
	{
		qDebug() << "Couldn't open the file. Province: " << province << " Filepath: " << filepath;
		return false;
	}

	return true;
}

QString ProvinceBaseManager::getProvinceName(int province)
{
	QString filepath = m_provinces_filepath[province];

	QFileInfo info(filepath);

	return info.baseName().split("-").last().trimmed();
}

void ProvinceBaseManager::parseProvinceFile(const QString& filepath, ParadoxGameData& game_data)
{
	FileReader reader(filepath);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open province file: " + filepath.toStdString());

	int provinceID = getProvinceIDFromFilepath(filepath);

	QString province_owner = getOwnerFromProvince(filepath);

	// choosable_provinces to distinguish sea provinces that aren't clickable
	game_data.choosable_provinces.push_back(provinceID);
	game_data.countries_provinces[province_owner].push_back(provinceID);
}

int ProvinceBaseManager::getProvinceIDFromFilepath(const QString& filepath)
{
	auto words = filepath.split("/");
	QString document = words.last();

	auto items = document.split("-");

	int provinceID = items.first().toInt();

	return provinceID;
}

QString ProvinceBaseManager::getOwnerFromProvince(const QString& filepath)
{
	QString owner = "NO_OWNER";

	FileReader province_file(filepath);

	if (!province_file.isOpen())
		throw std::runtime_error("Couldn't open the file from path: " + filepath.toStdString());

	int depth = 0;
	while (!province_file.atEnd())
	{
		QString line = province_file.readLine();

		int comment_pos = line.indexOf("#");

		if (comment_pos != -1)
			line = line.left(comment_pos);

		if (line.contains("{"))
			depth++;

		if (line.contains("owner =") && depth == 0)
		{
			owner = line.remove("owner =").trimmed();

			if (owner.size() != 3)
				qDebug() << "Strange owner: " << owner << " filepath: " << filepath;
		}

		if (line.contains("}"))
			depth--;
	}

	return owner;
}