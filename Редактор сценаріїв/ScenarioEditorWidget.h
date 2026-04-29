#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <QComboBox>
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "MapView.h"
#include "Province_manager.h"
#include "ProvinceInfoToolTip.h"
#include "SidePanel.h"


class ScenarioEditorWidget : public QWidget
{
	Q_OBJECT

public:
	ScenarioEditorWidget(QString directory, QWidget *parent = nullptr);
	~ScenarioEditorWidget();

public slots:
    //void prepare();

signals:
    //void isReadyToShow();


protected:

private:
    MapView* view;
    QGraphicsScene* scene;
    SidePanel* side_panel;
    Province_manager* province_manager;

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
};

