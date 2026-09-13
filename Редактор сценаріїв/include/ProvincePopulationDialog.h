#pragma once

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QColor>
#include <QHash>
#include <QScrollArea>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QWheelEvent>
#include "PopPieChartWidget.h"
#include "PopEditorDialog.h"
#include "PopSaveEditorDialog.h"

class ProvincePopulationDialog : public QDialog
{
	Q_OBJECT

public:
	ProvincePopulationDialog(bool isSaveData, QWidget* parent = nullptr);
	~ProvincePopulationDialog();


public slots:
	void setPopData(const QList<PopData>& pops);
	void setProvinceName(const QString& name);

	// Adding new groups requires new id
	void setCurrentMaxPopId(int popId);

	// Requires to show suggestions in PopManager
	void setCultures(const QList<DemographicCategory>& cultures);
	void setTypes(const QList<DemographicCategory>& types);
	void setReligions(const QList<DemographicCategory>& religions);
	// For initializing all available ideologies for PopIdeologyWidget
	void setIdeologies(const QList<DemographicCategory>& ideologies);


	QList<PopData> getPopData() const;
	int getCurrentMaxPopId() const;

signals:
	void closeWidget();
	void hasChanges();

protected slots:
	void mousePressEvent(QMouseEvent* event) override;
	void closeEvent(QCloseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	void addPopGroup(const PopData& pop);

	void updateCharts();
	void updateTotalPopulation(quint64 total_population);
	void setCulturesChart(QList<ChartData> cultures);
	void setTypesChart(QList<ChartData> types);
	void setReligionsChart(QList<ChartData> religions);
	int getNewPopId();

	void setupStyle();
	void setupConnections();

	void clearPopGroups();
	void saveChanges();

protected:
	QLabel* m_provinceName;
	QLabel* m_totalPopulationLabel;
	PopPieChartWidget* m_culturesChart;
	PopPieChartWidget* m_typesChart;
	PopPieChartWidget* m_religionsChart;

	QPushButton* closeButton;
	QPushButton* newGroup;
	QVBoxLayout* m_popGroupsLayout;

	QScrollArea* popGroupsScrollArea;
	QWidget * popGroupsWidget;

	// Represents color of type (culture/religion/job)
	QHash<QString, QColor> m_color_types;

	PopEditorDialog* m_popManager;

	QHash<int, PopData> m_pops;
	int m_currentMaxPopId;
	bool m_hasChanges;
};

