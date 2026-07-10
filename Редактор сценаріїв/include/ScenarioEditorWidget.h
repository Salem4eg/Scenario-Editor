#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "MapView.h"
#include "ProvinceInfoToolTip.h"
#include "SidePanel.h"
#include "MapDataManager.h"


class ScenarioEditorWidget : public QWidget
{
	Q_OBJECT

public:
	ScenarioEditorWidget(QString gameDirectory, QString saveFile, QWidget *parent = nullptr);
	~ScenarioEditorWidget();

public slots:
    void prepare();

signals:
    void isReadyToShow();
	void progressMade(int progress);


protected:

private:
    MapView* view;
    QGraphicsScene* scene;
    SidePanel* side_panel;
    MapDataManager* data_manager;

    QGraphicsPixmapItem* countries_map_item;
    QGraphicsPixmapItem* highlight_map_item;
    QImage borders_view_map;

    QString chosen_tag;
    QList<int> chosen_provinces;

    ProvinceInfoToolTip* province_tooltip;
    QGraphicsProxyWidget* tooltip;

    void set_map();
    void make_connections();
    void paintProvinces();

    void showHighlightedProvinces(bool show);

    void PaintBordersOverCountriesViewMap();



    // DEBUG FUNCTIONS
    QElapsedTimer timer;
    void startDebugTimer();
    void endDebugTimer();
};

