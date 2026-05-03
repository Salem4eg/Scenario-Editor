#include "ScenarioEditorWidget.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QCompleter>
#include <QDir>

ScenarioEditorWidget::ScenarioEditorWidget(QString directory, QWidget * parent): QWidget(parent)
{
    resize(1200, 800);

    QDir dir(directory);
    QString provinces_path(dir.filePath("history/provinces"));

    auto* main_layout = new QVBoxLayout(this);

    scene = new QGraphicsScene(this);
    view = new MapView(directory, scene, this);
    province_manager = new Province_manager(provinces_path);

    main_layout->addWidget(view);

    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    QVBoxLayout* view_layout = new QVBoxLayout(view);
    side_panel = new SidePanel;

    view_layout->addWidget(side_panel, 0, Qt::AlignBottom);
    view_layout->setContentsMargins(10, 0, 0, 20);

    

    side_panel->show();


    province_tooltip = new ProvinceInfoToolTip;
    tooltip = scene->addWidget(province_tooltip);
    tooltip->hide();
    tooltip->setFlag(QGraphicsItem::ItemIgnoresTransformations);

   

    //view->prepare();
    set_map();
    make_connections();


    countries_map_item->setZValue(0);
    highlight_map_item->setZValue(1);
    tooltip->setZValue(2);
}


ScenarioEditorWidget::~ScenarioEditorWidget()
{}

void ScenarioEditorWidget::set_map()
{
    QImage countries_view_map = view->GetCountriesViewMap();
    borders_view_map = view->GetBordersViewMap();
    QImage highlight_layer = QImage(countries_view_map.size(), QImage::Format_ARGB32);
    highlight_layer.fill(Qt::transparent);

    countries_map_item = scene->addPixmap(QPixmap::fromImage(countries_view_map));
    highlight_map_item = scene->addPixmap(QPixmap::fromImage(highlight_layer));

    PaintBordersOverCountriesViewMap();
}

void ScenarioEditorWidget::make_connections()
{

    //connect(view, &MapView::isReadyToShow, this, &ScenarioEditorWidget::isReadyToShow);
    connect(view, &MapView::highlightProvinces, this, &ScenarioEditorWidget::showHighlightedProvinces);

    connect(view, &MapView::addProvinceToHighlight, [&](const QList<QPoint>& province_pixels, int provinceID)
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

    connect(view, &MapView::removeProvinceFromHighlight, [&](const QList<QPoint>& province_pixels, int provinceID)
        {
            QImage highlight_layer = highlight_map_item->pixmap().toImage();

            for (QPoint pixel : province_pixels)
            {
                highlight_layer.setPixelColor(pixel, Qt::transparent);
            }

            chosen_provinces.removeOne(provinceID);
            highlight_map_item->setPixmap(QPixmap::fromImage(highlight_layer));
        });    

    connect(view, &MapView::getChosenProvinceInfo, [=](int provinceID)
        {
            ProvinceInfo info = province_manager->getProvinceInfo(provinceID);

            if (info.name.isEmpty())
                return;

            province_tooltip->setInfo(info);
            tooltip->show();

            QPoint viewPos = view->mapFromGlobal(QCursor::pos());
            QPointF scenePos = view->mapToScene(viewPos);

            tooltip->setPos(scenePos);
        });

    connect(province_tooltip, &ProvinceInfoToolTip::hideToolTip, this, [this]()
        {
            tooltip->hide();
        });

    connect(side_panel, &SidePanel::setChosenTag, this, [=](QString tag)
        {
            chosen_tag = tag;
        });

    connect(side_panel, &SidePanel::provinceChoosing, this, [=](bool isEnabled)
        {
            tooltip->hide();
            showHighlightedProvinces(isEnabled);
            view->showHighlighting(isEnabled);
            view->setProvinceChoosingMode(isEnabled);
        });
    
    connect(side_panel, &SidePanel::changeProvinceOwner, this, [=]()
        {
            province_manager->changeProvincesOwner(chosen_provinces, chosen_tag);

            paintProvinces();
        });

    connect(side_panel, &SidePanel::addCoreToProvinces, this, [=]()
        {
            province_manager->addCoreToProvinces(chosen_provinces, chosen_tag);
        });

    connect(side_panel, &SidePanel::removeCoreFromProvinces, this, [=]()
        {
            province_manager->removeCoreFromProvinces(chosen_provinces, chosen_tag);
        });

    connect(side_panel, &SidePanel::clearChosenProvinces, this, [=]()
        {
            view->clearChosenProvinces();
            view->showHighlighting(false);
        });

}



void ScenarioEditorWidget::paintProvinces()
{
    QRgb country_color = view->GetCountryColor(chosen_tag);
    QImage countries_map = countries_map_item->pixmap().toImage();
    auto pixels_to_paint = view->getProvincesPixels(chosen_provinces);

    for (auto& pixels_from_province : pixels_to_paint)
    {
        for (QPoint pixel : pixels_from_province)
        {
            countries_map.setPixelColor(pixel, country_color);
        }
    }

    countries_map_item->setPixmap(QPixmap::fromImage(countries_map));
    PaintBordersOverCountriesViewMap();
}

void ScenarioEditorWidget::showHighlightedProvinces(bool show)
{
    if (show)
    {
        countries_map_item->setZValue(0);
        highlight_map_item->setZValue(1);
    }
    else
    {
        countries_map_item->setZValue(1);
        highlight_map_item->setZValue(0);
    }
}

void ScenarioEditorWidget::PaintBordersOverCountriesViewMap()
{
    QImage countries_view_map = countries_map_item->pixmap().toImage();

    QPainter painter(&countries_view_map);

    painter.drawImage(0, 0, borders_view_map);
    painter.end();

    countries_map_item->setPixmap(QPixmap::fromImage(countries_view_map));
}

