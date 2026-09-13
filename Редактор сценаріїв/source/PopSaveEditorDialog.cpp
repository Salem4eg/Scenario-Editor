#include "PopSaveEditorDialog.h"

PopSaveEditorDialog::PopSaveEditorDialog(QWidget* parent)
{
	auto* rightWidget = new QWidget;
	auto* rightLayout = new QVBoxLayout(rightWidget);

	// ### RIGHT PART OF DATA

	auto* militancyWidget = new QWidget;
	auto* militancyLayout = new QHBoxLayout(militancyWidget);

	auto* militancyLabel = new QLabel("Militancy");
	m_militancyEdit = new QLineEdit();
	m_militancyEdit->setAlignment(Qt::AlignHCenter);

	militancyLayout->addWidget(militancyLabel);
	militancyLayout->addWidget(m_militancyEdit);

	auto* literacyWidget = new QWidget;
	auto* literacyLayout = new QHBoxLayout(literacyWidget);

	auto* literacyLabel = new QLabel("Literacy");
	m_literacyEdit = new QLineEdit();
	m_literacyEdit->setAlignment(Qt::AlignHCenter);

	literacyLayout->addWidget(literacyLabel);
	literacyLayout->addWidget(m_literacyEdit);

	auto* consciousnessWidget = new QWidget;
	auto* consciousnessLayout = new QHBoxLayout(consciousnessWidget);

	auto* consciousnessLabel = new QLabel("Consciousness");
	m_consciousnessEdit = new QLineEdit();
	m_consciousnessEdit->setAlignment(Qt::AlignHCenter);

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

	m_ideologyWidget = new PopIdeologyDialog(this);
	m_ideologyWidget->hide();

	QRegularExpression floatingPointValidator(R"(^(10([.]0{1,2})?|[0-9](\.[0-9]{1,2})?)$)");
	QIntValidator* intValidator = new QIntValidator(0, std::numeric_limits<int>::max(), this);
	QRegularExpression literacyValidator(R"(^(0(\.\d{1,5})?|1(\.0{1,5})?)$)");

	m_sizeEdit->setValidator(intValidator);
	m_militancyEdit->setValidator(new QRegularExpressionValidator(floatingPointValidator, this));
	m_consciousnessEdit->setValidator(new QRegularExpressionValidator(floatingPointValidator, this));
	m_literacyEdit->setValidator(new QRegularExpressionValidator(literacyValidator, this));

	topLayout->addWidget(rightWidget);
	

	setupStyle();
	setupConnections();	
}

void PopSaveEditorDialog::setPopData(const PopData& pop)
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

void PopSaveEditorDialog::setIdeologies(const QList<Ideology>& ideologies)
{
	m_ideologyWidget->createIdeologyWigdets(ideologies);
}

void PopSaveEditorDialog::setupConnections()
{
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

	connect(m_militancyEdit, &QLineEdit::textChanged, this, [this]()
	{
		if (m_militancyEdit->text().trimmed().isEmpty())
		{
			m_militancyEdit->setText("0");

			m_popData.militancy = 0.1f;
		}
		else
		{
			double militancy = m_militancyEdit->text().toDouble();
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
			double literacy = m_literacyEdit->text().toDouble();
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
			double consciousness = m_consciousnessEdit->text().toDouble();
			m_popData.consciousness = consciousness;
			hasChanges = true;
		}
	});
}

void PopSaveEditorDialog::setupStyle()
{
	backButton->setObjectName("control_button");
	deletePopButton->setObjectName("control_button");
	m_edit_ideologies->setObjectName("control_button");

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

