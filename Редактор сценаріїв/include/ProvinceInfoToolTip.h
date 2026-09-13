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
	ProvinceInfoToolTip(bool isSaveData, QWidget *parent = nullptr);
	~ProvinceInfoToolTip();

public slots:
	void setInfo(const Province& info);
	Province getInfo() const;

	void setCultures(const QList<DemographicCategory>& cultures);
	void setTypes(const QList<DemographicCategory>& types);
	void setReligions(const QList<DemographicCategory>& religions);
	void setIdeologies(const QList<DemographicCategory>& ideologies);
	
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

