#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsEffect>
#include "Structures.h"

class ProvinceInfoToolTip : public QWidget
{
	Q_OBJECT

public:
	ProvinceInfoToolTip(QWidget *parent = nullptr);
	~ProvinceInfoToolTip();

public slots:
	void setInfo(const ProvinceInfo& info);
	
signals:
	void hideToolTip();
	void showProvinceDetails();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	QLabel* m_name;
	QLabel* m_owner;
	QLabel* m_cores;
};

