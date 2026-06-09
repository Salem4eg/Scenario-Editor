#pragma once

#include <QApplication>
#include <QGraphicsView>
#include <QWheelEvent>
#include <ostream>
#include <QMutex>
#include <QTimer>
#include <atomic>

#include "MapDataManager.h"

class MapView : public QGraphicsView
{
	Q_OBJECT

public:
	MapView(QString directory_path, QString save_file, QGraphicsScene * scene, QWidget *parent = nullptr);
	~MapView();

public slots:
	void prepare();

	QRgb GetCountryColor(QString tag);
	QList<QList<QPoint>> getProvincesPixels(QList<int> provinces);

	QImage GetBordersViewMap();
	QImage GetCountriesViewMap();

	void setProvinceChoosingMode(bool choose);
	void showHighlighting(bool show);
	void clearChosenProvinces();

signals:
	void isReadyToShow();

	void getChosenProvinceInfo(int provinceID);
	void highlightProvinces(bool highlight);
	void addProvinceToHighlight(const QList<QPoint>& province_pixels, int provinceID);
	void removeProvinceFromHighlight(const QList<QPoint>& province_pixels, int provinceID);

	void progressMade(int progress);

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;


private:
	int scale_step = 0;
	QPoint leftButtonStartPos;
	bool mouseDragMode = false;
	
	bool provinces_highlighted = false;
	QTimer highlight_timer;

	MapDataManager * data_manager;

	bool enable_province_choosing;


	void highlightChosenProvinces();
	void handleClickAtProvince(int x, int y);
};

