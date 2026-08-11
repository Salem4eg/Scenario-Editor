#pragma once

#include <QWidget>
#include <QPushButton>
#include <QColor>
#include <QHash>
#include "PopPieChartWidget.h"
#include "PopEditorDialog.h"

class ProvincePopulationDialog : public QWidget
{
	Q_OBJECT

public:
	ProvincePopulationDialog(QWidget *parent = nullptr);
	~ProvincePopulationDialog();


public slots:
	void setPopData(const QList<PopData>& pops);
	void addChartDataType(QString type, QColor color);
	// Adding new groups requires new id
	void setCurrentMaxPopId(int popId);
	// setCultures/setTypes/setReligions sets all available cultures, types and religions from the game.
	// Requires to show suggestions in PopManager
	void setCultures(const QList<QString>& cultures);
	void setTypes(const QList<QString>& types);
	void setReligions(const QList<QString>& religions);
	// One time usage, for initializing all available ideologies for PopIdeologyWidget
	void setIdeologies(const QList<Ideology>& ideologies);

	QList<PopData> getPopData() const;
	int getCurrentMaxPopId() const;

signals:
	void closeWidget();
	void hasChanges();
private:
	QLabel* m_provinceName;
	PopPieChartWidget* m_culturesChart;
	PopPieChartWidget* m_typesChart;
	PopPieChartWidget* m_religionsChart;

	QVBoxLayout* m_popGroupsLayout;

	// Represents color of type (culture/religion/job)
	QHash<QString, QColor> m_color_types;

	PopEditorDialog* m_popManager;

	QHash<int, PopData> m_pops;
	int m_currentMaxPopId;
	bool m_hasChanges;

	void addPopGroup(const PopData& pop);

	void updateCharts();
	void setCulturesChart(QList<ChartData> cultures);
	void setTypesChart(QList<ChartData> types);
	void setReligionsChart(QList<ChartData> religions);
	int getNewPopId();

	void clearPopGroups();
};

