#include "ProvincePopulationDialog.h"

ProvincePopulationDialog::ProvincePopulationDialog(bool isSaveData, QWidget* parent)
	: QDialog(parent), m_hasChanges(false), m_currentMaxPopId(-1)
{
	auto* mainLayout = new QVBoxLayout(this);

	// ### CHARTS
	auto* chartsWidget = new QWidget;
	auto* chartsLayout = new QHBoxLayout(chartsWidget);

	m_culturesChart = new PopPieChartWidget;
	m_typesChart = new PopPieChartWidget;
	m_religionsChart = new PopPieChartWidget;

	chartsLayout->addWidget(m_culturesChart, 1, Qt::AlignCenter);
	chartsLayout->addWidget(m_typesChart, 1, Qt::AlignCenter);
	chartsLayout->addWidget(m_religionsChart, 1, Qt::AlignCenter);

	// ### TITLE
	auto* titleWidget = new QWidget;
	auto* titleLayout = new QHBoxLayout(titleWidget);

	m_provinceName = new QLabel("Province name: ");
	m_totalPopulationLabel = new QLabel("Total population: ");

	titleLayout->addStretch(2);
	titleLayout->addWidget(m_provinceName);
	titleLayout->addStretch(1);
	titleLayout->addWidget(m_totalPopulationLabel);
	titleLayout->addStretch(2);

	// ### POP GROUPS

	popGroupsScrollArea = new QScrollArea;

	popGroupsWidget = new QWidget();
	auto* popGroupsLayout = new QVBoxLayout(popGroupsWidget);

	popGroupsScrollArea->setWidgetResizable(true);
	popGroupsScrollArea->setWidget(popGroupsWidget);
	popGroupsScrollArea->setFixedHeight(300);

	auto* listHeaderWidget = new QWidget;
	auto* listHeaderLayout = new QHBoxLayout(listHeaderWidget);

	auto* sizeLabel = new QLabel("Size");
	auto* jobLabel = new QLabel("Job");
	auto* cultureLabel = new QLabel("Culture");
	auto* religionLabel = new QLabel("Religion");

	listHeaderLayout->addWidget(sizeLabel, 1);
	listHeaderLayout->addWidget(jobLabel, 1);
	listHeaderLayout->addWidget(cultureLabel, 1);
	listHeaderLayout->addWidget(religionLabel, 1);
	listHeaderLayout->addStretch(1);

	auto* popGroupsContainer = new QWidget;
	m_popGroupsLayout = new QVBoxLayout(popGroupsContainer);
	m_popGroupsLayout->setContentsMargins(0, 0, 0, 0);

	popGroupsLayout->addWidget(listHeaderWidget, 0);
	popGroupsLayout->addWidget(popGroupsContainer, 0);
	popGroupsLayout->addStretch(1);

	// ### CLOSE + NEW GROUP BUTTONS

	auto* controlWidget = new QWidget;
	auto* controlLayout = new QHBoxLayout(controlWidget);

	closeButton = new QPushButton("Close");
	newGroup = new QPushButton("New group");

	closeButton->setObjectName("control_button");
	newGroup->setObjectName("control_button");

	controlLayout->addStretch(1);
	controlLayout->addWidget(closeButton);
	controlLayout->addWidget(newGroup);

	mainLayout->addWidget(chartsWidget);
	mainLayout->addWidget(titleWidget);
	mainLayout->addWidget(popGroupsScrollArea, 1);
	mainLayout->addWidget(controlWidget);


	if (isSaveData)
		m_popManager = new PopSaveEditorDialog(this);
	else
		m_popManager = new PopEditorDialog(this);
		
	m_popManager->hide();

	m_popManager->setWindowFlag(Qt::Window);
	m_popManager->setWindowModality(Qt::WindowModal);


	setupStyle();
	setupConnections();
}

ProvincePopulationDialog::~ProvincePopulationDialog()
{}

void ProvincePopulationDialog::setPopData(const QList<PopData>& pops)
{
	m_hasChanges = false;
	m_pops.clear();

	for (const auto& pop : pops)
	{
		m_pops[pop.id] = pop;
	}

	updateCharts();
}

