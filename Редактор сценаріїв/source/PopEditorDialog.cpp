#include "PopEditorDialog.h"

PopEditorDialog::PopEditorDialog(QWidget *parent)
	: QWidget(parent)
{
	setWindowModality(Qt::WindowModal);

	auto* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	
	auto* centralWidget = new QWidget;
	auto* centralLayout = new QVBoxLayout(centralWidget);

	// ### POP DATA
	auto* topWidget = new QWidget;
	topLayout = new QHBoxLayout(topWidget);

	auto* leftWidget = new QWidget;
	auto* leftLayout = new QVBoxLayout(leftWidget);

	/// ### LEFT PART OF DATA

	// #### Size data
	auto* sizeWidget = new QWidget;
	auto* sizeLayout = new QHBoxLayout(sizeWidget);

	auto* sizeLabel = new QLabel("Size");
	m_sizeEdit = new QLineEdit();
	m_sizeEdit->setAlignment(Qt::AlignHCenter);

	sizeLayout->addWidget(sizeLabel);
	sizeLayout->addWidget(m_sizeEdit);

	// #### Type data

	auto* typeWidget = new QWidget;
	auto* typeLayout = new QHBoxLayout(typeWidget);

	auto* typeLabel = new QLabel("Type");
	m_typeComboBox = new QComboBox();
	setupComboBox(m_typeComboBox);

	typeLayout->addWidget(typeLabel);
	typeLayout->addWidget(m_typeComboBox);

	// #### Culture data

	auto* cultureWidget = new QWidget;
	auto* cultureLayout = new QHBoxLayout(cultureWidget);

	auto* cultureContainerWidget = new QWidget;
	auto* cultureContainerLayout = new QVBoxLayout(cultureContainerWidget);

	auto* cultureLabel = new QLabel("Culture");
	m_cultureComboBox = new QComboBox();
	setupComboBox(m_cultureComboBox);

	cultureLayout->addWidget(cultureLabel);
	cultureLayout->addWidget(m_cultureComboBox);

	// #### Religion data

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

	topLayout->addWidget(leftWidget);

	// ### EXIT FROM WIDGET + DELETE GROUP
	auto* bottomWidget = new QWidget;
	auto* bottomLayout = new QHBoxLayout(bottomWidget);

	backButton = new QPushButton("Back");
	deletePopButton = new QPushButton("Delete group");

	bottomLayout->addStretch(1);
	bottomLayout->addWidget(backButton, 1);
	bottomLayout->addWidget(deletePopButton, 1, Qt::AlignRight);

	
	mainLayout->addWidget(centralWidget);

	centralLayout->addWidget(topWidget, 0);
	centralLayout->addWidget(bottomWidget, 0);	


	centralWidget->setObjectName("background");
	setupStyle();
	setupConnections();
}

PopEditorDialog::~PopEditorDialog()
{}

void PopEditorDialog::closeEvent(QCloseEvent* event)
{
	saveChanges();
	event->accept();
}

void PopEditorDialog::saveChanges()
{
	if (hasChanges)
		emit popChanged(m_popData);
}

void PopEditorDialog::setPopData(const PopData& pop)
{
	m_popData = pop;

	m_sizeEdit->setText(QString::number(pop.size));
	m_typeComboBox->setCurrentText(pop.type);
	m_cultureComboBox->setCurrentText(pop.culture);
	m_religionComboBox->setCurrentText(pop.religion);
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

}


void PopEditorDialog::setupComboBox(QComboBox* combobox)
{
	combobox->setEditable(true);
	combobox->setMaxVisibleItems(15);
	combobox->setInsertPolicy(QComboBox::NoInsert);
	combobox->lineEdit()->setAlignment(Qt::AlignHCenter);


	auto* completer = new QCompleter(combobox->model(), combobox);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setFilterMode(Qt::MatchContains);
	completer->setCompletionMode(QCompleter::InlineCompletion);
	combobox->setCompleter(completer);
}

void PopEditorDialog::setupConnections()
{
	connect(backButton, &QPushButton::pressed, this, [this]()
	{
		saveChanges();
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
				double difference = double(size) / m_popData.size;
				double money = m_popData.money.toDouble();
				money *= difference;

				m_popData.money = QString::number(money, 'f', 5);

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
}

void PopEditorDialog::setupStyle()
{
	backButton->setObjectName("control_button");
	deletePopButton->setObjectName("control_button");

	// border - image: url(.. / images / test_combobox.png) 9 13 9 10 stretch;


	setStyleSheet(R"(
		#background
		{
			border-image: url(../images/dialog.png) 2 2 2 2 stretch stretch;
			border-width: 0px;
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

		QComboBox
		{
		    border-width: 10px;
		    font-size: 14px;
		    padding: 2px 2px 2px 2px;
		    background: transparent;
			border: none;
			border-bottom: 2px solid #e6a535;
		}

		QLineEdit
		{
			border-width: 10px;
			font-size: 14px;
			padding: 2px 2px 2px 2px;
			background: transparent;
			border: none;
			border-bottom: 2px solid #e6a535;
		}
		
		QComboBox QAbstractItemView
		{
			background: #2b1d16;
			border-image: url(../images/combobox_list.png) 10 13 10 10 stretch;
			border-width: 10px;
			selection-background-color: #4e3427;
			selection-color: #ffffff;            
			outline: none;             
			border-style: solid;
			padding: 0px;
			margin: -5px;
		}
		
		QComboBox QAbstractItemView::item
		{
			min-height: 25px;
			padding-left: 10px;
		}
		)");
}

