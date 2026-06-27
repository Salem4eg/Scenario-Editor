#include "FileReader.h"
#include <QTextStream>
#include <QDebug>
#include <filesystem>

FileReader::FileReader()
{

}

FileReader::FileReader(const QString& filepath, std::ios_base::openmode open_mode)
{
	isOpened = open(filepath, open_mode);
}

bool FileReader::open(const QString& filepath, std::ios_base::openmode open_mode)
{
	std::ifstream readFile;
	std::filesystem::path path(filepath.toStdWString());

	readFile.open(path, open_mode);
	isOpened = readFile.is_open();
	m_data.clear();

	if (isOpened)
	{
		std::string file_data((std::istreambuf_iterator<char>(readFile)),
			std::istreambuf_iterator<char>());

		auto decoder = QStringDecoder(QStringDecoder::Latin1);
		QString data = decoder(QByteArrayView(file_data.data(), file_data.size()));

		processData(data);
	}

	return isOpened;
}

QString FileReader::readLine()
{
	if (current_line + 1 > m_data.size())
		return {};

	return m_data[current_line++];
}

bool FileReader::isOpen() const
{
	return isOpened;
}

bool FileReader::atEnd()
{
	return m_data.size() <= current_line;
}

void FileReader::processData(const QString& file_data)
{
	QString normalized_data = normalizeData(file_data);
	QTextStream stream(&normalized_data);

	QString line;

	while (!stream.atEnd())
	{
		m_data.emplace_back(stream.readLine());
	}
}

QString FileReader::normalizeData(const QString& file_data)
{
	QString result;
	result.reserve(file_data.size());

	for (int i = 0; i < file_data.size(); i++)
	{
		if (file_data[i] == '\r')
		{
			result += '\n';

			if (i + 1 < file_data.size() && file_data[i + 1] == '\n')
				i++;
		}
		else
			result += file_data[i];
	}

	return result;
}

