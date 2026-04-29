#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QFile>

class SidePanel : public QWidget
{
	Q_OBJECT

public:
	SidePanel(QWidget *parent = nullptr);
	~SidePanel();

signals:
    void provinceChoosing(bool enable);
    void changeProvinceOwner();
    void addCoreToProvinces();
    void removeCoreFromProvinces();
    void clearChosenProvinces();
    void setChosenTag(QString tag);

private:

    QPushButton* choosing_mode;
    QComboBox*   countries_list;
    QPushButton* clear_chosen_provinces;
    QPushButton* change_owner;
    QPushButton* add_core;
    QPushButton* remove_core;
    QLabel* chosen_country;

    QString chosen_tag;

    bool enable_province_choosing = false;

    void fill_countries_list();
    QList<QPair<QString, QString>> readCountriesFile();
    void showButtonsForChangingProvinces(bool show);
};

