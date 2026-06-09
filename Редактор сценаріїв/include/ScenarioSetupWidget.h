#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsEffect>
#include <QComboBox>
#include "ProgressBar.h"
#include <QJsonObject>

class ScenarioSetupWidget : public QWidget
{
	Q_OBJECT

public:
	ScenarioSetupWidget(QWidget *parent = nullptr);
	~ScenarioSetupWidget();

	void addProgress(int value);


signals:
	void startProgram(QString gameDirectory, QString saveFile);

private:
	QPushButton* directory_browse_button;
	QPushButton* savefile_browse_button;
	QString savefile;
	QLabel* directory_path_label;
	QLabel* savefile_path_label;
	QComboBox* mod_combobox;
	bool hasValidPath = false;
	QString chosen_mod = "None";
	ProgressBar* progress_bar;

	bool isValidPath(const QString& directory_path);
	QGraphicsDropShadowEffect* getShadow();
	void getMods();

	void setGamePath(const QString& directory_path);
	void addMod(const QString& mod_name);
	void clearMods();
	void saveSettings();
	void loadSettings();

private:
	QString getFilename(const QString& path);
	bool isSavefile(const QString& path);
};

