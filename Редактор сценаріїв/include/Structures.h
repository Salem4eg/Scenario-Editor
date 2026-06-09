#pragma once
#include <QString>
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
	QList<int> choosable_provinces;
};