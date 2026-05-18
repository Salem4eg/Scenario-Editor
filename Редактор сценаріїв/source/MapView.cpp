#include "MapView.h"
#include <sstream>
#include <QtConcurrent/QtConcurrent>
#include <QRegularexpression>

MapView::MapView(QString directory, QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent), enable_province_choosing(false)
{
	directory_path = directory;
	QDir dir(directory);

	provinces_path = dir.filePath("map/provinces.bmp");
	provinces_definition = dir.filePath("map/definition.csv");
	countries_filepath = dir.filePath("common/countries.txt");
	provinces_directory_path = dir.filePath("history/provinces");

	province_map.load(provinces_path);
	province_map.flip(Qt::Vertical);

	

	connect(&highlight_timer, &QTimer::timeout, this, &MapView::highlightChosenProvinces);
}

MapView::~MapView()
{}

void MapView::prepare()
{


	qDebug() << "readProvincesDefinition()";
	startDebugTimer();

	readProvincesDefinition();
	QtConcurrent::run([this]()
		{
			emit progressMade(5);
		});

	endDebugTimer();

	qDebug() << "assignPixelsToProvince()";
	startDebugTimer();
	
	assignPixelsToProvince();
	QtConcurrent::run([this]()
		{
			emit progressMade(35);
		});
	endDebugTimer();

	qDebug() << "assignColorToCountries()";
	startDebugTimer();

	assignColorToCountries();
	QtConcurrent::run([this]()
		{
			emit progressMade(5);
		});
	endDebugTimer();

	qDebug() << "getCountries_provinces()";
	startDebugTimer();
	getCountries_provinces();
	QtConcurrent::run([this]()
		{
			emit progressMade(5);
		});
	endDebugTimer();


}

void MapView::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
	{
		if (scale_step < 8)
		{
			scale(1.25, 1.25);
			scale_step++;
		}
	}
	else
	{
		if (scale_step > -3)
		{
			scale(0.8, 0.8);
			scale_step--;
		}
	}
}

void MapView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		leftButtonStartPos = event->pos();
		mouseDragMode = false;
	}

	QGraphicsView::mousePressEvent(event);	
}

void MapView::mouseReleaseEvent(QMouseEvent* event)
{
	QGraphicsView::mouseReleaseEvent(event);

	if (event->isAccepted())
		return;


	if (event->button() == Qt::LeftButton)
	{
		QPoint leftButtonEndPos = event->pos();

		int distance = (leftButtonEndPos - leftButtonStartPos).manhattanLength();
		int dragDistance = QApplication::startDragDistance();

		if (distance >= dragDistance)
			mouseDragMode = true;

		if (!mouseDragMode)
		{
			QPoint pos = mapToScene(event->pos()).toPoint();
			handleClickAtProvince(pos.x(), pos.y());
		}
	}

}

void MapView::mouseMoveEvent(QMouseEvent* event)
{
	QGraphicsView::mouseMoveEvent(event);
}

void MapView::setThreadsCounter(int count)
{
	threads_working = count;
}

void MapView::decreaseAndCheckThreadsCounter()
{
	threads_working--;

	if (threads_working.load() == 0)
		emit isReadyToShow();
}

void MapView::readProvincesDefinition()
{
	FileReader file(provinces_definition);

	if (!file.isOpen())
		throw std::runtime_error("Cannot open definition csv");

	while (!file.atEnd())
	{
		QString line = file.readLine();

		QStringList items = line.split(';');

		// Skip provinces without IDs, such as lakes
		if (items[0].isEmpty() || items.size() < 4)
			continue;

		int provinceID = getOnlyDigits(items[0]);
		int red = getOnlyDigits(items[1]);
		int green = getOnlyDigits(items[2]);
		int blue = getOnlyDigits(items[3]);

		auto rgb = qRgb(red, green, blue);

		if (provinceID == 1825)
			qDebug() << "1825: " << rgb << " colors: " << red << " "<< green << " "<< blue;

		color_to_province.insert(rgb, provinceID);
	}
}

