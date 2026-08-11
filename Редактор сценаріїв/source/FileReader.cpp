#include "FileReader.h"
#include <QDebug>

FileReader::FileReader()
{}

FileReader::FileReader(const QString& filepath)
{
	isOpened = open(filepath);
}

bool FileReader::open(const QString& filepath)
{
	clearData();
	m_file.setFileName(filepath);

	// Відкриваємо файл засобами Qt в бінарному режимі для читання
	if (!m_file.open(QIODevice::ReadOnly))
	{
		isOpened = false;
		return false;
	}

	// Мапимо файл в адресний простір пам'яті
	uchar* memory_ptr = m_file.map(0, m_file.size());

	if (memory_ptr != nullptr)
	{
		isOpened = true;
		processMappedData(reinterpret_cast<const char*>(memory_ptr), m_file.size());

		// Після того, як дані оброблені й m_data заповнено, мапу можна зняти,
		// а файл закрити, щоб звільнити системні ресурси.
		m_file.unmap(memory_ptr);
		m_file.close();
	}
	else
	{
		// Якщо файл порожній або map не підтримується ОС для цього файлу
		isOpened = false;
		m_file.close();
	}

	return isOpened;
}

void FileReader::processMappedData(const char* memory_ptr, qint64 file_size)
{
	if (file_size == 0) return;

	// Reserve some memory to use less reallocations
	m_data.reserve(file_size / 40);

	qint64 start = 0;

	while (start < file_size)
	{
		qint64 end = start;

		// Шукаємо кінець рядка, який може бути символом \n або \r
		while (end < file_size && memory_ptr[end] != '\n' && memory_ptr[end] != '\r')
		{
			end++;
		}

		qint64 line_len = end - start;

		// Створюємо QString із заміною спецсимволів Windows-1252 на льоту
		QString line;
		if (line_len > 0)
		{
			line.reserve(line_len);
			for (qint64 i = start; i < end; ++i)
			{
				unsigned char uc = static_cast<unsigned char>(memory_ptr[i]);
				switch (uc)
				{
					case 0x91: line += u'‘'; break;
					case 0x92: line += u'’'; break;
					case 0x93: line += u'“'; break;
					case 0x94: line += u'”'; break;
					default:   line += QChar(static_cast<ushort>(uc)); break;
				}
			}
		}

		m_data.emplace_back(std::move(line));

		// Зсуваємо вказівник далі, пропускаючи символи перенесення рядка
		if (end < file_size)
		{
			if (memory_ptr[end] == '\r')
			{
				end++;
				// Обробка Windows-формату (\r\n): якщо за \r йде \n, пропускаємо і його
				if (end < file_size && memory_ptr[end] == '\n')
				{
					end++;
				}
			}
			else if (memory_ptr[end] == '\n')
			{
				end++;
			}
		}

		start = end;
	}
}

QList<QString> FileReader::releaseData()
{
	return std::move(m_data);
}

QString FileReader::readLine()
{
	if (current_line >= m_data.size())
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

void FileReader::clearData()
{
	m_data.clear();
	current_line = 0;
	isOpened = false;
}