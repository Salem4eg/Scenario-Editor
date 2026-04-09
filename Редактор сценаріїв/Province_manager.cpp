#include "Province_manager.h"
#include <QDirIterator>

Province_manager::Province_manager(QString provinces_directory)
{
	getProvincesFilepath(provinces_directory);
}

void Province_manager::changeProvincesOwner(QList<int> provinces, QString country_tag)
{
	for (int province : provinces)
		changeProvinceOwner(province, country_tag);
}

void Province_manager::addCoreToProvinces(QList<int> provinces, QString country_tag)
{
	for (int province : provinces)
		addCoreToProvince(province, country_tag);
}

void Province_manager::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{
	for (int province : provinces)
		removeCoreFromProvince(province, country_tag);
}

void Province_manager::getProvincesFilepath(QString provinces_directory)
{
	QDirIterator it(provinces_directory, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		QString filepath = it.next();

		int province = getProvinceFromFilepath(filepath);

		provinces_filepath.insert(province, filepath);
	}
}

int Province_manager::getProvinceFromFilepath(QString filepath)
{
	QString filename = filepath.split('/').last();

	int province = filename.split('-').first().trimmed().toInt();

	return province;
}

void Province_manager::changeProvinceOwner(int province, QString country_tag)
{
	QFile province_file;

	if (!getProvinceFile(province_file, province))
		return;

	QTextStream read(&province_file);

	QStringList text;

	text.append("owner = " + country_tag);
	text.append("controller = " + country_tag);

	while (!read.atEnd())
	{
		QString line = read.readLine();
		if (!line.contains("owner") && !line.contains("controller"))
			text.append(line);
	}

	writeToFile(province_file, text);
}

void Province_manager::addCoreToProvince(int province, QString country_tag)
{
	QFile province_file;

	if (!getProvinceFile(province_file, province))
		return;

	QTextStream read(&province_file);
	QStringList text;

	bool has_core = false;
	int insert_line_index = 0;

	QString line_to_add("add_core = " + country_tag);

	while (!read.atEnd())
	{
		QString line = read.readLine();

		if (line.contains(line_to_add))
			has_core = true;

		else if (line.contains("owner") || line.contains("controller"))
			insert_line_index = text.size() + 1;

		text.append(line);
	}

	if (has_core)
		return;

	text.insert(insert_line_index, line_to_add);

	writeToFile(province_file, text);
}

void Province_manager::removeCoreFromProvince(int province, QString country_tag)
{
	QFile province_file;

	if (!getProvinceFile(province_file, province))
		return;

	QTextStream read(&province_file);
	QStringList text;

	bool removed = false;

	while (!read.atEnd())
	{
		QString line = read.readLine();
		if (!line.contains("add_core = " + country_tag))
			text.append(line);
		else
			removed = true;
	}

	if (!removed)
		return;

	writeToFile(province_file, text);
}

void Province_manager::writeToFile(QFile& province_file, QStringList& text)
{
	province_file.close();
	province_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);

	QTextStream write(&province_file);

	for (const auto& line : text)
		write << line << "\n";
}

bool Province_manager::getProvinceFile(QFile& province_file, int province)
{
	QString filepath = provinces_filepath.value(province, "");

	if (filepath.isEmpty())
	{
		qDebug() << "Couldn't find filepath for province: " << province;
		return false;
	}

	province_file.setFileName(filepath);

	if (!province_file.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug() << "Couldn't open the file. Province: " << province << " Filepath: " << filepath;
		return false;
	}

	return true;
}

