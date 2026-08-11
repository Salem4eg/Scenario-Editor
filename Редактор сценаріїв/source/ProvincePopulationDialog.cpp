#include "ProvincePopulationDialog.h"

ProvincePopulationDialog::ProvincePopulationDialog(QWidget *parent)
	: QWidget(parent)
{
	setWindowModality(Qt::WindowModal);
	auto* mainLayout = new QVBoxLayout(this);

	// ### TITLE
	auto* headerWidget = new QWidget;
	auto* headerLayout = new QVBoxLayout(headerWidget);

	m_provinceName = new QLabel("Province name");
	headerLayout->addWidget(m_provinceName);

	// ### CHARTS
	auto* chartsWidget = new QWidget;
	auto* chartsLayout = new QHBoxLayout(chartsWidget);

	m_culturesChart = new PopPieChartWidget;
	m_typesChart = new PopPieChartWidget;
	m_religionsChart = new PopPieChartWidget;

	chartsLayout->addWidget(m_culturesChart, 1, Qt::AlignCenter);
	chartsLayout->addWidget(m_typesChart, 1, Qt::AlignCenter);
	chartsLayout->addWidget(m_religionsChart, 1, Qt::AlignCenter);

	// ### POP GROUPS

	auto* popGroupsWidget = new QWidget;
	auto* popGroupsLayout = new QVBoxLayout(popGroupsWidget);

	auto* popGroupsContainer = new QWidget;
	m_popGroupsLayout = new QVBoxLayout(popGroupsContainer);
	m_popGroupsLayout->setContentsMargins(0, 0, 0, 0);

	popGroupsLayout->addWidget(popGroupsContainer, 0);
	popGroupsLayout->addStretch(1);

	// ### CLOSE + NEW GROUP BUTTONS

	auto* controlWidget = new QWidget;
	auto* controlLayout = new QHBoxLayout(controlWidget);

	auto* closeButton = new QPushButton("Close");
	auto* newGroup = new QPushButton("New group");

	controlLayout->addStretch(1);
	controlLayout->addWidget(closeButton);
	controlLayout->addWidget(newGroup);

	mainLayout->addWidget(headerWidget);
	mainLayout->addWidget(chartsWidget);
	mainLayout->addWidget(popGroupsWidget);
	mainLayout->addWidget(controlWidget);



	m_popManager = new PopEditorDialog(this);
	m_popManager->hide();

	m_popManager->setWindowFlag(Qt::Window);
	m_popManager->setWindowModality(Qt::WindowModal);

	connect(closeButton, &QPushButton::pressed, this, &ProvincePopulationDialog::closeWidget);
	connect(closeButton, &QPushButton::pressed, this, [this]()
	{
		if (m_hasChanges)
			emit hasChanges();
	});

	connect(m_popManager, &PopEditorDialog::deletePop, this, [this](int popId)
	{
		m_popManager->hide();
		m_pops.remove(popId);
		m_hasChanges = true;

		updateCharts();
	});

	connect(m_popManager, &PopEditorDialog::backButtonPressed, m_popManager, &PopEditorDialog::hide);

	connect(m_popManager, &PopEditorDialog::popChanged, this, [this](PopData pop)
	{
		m_pops[pop.id] = pop;
		m_hasChanges = true;

		updateCharts();
	});

	connect(newGroup, &QPushButton::pressed, this, [this]()
	{
		PopData newPop;

		newPop.id = getNewPopId();

		m_pops.insert(newPop.id, newPop);

		m_hasChanges = true;

		addPopGroup(newPop);
		updateCharts();
	});
}

ProvincePopulationDialog::~ProvincePopulationDialog()
{}

void ProvincePopulationDialog::setPopData(const QList<PopData>& pops)
{
	m_hasChanges = false;
	m_pops.clear();

	for (const auto& pop : pops)
		m_pops[pop.id] = pop;

	updateCharts();
}

void ProvincePopulationDialog::addChartDataType(QString type, QColor color)
{
	m_color_types.insert(type, color);
}

void ProvincePopulationDialog::updateCharts()
{
	clearPopGroups();
	QHash<QString, ChartData> cultures;
	QHash<QString, ChartData> types;
	QHash<QString, ChartData> religions;

	for (const auto& pop : m_pops)
	{
		addPopGroup(pop);

		cultures[pop.culture].type = pop.culture;
		cultures[pop.culture].size += pop.size;
		cultures[pop.culture].color = m_color_types[pop.culture];

		types[pop.type].type = pop.type;
		types[pop.type].size += pop.size;
		types[pop.type].color = m_color_types[pop.type];

		religions[pop.religion].type = pop.religion;
		religions[pop.religion].size += pop.size;
		religions[pop.religion].color = m_color_types[pop.religion];
	}

	setCulturesChart(cultures.values());
	setTypesChart(types.values());
	setReligionsChart(religions.values());
}

void ProvincePopulationDialog::setCurrentMaxPopId(int popId)
{
	m_currentMaxPopId = popId;
}

void ProvincePopulationDialog::setCultures(const QList<QString>& cultures)
{
	m_popManager->setCultures(cultures);
}

void ProvincePopulationDialog::setTypes(const QList<QString>& types)
{
	m_popManager->setTypes(types);
}

void ProvincePopulationDialog::setReligions(const QList<QString>& religions)
{
	m_popManager->setReligions(religions);
}

void ProvincePopulationDialog::setIdeologies(const QList<Ideology>& ideologies)
{
	m_popManager->setIdeologies(ideologies);
}

QList<PopData> ProvincePopulationDialog::getPopData() const
{
	return m_pops.values();
}

int ProvincePopulationDialog::getCurrentMaxPopId() const
{
	return m_currentMaxPopId;
}

void ProvincePopulationDialog::addPopGroup(const PopData& pop)
{
	auto* popWidget = new QWidget;
	auto* popLayout = new QHBoxLayout(popWidget);

	auto* sizeLabel = new QLabel(QString::number(pop.size));
	auto* typeLabel = new QLabel(pop.type);
	auto* cultureLabel = new QLabel(pop.culture);
	auto* religionLabel = new QLabel(pop.religion);
	auto* editButton = new QPushButton("Edit");

	popLayout->addWidget(sizeLabel);
	popLayout->addWidget(typeLabel);
	popLayout->addWidget(cultureLabel);
	popLayout->addWidget(religionLabel);
	popLayout->addWidget(editButton);

	connect(editButton, &QPushButton::pressed, this, [this, pop]()
	{
		m_popManager->show();
		m_popManager->setPopData(m_pops[pop.id]);
	});

	m_popGroupsLayout->addWidget(popWidget);
}

void ProvincePopulationDialog::setCulturesChart(QList<ChartData> cultures)
{
	m_culturesChart->setPopData(cultures, "Cultures");
}

void ProvincePopulationDialog::setTypesChart(QList<ChartData> types)
{
	m_typesChart->setPopData(types, "Types");
}

void ProvincePopulationDialog::setReligionsChart(QList<ChartData> religions)
{
	m_religionsChart->setPopData(religions, "Religions");
}

int ProvincePopulationDialog::getNewPopId()
{
	return m_currentMaxPopId++;
}

void ProvincePopulationDialog::clearPopGroups()
{
	if (!m_popGroupsLayout)
		return;

	QLayoutItem* item;

	while ((item = m_popGroupsLayout->takeAt(0)) != nullptr)
	{
		if (QWidget* widget = item->widget())
		{
			widget->deleteLater();
		}
		delete item;
	}
}

