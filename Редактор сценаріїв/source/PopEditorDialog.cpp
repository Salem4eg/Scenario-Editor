#include "PopEditorDialog.h"

PopEditorDialog::PopEditorDialog(QWidget *parent)
	: QWidget(parent)
{
	setWindowModality(Qt::WindowModal);

	auto* mainLayout = new QVBoxLayout(this);
	
	// ### POP DATA
	auto* topWidget = new QWidget;
	auto* topLayout = new QHBoxLayout(topWidget);

	auto* leftWidget = new QWidget;
	auto* leftLayout = new QVBoxLayout(leftWidget);

	/// ### LEFT PART OF DATA
	auto* sizeWidget = new QWidget;
	auto* sizeLayout = new QHBoxLayout(sizeWidget);

	auto* sizeLabel = new QLabel("Size");
	m_sizeEdit = new QLineEdit();

	sizeLayout->addWidget(sizeLabel);
	sizeLayout->addWidget(m_sizeEdit);

	auto* typeWidget = new QWidget;
	auto* typeLayout = new QHBoxLayout(typeWidget);

	auto* typeLabel = new QLabel("Type");
	m_typeComboBox = new QComboBox();
	setupComboBox(m_typeComboBox);

	typeLayout->addWidget(typeLabel);
	typeLayout->addWidget(m_typeComboBox);

	auto* cultureWidget = new QWidget;
	auto* cultureLayout = new QHBoxLayout(cultureWidget);

	auto* cultureLabel = new QLabel("Culture");
	m_cultureComboBox = new QComboBox();
	setupComboBox(m_cultureComboBox);

	cultureLayout->addWidget(cultureLabel);
	cultureLayout->addWidget(m_cultureComboBox);

	auto* religionWidget = new QWidget;
	auto* religionLayout = new QHBoxLayout(religionWidget);

	auto* religionLabel = new QLabel("Religion");
	m_religionComboBox = new QComboBox();
	setupComboBox(m_religionComboBox);

	religionLayout->addWidget(religionLabel);
	religionLayout->addWidget(m_religionComboBox);

	leftLayout->addWidget(sizeWidget);
	leftLayout->addWidget(cultureWidget);
	leftLayout->addWidget(typeWidget);
	leftLayout->addWidget(religionWidget);
	leftLayout->addStretch(1);

	auto* rightWidget = new QWidget;
	auto* rightLayout = new QVBoxLayout(rightWidget);

	// ### RIGHT PART OF DATA

	auto* militancyWidget = new QWidget;
	auto* militancyLayout = new QHBoxLayout(militancyWidget);

	auto* militancyLabel = new QLabel("Militancy");
	m_militancyEdit = new QLineEdit();

	militancyLayout->addWidget(militancyLabel);
	militancyLayout->addWidget(m_militancyEdit);

	auto* literacyWidget = new QWidget;
	auto* literacyLayout = new QHBoxLayout(literacyWidget);

	auto* literacyLabel = new QLabel("Literacy");
	m_literacyEdit = new QLineEdit();

	literacyLayout->addWidget(literacyLabel);
	literacyLayout->addWidget(m_literacyEdit);

	auto* consciousnessWidget = new QWidget;
	auto* consciousnessLayout = new QHBoxLayout(consciousnessWidget);

	auto* consciousnessLabel = new QLabel("Consciousness");
	m_consciousnessEdit = new QLineEdit();

	consciousnessLayout->addWidget(consciousnessLabel);
	consciousnessLayout->addWidget(m_consciousnessEdit);

	auto* ideologyWidget = new QWidget;
	auto* ideologyLayout = new QHBoxLayout(ideologyWidget);

	auto* ideologyLabel = new QLabel("Ideology");
	m_edit_ideologies = new QPushButton("Edit");

	ideologyLayout->addWidget(ideologyLabel);
	ideologyLayout->addWidget(m_edit_ideologies);


	rightLayout->addWidget(militancyWidget);
	rightLayout->addWidget(literacyWidget);
	rightLayout->addWidget(consciousnessWidget);
	rightLayout->addWidget(ideologyWidget);
	rightLayout->addStretch(1);

	topLayout->addWidget(leftWidget);
	topLayout->addWidget(rightWidget);

	// ### EXIT FROM WIDGET + DELETE GROUP
	auto* bottomWidget = new QWidget;
	auto* bottomLayout = new QHBoxLayout(bottomWidget);

	auto* backButton = new QPushButton("Back");
	auto* deletePopButton = new QPushButton("Delete group");

	bottomLayout->addStretch(1);
	bottomLayout->addWidget(backButton, 1);
	bottomLayout->addWidget(deletePopButton, 1, Qt::AlignRight);

	
	mainLayout->addWidget(topWidget, 0);
	mainLayout->addWidget(bottomWidget, 0);

	m_ideologyWidget = new PopIdeologyDialog(this);
	m_ideologyWidget->hide();

	QRegularExpression floatingPointValidator(R"(^(10([.]0{1,2})?|[0-9](\.[0-9]{1,2})?)$)");
	QIntValidator* intValidator = new QIntValidator(0, std::numeric_limits<int>::max(), this);
	QRegularExpression literacyValidator(R"(^(0(\.\d{1,5})?|1(\.0{1,5})?)$)");

	m_sizeEdit->setValidator(intValidator);
	m_militancyEdit->setValidator(new QRegularExpressionValidator(floatingPointValidator, this));
	m_consciousnessEdit->setValidator(new QRegularExpressionValidator(floatingPointValidator, this));
	m_literacyEdit->setValidator(new QRegularExpressionValidator(literacyValidator, this));

	connect(m_edit_ideologies, &QPushButton::pressed, this, [this]()
	{
		m_ideologyWidget->show();
	});

	connect(m_ideologyWidget, &PopIdeologyDialog::backButtonPressed, this, [this]()
	{
		m_ideologyWidget->hide();
	});

	connect(m_ideologyWidget, &PopIdeologyDialog::ideologiesChanged, this, [this](QList<Ideology> ideologies)
	{
		m_popData.ideologies = ideologies;

		hasChanges = true;
	});

	connect(backButton, &QPushButton::pressed, this, [this]()
	{
		if (hasChanges)
			emit popChanged(m_popData);
		emit backButtonPressed();
	});

	connect(deletePopButton, &QPushButton::pressed, this, [this]()
	{
		emit deletePop(m_popData.id);
	});

	connect(m_sizeEdit, &QLineEdit::textChanged, this, [this]()
	{
		if (m_sizeEdit->text().trimmed().isEmpty())
		{
			m_sizeEdit->setText("10000");

			m_popData.size = 10'000;
		}
		else
		{
			int size = m_sizeEdit->text().toInt();
			if (size > 0)
			{
				bool isOldSizeBigger = m_popData.size > size;
				float difference = float(size) / m_popData.size;
				m_popData.money *= difference;

				m_popData.size = size;
				hasChanges = true;
			}
		}
	});

	connect(m_typeComboBox, &QComboBox::currentTextChanged, this, [this](const QString& text)
	{
		m_popData.type = text;
		hasChanges = true;
	});

	connect(m_cultureComboBox, &QComboBox::currentTextChanged, this, [this](const QString& text)
	{
		m_popData.culture = text;
		hasChanges = true;
	});

	connect(m_religionComboBox, &QComboBox::currentTextChanged, this, [this](const QString& text)
	{
		m_popData.religion = text;
		hasChanges = true;
	});

	connect(m_militancyEdit, &QLineEdit::textChanged, this, [this]()
	{
		if (m_militancyEdit->text().trimmed().isEmpty())
		{
			m_militancyEdit->setText("0");

			m_popData.militancy = 0.1f;
		}
		else
		{
			float militancy = m_militancyEdit->text().toFloat();
			m_popData.militancy = militancy;
			hasChanges = true;
		}
	});

	connect(m_literacyEdit, &QLineEdit::textChanged, this, [this]()
	{
		if (m_literacyEdit->text().trimmed().isEmpty())
		{
			m_literacyEdit->setText("0");

			m_popData.literacy = 0.1f;
		}
		else
		{
			float literacy = m_literacyEdit->text().toFloat();
			m_popData.literacy = literacy;
			hasChanges = true;
		}
	});

	connect(m_consciousnessEdit, &QLineEdit::textChanged, this, [this]()
	{
		if (m_consciousnessEdit->text().trimmed().isEmpty())
		{
			m_consciousnessEdit->setText("0");

			m_popData.consciousness = 0.1f;
		}
		else
		{
			float consciousness = m_consciousnessEdit->text().toFloat();
			m_popData.consciousness = consciousness;
			hasChanges = true;
		}		
	});
}

