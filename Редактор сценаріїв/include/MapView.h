#pragma once

#include <QApplication>
#include <QGraphicsView>
#include <QWheelEvent>
#include <ostream>


class MapView : public QGraphicsView
{
	Q_OBJECT

public:
	MapView(QGraphicsScene * scene, QWidget *parent = nullptr);
	~MapView();

signals:
	void handleClickAtProvince(int x, int y);

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;


private:
	int scale_step = 0;
	QPoint leftButtonStartPos;
	bool mouseDragMode = false;
	
	


};

