#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
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

private:
	QLabel* m_name;
	QLabel* m_owner;
	QLabel* m_cores;
};

