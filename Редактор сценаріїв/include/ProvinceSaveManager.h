#pragma once

#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include <QDirIterator>
#include "Structures.h"
#include "FileReader.h"

// Manages provinces in a save file
class ProvinceSaveManager  : public QObject
{
	Q_OBJECT

public:
	ProvinceSaveManager(QString save_directory, QString provinces_directory, QObject *parent = nullptr);
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
	QString m_provinces_directory;

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
};

