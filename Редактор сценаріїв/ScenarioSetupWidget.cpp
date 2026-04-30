#include "ScenarioSetupWidget.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>

ScenarioSetupWidget::ScenarioSetupWidget(QWidget* parent)
    : QWidget(parent)
{
	auto layout = new QVBoxLayout(this);
	auto widget = new QWidget;
	layout->addWidget(widget);
	layout->setContentsMargins(0, 0, 0, 0);



	auto* content_layout = new QVBoxLayout(widget);

	auto* title = new QLabel("Scenario Editor");
	title->setObjectName("title");

	auto* settings_panel = new QWidget;
	//settings_panel->setStyleSheet("background: none; background-color: gray; ");
	//settings_panel->setFixedSize(800, 200);
	auto* settings_panel_layout = new QHBoxLayout(settings_panel);

	auto* directory_setting_container = new QWidget;
	//directory_setting_container->setFixedSize(200, 200);
	//directory_setting_container->setStyleSheet("background: none; background-color: yellow; ");
	auto* directory_setting_layout = new QVBoxLayout(directory_setting_container);

	auto* directory_subtitle_label = new QLabel("Victoria 2 directory path");
	directory_subtitle_label->setObjectName("subtitle");
	directory_path_label = new QLabel("Set game's directory path 123123123123");
	directory_path_label->setObjectName("path");
	directory_path_label->setAlignment(Qt::AlignCenter);
	directory_path_label->setGraphicsEffect(getShadow());

	browse_button = new QPushButton("Browse");
	browse_button->setFixedSize(200, 50);
	browse_button->setObjectName("button");
	browse_button->setGraphicsEffect(getShadow());

	directory_setting_layout->addWidget(directory_subtitle_label, 1, Qt::AlignHCenter);
	directory_setting_layout->addWidget(directory_path_label, 1);
	directory_setting_layout->addWidget(browse_button, 1, Qt::AlignHCenter);

	auto* mod_setting_container = new QWidget;
	//mod_setting_container->setFixedSize(200, 200);
	//mod_setting_container->setStyleSheet("background: none; background-color: blue; ");
	auto* mod_setting_layout = new QVBoxLayout(mod_setting_container);

	auto* mod_subtitle_label = new QLabel("Mod");
	mod_subtitle_label->setObjectName("subtitle");

	auto* mod_combobox = new QComboBox();

	mod_combobox->addItem("None");
	mod_combobox->setGraphicsEffect(getShadow());
	mod_combobox->setEnabled(true);

	mod_setting_layout->addWidget(mod_subtitle_label, 1, Qt::AlignHCenter);
	mod_setting_layout->addWidget(mod_combobox, 1, Qt::AlignHCenter);
	mod_setting_layout->addStretch(1);


	settings_panel_layout->addWidget(directory_setting_container, 1, Qt::AlignHCenter | Qt::AlignTop);
	settings_panel_layout->addStretch(1);
	settings_panel_layout->addWidget(mod_setting_container, 1, Qt::AlignHCenter | Qt::AlignTop);
	settings_panel_layout->setContentsMargins(30, 0, 30, 0);

	auto* start_button = new QPushButton("Start");
	start_button->setObjectName("button");
	start_button->setFixedSize(150, 60);

	

	start_button->setGraphicsEffect(getShadow());
	auto* hint_label = new QLabel("Starting the program might take awhile");
	hint_label->setObjectName("hint");


	content_layout->setContentsMargins(20, 20, 20, 20);
	content_layout->addWidget(title, 0, Qt::AlignHCenter | Qt::AlignTop);
	content_layout->addWidget(settings_panel, 1);
	content_layout->addStretch(1);
	content_layout->addWidget(start_button, 0, Qt::AlignHCenter | Qt::AlignBottom);
	content_layout->addWidget(hint_label, 0, Qt::AlignHCenter | Qt::AlignBottom);



	widget->setObjectName("background");
	widget->setStyleSheet(R"(
		#background 
		{ 
			background-image: url(images/setup_widget_background_normal.png);
			background-position: center;
			background-repeat: no-repeat;
		}

		#title
		{
			background: none;
			color: #E9E2CD;
			font-size: 28px;
		}

		#subtitle
		{
			background: none;
			color: #E9E2CD;
			font-size: 22px;
			
		}
				
		
		
		QComboBox {
		    border-image: url(images/label_background.png) 9 13 9 10 stretch;
		    border-width: 10px;
		    color: #E9E2CD;
		    font-size: 18px;
		    padding: 2px 30px 2px 7px;
		    background: transparent;
		}
		
		QComboBox::drop-down {
		    subcontrol-origin: margin;
		    subcontrol-position: top right;
		    width: 20px;
		    border-left: none;
			margin-right: 15px;
		}
		
		QComboBox::down-arrow {
		    image: url(images/more_button.png);
		    width: 26px;
		    height: 26px;
		}
		
		QComboBox QAbstractItemView
		{
			background: #2b1d16;
			border-image: url(images/label_background.png) 10 13 10 10 stretch;
			border-width: 10px;
			color: #E9E2CD;            /* Бежевий текст */
			selection-background-color: #4e3427; /* Колір виділення (при наведенні мишкою) */
			selection-color: #ffffff;            /* Колір тексту при наведенні */
			outline: none;             /* Прибираємо пунктирну рамку фокусу */
			border-style: solid;
			padding: 0px;
			margin: -5px;
		}
		
		QComboBox QAbstractItemView::item
		{
			min-height: 25px; /* Щоб пункти не були занадто зліплені */
			padding-left: 10px;
		}

		
		#path
		{
			border-image: url(images/label_background.png) 10 10 10 10 stretch;
			border-width: 10px;
			color: #E9E2CD;
			font-size: 18px;
			padding: 3px 7px;
		}
		
		
		#button
		{
			border-image: url(images/button_background.png) 2 2 2 2 stretch;
			color: #E9E2CD;
			font-size: 24px;
			padding: 3px 5px;
			
		}
		
		#hint
		{
			background: none;
			color: #E9E2CD;
			font-size: 16px;
			
		}
		
		)");

	setFixedSize(800,500);

	connect(browse_button, &QPushButton::pressed, this, [=]()
		{
			QString filepath = QFileDialog::getExistingDirectory();

			if (!isValidPath(filepath))
				return;

			directory_path_label->setText(filepath);

		});

	connect(start_button, &QPushButton::pressed, this, [=]()
		{
			emit startProgram(directory_path_label->text());
		});
}

ScenarioSetupWidget::~ScenarioSetupWidget()
{}

bool ScenarioSetupWidget::isValidPath(const QString& directory_path)
{
	QDir directory(directory_path);

	QFile definition(directory.filePath("map/definition.csv"));

	if (!definition.exists())
		return false;

	QFile countries(directory.filePath("common/countries.txt"));

	if (!countries.exists())
		return false;

	QFileInfo provincesInfo(directory.filePath("history/provinces"));

	if (!provincesInfo.exists() || !provincesInfo.isDir())
		return false;

	return true;
}

QGraphicsDropShadowEffect* ScenarioSetupWidget::getShadow()
{
	auto* shadow = new QGraphicsDropShadowEffect;

	shadow->setBlurRadius(10);
	shadow->setOffset(-2, 4);
	shadow->setColor(QColor(0, 0, 0, 160));

	return shadow;
}

