#pragma once

#include "ProvinceBasePopulationDialog.h"

class ProvinceSavePopulationDialog : public ProvinceBasePopulationDialog
{
	Q_OBJECT;
public:
	ProvinceSavePopulationDialog(QWidget* parent = nullptr);


	void setPopData(const QList<PopData>& pops) override;
	// For initializing all available ideologies for PopIdeologyWidget
	void setIdeologies(const QList<DemographicCategory>& ideologies);

private:
};