#pragma once

#include <QApplication>
#include <QGraphicsView>
#include <QWheelEvent>
#include <ostream>
#include <QMutex>
#include <QTimer>

class MapView : public QGraphicsView
{
	Q_OBJECT

public:
	MapView(QWidget *parent = nullptr);
	MapView(QGraphicsScene * scene, QWidget *parent = nullptr);
	~MapView();

	QImage getCountriesViewMap();
	QRgb GetCountryColor(QString tag);
	QList<QList<QPoint>> getProvincesPixels(QList<int> provinces);

signals:
	void highlightProvinces(bool highlight);
	void addProvinceToHighlight(QList<QPoint> province_pixels, int provinceID);
	void removeProvinceFromHighlight(QList<QPoint> province_pixels, int provinceID);

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
private:
	int scale_step = 0;
	QPoint leftButtonStartPos;
	bool mouseDragMode = false;
	QImage province_map;

	QHash<QRgb, int> color_to_province;
	QHash<int, QList<QPoint>> province_pixels;

	QList<int> chosen_provinces;
	bool provinces_highlighted = false;
	QTimer highlight_timer;

	QMutex mutex;

	QHash<QString, QRgb> countries_color;

	QString mod_filepath;

	QHash<QString, QList<int>> countries_provinces;
	QList<int> choosable_provinces;

	void readProvincesDefinition();
	void assignPixelsToProvince();
	void assignPartOfPixelsToProvince(int start_index, int indexes_to_process);

	void highlightChosenProvinces();
	void handleClickAtProvince(int x, int y);

	void addChosenProvince(int provinceID);
	void removeChosenProvince(int provinceID);

	void assignColorToCountries();
	QList<QString> readCountriesFile();
	QList<QPair<QString, QString>> getCountriesFilePath(QList<QString>& countries_rows);
	void getCountriesColor(QList<QPair<QString, QString>>& countries_filepaths);

	
	void getCountries_provinces();
	void getOwnerFromProvince(QString filepath);
	int getProvinceIDFromFilepath(QString filepath);

	QImage makeOutlineForProvinces();
	void makeOutlineForPartOfMap(QImage& outline_map, int start_index, int indexes_to_process);
	bool hasAnotherProvinceNear(int x, int y);

	QImage makeCountriesViewMap();
	void paintProvince(QImage& countries_view_map,int province, QRgb color);

	//void paintOutlineOverCountriesViewMap(QImage& countries_view_map, QImage& outline_map);

	

	// DEBUG FUNCTIONS
	QElapsedTimer timer;
	void startDebugTimer();
	void endDebugTimer();


};

