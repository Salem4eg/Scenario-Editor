#include "PopIdeologyDialog.h"

PopIdeologyDialog::PopIdeologyDialog(QWidget *parent)
	: QWidget(parent)
{
	setWindowModality(Qt::WindowModal);
	setWindowFlag(Qt::Window);

	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	auto* centralWidget = new QWidget;
	auto* centralLayout = new QVBoxLayout(centralWidget);
	centralWidget->setObjectName("background");

	auto* widgetLabel = new QLabel("Ideologies");
	widgetLabel->setObjectName("title");

	auto* ideologiesWidget = new QWidget;
	auto* containerLayout = new QHBoxLayout(ideologiesWidget);

	auto* leftIdeologiesWidget = new QWidget;
	leftIdeologiesContainer = new QVBoxLayout(leftIdeologiesWidget);

	auto* rightIdeologiesWidget = new QWidget;
	rightIdeologiesContainer = new QVBoxLayout(rightIdeologiesWidget);

	containerLayout->addWidget(leftIdeologiesWidget);
	containerLayout->addWidget(rightIdeologiesWidget);

	auto* bottomWidget = new QWidget;
	auto* bottomLayout = new QVBoxLayout(bottomWidget);

	totalPercentageWidget = new QLabel("Total percentage 100%");

	auto* backButton = new QPushButton("Back");
	backButton->setObjectName("control_button");

	bottomLayout->addStretch(1);
	bottomLayout->addWidget(totalPercentageWidget, 1);
	bottomLayout->addWidget(backButton, 1);

	connect(backButton, &QPushButton::pressed, [this]()
	{
		if (m_has_changes)
			emit ideologiesChanged(m_ideologies.values());

		emit backButtonPressed();
	});

	centralLayout->addWidget(widgetLabel, 0, Qt::AlignHCenter);
	centralLayout->addWidget(ideologiesWidget, 1);
	centralLayout->addWidget(bottomWidget, 0, Qt::AlignHCenter);

	mainLayout->addWidget(centralWidget);

	setupStyle();
}

PopIdeologyDialog::~PopIdeologyDialog()
{}

void PopIdeologyDialog::updateIdeologies(const QList<Ideology>& ideologies)
{
	// Setting default values
	for (const auto& ideologyId : m_ideologies.keys())
	{
		m_ideologies[ideologyId].percentage = 0;
		m_ideologiesLineEdits[ideologyId]->setText("0");
	}

	// Updating with new values
	for (const auto& ideology : ideologies)
	{
		m_ideologies[ideology.id].percentage = ideology.percentage;
		m_ideologiesLineEdits[ideology.id]->setText(QString::number(ideology.percentage));
	}

	checkTotalPercentage();
}

void PopIdeologyDialog::createIdeologyWigdets(const QList<Ideology>&ideologies)
{
	m_has_changes = false;
	int leftPartSize = (ideologies.size() + 2 - 1) / 2;

	for (const auto& ideology : ideologies)
	{
		auto* ideologyWidget = new QWidget;
		auto* ideologyLayout = new QHBoxLayout(ideologyWidget);

		auto* ideologyLabel = new QLabel(ideology.name);
		auto* ideologyEdit = new QLineEdit(QLocale::c().toString(ideology.percentage, 'g', 5));
		ideologyEdit->setAlignment(Qt::AlignHCenter);
		ideologyEdit->setMaximumWidth(80);

		QDoubleValidator* validator = new QDoubleValidator(0.0, 100.0, 5, ideologyEdit);
		validator->setLocale(QLocale::c());

		ideologyEdit->setValidator(validator);

		ideologyLayout->addWidget(ideologyLabel);
		ideologyLayout->addWidget(ideologyEdit);

		if (leftPartSize == 0)
			rightIdeologiesContainer->addWidget(ideologyWidget);
		else
		{
			leftIdeologiesContainer->addWidget(ideologyWidget);
			leftPartSize--;
		}

		if (ideology.id == 9)
			bool debug = true;

		m_ideologies[ideology.id] = ideology;
		m_ideologiesLineEdits[ideology.id] = ideologyEdit;

		connect(ideologyEdit, &QLineEdit::textChanged, [this, ideologyEdit, ideology]()
		{
			double newPercentage = ideologyEdit->text().toDouble();
			double oldPercentage = m_ideologies[ideology.id].percentage;

			bool isEqual = std::abs(newPercentage - oldPercentage) < 0.00001;

			if (isEqual)
				return;

			m_ideologies[ideology.id].percentage = newPercentage;
			
			checkTotalPercentage();
		});
	}

	checkTotalPercentage();
}

void PopIdeologyDialog::clearLayout(QLayout * layout)
{
	if (!layout)
		return;

	QLayoutItem* item;

	while ((item = layout->takeAt(0)) != nullptr)
	{
		if (QWidget* widget = item->widget())
		{
			widget->deleteLater();
		}
		delete item;
	}
}

void PopIdeologyDialog::checkTotalPercentage()
{
	double totalPercentage = std::accumulate(m_ideologies.begin(), m_ideologies.end(), 0.0, [](double sum, const Ideology& ideology)
	{
		return sum + ideology.percentage;
	});


	bool isEqual = std::abs(totalPercentage - 100.0) < 0.00001;

	if (isEqual)
	{
		totalPercentageWidget->setText(QString("Total percentage (%1%)").arg(100));
		m_has_changes = true;
	}
	else
	{
		// Not saving wrong data
		QString formatedPercentage = QLocale::c().toString(totalPercentage, 'f', 5);
		totalPercentageWidget->setText(QString("Total percentage (%1%)").arg(formatedPercentage));
		m_has_changes = false;
	}
}

void PopIdeologyDialog::setupStyle()
{
	setStyleSheet(R"(
		
		#background
		{
			border-image: url(../images/dialog.png) 2 2 2 2 stretch stretch;
			border-width: 0px;
		
		}
		
		#control_button
		{
			font-size: 16px;
			font-weight: bold;
			background: transparent;
			border: none;
			
		}


		#title
		{
			font-size: 16px;
			font-weight: bold;
			background: transparent;
			border: none;
			
		}
		
	)");
}