void ProvincePopulationDialog::updateCharts()
{
	clearPopGroups();
	QHash<QString, ChartData> cultures;
	QHash<QString, ChartData> types;
	QHash<QString, ChartData> religions;

	QList<PopData> sortedPops = m_pops.values();

	std::sort(sortedPops.begin(), sortedPops.end(), [](const PopData& first, const PopData& second)
	{
		return first.size > second.size;
	});

	quint64 total_population = 0;

	for (const auto& pop : sortedPops)
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
		
		total_population += pop.size;
	}

	setCulturesChart(cultures.values());
	setTypesChart(types.values());
	setReligionsChart(religions.values());

	updateTotalPopulation(total_population);
}

void ProvincePopulationDialog::updateTotalPopulation(quint64 total_population)
{
	m_totalPopulationLabel->setText(QString("Total population: %1").arg(total_population));
}

void ProvincePopulationDialog::setProvinceName(const QString& name)
{
	m_provinceName->setText("Province name: " + name);
}

void ProvincePopulationDialog::setCurrentMaxPopId(int popId)
{
	m_currentMaxPopId = popId;
}

void ProvincePopulationDialog::setCultures(const QList<DemographicCategory>& cultures)
{
	QStringList culture_names;

	for (const auto& culture : cultures)
		culture_names.push_back(culture.type);

	m_popManager->setCultures(culture_names);

	for (auto& [type, color] : cultures)
		m_color_types.insert(type, color);
}

void ProvincePopulationDialog::setTypes(const QList<DemographicCategory>& types)
{
	QStringList type_names;

	for (const auto& popType : types)
		type_names.push_back(popType.type);

	m_popManager->setTypes(type_names);

	for (auto& [type, color] : types)
		m_color_types.insert(type, color);
}

void ProvincePopulationDialog::setReligions(const QList<DemographicCategory>& religions)
{
	QStringList religion_names;

	for (const auto& religion : religions)
		religion_names.push_back(religion.type);

	m_popManager->setReligions(religion_names);

	for (auto& [type, color] : religions)
		m_color_types.insert(type, color);
}

void ProvincePopulationDialog::setIdeologies(const QList<DemographicCategory>& ideologies)
{
	QList<Ideology> normal_ideologies;

	for (const auto& demographic_category : ideologies)
	{
		Ideology ideology;
		ideology.name = demographic_category.type;
		ideology.id = normal_ideologies.size() + 1;

		normal_ideologies.push_back(ideology);
	}

	m_popManager->setIdeologies(normal_ideologies);
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

void ProvincePopulationDialog::setupStyle()
{
	this->setObjectName("background");
	popGroupsWidget->setStyleSheet("background: transparent");
	popGroupsScrollArea->setObjectName("background");
	m_provinceName->setObjectName("sub-title");
	m_totalPopulationLabel->setObjectName("sub-title");
	

	setStyleSheet(R"(
		#background 
		{ 
			border: 3px solid transparent;
			border-image: url(../images/dialog.png) 3 stretch;
		}
		
		QPushButton 
		{
		    font-weight: bold;
		    color: #FFF0C2;                     
		    font-size: 14px;
		}

		QPushButton:hover
		{
			color: #FFFFFF;
		
		}

		#control_button
		{
			font-size: 16px;
			font-weight: bold;
			background: transparent;
			border: none;
			
		}

		#control_button:hover
		{
			color: #FFFFFF;
		}
		

		QWidget
		{
			font-family: Georgia;
			color: #f5cc93;
		}

		#sub-title
		{
			font-size: 14px;
			font-weight: bold;
		
		}
		QScrollBar:vertical
        {
            background: #241812;
            width: 14px;
            margin: 0px;
        }
        
        QScrollBar::handle:vertical
        {
            background: #4A121A;
            min-height: 20px;
            border: 1px solid #240a0e;
        }
        
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical
        {
            height: 0px;
        }
        
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical
        {
            background: transparent;
        }
		)");
}

void ProvincePopulationDialog::setupConnections()
{
	connect(closeButton, &QPushButton::pressed, this, &ProvincePopulationDialog::closeWidget);
	connect(closeButton, &QPushButton::pressed, this, &ProvincePopulationDialog::saveChanges);

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
			widget->hide();
		}
		delete item;
	}
}

void ProvincePopulationDialog::saveChanges()
{
	if (m_hasChanges)
		emit hasChanges();
}

void ProvincePopulationDialog::mousePressEvent(QMouseEvent* event)
{
	event->accept();
}

void ProvincePopulationDialog::closeEvent(QCloseEvent* event)
{
	saveChanges();
	event->accept();
}

void ProvincePopulationDialog::wheelEvent(QWheelEvent* event)
{
	event->accept();
}