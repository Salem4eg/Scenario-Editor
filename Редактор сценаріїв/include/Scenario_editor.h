#pragma once

#include <QtWidgets/QMainWindow>
#include "ScenarioEditorWidget.h"
#include "ScenarioSetupWidget.h"

class Scenario_editor : public QMainWindow
{
    Q_OBJECT

public:
    Scenario_editor(QWidget *parent = nullptr);
    ~Scenario_editor();

    void startProgram(QString gameDirectory, QString saveFile);

protected:
    
private:
    bool isStarted = false;
    ScenarioSetupWidget* setup;

};