PopEditorDialog::~PopEditorDialog()
{}

void PopEditorDialog::setPopData(const PopData & pop)
{
	m_popData = pop;

	m_sizeEdit->setText(QString::number(pop.size));
	m_typeComboBox->setCurrentText(pop.type);
	m_cultureComboBox->setCurrentText(pop.culture);
	m_religionComboBox->setCurrentText(pop.religion);
	m_militancyEdit->setText(QString::number(pop.militancy));
	m_literacyEdit->setText(QString::number(pop.literacy));
	m_consciousnessEdit->setText(QString::number(pop.consciousness));

	m_ideologyWidget->updateIdeologies(pop.ideologies);
}

void PopEditorDialog::setCultures(const QList<QString>& cultures)
{
	m_cultureComboBox->addItems(cultures);
	m_cultureComboBox->completer()->setModel(m_cultureComboBox->model());
}

void PopEditorDialog::setTypes(const QList<QString>& types)
{
	m_typeComboBox->addItems(types);
	m_typeComboBox->completer()->setModel(m_typeComboBox->model());
}

void PopEditorDialog::setReligions(const QList<QString>& religions)
{
	m_religionComboBox->addItems(religions);
	m_religionComboBox->completer()->setModel(m_religionComboBox->model());
}

void PopEditorDialog::setIdeologies(const QList<Ideology>& ideologies)
{
	m_ideologyWidget->createIdeologyWigdets(ideologies);
}

void PopEditorDialog::setupComboBox(QComboBox* combobox)
{
	combobox->setEditable(true);
	combobox->setMaxVisibleItems(15);
	combobox->setInsertPolicy(QComboBox::NoInsert);

	auto* completer = new QCompleter(combobox->model(), combobox);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setFilterMode(Qt::MatchContains);
	completer->setCompletionMode(QCompleter::InlineCompletion);
	combobox->setCompleter(completer);
}