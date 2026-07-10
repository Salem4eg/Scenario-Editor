#pragma once

#include <QString>
#include <QList>
#include <QFile>

// QFile can't normally read text files with Macintosh(CR) format, need to use this instead.
class FileReader
{
public:
	FileReader();
	FileReader(const QString& filepath);

	bool open(const QString& filepath);
	QString readLine();
	bool isOpen() const;
	bool atEnd();
	void clearData();
	QList<QString> releaseData();

private:
	QFile m_file;
	QList<QString> m_data;

	int current_line = 0;
	bool isOpened = false;

	void processMappedData(const char* memory_ptr, qint64 file_size);
};

