#pragma once
#include <QString>
#include <QHash>
#include <QColor>
#include <QRgb>
#include <QStringList>

struct ParadoxGameData
{
	QHash<QRgb, int> color_to_province;
	QHash<QString, QRgb> countries_color;
	QHash<QString, QList<int>> countries_provinces;
	// Non-sea provinces, because they cannot be changed and they have no information.
	QList<int> choosable_provinces;
};

// Used for showing data in charts in PopChartWidget
struct ChartData
{
	QString type;
	int size;
	QColor color;
};

struct DemographicCategory
{
	// Holds cultures/religions/job types
	QString type;
	QColor color;
};

struct Ideology
{
	int id = -1;
	QString name = "";
	double percentage = 0.0;
};

// Data describing population group in a province.
// Base game (no save file) uses only [size, type, culture, religion]
struct PopData
{
	int id = -1;
	int size = 10'000;
	QString type = "no_type";
	QString culture = "no_culture";
	QString religion = "no_religion";
	double militancy = 0;
	double literacy = 0;
	double consciousness = 0;
	double money = 100;
	QList<Ideology> ideologies;
	QList<QPair<int, double>> issues; // issue_id, percentage

	QStringList rawLines;
};

struct Province
{
	int id;
	QString owner;
	QString controller;
	QStringList cores;
	QString name;

	QList<PopData> population;
	// rawLines for ProvinceSaveManager
	QStringList rawLinesBeforePops;
	QStringList rawLinesAfterPops;

	bool isModified = false;

	qint64 startOffSet = 0;
	qint64 endOffSet = 0;
};

// Types of tokens we encounter in the save file
enum class TokenType
{
	Identifier,  // Province name, country tag, key (id, owner)
	OpenBrace,   // {
	CloseBrace,  // }
	Equals,      // =
	Comment,     // # ...
	EndOfFile
};

struct CountryData
{
	QString tag;
	QList<QList<int>> states;
	quint64 startCountryOffset = -1;
	 // If there is no states, before and after States offset will remain -1
	quint64 beforeStatesOffset = -1;
	quint64 afterStatesOffset = -1;
	quint64 endCountryOffset = -1; 
};