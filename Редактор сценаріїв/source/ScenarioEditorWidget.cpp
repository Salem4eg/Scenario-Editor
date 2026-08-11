#include "ScenarioEditorWidget.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QCompleter>
#include <QDir>
#include <QtConcurrent/QtConcurrent>

ScenarioEditorWidget::ScenarioEditorWidget(QString gameDirectory, QString saveFile, QWidget * parent): QWidget(parent)
{
    resize(1200, 800);


    auto* main_layout = new QVBoxLayout(this);

    scene = new QGraphicsScene(this);
    view = new MapView(scene, this);
    data_manager = new MapDataManager(gameDirectory, saveFile, this);

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


    connect(data_manager, &MapDataManager::progressMade, this, &ScenarioEditorWidget::progressMade);
	connect(province_tooltip, &ProvinceInfoToolTip::hasChanges, this, [=]()
	{
        auto province_info = province_tooltip->getInfo();

		data_manager->setProvincePopData(province_info.id, province_info.population);
	});
}


ScenarioEditorWidget::~ScenarioEditorWidget()
{}

void ScenarioEditorWidget::prepare()
{
	QtConcurrent::run([this]()
    {
        data_manager->prepare();

        set_map();

        

        make_connections();

        countries_map_item->setZValue(0);
        highlight_map_item->setZValue(1);
        tooltip->setZValue(2);

        emit isReadyToShow();
    });
}

void ScenarioEditorWidget::set_map()
{
    qDebug() << "getCountriesViewMap()";
    startDebugTimer();

    QImage countries_view_map = data_manager->getCountriesViewMap();
    QtConcurrent::run([this]()
    {
        emit progressMade(5);
    });

    endDebugTimer();

    qDebug() << "getBordersViewMap()";
    startDebugTimer();

    borders_view_map = data_manager->getBordersViewMap();
    QtConcurrent::run([this]()
        {
            emit progressMade(40);
        });

    endDebugTimer();


    QImage highlight_layer = QImage(countries_view_map.size(), QImage::Format_ARGB32);
    highlight_layer.fill(Qt::transparent);

    countries_map_item = scene->addPixmap(QPixmap::fromImage(countries_view_map));
    highlight_map_item = scene->addPixmap(QPixmap::fromImage(highlight_layer));

    qDebug() << "PaintBordersOverCountriesViewMap()";
    startDebugTimer();

    PaintBordersOverCountriesViewMap();
    QtConcurrent::run([this]()
        {
            emit progressMade(5);
        });

    endDebugTimer();
}

void ScenarioEditorWidget::make_connections()
{
	connect(view, &MapView::handleClickAtProvince, data_manager, &MapDataManager::handleClickAtProvince);
    //connect(view, &MapView::isReadyToShow, this, &ScenarioEditorWidget::isReadyToShow);
    connect(data_manager, &MapDataManager::highlightProvinces, this, &ScenarioEditorWidget::showHighlightedProvinces);

    connect(data_manager, &MapDataManager::addProvinceToHighlight, [&](const QList<QPoint>& province_pixels, int provinceID)
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

    connect(data_manager, &MapDataManager::removeProvinceFromHighlight, [&](const QList<QPoint>& province_pixels, int provinceID)
        {
            QImage highlight_layer = highlight_map_item->pixmap().toImage();

            for (QPoint pixel : province_pixels)
            {
                highlight_layer.setPixelColor(pixel, Qt::transparent);
            }

            chosen_provinces.removeOne(provinceID);
            highlight_map_item->setPixmap(QPixmap::fromImage(highlight_layer));
        });    

    connect(data_manager, &MapDataManager::getChosenProvinceInfo, [=](int provinceID)
        {
            Province info = data_manager->getProvinceInfo(provinceID);

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
            data_manager->showHighlighting(isEnabled);
            data_manager->setProvinceChoosingMode(isEnabled);
        });
    
    connect(side_panel, &SidePanel::changeProvinceOwner, this, [=]()
        {
            data_manager->changeProvincesOwner(chosen_provinces, chosen_tag);

            paintProvinces();
        });

    connect(side_panel, &SidePanel::addCoreToProvinces, this, [=]()
        {
            data_manager->addCoreToProvinces(chosen_provinces, chosen_tag);
        });

    connect(side_panel, &SidePanel::removeCoreFromProvinces, this, [=]()
        {
            data_manager->removeCoreFromProvinces(chosen_provinces, chosen_tag);
        });

    connect(side_panel, &SidePanel::clearChosenProvinces, this, [=]()
        {
            data_manager->clearChosenProvinces();
            data_manager->showHighlighting(false);
        });

}

void ScenarioEditorWidget::paintProvinces()
{
    QRgb country_color = data_manager->GetCountryColor(chosen_tag);
    QImage countries_map = countries_map_item->pixmap().toImage();
    auto pixels_to_paint = data_manager->getProvincesPixels(chosen_provinces);

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

void ScenarioEditorWidget::startDebugTimer()
{
    timer.start();
}

void ScenarioEditorWidget::endDebugTimer()
{
    auto time_elapsed = timer.elapsed();
    int time_elapsed_sec = time_elapsed / 1000;
    int ms_remaining = time_elapsed % 1000;

    qDebug() << "Time: " << time_elapsed_sec << "s " << ms_remaining << "ms";
}

