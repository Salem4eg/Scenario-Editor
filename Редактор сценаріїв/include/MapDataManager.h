#pragma once

#include <QObject>
#include <QRgb>
#include <QElapsedTimer>

#include "ParadoxParser.h"
#include "MapImageProcessor.h"

class MapDataManager  : public QObject
{
	Q_OBJECT

public:
	MapDataManager(QString directory_path, QString save_file_path, QObject *parent);
	~MapDataManager();


	void prepare();

	int provinceAt(int x, int y);
	QList<QPoint> pixelsOfProvince(int provinceID);
	QList<QList<QPoint>> pixelsOfProvinces(QList<int> provinces);
	QRgb colorOfCountry(QString countryTag);

	void addChosenProvince(int provinceID);
	void removeChosenProvince(int provinceID);
	const QList<int>& getChosenProvinces() const;
	void clearChosenProvinces();

signals:
	void progressMade(int progress);
	void addProvinceToHighlight(const QList<QPoint>& province_pixels, int provinceID);
	void removeProvinceFromHighlight(const QList<QPoint>& province_pixels, int provinceID);


private:


private:
	MapImageProcessor* image_processor;

	QString m_directory_path;
	QString m_save_file_path;

	QList<int> choosable_provinces;
	QList<int> chosen_provinces;


	// DEBUG FUNCTIONS
	QElapsedTimer timer;
	void startDebugTimer();
	void endDebugTimer();
};

