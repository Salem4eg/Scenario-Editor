#pragma once
#include <QString>
#include <QHash>
#include <QRgb>
#include <QStringList>

struct ProvinceInfo
{
	QString name;
	QString owner;
	QStringList cores;
};

struct ParadoxGameData
{
	QHash<QRgb, int> color_to_province;
	QHash<QString, QRgb> countries_color;
	QHash<QString, QList<int>> countries_provinces;
	// Non-sea provinces, because they cannot be changed and they have no information.
	QList<int> choosable_provinces;
};

struct Province
{
	int id;
	QString owner;
	QString controller;
	QStringList cores;
	QString name;

	QStringList rawLines;
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