#include "SidePanel.h"
#include <QCompleter>
#include <QVBoxLayout>

SidePanel::SidePanel(QWidget *parent)
	: QWidget(parent)
{
    setFixedSize(200, 300);
    setStyleSheet("background: rgba(30,30,30,100);");


    choosing_mode = new QPushButton("Enable choosing provinces");
    chosen_country = new QLabel("Choosen country: ");
    change_owner = new QPushButton("Change province owner");
    add_core = new QPushButton("Add core");
    remove_core = new QPushButton("Remove core");
    clear_chosen_provinces = new QPushButton("Clear chosen provinces");


    countries_list = new QComboBox(this);
    countries_list->setMaxVisibleItems(15);
    countries_list->setEditable(true);
    countries_list->lineEdit()->setPlaceholderText("Finding country..");
    countries_list->lineEdit()->setAlignment(Qt::AlignCenter);
    fill_countries_list();

    choosing_mode->setFixedHeight(40);
    chosen_country->setFixedHeight(40);
    change_owner->setFixedHeight(40);
    add_core->setFixedHeight(40);
    remove_core->setFixedHeight(40);
    clear_chosen_provinces->setFixedHeight(40);
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
    }
    else
    {
        countries_list->hide();
        clear_chosen_provinces->hide();
        change_owner->hide();
        add_core->hide();
        remove_core->hide();
        chosen_country->hide();
    }
}
