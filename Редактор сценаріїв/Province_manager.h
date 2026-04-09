#pragma once
#include <QFile>
#include <QList>
#include <QHash>

class Province_manager
{
public slots:
	Province_manager(QString provinces_directory);
	// Changes owner and controller of provinces
	void changeProvincesOwner(QList<int> provinces, QString country_tag);
	// Add core to provinces if they don't have it already
	void addCoreToProvinces(QList<int> provinces, QString country_tag);
	// Removes core from provinces if they have it
	void removeCoreFromProvinces(QList<int> provinces, QString country_tag);


private:
	QHash<int, QString> provinces_filepath;

	void getProvincesFilepath(QString provinces_directory);
	int getProvinceFromFilepath(QString filepath);

	void changeProvinceOwner(int province, QString country_tag);
	void addCoreToProvince(int province, QString country_tag);
	void removeCoreFromProvince(int province, QString country_tag);

	void writeToFile(QFile& province_file, QStringList& text);
	bool getProvinceFile(QFile& province_file, int province);
};

