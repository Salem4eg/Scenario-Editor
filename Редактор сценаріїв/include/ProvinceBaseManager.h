#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include <QMap>
#include <QSaveFile>
#include "Structures.h"
#include "FileReader.h"
#include "FileStreamScanner.h"
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

	Province getProvinceInfo(int province) override;

	//loadProvinces() requires to use setTypes() beforehand in order to load population from provinces 
	void loadProvinces(ParadoxGameData& game_data) override;

	void setTypes(const QStringList& popTypes) override;
	void setProvincePopData(int provinceID, const QList<PopData>& population) override;

private:
	QHash<int, QString> m_provinces_filepath;
	QString m_provinces_directory;
	QString m_pops_directory;

	QStringList m_popTypes;
	QMap<int, Province> m_provinces;
	// Stores provinceID as key and country population filepath id as value
	// Country population filepath locates at history/pop
	QHash<int, int> m_provinceToPopPathIndex;
	QList<QString> m_countryPopFilePaths;

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

	void parsePopFiles(QString& country_filepath);
	PopData parsePopInProvince(FileStreamScanner& scanner, TokenType token, QString& value, int provinceID, int& depth);

	void changePopulationFiles();
	void changeProvincePopulationData(QString& country_population_filepath, const QList<int>& provinces);
	QByteArray serializeProvincePopulation(const Province& province);
};

