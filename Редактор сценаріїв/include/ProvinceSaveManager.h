#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include "Structures.h"
#include "FileReader.h"

// Manages provinces in a save file
class ProvinceSaveManager  : public QObject
{
	Q_OBJECT

public:
	ProvinceSaveManager(QString save_directory, QObject *parent = nullptr);
	~ProvinceSaveManager();

public slots:
	// Changes owner and controller of provinces
	void changeProvincesOwner(QList<int> provinces, QString country_tag);
	// Add core to provinces if they don't have it already
	void addCoreToProvinces(QList<int> provinces, QString country_tag);
	// Removes core from provinces if they have it
	void removeCoreFromProvinces(QList<int> provinces, QString country_tag);

	ProvinceInfo getProvinceInfo(int province);

	void loadProvincesFromSavefile(ParadoxGameData& game_data);

private:
	QString m_save_directory;

	// The savefile loaded into memory
	QList<QString> m_savefile;
	// Maps province id to line number in savefile
	QHash<int, int> m_province_line_numbers;

	void loadSaveFile();
	void loadProvinceInfo(int provinceId, int lineNumber, ParadoxGameData& game_data);
};

