#include "MapDataManager.h"

MapDataManager::MapDataManager(QString directory_path, QString save_file_path, QObject* parent)
	: QObject(parent), m_directory_path(directory_path), m_save_file_path(save_file_path)
{
	image_processor = new MapImageProcessor(directory_path, this);
}

MapDataManager::~MapDataManager()
{}

void MapDataManager::prepare()
{
	ParadoxParser parser(m_directory_path, m_save_file_path);
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

	parser.loadCountriesProvinces(game_data);
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

	if (!choosable_provinces.contains(province))
		return -1;

	return province;
}

QList<QList<QPoint>> MapDataManager::pixelsOfProvinces(QList<int> provinces)
{
	QList<QList<QPoint>> result;

	for (int province : provinces)
		result.push_back(pixelsOfProvince(province));

	return result;
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