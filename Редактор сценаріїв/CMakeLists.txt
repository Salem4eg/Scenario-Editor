cmake_minimum_required(VERSION 3.20)

project(ScenarioEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

set(HEADERS
	include/FileReader.h
	include/MapView.h
	include/MapImageProcessor.h
	include/MapDataManager.h
	include/ParadoxParser.h
	include/ProgressBar.h
	include/ProvinceController.h
	include/ProvinceBaseManager.h
	include/ProvinceSaveManager.h
	include/ProvinceInfoToolTip.h
	include/Scenario_editor.h
	include/ScenarioEditorWidget.h
	include/ScenarioSetupWidget.h
	include/SidePanel.h
	include/Structures.h
)

set(SOURCES
	source/main.cpp
	source/FileReader.cpp
	source/MapView.cpp
	source/MapDataManager.cpp
	source/MapImageProcessor.cpp
	source/ParadoxParser.cpp
	source/ProgressBar.cpp
	source/ProvinceController.cpp
	source/ProvinceBaseManager.cpp
	source/ProvinceSaveManager.cpp
	source/ProvinceInfoToolTip.cpp
	source/Scenario_editor.cpp
	source/ScenarioEditorWidget.cpp
	source/ScenarioSetupWidget.cpp
	source/SidePanel.cpp
)

source_group("Source files" FILES ${SOURCES})
source_group("Header files" FILES ${HEADERS})



qt_add_executable(ScenarioEditor
	${SOURCES}
	${HEADERS}
)

add_custom_command(TARGET ScenarioEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/images
    $<TARGET_FILE_DIR:ScenarioEditor>/images
)

target_include_directories(ScenarioEditor PRIVATE include)
target_link_libraries(ScenarioEditor PRIVATE Qt6::Widgets)