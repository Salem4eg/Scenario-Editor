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