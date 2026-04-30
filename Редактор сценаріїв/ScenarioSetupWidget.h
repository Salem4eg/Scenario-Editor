#pragma once

#include <QWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsEffect>

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
	bool hasValidPath = false;

	bool isValidPath(const QString& directory_path);
	QGraphicsDropShadowEffect* getShadow();
};

