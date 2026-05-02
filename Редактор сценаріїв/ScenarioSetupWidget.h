#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsEffect>
#include <QComboBox>

class ScenarioSetupWidget : public QWidget
{
	Q_OBJECT

public:
	ScenarioSetupWidget(QWidget *parent = nullptr);
	~ScenarioSetupWidget();


signals:
	void startProgram(QString directory_path);

private:
	QPushButton* browse_button;
	QLabel* directory_path_label;
	QComboBox* mod_combobox;
	bool hasValidPath = false;
	QString chosen_mod = "None";

	bool isValidPath(const QString& directory_path);
	QGraphicsDropShadowEffect* getShadow();
	void getMods();


	void addMod(const QString& mod_name);
	void clearMods();

};

