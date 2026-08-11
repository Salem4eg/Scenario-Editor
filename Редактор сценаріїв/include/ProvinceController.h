#pragma once
#include <QObject>
#include <QFile>
#include <QList>
#include <QHash>
#include "Structures.h"
#include "FileReader.h"
#include "ProvinceBaseManager.h"
#include "ProvinceSaveManager.h"
#include "ProvinceManager.h"

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

	Province getProvinceInfo(int province);

	void loadCountriesProvinces(ParadoxGameData& game_data);

	void setProvincePopData(int provinceID, const QList<PopData>& population);

private:
	ProvinceManager* manager;
};

