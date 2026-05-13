#pragma once

#include <QString>
#include <QList>
#include <fstream>

// QFile can't normally read text files with Macintosh(CR) format, need to use this instead.
class FileReader
{
public:
	FileReader();
	FileReader(const QString& filepath, std::ios_base::openmode open_mode = std::ios_base::out);

	bool open(const QString& filepath, std::ios_base::openmode open_mode = std::ios_base::out);
	QString readLine();
	bool isOpen() const;
	bool atEnd();
private:
	QList<QString> m_data;

	int current_line = 0;
	bool isOpened = false;

	void processData(const QString& file_data);
	QString normalizeData(const QString& file_data);
};

