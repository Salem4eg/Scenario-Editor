#include "SidePanel.h"
#include <QCompleter>
#include <QVBoxLayout>
#include <QAbstractItemView>

SidePanel::SidePanel(QWidget *parent)
	: QWidget(parent)
{
    setFixedSize(200, 350);
    setStyleSheet("background: rgba(30,30,30,100);");


    choosing_mode = new QPushButton("Enable choosing provinces");
    chosen_country = new QLabel("Chosen country: ");
    change_owner = new QPushButton("Change province owner");
    add_core = new QPushButton("Add core");
    remove_core = new QPushButton("Remove core");
    clear_chosen_provinces = new QPushButton("Clear chosen provinces");


    countries_list = new QComboBox(this);
    countries_list->setMaxVisibleItems(15);
    countries_list->setEditable(true);
    countries_list->lineEdit()->setPlaceholderText("Name a country");
    countries_list->lineEdit()->setAlignment(Qt::AlignCenter);
    fill_countries_list();

	int width = 185;

    choosing_mode->setFixedSize(width, 40);
    chosen_country->setFixedSize(width, 40);
    change_owner->setFixedSize(width, 40);
    add_core->setFixedSize(width, 40);
    remove_core->setFixedSize(width, 40);
    clear_chosen_provinces->setFixedSize(width, 40);
    chosen_country->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addStretch(1);
    layout->addWidget(chosen_country, 0, Qt::AlignTop);
    layout->addWidget(countries_list, 0, Qt::AlignTop);

    layout->addWidget(change_owner, 0, Qt::AlignBottom);
    layout->addWidget(add_core, 0, Qt::AlignBottom);
    layout->addWidget(remove_core, 0, Qt::AlignBottom);
    layout->addWidget(clear_chosen_provinces, 0, Qt::AlignBottom);
    layout->addWidget(choosing_mode, 0, Qt::AlignTop);

    showButtonsForChangingProvinces(enable_province_choosing);

    connect(countries_list, &QComboBox::activated, [=]()
        {
            chosen_tag = countries_list->currentData().toString();
            qDebug() << "Chosen tag: " << chosen_tag;

            emit setChosenTag(chosen_tag);
        });

    connect(choosing_mode, &QPushButton::pressed, [=]()
        {
            enable_province_choosing = !enable_province_choosing;

            if (enable_province_choosing)
                choosing_mode->setText("Disable province choosing");
            else
                choosing_mode->setText("Enable province choosing");

            showButtonsForChangingProvinces(enable_province_choosing);

            emit provinceChoosing(enable_province_choosing);
        });

    connect(change_owner, &QPushButton::pressed, [=]()
        {
            if (chosen_tag.isEmpty())
                return;
           
            emit changeProvinceOwner();
        });

    connect(add_core, &QPushButton::pressed, [=]()
        {
            if (chosen_tag.isEmpty())
                return;
            
            emit addCoreToProvinces();
        });

    connect(remove_core, &QPushButton::pressed, [=]()
        {
            if (chosen_tag.isEmpty())
                return;

            emit removeCoreFromProvinces();
        });

    connect(clear_chosen_provinces, &QPushButton::pressed, [=]()
        {
            emit clearChosenProvinces();
        });

    setStyleSheet(
        R"(
            QWidget 
            {
                font-size: 14px;
                color: #f5cc93;
            
            
            }
            
            QPushButton
            {
                border-image: url(../images/button.png) 2 2 2 4 stretch;
            
            }
            
            QPushButton:hover
            {
                border-image: url(../images/button_hovered.png) 2 2 2 2 stretch;
            
            
            }
            
            QLabel
            {
                border-image: url(../images/label.png) 2 2 2 2 stretch;
            }
            
            QComboBox 
		    {
		        border-image: url(../images/test_combobox.png) 9 13 9 10 stretch;
		        border-width: 10px;
		        font-size: 14px;
		        padding: 2px 2px 2px 2px;
		        background: transparent;
		    }

		    
		    QComboBox::drop-down 
		    {
		        subcontrol-origin: margin;
		        subcontrol-position: top right;
		        width: 20px;
		        border-left: none;
		    	margin-right: 10px;
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
                background: #6b4a37;
                min-height: 20px;
                border: 1px solid #9b7457;
            }
            
            QScrollBar::handle:vertical:hover
            {
                background: #8a6047;
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

SidePanel::~SidePanel()
{}

void SidePanel::fill_countries_list()
{
    auto countries = readCountriesFile();
    QStringList countries_and_tags;

    for (auto& [tag, country_and_tag] : countries)
    {
        countries_list->addItem(country_and_tag, tag);
        countries_and_tags.push_back(country_and_tag);
    }

    auto completer = new QCompleter(countries_and_tags, countries_list);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    countries_list->setCompleter(completer);

    countries_list->setCurrentIndex(-1);

    completer->popup()->setStyleSheet(
        R"(
            QAbstractItemView
            {
                background: #2b1d16;
                border-image: url(../images/combobox_list.png) 10 13 10 10 stretch;
                border-width: 10px;
                border-style: solid;
            
                selection-background-color: #4e3427;
                selection-color: white;
            
                outline: none;
                padding: 0px;
                margin: -5px;
                color: #f5cc93;
            }
            
            QAbstractItemView::item
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
                background: #6b4a37;
                min-height: 20px;
                border: 1px solid #9b7457;
            }
            
            QScrollBar::handle:vertical:hover
            {
                background: #8a6047;
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

QList<QPair<QString, QString>> SidePanel::readCountriesFile()
{
    QList<QPair<QString, QString>> countries;

    QString mod_filepath = "E:/Steam/steamapps/common/Victoria 2/mod/TGC";
    QString countries_filepath = mod_filepath + "/common/countries.txt";

    QFile countries_file(countries_filepath);

    if (!countries_file.open(QFile::ReadOnly | QFile::Text))
    {
        throw std::runtime_error("Couldn't open " + countries_filepath.toStdString());
    }

    QTextStream read(&countries_file);

    while (!read.atEnd())
    {
        QString line = read.readLine();

        int commentIndex = line.indexOf("#");

        if (commentIndex != -1)
            line = line.left(commentIndex);

        line = line.trimmed();

        if (line.isEmpty() || !line.endsWith(".txt\""))
            continue;

        auto splitted = line.split("=");

        if (splitted.size() != 2)
            continue;

        QString tag = splitted[0].trimmed();
        QString path = splitted[1].trimmed();

        auto splitted_path = path.split("/");

        QString country = splitted_path[1].trimmed().remove(".txt\"");
        QString country_tag = tag + " - " + country;

        countries.push_back({ tag, country_tag });
    }

    return countries;
}

void SidePanel::showButtonsForChangingProvinces(bool show)
{
    if (show)
    {
        countries_list->show();
        clear_chosen_provinces->show();
        change_owner->show();
        add_core->show();
        remove_core->show();
        chosen_country->show();

        setFixedSize(200, 350);
    }
    else
    {
        countries_list->hide();
        clear_chosen_provinces->hide();
        change_owner->hide();
        add_core->hide();
        remove_core->hide();
        chosen_country->hide();

        setFixedSize(200, 60);
    }
}
