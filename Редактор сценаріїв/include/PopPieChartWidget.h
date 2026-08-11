#pragma once

#include <QWidget>
#include <QPieSeries>
#include <QChartView>
#include <QChart>
#include <QVBoxLayout>
#include <QLabel>
#include <algorithm>
#include "Structures.h"


class PopPieChartWidget : public QWidget
{
	Q_OBJECT

public:
	PopPieChartWidget(QWidget *parent = nullptr);
	~PopPieChartWidget();

	void setPopData(QList<ChartData> pops, QString title);
private:
	QChart* m_chart;
	QPieSeries* m_series;
	QChartView* m_chartView;
	QLabel* m_title;
	QVBoxLayout* m_legendLayout;

	void addLegendPart(QString text, QColor color);
	void clearLegend();
};

