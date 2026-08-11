#include "PopPieChartWidget.h"

PopPieChartWidget::PopPieChartWidget(QWidget *parent)
	: QWidget(parent)
{
	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	auto* topWidget = new QWidget;
	auto* topLayout = new QHBoxLayout(topWidget);
	topLayout->setContentsMargins(0, 0, 0, 0);
	m_title = new QLabel("Types");
	
	topLayout->addSpacing(30);
	topLayout->addWidget(m_title, 0, Qt::AlignVCenter | Qt::AlignBottom);

	auto* centralWidget = new QWidget;
	centralWidget->setFixedHeight(150);
	centralWidget->setFixedWidth(250);
	auto* centralLayout = new QHBoxLayout(centralWidget);
	centralLayout->setContentsMargins(0, 0, 0, 0);
	
	// ### LEFT PART OF CENTRAL WIDGET - DIAGRAM
	m_series = new QPieSeries;
	m_series->setPieSize(0.98);

	m_chart = new QChart;
	m_chart->addSeries(m_series);
	m_chart->legend()->setVisible(false);
	m_chart->setMargins(QMargins(0, 0, 0, 0));
	m_chart->setBackgroundBrush(Qt::transparent);

	m_chartView = new QChartView(m_chart);
	m_chartView->setRenderHint(QPainter::Antialiasing);
	m_chartView->setFixedSize(100, 100);

	// ### RIGHT PART OF CENTRAL WIDGET - DIAGRAM'S LEGEND
	auto* legendWidget = new QWidget;
	m_legendLayout = new QVBoxLayout(legendWidget);
	m_legendLayout->setAlignment(Qt::AlignCenter);
	m_legendLayout->setContentsMargins(0, 0, 0, 0);

	centralLayout->addWidget(m_chartView, 0, Qt::AlignTop);
	centralLayout->addWidget(legendWidget, 1, Qt::AlignLeft | Qt::AlignTop);

	layout->addWidget(topWidget, 0);
	layout->addWidget(centralWidget, 0);
}

PopPieChartWidget::~PopPieChartWidget()
{}

void PopPieChartWidget::setPopData(QList<ChartData> pops, QString title)
{
	clearLegend();
	m_title->setText(title);
	m_series->clear();

	int totalPopulation = 0;

	for (const auto& pop : pops)
	{
		totalPopulation += pop.size;
	}

	if (totalPopulation == 0)
		return;

	int otherPopulation = 0;

	std::sort(pops.begin(), pops.end(), [](const ChartData& first, const ChartData& second)
	{
		return first.size > second.size;
	});

	for (const auto& pop : pops)
	{
		double percentage = static_cast<double>(pop.size) / totalPopulation * 100.0;

		if (percentage > 5.0 || pops.size() <= 5)
		{
			QString label = QString("%1 (%2%)").arg(pop.type).arg(percentage, 0, 'f', 1);
			QPieSlice* slice = m_series->append(label, pop.size);
			slice->setColor(pop.color);
			addLegendPart(label, pop.color);
		}
		else
		{
			otherPopulation += pop.size;
		}

	}

	if (otherPopulation > 0)
	{
		double percentage = static_cast<double>(otherPopulation) / totalPopulation * 100.0;
		QString label = QString("Other (%1%)").arg(percentage, 0, 'f', 1);
		QPieSlice* slice = m_series->append(label, otherPopulation);
		if (percentage < 3.0)
			slice->setLabelVisible(false);
		slice->setColor(QColor(Qt::gray));
		addLegendPart(label, Qt::gray);
	}
}

void PopPieChartWidget::addLegendPart(QString text, QColor color)
{
	auto* rowWidget = new QWidget;
	auto* rowLayout = new QHBoxLayout(rowWidget);
	rowLayout->setContentsMargins(0, 0, 0, 0);
	
	QLabel* colorMarker = new QLabel();
	colorMarker->setFixedSize(10, 10);
	colorMarker->setStyleSheet(QString("background-color: %1; border-radius: 2px;").arg(color.name()));

	QLabel* label = new QLabel(text);

	rowLayout->addWidget(colorMarker, 0);
	rowLayout->addWidget(label, 0, Qt::AlignLeft);

	m_legendLayout->addWidget(rowWidget, 0, Qt::AlignLeft);
}

void PopPieChartWidget::clearLegend()
{	
	QLayoutItem* item;

	while ((item = m_legendLayout->takeAt(0)) != nullptr) 
	{
		if (QWidget* widget = item->widget()) 
		{
			widget->deleteLater();
		}
	
		delete item;
	}
	
}

