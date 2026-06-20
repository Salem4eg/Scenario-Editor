#include "ProvinceController.h"
#include <QDirIterator>

ProvinceController::ProvinceController(QString game_directory, QString save_file_path, QObject* parent)
	: QObject(parent)
{
	if (save_file_path.isEmpty())
		manager = new ProvinceBaseManager(game_directory, this);
	else
		manager = new ProvinceSaveManager(save_file_path, game_directory, this);
}

ProvinceController::~ProvinceController()
{

}

void ProvinceController::changeProvincesOwner(QList<int> provinces, QString country_tag)
{
	manager->changeProvincesOwner(provinces, country_tag);
}

void ProvinceController::addCoreToProvinces(QList<int> provinces, QString country_tag)
{
	manager->addCoreToProvinces(provinces, country_tag);
}

void ProvinceController::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{
	manager->removeCoreFromProvinces(provinces, country_tag);
}

ProvinceInfo ProvinceController::getProvinceInfo(int province)
{
	return manager->getProvinceInfo(province);
}

void ProvinceController::loadCountriesProvinces(ParadoxGameData& game_data)
{
	manager->loadProvinces(game_data);
}

