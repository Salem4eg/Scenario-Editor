#include "ScenarioSetupWidget.h"
#include <QVBoxLayout>
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
	title->setGraphicsEffect(getShadow());

	auto* settings_panel = new QWidget;
	//settings_panel->setStyleSheet("background: none; background-color: gray; ");
	//settings_panel->setFixedSize(800, 200);
	auto* settings_panel_layout = new QHBoxLayout(settings_panel);

	auto* directory_setting_container = new QWidget;
	//directory_setting_container->setFixedSize(200, 200);
	//directory_setting_container->setStyleSheet("background: none; background-color: yellow; ");
	auto* directory_setting_layout = new QVBoxLayout(directory_setting_container);

	auto* directory_subtitle_label = new QLabel("Victoria 2 directory path");
	directory_subtitle_label->setGraphicsEffect(getShadow());
	directory_subtitle_label->setObjectName("subtitle");

	directory_path_label = new QLabel("Set game's directory path");
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
	mod_subtitle_label->setGraphicsEffect(getShadow());

	mod_combobox = new QComboBox();

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

	progress_bar = new ProgressBar;
	progress_bar->setGraphicsEffect(getShadow());
	
	auto* hint_label = new QLabel("Starting the program might take a while");
	hint_label->setObjectName("hint");
	hint_label->setGraphicsEffect(getShadow());

	content_layout->setContentsMargins(20, 20, 20, 20);
	content_layout->addWidget(title, 0, Qt::AlignHCenter | Qt::AlignTop);
	content_layout->addSpacing(35);
	content_layout->addWidget(settings_panel, 1);
	content_layout->addStretch(1);
	content_layout->addWidget(start_button, 0, Qt::AlignHCenter | Qt::AlignBottom);
	content_layout->addWidget(progress_bar, 0, Qt::AlignHCenter | Qt::AlignBottom);
	content_layout->addWidget(hint_label, 0, Qt::AlignHCenter | Qt::AlignBottom);


	//#cc9e5e
	//#edbd79
	widget->setObjectName("background");
	widget->setStyleSheet(R"(
		#background 
		{ 
			background-image: url(../images/setup.png);
			background-position: center;
			background-repeat: no-repeat;
		}
		QWidget
		{
			font-family: Georgia;
			color: #f5cc93;
		}
		
		#title
		{
			background: none;
			font-size: 28px;
			font-weight: bold;
		}

		#subtitle
		{
			background: none;
			font-size: 22px;
			font-weight: bold;
			
		}
				
		
		
		QComboBox 
		{
		    border-image: url(../images/label.png) 9 13 9 10 stretch;
		    border-width: 10px;
		    font-size: 18px;
		    padding: 2px 30px 2px 7px;
		    background: transparent;
		}

		
		QComboBox::drop-down 
		{
		    subcontrol-origin: margin;
		    subcontrol-position: top right;
		    width: 20px;
		    border-left: none;
			margin-right: 15px;
		}
		
		QComboBox::down-arrow 
		{
		    image: url(../images/arrow_down.png);
		    width: 26px;
		    height: 26px;
		}
		
		QComboBox QAbstractItemView
		{
			background: #2b1d16;
			border-image: url(../images/label.png) 10 13 10 10 stretch;
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

		
		#path
		{
			border-image: url(../images/label.png) 10 10 10 10 stretch;
			border-width: 10px;
			font-size: 18px;
			padding: 3px 7px;
		}
		
		
		#button
		{
			border-image: url(../images/button.png) 2 2 2 2 stretch;
			font-size: 24px;
			padding: 3px 5px;
			
			font-weight: bold;
			
		}

		#button:hover
		{
			border-image: url(../images/button_hovered.png) 2 2 2 2 stretch;
		
		}
		
		
		#button:pressed
		{
		    padding-left: 6px;
		    padding-top: 6px;
		}
		
		#hint
		{
			background: none;
			font-size: 14px;
			font-style: italic;
		}
		
		)");

	setFixedSize(800,500);

	connect(browse_button, &QPushButton::pressed, this, [=]()
		{
			QString filepath = QFileDialog::getExistingDirectory();
			hasValidPath = false;
			clearMods();

			if (!isValidPath(filepath))
			{
				directory_path_label->setText("Invalid path to game's directory");
				return;
			}

			setGamePath(filepath);

		});

	connect(start_button, &QPushButton::pressed, this, [=]()
		{
			if (hasValidPath)
			{
				QString path = directory_path_label->text();

				if (chosen_mod != "None" && !chosen_mod.isEmpty())
					path += "/mod/" + chosen_mod;

				emit startProgram(path);
			}
		});

	connect(mod_combobox, &QComboBox::currentTextChanged, this, [=](const QString& mod)
		{
			chosen_mod = mod;
		});

	loadSettings();
}

ScenarioSetupWidget::~ScenarioSetupWidget()
{
	if (hasValidPath)
		saveSettings();
}

void ScenarioSetupWidget::addProgress(int value)
{
	progress_bar->addProgress(value);
}

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

void ScenarioSetupWidget::getMods()
{
	QString game_path = directory_path_label->text();
	QString mod_directory_path = game_path + "/mod";

	QDir mod_directory(mod_directory_path);

	auto mods = mod_directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

	addMod("None");
	for (const auto& mod : mods)
	{
		QString mod_path = mod_directory_path + "/" + mod;
		if (isValidPath(mod_path))
			addMod(mod);
	}

}

void ScenarioSetupWidget::setGamePath(const QString& directory_path)
{
	hasValidPath = true;
	directory_path_label->setText(directory_path);
	getMods();
}

void ScenarioSetupWidget::addMod(const QString& mod_name)
{
	mod_combobox->addItem(mod_name);
}

void ScenarioSetupWidget::clearMods()
{
	chosen_mod = "None";
	mod_combobox->clear();
}

void ScenarioSetupWidget::saveSettings()
{
	QJsonObject settings;

	settings["game_path"] = directory_path_label->text();
	settings["chosen_mod"] = chosen_mod;

	QJsonDocument doc(settings);

	QFile file("settings.json");

	if (file.open(QIODevice::WriteOnly))
	{
		file.write(doc.toJson(QJsonDocument::Indented));
		file.close();
	}
}

void ScenarioSetupWidget::loadSettings()
{
	QFile file("settings.json");

	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QByteArray data = file.readAll();

	QJsonDocument doc = QJsonDocument::fromJson(data);

	QJsonObject settings = doc.object();

	if (const QJsonValue path_value = settings["game_path"]; path_value.isString())
	{
		setGamePath(path_value.toString());

		if (const QJsonValue mod_value = settings["chosen_mod"]; mod_value.isString())
		{
			chosen_mod = mod_value.toString();

			int index = mod_combobox->findText(chosen_mod);
			if (index != -1)
				mod_combobox->setCurrentIndex(index);
		}
	}
	
}

