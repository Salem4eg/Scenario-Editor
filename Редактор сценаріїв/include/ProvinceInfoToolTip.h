#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsEffect>
#include "Structures.h"
#include "ProvincePopulationDialog.h"

class ProvinceInfoToolTip : public QWidget
{
	Q_OBJECT

public:
	ProvinceInfoToolTip(QWidget *parent = nullptr);
	~ProvinceInfoToolTip();

public slots:
	void setInfo(const Province& info);
	Province getInfo() const;
	
signals:
	void hideToolTip();
	void showProvinceDetails();
	void hasChanges();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	QLabel* m_name;
	QLabel* m_owner;
	QLabel* m_cores;
	Province m_provinceInfo;
	ProvincePopulationDialog* m_populationDialog;
};

