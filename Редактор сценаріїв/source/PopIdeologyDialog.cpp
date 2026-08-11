#include "PopIdeologyDialog.h"

PopIdeologyDialog::PopIdeologyDialog(QWidget *parent)
	: QWidget(parent)
{
	setWindowModality(Qt::WindowModal);
	setWindowFlag(Qt::Window);

	auto* mainLayout = new QVBoxLayout(this);

	auto* widgetLabel = new QLabel("Ideologies");

	auto* ideologiesWidget = new QWidget;
	auto* containerLayout = new QHBoxLayout(ideologiesWidget);

	auto* leftIdeologiesWidget = new QWidget;
	leftIdeologiesContainer = new QVBoxLayout(leftIdeologiesWidget);

	auto* rightIdeologiesWidget = new QWidget;
	rightIdeologiesContainer = new QVBoxLayout(rightIdeologiesWidget);

	containerLayout->addWidget(leftIdeologiesWidget);
	containerLayout->addWidget(rightIdeologiesWidget);

	auto* bottomWidget = new QWidget;
	auto* bottomLayout = new QHBoxLayout(bottomWidget);

	totalPercentageWidget = new QLabel("Total percentage 100%");

	auto* backButton = new QPushButton("Back");

	bottomLayout->addStretch(1);
	bottomLayout->addWidget(backButton, 1);
	bottomLayout->addWidget(totalPercentageWidget, 1);

	connect(backButton, &QPushButton::pressed, [this]()
	{
		if (m_has_changes)
			emit ideologiesChanged(m_ideologies.values());

		emit backButtonPressed();
	});

	mainLayout->addWidget(widgetLabel, 0, Qt::AlignHCenter);
	mainLayout->addWidget(ideologiesWidget, 1);
	mainLayout->addWidget(bottomWidget, 0, Qt::AlignHCenter);
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
		auto* ideologyEdit = new QLineEdit(QString::number(ideology.percentage));
		
		QDoubleValidator* validator = new QDoubleValidator(0.0, 100.0, 2, ideologyEdit);

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

		m_ideologies[ideology.id] = ideology;
		m_ideologiesLineEdits[ideology.id] = ideologyEdit;

		connect(ideologyEdit, &QLineEdit::textChanged, [this, ideologyEdit, ideology]()
		{
			float percentage = ideologyEdit->text().toFloat();
			if (percentage == m_ideologies[ideology.id].percentage)
				return;

			m_ideologies[ideology.id].percentage = percentage;
			
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
	float totalPercentage = 0;

	for (const auto& ideology : m_ideologies)
		totalPercentage += ideology.percentage;

	if (totalPercentage > 100.f)
	{
		totalPercentageWidget->setText(QString("Total percentage exceeds 100% (%1%)").arg(totalPercentage));
		// Not saving wrong data
		m_has_changes = false;
	}
	else
	{
		totalPercentageWidget->setText(QString("Total percentage (%1%)").arg(totalPercentage));
		m_has_changes = true;
	}
}
