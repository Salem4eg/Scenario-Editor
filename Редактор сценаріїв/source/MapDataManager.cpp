#include "MapDataManager.h"

MapDataManager::MapDataManager(QString directory_path, QString save_file_path, QObject* parent)
	: QObject(parent), m_directory_path(directory_path), m_save_file_path(save_file_path)
{

	image_processor = new MapImageProcessor(directory_path, this);
	province_manager = new ProvinceController(directory_path, save_file_path, this);


	connect(&highlight_timer, &QTimer::timeout, this, &MapDataManager::highlightChosenProvinces);
}

MapDataManager::~MapDataManager()
{
	qDebug() << "~MapDataManager()";
}

void MapDataManager::prepare()
{
	ParadoxParser parser(m_directory_path);
	ParadoxGameData game_data;

	qDebug() << "loadProvincesDefinition()";
	startDebugTimer();

	parser.loadProvincesDefinition(game_data);
	QtConcurrent::run([this]()
	{
		emit progressMade(5);
	});

	endDebugTimer();

	qDebug() << "mapPixelsToProvinces()";
	startDebugTimer();

	image_processor->setColorToProvince(std::move(game_data.color_to_province));
	image_processor->mapPixelsToProvinces();
	QtConcurrent::run([this]()
	{
		emit progressMade(35);
	});
	endDebugTimer();



	qDebug() << "loadColorToCountries()";
	startDebugTimer();

	parser.loadCountriesColors(game_data);
	QtConcurrent::run([this]()
	{
		emit progressMade(5);
	});
	endDebugTimer();



	qDebug() << "loadCountriesProvinces()";
	startDebugTimer();

	province_manager->loadCountriesProvinces(game_data);
	QtConcurrent::run([this]()
	{
		emit progressMade(5);
	});
	endDebugTimer();


	choosable_provinces = std::move(game_data.choosable_provinces);

	image_processor->setCountriesProvinces(std::move(game_data.countries_provinces));
	image_processor->setCountriesColor(std::move(game_data.countries_color));
}

int MapDataManager::provinceAt(int x, int y)
{
	int province = image_processor->provinceAt(x, y);

	qDebug() << "Province clicked: " << province;

	if (!choosable_provinces.contains(province))
		return -1;

	qDebug() << province << " is choosable";

	return province;
}

QList<QPoint> MapDataManager::pixelsOfProvince(int provinceID)
{
	return image_processor->pixelsOfProvince(provinceID);
}

QList<QList<QPoint>> MapDataManager::getProvincesPixels(QList<int> provinces)
{
	QList<QList<QPoint>> result;

	for (int province : provinces)
		result.push_back(pixelsOfProvince(province));

	return result;
}

QRgb MapDataManager::GetCountryColor(QString countryTag)
{
	return image_processor->colorOfCountry(countryTag);
}

void MapDataManager::addChosenProvince(int provinceID)
{
	emit addProvinceToHighlight(pixelsOfProvince(provinceID), provinceID);

	chosen_provinces.push_back(provinceID);
}

void MapDataManager::removeChosenProvince(int provinceID)
{
	emit removeProvinceFromHighlight(pixelsOfProvince(provinceID), provinceID);

	chosen_provinces.removeOne(provinceID);
}

const QList<int>& MapDataManager::getChosenProvinces() const
{
	return chosen_provinces;
}

void MapDataManager::clearChosenProvinces()
{
	for (int province : chosen_provinces)
	{
		emit removeProvinceFromHighlight(pixelsOfProvince(province), province);
	}

	chosen_provinces.clear();
}

QImage MapDataManager::getBordersViewMap()
{
	return image_processor->GetBordersViewMap();
}

QImage MapDataManager::getCountriesViewMap()
{
	return image_processor->GetCountriesViewMap();
}

void MapDataManager::changeProvincesOwner(QList<int> provinces, QString country_tag)
{
	province_manager->changeProvincesOwner(provinces, country_tag);
}

void MapDataManager::addCoreToProvinces(QList<int> provinces, QString country_tag)
{
	province_manager->addCoreToProvinces(provinces, country_tag);
}

void MapDataManager::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{
	province_manager->removeCoreFromProvinces(provinces, country_tag);
}

Province MapDataManager::getProvinceInfo(int province)
{
	return province_manager->getProvinceInfo(province);
}

void MapDataManager::startDebugTimer()
{
	timer.start();
}

void MapDataManager::endDebugTimer()
{
	auto time_elapsed = timer.elapsed();
	int time_elapsed_sec = time_elapsed / 1000;
	int ms_remaining = time_elapsed % 1000;

	qDebug() << "Time: " << time_elapsed_sec << "s " << ms_remaining << "ms";
}

void MapDataManager::highlightChosenProvinces()
{
	if (!provinces_highlighted)
	{
		emit highlightProvinces(true);
	}
	else
	{
		emit highlightProvinces(false);
	}

	provinces_highlighted = !provinces_highlighted;
}

void MapDataManager::handleClickAtProvince(int x, int y)
{
	int province = provinceAt(x, y);

	if (province == -1)
		return;

	if (enable_province_choosing == false)
	{
		emit getChosenProvinceInfo(province);
		return;
	}

	if (!getChosenProvinces().contains(province))
		addChosenProvince(province);
	else
		removeChosenProvince(province);

	qDebug() << "Province clicked: " << province;

	provinces_highlighted = false;
	highlightChosenProvinces();

	if (getChosenProvinces().isEmpty())
		highlight_timer.stop();
	else
		highlight_timer.start(1000);
}

void MapDataManager::setProvincePopData(int provinceID, const QList<PopData>& population)
{
	province_manager->setProvincePopData(provinceID, population);
}



void MapDataManager::setProvinceChoosingMode(bool choose)
{
	enable_province_choosing = choose;
}

void MapDataManager::showHighlighting(bool show)
{
	if (show)
	{
		if (!getChosenProvinces().isEmpty())
		{
			highlight_timer.start(1000);
		}
	}
	else
		highlight_timer.stop();
}