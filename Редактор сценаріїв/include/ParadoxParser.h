#pragma once

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QColor>
#include <QRegularExpression>

#include "Province_manager.h"
#include "FileReader.h"
#include "Structures.h"

class ParadoxParser  : public QObject
{
	Q_OBJECT

public:
	ParadoxParser(QString directory, QString save_file, QObject *parent = nullptr);
	~ParadoxParser();

	void loadProvincesDefinition(ParadoxGameData& game_data);
	void loadCountriesColors(ParadoxGameData& game_data);
	void loadCountriesProvinces(ParadoxGameData& game_data);


private:
	QString provinces_definition;

	QString countries_filepath;
	QString directory_path;

	QString save_file_path;
	bool save_file_exists = false;
	QString provinces_directory_path;


private:
	int getOnlyDigits(const QString& text);

	QList<QString> readCountriesFile();
	QList<QPair<QString, QString>> parseCountriesFilePaths(QList<QString>& countries_rows);
	void extractCountriesColors(QList<QPair<QString, QString>>& countries_filepaths, ParadoxGameData& game_data);
	
	int getProvinceIDFromFilepath(const QString& filepath);
	void parseProvinceFile(const QString& filepath, ParadoxGameData& game_data);
	void loadProvincesFromSavefile(ParadoxGameData& game_data);



	


};