int MapView::getOnlyDigits(const QString& text)
{
	QString numbers;
	numbers.reserve(text.size());

	for (QChar ch : text)
	{
		if (ch.isDigit())
			numbers += ch;
	}

	return numbers.toInt();
}

void MapView::assignPixelsToProvince()
{
	int thread_count = QThread::idealThreadCount();
	
	if (thread_count == 0)
	{
		thread_count = 1;
	}

	//setThreadsCounter(thread_count);

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
				assignPartOfPixelsToProvince(start_index, count);

				//decreaseAndCheckThreadsCounter();
			});

		futures.push_back(future);
	}

	for (auto& future : futures)
		future.waitForFinished();
}

void MapView::assignPartOfPixelsToProvince(int start_index, int indexes_to_process)
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
		
		local_province_pixels[provinceID].push_back({ x,y });
	}

	QMutexLocker locker(&mutex);

	for (auto [key, value] : local_province_pixels.asKeyValueRange())
	{
		province_pixels[key].append(value);
	}
}


void MapView::highlightChosenProvinces()
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

void MapView::handleClickAtProvince(int x, int y)
{
	if (x < 0 || y < 0 ||
		x >= province_map.width() ||
		y >= province_map.height())
		return;

	QColor pixel = province_map.pixelColor(x, y);

	QRgb rgb = qRgb(pixel.red(), pixel.green(), pixel.blue());

	qDebug() << "rgb: " << rgb << " pixel: " << pixel.red() << " " << pixel.green() << " " << pixel.blue();

	int province = color_to_province.value(rgb, -1);

	if (province == -1 || !choosable_provinces.contains(province))
		return;

	if (enable_province_choosing == false)
	{
		emit getChosenProvinceInfo(province);
		return;
	}

	if (!chosen_provinces.contains(province))
		addChosenProvince(province);
	else
		removeChosenProvince(province);

	qDebug() << "Province clicked: " << province;

	provinces_highlighted = false;
	highlightChosenProvinces();

	if (chosen_provinces.isEmpty())
		highlight_timer.stop();
	else
		highlight_timer.start(1000);
}

void MapView::addChosenProvince(int provinceID)
{
	emit addProvinceToHighlight(province_pixels[provinceID], provinceID);

	chosen_provinces.push_back(provinceID);
}

void MapView::removeChosenProvince(int provinceID)
{
	emit removeProvinceFromHighlight(province_pixels[provinceID], provinceID);

	chosen_provinces.removeOne(provinceID);
}

void MapView::assignColorToCountries()
{
	auto countrie_rows = readCountriesFile();
	auto countries_filepaths = getCountriesFilePath(countrie_rows);
	getCountriesColor(countries_filepaths);
}

QList<QString> MapView::readCountriesFile()
{
	QList<QString> countries_rows;

	FileReader countries_file(countries_filepath);

	if (!countries_file.isOpen())
		throw std::runtime_error("Cannot open countries.txt");

	while (!countries_file.atEnd())
	{
		QString row = countries_file.readLine();

		int commentIndex = row.indexOf("#");

		if (commentIndex != -1)
			row = row.left(commentIndex);

		row = row.trimmed();

		if (!row.isEmpty())
			countries_rows.push_back(row);
	}

	return countries_rows;
}

QList<QPair<QString, QString>> MapView::getCountriesFilePath(QList<QString>& countries_rows)
{
	QList<QPair<QString, QString>> countries_filepaths;

	for (QString& row : countries_rows)
	{
		QStringList parts = row.split('=');

		if (parts.size() != 2)
			continue;

		QString tag = parts[0].trimmed();
		QString path = parts[1].trimmed();

		path = path.mid(1, path.length() - 2);

		if (path.endsWith(".txt", Qt::CaseInsensitive))
			countries_filepaths.emplace_back(tag, path);
	}

	return countries_filepaths;
}

