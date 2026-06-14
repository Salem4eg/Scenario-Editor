#include "ProvinceSaveManager.h"
#include <QRegularExpression>

ProvinceSaveManager::ProvinceSaveManager(QString save_directory, QString provinces_directory, QObject *parent)
	: QObject(parent), m_save_directory(save_directory), m_provinces_directory(provinces_directory)
{}

ProvinceSaveManager::~ProvinceSaveManager()
{}

void ProvinceSaveManager::changeProvincesOwner(QList<int> provinces, QString country_tag)
{

}

void ProvinceSaveManager::addCoreToProvinces(QList<int> provinces, QString country_tag)
{

}

void ProvinceSaveManager::removeCoreFromProvinces(QList<int> provinces, QString country_tag)
{

}

ProvinceInfo ProvinceSaveManager::getProvinceInfo(int province)
{
	return ProvinceInfo();
}

void ProvinceSaveManager::loadProvincesFromSavefile(ParadoxGameData& game_data)
{
	loadSaveFile();

	for (auto [provinceId, lineNumber] : m_province_line_numbers.asKeyValueRange())
	{
		loadProvinceInfo(provinceId, lineNumber, game_data);
	}

	loadChosableProvinces(game_data);
}

void ProvinceSaveManager::loadSaveFile()
{
	FileReader reader(m_save_directory);

	if (!reader.isOpen())
		throw std::runtime_error("Couldn't open save file: " + m_save_directory.toStdString());

	while (!reader.atEnd())
	{
		QString line = reader.readLine();

		const QRegularExpression provKeyRegex("^(\\d+)=\\s*$");

		auto match = provKeyRegex.match(line.trimmed());

		if (match.hasMatch())
		{
			int provinceId = match.captured(1).toInt();
			m_province_line_numbers[provinceId] = m_savefile.size();
		}

		m_savefile.push_back(line);
	}
}

void ProvinceSaveManager::loadProvinceInfo(int provinceId, int lineNumber, ParadoxGameData& game_data)
{
	int currentLine = lineNumber;

	const QRegularExpression provKeyRegex("^(\\d+)=\\s*$");

	auto match = provKeyRegex.match(m_savefile[currentLine].trimmed());

	if (!match.hasMatch() || match.captured(1).toInt() != provinceId)
		throw std::runtime_error(std::format("Expected province {} at line {}", provinceId, currentLine));

	bool inProvinceBlock = m_savefile[currentLine].contains('{');

	while (!inProvinceBlock && currentLine < m_savefile.size())
	{
		currentLine++;
		inProvinceBlock = m_savefile[currentLine].contains('{');
	}

	int openBraces = 1;
	int closeBraces = 0;
	bool hasOwner = false;

	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;
		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');

		const QRegularExpression ownerRegex("^\\s*owner\\s*=\\s*(\\w+)\\s*$");

		auto ownerMatch = ownerRegex.match(line);

		if (ownerMatch.hasMatch())
		{
			game_data.countries_provinces[ownerMatch.captured(1)].push_back(provinceId);
			hasOwner = true;
			continue;
		}
	}

	if (!hasOwner)
	{
		game_data.countries_provinces["NO_OWNER"].push_back(provinceId);
	}
}

void ProvinceSaveManager::loadChosableProvinces(ParadoxGameData& game_data)
{
	QDirIterator it(m_provinces_directory, { "*.txt" }, QDir::Files, QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		int provinceID = getProvinceIDFromFilepath(it.next());

		game_data.choosable_provinces.push_back(provinceID);
	}

}

int ProvinceSaveManager::getProvinceIDFromFilepath(const QString& filepath)
{
	auto words = filepath.split("/");
	QString document = words.last();

	auto items = document.split("-");

	int provinceID = items.first().toInt();

	return provinceID;
}
