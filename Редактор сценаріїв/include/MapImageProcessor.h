#pragma once

#include <QObject>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include <QRgb>
#include <QImage>

class MapImageProcessor  : public QObject
{
	Q_OBJECT

public:
	MapImageProcessor(QString directory_path, QObject *parent);
	~MapImageProcessor();


public slots:
	void mapPixelsToProvinces();
	QImage GetBordersViewMap();
	QImage GetCountriesViewMap();

	QRgb colorOfCountry(QString countryTag);
	QList<QPoint> pixelsOfProvince(int province);

	// Must be set before calling GetCountriesViewMap
	void setCountriesProvinces(const QHash<QString, QList<int>>& countries_provinces);
	// Must be set before calling GetCountriesViewMap
	void setCountriesColor(const QHash<QString, QRgb>& countries_color);
	// Must be set before calling mapPixelsToProvinces
	void setColorToProvince(const QHash<QRgb, int>& color_to_province);

	int getMapWidth();
	int getMapHeight();


	int provinceAt(int x, int y);
private:
	QImage province_map;
	QMutex mutex;
	QHash<int, QList<QPoint>> province_pixels;

	QHash<QRgb, int> color_to_province;
	QHash<QString, QList<int>> countries_provinces;
	QHash<QString, QRgb> countries_color;


private slots:
	void mapPartOfPixelsToProvinces(int start_index, int indexes_to_process);
	void paintProvince(QImage& countries_view_map, int province, QRgb color);

	void makeOutlineForPartOfMap(QImage& outline_map, int start_index, int indexes_to_process);
	bool hasAnotherProvinceNear(int x, int y);

	QColor colorAtPixel(int x, int y);
};

