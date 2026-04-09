#pragma once

#include <QtWidgets/QMainWindow>
#include "MapView.h"
#include <QGraphicsScene>
#include <QComboBox>
#include <QPushButton>
#include "Province_manager.h"

class Scenario_editor : public QMainWindow
{
    Q_OBJECT

public:
    Scenario_editor(QWidget *parent = nullptr);
    ~Scenario_editor();


protected:
    
private:

    MapView * view;
    QGraphicsScene * scene;
    QWidget * side_panel;
    QComboBox* countries_list;
    Province_manager province_manager;

    QString provinces_path;
    QGraphicsPixmapItem* map_item;
    QGraphicsPixmapItem* highlight_map_item;

    QPushButton* change_owner;
    QPushButton* add_core;
    QPushButton* remove_core;

    QString chosen_tag;
    QList<int> chosen_provinces;

    void set_map();
    void make_connections();
    void fill_countries_list();
    void paintProvinces();

    QList<QPair<QString, QString>> readCountriesFile();
};

