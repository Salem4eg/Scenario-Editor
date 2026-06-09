#include "MapImageProcessor.h"

MapImageProcessor::MapImageProcessor(QString directory_path, QObject *parent)
	: QObject(parent)
{
	QDir dir(directory_path);

	QString province_map_path = dir.filePath("map/provinces.bmp");
	province_map = QImage(province_map_path);

	// The map image comes flipped, so flipping needed
	province_map.flip(Qt::Vertical);
}

MapImageProcessor::~MapImageProcessor()
{}

void MapImageProcessor::mapPixelsToProvinces()
{
	int thread_count = QThread::idealThreadCount();

	if (thread_count == 0)
	{
		thread_count = 1;
	}

	int pixels = province_map.width() * province_map.height();

	int pixels_per_thread = pixels / thread_count;

	QList<QFuture<void>> futures;

	for (int i = 0; i < thread_count; i++)
	{
		int start_index = pixels_per_thread * i;

		int count = pixels_per_thread;

		if (i == thread_count - 1)
			count = pixels - start_index;

		QFuture<void> future = QtConcurrent::run([=]()
		{
			mapPartOfPixelsToProvinces(start_index, count);
		});

		futures.push_back(future);
	}

	for (auto& future : futures)
		future.waitForFinished();
}

void MapImageProcessor::mapPartOfPixelsToProvinces(int start_index, int indexes_to_process)
{
	int end_index = start_index + indexes_to_process;

	QHash<int, QList<QPoint>> local_province_pixels;

	for (int pixel_index = start_index; pixel_index < end_index; pixel_index++)
	{
		int x = pixel_index % province_map.width();
		int y = pixel_index / province_map.width();

		QRgb pixel_color = province_map.pixel(x, y);

		int provinceID = color_to_province.value(pixel_color, -1);

		if (provinceID == -1)
			continue;

		local_province_pixels[provinceID].push_back({ x, y });
	}

	QMutexLocker locker(&mutex);

	for (auto [key, value] : local_province_pixels.asKeyValueRange())
	{
		province_pixels[key].append(value);
	}
}


QImage MapImageProcessor::GetBordersViewMap()
{
	QImage outline_map = QImage(province_map.size(), QImage::Format_ARGB32);
	outline_map.fill(Qt::transparent);

	int thread_count = QThread::idealThreadCount();

	if (thread_count == 0)
	{
		thread_count = 1;
	}

	int pixels = outline_map.width() * outline_map.height();

	int pixels_per_thread = pixels / thread_count;

	QList<QFuture<void>> futures;

	for (int i = 0; i < thread_count; i++)
	{
		int start_index = pixels_per_thread * i;

		int count = pixels_per_thread;

		if (i == thread_count - 1)
			count = pixels - start_index;

		QFuture<void> future = QtConcurrent::run([&, start_index, count]()
		{
			makeOutlineForPartOfMap(outline_map, start_index, count);
		});
		futures.push_back(future);
	}

	for (auto& future : futures)
		future.waitForFinished();

	return outline_map;
}

void MapImageProcessor::makeOutlineForPartOfMap(QImage& outline_map, int start_index, int indexes_to_process)
{
	int end_index = start_index + indexes_to_process;

	QList<QPoint> outline_pixels;

	for (int pixel_index = start_index; pixel_index < end_index; pixel_index++)
	{
		int x = pixel_index % province_map.width();
		int y = pixel_index / province_map.width();

		if (hasAnotherProvinceNear(x, y))
			outline_pixels.push_back({ x, y });
	}

	QMutexLocker lock(&mutex);

	for (auto& coords : outline_pixels)
	{
		outline_map.setPixel(coords, qRgb(0, 0, 0));
	}
}

bool MapImageProcessor::hasAnotherProvinceNear(int x, int y)
{
	QRgb pixel = province_map.pixel(x, y);

	if (x - 1 >= 0)
	{
		QRgb left_pixel = province_map.pixel(x - 1, y);
		if (left_pixel != pixel)
		{
			return true;
		}
	}
	if (x + 1 < province_map.width())
	{
		QRgb right_pixel = province_map.pixel(x + 1, y);
		if (right_pixel != pixel)
		{
			return true;
		}
	}
	if (y - 1 >= 0)
	{
		QRgb upper_pixel = province_map.pixel(x, y - 1);
		if (upper_pixel != pixel)
		{
			return true;
		}
	}
	if (y + 1 < province_map.height())
	{
		QRgb bottom_pixel = province_map.pixel(x, y + 1);
		if (bottom_pixel != pixel)
		{
			return true;
		}
	}
	return false;
}

QImage MapImageProcessor::GetCountriesViewMap()
{
	QImage countries_view_map = QImage(province_map.size(), QImage::Format_ARGB32);
	countries_view_map.fill({ 30, 30, 30 });

	for (auto [tag, provinces] : countries_provinces.asKeyValueRange())
	{
		auto it = countries_color.find(tag);

		if (it == countries_color.end())
			continue;

		auto color = it.value();

		for (int province : provinces)
		{
			paintProvince(countries_view_map, province, color);
		}
	}

	return countries_view_map;
}

QRgb MapImageProcessor::colorOfCountry(QString countryTag)
{
	return countries_color.value(countryTag, QRgb());
}

QList<QPoint> MapImageProcessor::pixelsOfProvince(int province)
{
	return province_pixels.value(province, QList<QPoint>());
}

void MapImageProcessor::setCountriesProvinces(const QHash<QString, QList<int>>& countries_provinces)
{
	this->countries_provinces = countries_provinces;
}

void MapImageProcessor::setCountriesColor(const QHash<QString, QRgb>& countries_color)
{
	this->countries_color = countries_color;
}

void MapImageProcessor::setColorToProvince(const QHash<QRgb, int>& color_to_province)
{
	this->color_to_province = color_to_province;
}

int MapImageProcessor::getMapWidth()
{
	return province_map.width();
}

int MapImageProcessor::getMapHeight()
{
	return province_map.height();
}

int MapImageProcessor::provinceAt(int x, int y)
{
	if (x < 0 || y < 0 ||
		x >= getMapWidth() ||
		y >= getMapHeight())
		return -1;

	QColor pixel = colorAtPixel(x, y);

	QRgb rgb = qRgb(pixel.red(), pixel.green(), pixel.blue());

	int province = color_to_province.value(rgb, -1);

	return province;
}

QColor MapImageProcessor::colorAtPixel(int x, int y)
{
	return province_map.pixelColor(x, y);
}

void MapImageProcessor::paintProvince(QImage& countries_view_map, int province, QRgb color)
{
	for (auto pixel : province_pixels[province])
		countries_view_map.setPixel(pixel, color);
}