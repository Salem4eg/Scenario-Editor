#pragma once
#include <QFile>
#include <QList>
#include <QHash>
#include "Structures.h"
#include "FileReader.h"


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

	ProvinceInfo getProvinceInfo(int province);
	static QString getOwnerFromProvince(const QString& filepath);

private:
	QHash<int, QString> provinces_filepath;

	void getProvincesFilepath(QString provinces_directory);
	int getProvinceFromFilepath(QString filepath);

	void changeProvinceOwner(int province, QString country_tag);
	void addCoreToProvince(int province, QString country_tag);
	void removeCoreFromProvince(int province, QString country_tag);

	void writeToFile(int province, QStringList& text);
	bool openProvinceFile(FileReader& province_file, int province);

	QString getProvinceName(int province);
};

