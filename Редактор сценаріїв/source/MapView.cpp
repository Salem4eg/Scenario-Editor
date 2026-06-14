#include "MapView.h"

MapView::MapView(QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent)
{
	
}

MapView::~MapView()
{}

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
			emit handleClickAtProvince(pos.x(), pos.y());
		}
	}

}

void MapView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
}

