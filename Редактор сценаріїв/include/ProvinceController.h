#pragma once
#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include "Structures.h"
#include "FileReader.h"
#include "ProvinceBaseManager.h"
#include "ProvinceSaveManager.h"

class ProvinceController : public QObject
{
	Q_OBJECT
public:
	ProvinceController(QString provinces_directory, QString save_directory, QObject *parent = nullptr);
	~ProvinceController();
public slots:
	// Changes owner and controller of provinces
	void changeProvincesOwner(QList<int> provinces, QString country_tag);
	// Add core to provinces if they don't have it already
	void addCoreToProvinces(QList<int> provinces, QString country_tag);
	// Removes core from provinces if they have it
	void removeCoreFromProvinces(QList<int> provinces, QString country_tag);

	ProvinceInfo getProvinceInfo(int province);

	void loadCountriesProvinces(ParadoxGameData& game_data);

private:
	ProvinceBaseManager m_base_manager;
	ProvinceSaveManager m_save_manager;

	bool m_use_base_manager;
};

