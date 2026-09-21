#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include <QDirIterator>
#include <QSaveFile>
#include "Structures.h"
#include "FileReader.h"
#include "ProvinceManager.h"
#include "FileStreamScanner.h"

// Manages provinces in a save file
class ProvinceSaveManager  : public ProvinceManager
{
	Q_OBJECT

public:
	ProvinceSaveManager(QString save_path, QString game_directory, QObject *parent = nullptr);
	~ProvinceSaveManager();

public slots:
	// Changes owner and controller of provinces
	void changeProvincesOwner(QList<int> provinces, QString country_tag) override;
	// Add core to provinces if they don't have it already
	void addCoreToProvinces(QList<int> provinces, QString country_tag) override;
	// Removes core from provinces if they have it
	void removeCoreFromProvinces(QList<int> provinces, QString country_tag) override;

	Province getProvinceInfo(int provinceID) override;

	//loadProvinces() requires to use setTypes() beforehand in order to load population from provinces 
	void loadProvinces(ParadoxGameData& game_data) override;
	void setTypes(const QStringList& popTypes) override;

	void setProvincePopData(int provinceID, const QList<PopData>& population) override;

private:
	QString m_save_path;
	QString m_provinces_directory;
	QString m_default_map;
	QString m_region_file_path;

	QMap<int, Province> m_provinces;

	QMap<int, QList<int>> m_regions; // region_id -> province_ids
	QMap<int, int> m_province_to_region; // province_id -> region_id
	QHash<QString, CountryData> m_countries;
	
	QStringList m_popTypes;

	void loadSaveFile();
	void loadProvinceInfo(int provinceID, ParadoxGameData& game_data);
	void loadChosableProvinces(ParadoxGameData& game_data);

	int getProvinceIDFromFilepath(const QString& filepath);

	void changeProvinceOwner(int provinceID, QString country_tag);
	void addCoreToProvince(int provinceID, QString country_tag);
	void removeCoreFromProvince(int provinceID, QString country_tag);

	void saveFile();
	// Reads until province occurs, returns first province line position
	int parseHeader(FileStreamScanner* scanner);
	CountryData parseProvincesBlock(FileStreamScanner* scanner, int firstProvinceId);
	void parseProvinceBlock(FileStreamScanner* scanner, Province& province);
	void parsePopInProvinceBlock(FileStreamScanner* scanner, Province& province, const QString& popType);

	QByteArray serializeProvince(const Province& province);

	void parseRegionFile();
	void parseCountryBlock(FileStreamScanner* scanner, CountryData& countryData);
	void parseCountry(FileStreamScanner* scanner, CountryData& countryData);
	QList<int> parseStateBlock(FileStreamScanner* scanner);

	void moveProvinceBetweenRegions(int provinceID, const QString& formerOwner, const QString& newOwner);
	void removeProvinceFromCountry(int provinceID, const QString& country_tag);
	void addProvinceToCountry(int provinceID, const QString& country_tag);
	QByteArray serializeCountry(const CountryData& countryData, char* mappedData);
	quint64 getRegionStartPosition(char * mappedData, quint64 startOffset, quint64 endOffset);
	QByteArray serializeRegion(const QList<int>& region, int regionId);
};

