#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include <QDirIterator>
#include "Structures.h"
#include "FileReader.h"
#include "ProvinceManager.h"

// Manages provinces in a save file
class ProvinceSaveManager  : public QObject, public ProvinceManager
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
	int m_max_provinces;

	// The savefile loaded into memory
	QList<QString> m_savefile;
	// Maps province id to line number in savefile
	QHash<int, int> m_province_line_numbers;

	void loadSaveFile();
	void loadProvinceInfo(int provinceID, int lineNumber, ParadoxGameData& game_data);
	void loadChosableProvinces(ParadoxGameData& game_data);

	int getProvinceIDFromFilepath(const QString& filepath);

	void changeProvinceOwner(int provinceID, QString country_tag);
	void addCoreToProvince(int provinceID, QString country_tag);
	void removeCoreFromProvince(int provinceID, QString country_tag);

	// If cannot find the province block, throws an exception
	void moveCurrentLineIntoProvinceBlock(int provinceID, int& currentLine);
	// Update line numbers for provinces after the inserted line
	void updateProvincesLineNumber(int lineNumber, bool newLineInserted);

	void saveFile();

	void loadMaxProvinces();

};

