#pragma once

#include <QFile>
#include <QStringView>
#include <QDebug>
#include "Structures.h"


class SaveGameStreamScanner 
{
private:
    const char* m_data; 
    qint64 m_size;      
    qint64 m_pos;       

public:
    SaveGameStreamScanner(const uchar* mappedData, qint64 size)
        : m_data(reinterpret_cast<const char*>(mappedData)), m_size(size), m_pos(0)
    {}

    
    TokenType nextToken(QString& outValue)
    {
        while (m_pos < m_size) 
        {
            char c = m_data[m_pos];
            
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') 
            {
                m_pos++;
                continue;
            }

            
            if (c == '#') 
            {
                while (m_pos < m_size && m_data[m_pos] != '\n') m_pos++;
                continue;
            }

            
            if (c == '=') 
            {
                m_pos++; return TokenType::Equals;
            }
            if (c == '{') 
            {
                m_pos++; return TokenType::OpenBrace;
            }
            if (c == '}') 
            {
                m_pos++; return TokenType::CloseBrace;
            }

            
            qint64 start = m_pos;

            
            if (c == '"') 
            {
                start++;
                m_pos++;
                while (m_pos < m_size && m_data[m_pos] != '"') 
                    m_pos++;
                
                QStringDecoder decoder(QStringDecoder::Latin1);
                outValue = decoder.decode(QByteArrayView(m_data + start, m_pos - start));

                if (m_pos < m_size) 
                {
                    m_pos++;
                    return TokenType::Identifier;
                }
            }

            
            while (m_pos < m_size) 
            {
                char nextC = m_data[m_pos];
                if (nextC == ' ' || nextC == '\t' || nextC == '\r' || nextC == '\n' || nextC == '=' || nextC == '{' || nextC == '}' || nextC == '#') 
                {
                    break;
                }
                m_pos++;
            }

            outValue = QString::fromLatin1(m_data + start, m_pos - start);

            return TokenType::Identifier;
        }

        return TokenType::EndOfFile;
    }

    void skipBlock()
    {
        int bracketBalance = 1; // Called after finding '{'
        while (m_pos < m_size && bracketBalance > 0) 
        {
            char c = m_data[m_pos++];
            if (c == '{') bracketBalance++;
            else if (c == '}') bracketBalance--;
            else if (c == '#') // Skip comments
            { 
                while (m_pos < m_size && m_data[m_pos] != '\n') m_pos++;
            }
        }
    }

	QString getTextBetween(qint64 start, qint64 end) const
	{
		if (start < 0 || end > m_size || start >= end)
			return QString();

		return QString::fromLatin1(m_data + start, end - start);
	}

	qint64 currentOffset() const
	{
		return m_pos;
	}
};