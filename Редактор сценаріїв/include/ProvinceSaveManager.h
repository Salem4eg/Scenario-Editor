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
#include "SaveGameStreamScanner.h"

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

	ProvinceInfo getProvinceInfo(int provinceID) override;

	void loadProvinces(ParadoxGameData& game_data) override;

private:
	QString m_save_path;
	QString m_provinces_directory;
	QString m_default_map;

	QMap<int, Province> m_provinces;

	void loadSaveFile();
	void loadProvinceInfo(int provinceID, ParadoxGameData& game_data);
	void loadChosableProvinces(ParadoxGameData& game_data);

	int getProvinceIDFromFilepath(const QString& filepath);

	void changeProvinceOwner(int provinceID, QString country_tag);
	void addCoreToProvince(int provinceID, QString country_tag);
	void removeCoreFromProvince(int provinceID, QString country_tag);

	void saveFile();
	// Reads until province occurs, returns first province line position
	int parseHeader(SaveGameStreamScanner * scanner);
	void parseProvincesBlock(SaveGameStreamScanner * scanner, int firstProvinceId);
	void parseProvinceBlock(SaveGameStreamScanner* scanner, Province& province);

	QByteArray serializeProvince(const Province& province);
	
};

