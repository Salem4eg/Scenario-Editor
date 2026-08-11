#pragma once

#include <QObject>
#include "Structures.h"

class ProvinceManager  : public QObject
{
	Q_OBJECT

public:
	ProvinceManager(QObject* parent) : QObject(parent)
	{}
	virtual ~ProvinceManager() = default;

public slots:
	// Changes owner and controller of provinces
	virtual void changeProvincesOwner(QList<int> provinces, QString country_tag) = 0;
	// Add core to provinces if they don't have it already
	virtual void addCoreToProvinces(QList<int> provinces, QString country_tag) = 0;
	// Removes core from provinces if they have it
	virtual void removeCoreFromProvinces(QList<int> provinces, QString country_tag) = 0;

	virtual Province getProvinceInfo(int provinceID) = 0;
	virtual void loadProvinces(ParadoxGameData& game_data) = 0;

	virtual void setTypes(const QStringList& popTypes) = 0;

	virtual void setProvincePopData(int provinceID, const QList<PopData>& population) = 0;
};

