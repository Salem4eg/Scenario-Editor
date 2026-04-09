#include "Scenario_editor.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include <QCompleter>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QLabel>

Scenario_editor::Scenario_editor(QWidget *parent)
    : QMainWindow(parent), province_manager("E:/Steam/steamapps/common/Victoria 2/mod/TGC/history/provinces")
{
    resize(1200, 800);

    scene = new QGraphicsScene(this);
    view = new MapView(scene, this);

    setCentralWidget(view);

    // TEMPORARY
    provinces_path = "E:/Steam/steamapps/common/Victoria 2/mod/TGC/map/provinces.bmp";

    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    QVBoxLayout* view_layout = new QVBoxLayout(view);
    side_panel = new QWidget;
    side_panel->setFixedSize(200, 250);
    side_panel->setStyleSheet("background: rgba(30,30,30,100);");

    view_layout->addWidget(side_panel, 0, Qt::AlignBottom);
    view_layout->setContentsMargins(10, 0, 0, 20);

    auto chosen_county = new QLabel("Choosen country: ");
    change_owner = new QPushButton("Change province owner");
    add_core = new QPushButton("Add core");
    remove_core = new QPushButton("Remove core");

    countries_list = new QComboBox(this);
    countries_list->setMaxVisibleItems(15);
    countries_list->setEditable(true);
    countries_list->lineEdit()->setPlaceholderText("Finding country..");
    countries_list->lineEdit()->setAlignment(Qt::AlignCenter);
    fill_countries_list();
    
    chosen_county->setFixedHeight(40);
    change_owner->setFixedHeight(40);
    add_core->setFixedHeight(40);
    remove_core->setFixedHeight(40);
    chosen_county->setAlignment(Qt::AlignCenter);

    QVBoxLayout* panel_layout = new QVBoxLayout(side_panel);

    panel_layout->addWidget(chosen_county, 0, Qt::AlignTop);
    panel_layout->addWidget(countries_list, 0, Qt::AlignTop);
    
    panel_layout->addWidget(change_owner, 0, Qt::AlignBottom);
    panel_layout->addWidget(add_core, 0, Qt::AlignBottom);
    panel_layout->addWidget(remove_core, 0, Qt::AlignBottom);

    side_panel->show();

   
    set_map();
    make_connections();
}

Scenario_editor::~Scenario_editor()
{}

void Scenario_editor::set_map()
{
    QImage countries_view_map = view->getCountriesViewMap();
    QImage highlight_layer = QImage(countries_view_map.size(), QImage::Format_ARGB32);
    highlight_layer.fill(Qt::transparent);

    map_item = scene->addPixmap(QPixmap::fromImage(countries_view_map));
    highlight_map_item = scene->addPixmap(QPixmap::fromImage(highlight_layer));

    highlight_map_item->setZValue(1);
    map_item->setZValue(0);
}

void Scenario_editor::make_connections()
{
    connect(view, &MapView::highlightProvinces, [=](bool highlight)
        {
            if (highlight)
            {
                map_item->setZValue(0);
                highlight_map_item->setZValue(1);
            }
            else
            {
                map_item->setZValue(1);
                highlight_map_item->setZValue(0);
            }
        });

    connect(view, &MapView::addProvinceToHighlight, [=](QList<QPoint> province_pixels, int provinceID)
        {
            QImage highlight_layer = highlight_map_item->pixmap().toImage();

            QColor highlight_color(255, 255, 255);

            for (QPoint pixel : province_pixels)
            {
                highlight_layer.setPixelColor(pixel, highlight_color);
            }

            chosen_provinces.push_back(provinceID);
            highlight_map_item->setPixmap(QPixmap::fromImage(highlight_layer));
        });

    connect(view, &MapView::removeProvinceFromHighlight, [=](QList<QPoint> province_pixels, int provinceID)
        {
            QImage highlight_layer = highlight_map_item->pixmap().toImage();

            for (QPoint pixel : province_pixels)
            {
                highlight_layer.setPixelColor(pixel, Qt::transparent);
            }

            chosen_provinces.removeOne(provinceID);
            highlight_map_item->setPixmap(QPixmap::fromImage(highlight_layer));
        });

    connect(countries_list, &QComboBox::currentTextChanged, [=](const QString&)
        {
            chosen_tag = countries_list->currentData().toString();
        });

    connect(change_owner, &QPushButton::pressed, [=]()
        {
            province_manager.changeProvincesOwner(chosen_provinces, chosen_tag);
            
            paintProvinces();
        });

    connect(add_core, &QPushButton::pressed, [=]()
        {
            province_manager.addCoreToProvinces(chosen_provinces, chosen_tag);
        });

    connect(remove_core, &QPushButton::pressed, [=]()
        {
            province_manager.removeCoreFromProvinces(chosen_provinces, chosen_tag);
        });

}

void Scenario_editor::fill_countries_list()
{
    auto countries = readCountriesFile();
    QStringList countries_and_tags;

    for (auto& [tag, country_and_tag] : countries)
    {
        countries_list->addItem(country_and_tag, tag);
        countries_and_tags.push_back(country_and_tag);
    }

    auto completer = new QCompleter(countries_and_tags, countries_list);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    countries_list->setCompleter(completer);

    countries_list->setCurrentIndex(-1);
}

void Scenario_editor::paintProvinces()
{
    QRgb country_color = view->GetCountryColor(chosen_tag);
    QImage countries_map = map_item->pixmap().toImage();
    auto pixels_to_paint = view->getProvincesPixels(chosen_provinces);

    for (auto& pixels_from_province : pixels_to_paint)
    {
        for (QPoint pixel : pixels_from_province)
        {
            countries_map.setPixelColor(pixel, country_color);
        }
    }

    
    map_item->setPixmap(QPixmap::fromImage(countries_map));
}


QList<QPair<QString, QString>> Scenario_editor::readCountriesFile()
{
    QList<QPair<QString, QString>> countries;

    QString mod_filepath = "E:/Steam/steamapps/common/Victoria 2/mod/TGC";
    QString countries_filepath = mod_filepath + "/common/countries.txt";

    QFile countries_file(countries_filepath);

    if (!countries_file.open(QFile::ReadOnly | QFile::Text))
    {

    }

    QTextStream read(&countries_file);

    while (!read.atEnd())
    {
        QString line = read.readLine();

        if (line.contains("#"))
            continue;

        auto splitted = line.split("=");

        if (splitted.size() != 2)
            continue;

        QString tag = splitted[0].trimmed();
        QString path = splitted[1].trimmed();

        auto splitted_path = path.split("/");

        QString country = splitted_path[1].trimmed().remove(".txt\"");
        QString country_tag = tag + " - " + country;

        countries.push_back({ tag, country_tag });
    }

    return countries;
}

