#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include "Structures.h"
#include "FileReader.h"
#include "ProvinceManager.h"

// Manages province files in the base game directory.
class ProvinceBaseManager : public ProvinceManager
{
	Q_OBJECT

public:
	ProvinceBaseManager(QString game_directory, QObject *parent = nullptr);
	~ProvinceBaseManager();

public slots:
	// Changes owner and controller of provinces
	void changeProvincesOwner(QList<int> provinces, QString country_tag) override;
	// Add core to provinces if they don't have it already
	void addCoreToProvinces(QList<int> provinces, QString country_tag) override;
	// Removes core from provinces if they have it
	void removeCoreFromProvinces(QList<int> provinces, QString country_tag) override;

	ProvinceInfo getProvinceInfo(int province) override;

	void loadProvinces(ParadoxGameData& game_data) override;

private:
	QHash<int, QString> m_provinces_filepath;
	QString m_provinces_directory;

	void getProvincesFilepath(QString provinces_directory);
	int getProvinceFromFilepath(QString filepath);

	void changeProvinceOwner(int province, QString country_tag);
	void addCoreToProvince(int province, QString country_tag);
	void removeCoreFromProvince(int province, QString country_tag);

	void writeToFile(int province, QStringList& text);
	bool openProvinceFile(FileReader& province_file, int province);

	QString getProvinceName(int province);

	void parseProvinceFile(const QString& filepath, ParadoxGameData& game_data);
	int getProvinceIDFromFilepath(const QString& filepath);
	QString getOwnerFromProvince(const QString& filepath);
};

