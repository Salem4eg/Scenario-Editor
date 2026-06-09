#include "MapView.h"
#include <sstream>
#include <QtConcurrent/QtConcurrent>
#include <QRegularexpression>

MapView::MapView(QString directory, QString save_file, QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent), enable_province_choosing(false)
{
	data_manager = new MapDataManager(directory, save_file, this);
	
	connect(&highlight_timer, &QTimer::timeout, this, &MapView::highlightChosenProvinces);
	connect(data_manager, &MapDataManager::progressMade, this, &MapView::progressMade);
	connect(data_manager, &MapDataManager::addProvinceToHighlight, this, &MapView::addProvinceToHighlight);
	connect(data_manager, &MapDataManager::removeProvinceFromHighlight, this, &MapView::removeProvinceFromHighlight);
}

MapView::~MapView()
{}

void MapView::prepare()
{
	data_manager->prepare();
}

void MapView::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
	{
		if (scale_step < 8)
		{
			scale(1.25, 1.25);
			scale_step++;
		}
	}
	else
	{
		if (scale_step > -3)
		{
			scale(0.8, 0.8);
			scale_step--;
		}
	}
}

void MapView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		leftButtonStartPos = event->pos();
		mouseDragMode = false;
	}

	QGraphicsView::mousePressEvent(event);	
}

void MapView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);

	if (event->isAccepted())
		return;


	if (event->button() == Qt::LeftButton)
	{
		QPoint leftButtonEndPos = event->pos();

		int distance = (leftButtonEndPos - leftButtonStartPos).manhattanLength();
		int dragDistance = QApplication::startDragDistance();

		if (distance >= dragDistance)
			mouseDragMode = true;

		if (!mouseDragMode)
		{
			QPoint pos = mapToScene(event->pos()).toPoint();
			handleClickAtProvince(pos.x(), pos.y());
		}
	}

}

void MapView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
}

void MapView::highlightChosenProvinces()
{
	if (!provinces_highlighted)
	{
		emit highlightProvinces(true);
	}
	else
	{
		emit highlightProvinces(false);
	}
	
	provinces_highlighted = !provinces_highlighted;
}

void MapView::handleClickAtProvince(int x, int y)
{
	int province = data_manager->provinceAt(x, y);

	if (province == -1)
		return;

	if (enable_province_choosing == false)
	{
		emit getChosenProvinceInfo(province);
		return;
	}

	if (!data_manager->getChosenProvinces().contains(province))
		data_manager->addChosenProvince(province);
	else
		data_manager->removeChosenProvince(province);

	qDebug() << "Province clicked: " << province;

	provinces_highlighted = false;
	highlightChosenProvinces();

	if (data_manager->getChosenProvinces().isEmpty())
		highlight_timer.stop();
	else
		highlight_timer.start(1000);
}



void MapView::setProvinceChoosingMode(bool choose)
{
	enable_province_choosing = choose;
}

void MapView::showHighlighting(bool show)
{
	if (show)
	{
		if (!data_manager->getChosenProvinces().isEmpty())
		{
			highlight_timer.start(1000);
		}
	}
	else
		highlight_timer.stop();
}

void MapView::clearChosenProvinces()
{
	data_manager->clearChosenProvinces();
}

QRgb MapView::GetCountryColor(QString tag)
{
	return data_manager->colorOfCountry(tag);
}

QList<QList<QPoint>> MapView::getProvincesPixels(QList<int> provinces)
{
	return data_manager->pixelsOfProvinces(provinces);
}

