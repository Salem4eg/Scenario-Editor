#pragma once

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QColor>
#include <QRegularExpression>

#include "ProvinceController.h"
#include "FileReader.h"
#include "FileStreamScanner.h"
#include "Structures.h"

class ParadoxParser  : public QObject
{
	Q_OBJECT

public:
	ParadoxParser(QString directory, QObject *parent = nullptr);
	~ParadoxParser();

	void loadProvincesDefinition(ParadoxGameData& game_data);
	void loadCountriesColors(ParadoxGameData& game_data);
	
	QList<DemographicCategory> loadIdeologies();
	QList<DemographicCategory> loadCultures();
	QList<DemographicCategory> loadReligions();
	QList<DemographicCategory> loadPopTypes();

private:
	QString provinces_definition;

	QString countries_filepath;
	QString directory_path;

	QString provinces_directory_path;


private:
	int getOnlyDigits(const QString& text);

	QList<QString> readCountriesFile();
	QList<QPair<QString, QString>> parseCountriesFilePaths(QList<QString>& countries_rows);
	void extractCountriesColors(QList<QPair<QString, QString>>& countries_filepaths, ParadoxGameData& game_data);

	QColor getPopTypeColor(QString filepath);
	// Checks for errors, throws if any problem
	uchar* getMappedData(QFile& file);
	QList<DemographicCategory> getDemographicCategories(QString filepath, bool isFloat = false);
};

