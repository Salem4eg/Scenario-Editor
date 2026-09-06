#pragma once

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QColor>
#include <QHash>
#include <QScrollArea>
#include "PopPieChartWidget.h"
#include "PopEditorDialog.h"

class ProvinceBasePopulationDialog : public QDialog
{
	Q_OBJECT

public:
	ProvinceBasePopulationDialog(QWidget *parent = nullptr);
	~ProvinceBasePopulationDialog();

	
public slots:
	void setPopData(const QList<PopData>& pops);

	// Adding new groups requires new id
	void setCurrentMaxPopId(int popId);

	// Requires to show suggestions in PopManager
	void setCultures(const QList<DemographicCategory>& cultures);
	void setTypes(const QList<DemographicCategory>& types);
	void setReligions(const QList<DemographicCategory>& religions);
	

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

