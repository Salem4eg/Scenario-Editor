#include "ProvinceController.h"
#include <QDirIterator>

ProvinceController::ProvinceController(QString provinces_directory, QString save_file_path, QObject* parent)
	: QObject(parent), m_base_manager(provinces_directory, this), m_save_manager(save_file_path, provinces_directory, this)
{
	m_use_base_manager = save_file_path.isEmpty();
}

ProvinceController::~ProvinceController()
{

}

void ProvinceController::changeProvincesOwner(QList<int> provinces, QString country_tag)
{
	if (m_use_base_manager)
		m_base_manager.changeProvincesOwner(provinces, country_tag);
	else
		m_save_manager.changeProvincesOwner(provinces, country_tag);
}

void ProvinceController::addCoreToProvinces(QList<int> provinces, QString country_tag)
{
	if (m_use_base_manager)
		m_base_manager.addCoreToProvinces(provinces, country_tag);
	else
		m_save_manager.addCoreToProvinces(provinces, country_tag);
}

void ProvinceController::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{
	if (m_use_base_manager)
		m_base_manager.removeCoreFromProvinces(provinces, country_tag);
	else
		m_save_manager.removeCoreFromProvinces(provinces, country_tag);
}

ProvinceInfo ProvinceController::getProvinceInfo(int province)
{
	if (m_use_base_manager)
		return m_base_manager.getProvinceInfo(province);
	else
		return m_save_manager.getProvinceInfo(province);
}

void ProvinceController::loadCountriesProvinces(ParadoxGameData& game_data)
{
	if (m_use_base_manager)
		m_base_manager.loadProvincesFromBaseGame(game_data);
	else
		m_save_manager.loadProvincesFromSavefile(game_data);
}

