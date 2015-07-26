#include "reader.h"

/// @brief ´ÓÎÄ±¾¹¹Ôì
/// @param[in] srcText Òª½øÐÐÆ¥ÅäµÄÔ­Ê¼×Ö·û´®
Reader::Reader(const std::string& srcText)
	:m_str(srcText), m_pos(0)
{
	m_len = m_str.size();
}

/// @brief ´ÓÁ÷¹¹Ôì
/// @param[in] pStream Òª½øÐÐÆ¥ÅäµÄÔ­Ê¼Êý¾ÝÁ÷
Reader::Reader(std::ifstream* pStream)
	:m_pos(0)
{
	char buf[128];
	if (!pStream)
		throw Exception("Reader::Reader", "Param 'pStream' is null.");
	while (!pStream->eof())
	{
		pStream->getline(buf, 128);
		m_str += buf;
	}
	m_len = m_str.size();
}

Reader::~Reader()
{
}

bool Reader::isEOF()  ///< @brief ÊÇ·ñ½áÊø
{
	return (m_pos >= m_len);
}

int Reader::getLine()
{
	int tLine = 0;
	char tLast = '\0';
	for (int i = 0; i < m_pos; ++i)
	{
		if (tLast == '\n')
			tLine++;
		tLast = m_str[i];
	}
	return tLine;
}

int Reader::getRow()
{
	int tRow = 0;
	char tLast = '\0';
	for (int i = 0; i < m_pos; ++i)
	{
		if (tLast == '\n')
			tRow = 0;
		tRow++;
		tLast = m_str[i];
	}
	return tRow;
}

char Reader::readChar()  ///< @brief ¶ÁÈ¡Ò»¸ö×Ö·û
{
	if (isEOF())
		throw ReaderException("Reader::readChar", "EOF.", getLine(), getRow());
	char tRet = m_str[m_pos];
	m_pos++;
	return tRet;
}

char Reader::peekChar()  ///< @brief Ô¤¶ÁÈ¡Ò»¸ö×Ö·û
{
	if (isEOF())
		throw ReaderException("Reader::readChar", "EOF.", getLine(), getRow());
	char tRet = m_str[m_pos];
	return tRet;
}

void Reader::ignoreSpace()  ///< @brief Ìø¹ý¿Õ°××Ö·û
{
	while (!isEOF())
	{
		char tRet = peekChar();
		if (isspace(tRet))
			readChar();
		else
			break;
	}
}

/// @brief Æ¥ÅäÒ»¸ö×Ö·û
/// @note  Æ¥ÅäÒ»¸ö×Ö·û£¬Ê§°ÜÊä³ö´íÎó
/// @param[in] ch ÒªÆ¥ÅäµÄ×Ö·û
/// @parampin] bIgnoreSpace ºöÂÔ¿Õ¸ñ
void Reader::match(char ch, bool bIgnoreSpace)
{
	if (bIgnoreSpace)
		ignoreSpace();

	char tChar = readChar();
	if (tChar != ch)
	{
		char tTextBuffer[1024];
		sprintf_s(tTextBuffer, "'%c' expected, buf found '%c'.", ch, tChar);
		throw ReaderException("Reader::match", tTextBuffer, getLine(), getRow());
	}
}

/// @brief Æ¥ÅäÒ»¸ö×Ö·û´®
/// @note  Æ¥ÅäÒ»¸ö×Ö·û´®£¬Ê§°ÜÊä³ö´íÎó
/// @param[in] str ÒªÆ¥ÅäµÄ×Ö·û´®
/// @param[in] bIgnoreSpace ºöÂÔ¿Õ¸ñ
void Reader::match(std::string str, bool bIgnoreSpace)
{
	if (bIgnoreSpace)
		ignoreSpace();

	int tLen = str.size();

	for (int i = 0; i < tLen; ++i)
	{
		char tChar = readChar();
		if (tChar != str[i])
		{
			char tTextBuffer[1024];
			sprintf_s(tTextBuffer, "'%s' expected, buf found '%c'.", str, tChar);
			throw ReaderException("Reader::match", tTextBuffer, getLine(), getRow());
		}
	}
}

/// @brief ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û
/// @note  ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û´®£¬Ê§°ÜÊä³ö´íÎó
/// @param[in] ch ÒªÆ¥ÅäµÄ×Ö·û
/// @param[in] bIgnoreSpace ºöÂÔ¿Õ¸ñ
/// @param[in] bMatch Èç¹ûÄÜÆ¥Åä£¬ÊÇ·ñÆ¥ÅäÕâ¸ö×Ö·û
/// @return  true=¿ÉÒÔÆ¥Åä£¬false=Æ¥ÅäÊ§°Ü
bool Reader::tryMatch(char ch, bool bIgnoreSpace, bool bMatch)
{
	int tPos = m_pos;

	if (bIgnoreSpace)
		ignoreSpace();

	if (isEOF())
	{
		m_pos = tPos;
		return false;
	}

	char tChar = readChar();

	if (tChar != ch)
	{
		//²»Æ¥Åä£¬·µ»Ø¼Ù
		m_pos = tPos;
		return false;
	}
	else
	{
		//Æ¥Åä
		if (!bMatch)
		{
			m_pos = tPos;
		}
		return true;
	}
}

/// @brief ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û´®
/// @note  ³¢ÊÔÆ¥ÅäÒ»¸ö×Ö·û´®£¬Ê§°ÜÊä³ö´íÎó
/// @param[in] str ÒªÆ¥ÅäµÄ×Ö·û´®
/// @param[in] bIgnoreSpace ºöÂÔ¿Õ¸ñ
/// @param[in] bMatch Èç¹ûÄÜÆ¥Åä£¬ÊÇ·ñÆ¥ÅäÕâ¸ö×Ö·û
/// @return  true=¿ÉÒÔÆ¥Åä£¬false=Æ¥ÅäÊ§°Ü
bool Reader::tryMatch(std::string str, bool bIgnoreSpace, bool bMatch)
{
	int tPos = m_pos;
	int tLen = str.size();
	if (bIgnoreSpace)
		ignoreSpace();

	if (m_str.size() - m_pos < tLen)
	{
		//³¤¶È²»×ã£¬²»ÄÜÆ¥Åä
		m_pos = tPos;
		return false;
	}

	for (int i = 0; i < tLen; ++i)
	{
		if (m_str[m_pos + i] != str[i])
		{
			//²»ÄÜÆ¥Åä
			m_pos = tPos;
			return false;
		}
	}

	if (bMatch)
	{
		m_pos += tLen;
	}
	else
	{
		m_pos = tPos;
	}
	return true;
}


