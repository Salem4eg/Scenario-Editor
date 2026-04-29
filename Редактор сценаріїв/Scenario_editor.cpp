#include "Scenario_editor.h"

Scenario_editor::Scenario_editor(QWidget* parent)
{
	auto setup = new ScenarioSetupWidget(this);
	setCentralWidget(setup);

	connect(setup, &ScenarioSetupWidget::startProgram, this, &Scenario_editor::startProgram);

	//startProgram("E:/Steam/steamapps/common/Victoria 2");
	//startProgram("E:/Steam/steamapps/common/Victoria 2/mod/TGC");
}

Scenario_editor::~Scenario_editor()
{

}

void Scenario_editor::startProgram(QString directory_path)
{
	if (isStarted)
		return;

	isStarted = true;
	auto editor = new ScenarioEditorWidget(directory_path, this);

	setCentralWidget(editor);
	resize(editor->size());

	//connect(editor, &ScenarioEditorWidget::isReadyToShow, this, [=]()
	//	{
	//		
	//	});

	//editor->prepare();
	
}
