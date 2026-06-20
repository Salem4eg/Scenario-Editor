#pragma once

#include <QObject>
#include <QRgb>
#include <QElapsedTimer>

#include <QMutex>
#include <QTimer>
#include <atomic>

#include "ParadoxParser.h"
#include "MapImageProcessor.h"
#include "ProvinceController.h"

class MapDataManager  : public QObject
{
	Q_OBJECT

public:
	MapDataManager(QString directory_path, QString save_file_path, QObject *parent);
	~MapDataManager();

public slots:

	void prepare();

	int provinceAt(int x, int y);
	QList<QPoint> pixelsOfProvince(int provinceID);
	QList<QList<QPoint>> getProvincesPixels(QList<int> provinces);
	QRgb GetCountryColor(QString countryTag);

	void addChosenProvince(int provinceID);
	void removeChosenProvince(int provinceID);
	const QList<int>& getChosenProvinces() const;
	void clearChosenProvinces();

	QImage getBordersViewMap();
	QImage getCountriesViewMap();

	void changeProvincesOwner(QList<int> provinces, QString country_tag);
	void addCoreToProvinces(QList<int> provinces, QString country_tag);
	void removeCoreFromProvinces(QList<int> provinces, QString country_tag);

	ProvinceInfo getProvinceInfo(int province);

	void setProvinceChoosingMode(bool choose);
	void showHighlighting(bool show);

	void handleClickAtProvince(int x, int y);


signals:
	void progressMade(int progress);
	void addProvinceToHighlight(const QList<QPoint>& province_pixels, int provinceID);
	void removeProvinceFromHighlight(const QList<QPoint>& province_pixels, int provinceID);

	void getChosenProvinceInfo(int provinceID);
	void highlightProvinces(bool highlight);

private:
	void highlightChosenProvinces();

private:
	MapImageProcessor* image_processor;
	ProvinceController* province_manager;


	QString m_directory_path;
	QString m_save_file_path;

	QList<int> choosable_provinces;
	QList<int> chosen_provinces;


	bool provinces_highlighted = false;
	QTimer highlight_timer;

	bool enable_province_choosing = false;

	// DEBUG FUNCTIONS
	QElapsedTimer timer;
	void startDebugTimer();
	void endDebugTimer();
};

