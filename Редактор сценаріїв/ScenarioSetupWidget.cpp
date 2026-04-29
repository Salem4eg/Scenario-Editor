#include "ScenarioSetupWidget.h"
#include <QVBoxLayout>

ScenarioSetupWidget::ScenarioSetupWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(500, 320);

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(30, 30, 30, 30);

    root->addStretch();

    auto* card = new QFrame;
    card->setObjectName("card");

    auto* layout = new QVBoxLayout(card);
    layout->setSpacing(15);
    layout->setContentsMargins(25, 25, 25, 25);

    auto* title = new QLabel("Victoria 2 Scenario Editor");
    title->setObjectName("title");

    auto* subtitle = new QLabel("Choose game or mod directory");
    subtitle->setObjectName("subtitle");

    path_label = new QLabel("Not selected");
    path_label->setObjectName("pathLabel");
    path_label->setWordWrap(true);
    path_label->setAlignment(Qt::AlignHCenter);

    browse_button = new QPushButton("Browse");
    auto* start_button = new QPushButton("Start Editor");

    browse_button->setMinimumHeight(36);
    start_button->setMinimumHeight(36);

    layout->addWidget(title, 0, Qt::AlignHCenter);
    layout->addWidget(subtitle, 0, Qt::AlignHCenter);
    layout->addWidget(path_label);
    layout->addSpacing(10);
    layout->addWidget(browse_button, 0, Qt::AlignHCenter);
    layout->addWidget(start_button, 0, Qt::AlignHCenter);

    root->addWidget(card);
    root->addStretch();

    setStyleSheet(R"(
        QWidget {
            background-color: #1e1e1e;
            color: white;
            font-size: 14px;
        }

        #card {
            background-color: #2d2d2d;
            border-radius: 14px;
            border: 1px solid #3a3a3a;
        }

        #title {
            background-color: #2d2d2d;
            font-size: 22px;
            font-weight: bold;
        }

        #subtitle {
            background-color: #2d2d2d;
            font-size: 14px;
        }

        #pathLabel {
            background-color: #202020;
            border: 1px solid #444;
            border-radius: 8px;
            padding: 10px;
        }

        QPushButton {
            background-color: #3b82f6;
            border: none;
            border-radius: 8px;
            padding: 8px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #2563eb;
        }

        QPushButton:pressed {
            background-color: #1d4ed8;
        }
    )");

    connect(browse_button, &QPushButton::clicked, this, [this]()
        {
            QString path = QFileDialog::getExistingDirectory(
                this,
                "Choose directory",
                QDir::homePath()
            );

            if (!path.isEmpty() && isValidPath(path))
            {
                path_label->setText(path);
                hasValidPath = true;
            }
            else
            {
                path_label->setText("Invalid directory");
                hasValidPath = false;
            }
        });

    connect(start_button, &QPushButton::clicked, this, [this]()
        {
            if (hasValidPath)
                emit startProgram(path_label->text());
            else
                path_label->setText("Choose valid directory first");
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