void MapView::getCountriesColor(QList<QPair<QString, QString>>& countries_filepaths)
{
	QDir dir(directory_path);

	for (auto& country : countries_filepaths)
	{
		// first = tag
		// second = filepath
		QString full_filepath = dir.filePath("common/" + country.second);
		FileReader countryFile(full_filepath);

		if (!countryFile.isOpen())
			throw std::runtime_error("Couldn't open country's filepath");

		while (!countryFile.atEnd())
		{
			QString line = countryFile.readLine();

			if (line.contains("color"))
			{
				QString numbers = line.mid(line.indexOf('{') + 1);
				numbers = numbers.left(numbers.indexOf('}'));

				auto items = numbers.split(' ', Qt::SkipEmptyParts);

				int red = items[0].toInt();
				int green = items[1].toInt();
				int blue = items[2].toInt();

				countries_color.insert(country.first, qRgb(red, green, blue));

				break;
			}
		}
	}
	countries_color.insert("NO_OWNER", qRgb(50,50,50));
}

void MapView::getCountries_provinces()
{
	QDirIterator it(provinces_directory_path, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		getOwnerFromProvince(it.next());
	}
}

void MapView::getOwnerFromProvince(const QString& filepath)
{
	FileReader reader(filepath);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open province file: " + filepath.toStdString());

	int provinceID = getProvinceIDFromFilepath(filepath);

	QString province_owner = Province_manager::getOwnerFromProvince(filepath);

	// choosable_provinces для уникання морських провінцій
	choosable_provinces.push_back(provinceID);
	countries_provinces[province_owner].push_back(provinceID);
}

int MapView::getProvinceIDFromFilepath(const QString& filepath)
{
	auto words = filepath.split("/");
	QString document = words.last();

	auto items = document.split("-");

	int provinceID = items.first().toInt();

	return provinceID;
}

QImage MapView::GetBordersViewMap()
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

void MapView::makeOutlineForPartOfMap(QImage& outline_map, int start_index, int indexes_to_process)
{
	int end_index = start_index + indexes_to_process;

	QList<QPoint> outline_pixels;

	for (int pixel_index = start_index; pixel_index < end_index; pixel_index++)
	{
		int x = pixel_index % province_map.width();
		int y = pixel_index / province_map.width();

		if (hasAnotherProvinceNear(x,y))
			outline_pixels.push_back({ x, y });
	}

	QMutexLocker lock(&mutex);

	for (auto& coords : outline_pixels)
	{
		outline_map.setPixel(coords, qRgb(0,0,0));
	}
}

bool MapView::hasAnotherProvinceNear(int x, int y)
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

QImage MapView::GetCountriesViewMap()
{
	QImage countries_view_map = QImage(province_map.size(), QImage::Format_ARGB32);
	countries_view_map.fill({30,30,30});

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

void MapView::setProvinceChoosingMode(bool choose)
{
	enable_province_choosing = choose;
}

void MapView::showHighlighting(bool show)
{
	if (show)
	{
		if (!chosen_provinces.isEmpty())
		{
			highlight_timer.start(1000);
		}
	}
	else
		highlight_timer.stop();
}

void MapView::clearChosenProvinces()
{
	for (int province : chosen_provinces)
	{
		emit removeProvinceFromHighlight(province_pixels[province], province);
	}

	chosen_provinces.clear();
}

void MapView::paintProvince(QImage& countries_view_map, int province, QRgb color)
{
	for (auto pixel : province_pixels[province])
		countries_view_map.setPixel(pixel, color);
}

//void MapView::paintOutlineOverCountriesViewMap(QImage& countries_view_map, QImage& outline_map)
//{
//	QPainter painter(&countries_view_map);
//
//	painter.drawImage(0, 0, outline_map);
//	painter.end();
//}

void MapView::startDebugTimer()
{
	timer.start();
}

void MapView::endDebugTimer()
{
	auto time_elapsed = timer.elapsed();
	int time_elapsed_sec = time_elapsed / 1000;
	int ms_remaining = time_elapsed % 1000;

	qDebug() << "Time: " << time_elapsed_sec << "s " << ms_remaining << "ms";
}

QRgb MapView::GetCountryColor(QString tag)
{
	return countries_color[tag];
}

QList<QList<QPoint>> MapView::getProvincesPixels(QList<int> provinces)
{
	QList<QList<QPoint>> result;

	for (int province : provinces)
		result.push_back(province_pixels[province]);

	return result;
}

