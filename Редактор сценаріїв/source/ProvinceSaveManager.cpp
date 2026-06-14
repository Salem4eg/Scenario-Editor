#include "ProvinceSaveManager.h"
#include <QRegularExpression>

ProvinceSaveManager::ProvinceSaveManager(QString save_directory, QObject *parent)
	: QObject(parent), m_save_directory(save_directory)
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

void ProvinceSaveManager::loadProvincesFromSavefile(ParadoxGameData& game_data)
{
	loadSaveFile();

	for (auto [provinceId, lineNumber] : m_province_line_numbers.asKeyValueRange())
	{
		loadProvinceInfo(provinceId, lineNumber, game_data);
	}
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

	while (openBraces != closeBraces && currentLine < m_savefile.size())
	{
		currentLine++;
		QString line = m_savefile[currentLine];

		openBraces += line.count('{');
		closeBraces += line.count('}');

		const QRegularExpression ownerRegex("^\\s*owner\\s*=\\s*(\\w+)\\s*$");
		const QRegularExpression controllerRegex("^\\s*controller\\s*=\\s*(\\w+)\\s*$");
		const QRegularExpression coreRegex("^\\s*add_core\\s*=\\s*(\\w+)\\s*$");

		auto ownerMatch = ownerRegex.match(line);
		auto controllerMatch = controllerRegex.match(line);
		auto coreMatch = coreRegex.match(line);

		if (ownerMatch.hasMatch())
			game_data.countries_provinces[ownerMatch.captured(1)].push_back(provinceId);
		if (controllerMatch.hasMatch())
			game_data.countries_provinces[controllerMatch.captured(1)].push_back(provinceId);
		if (coreMatch.hasMatch())
			game_data.countries_provinces[coreMatch.captured(1)].push_back(provinceId);
	}



}
