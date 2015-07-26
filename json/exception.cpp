#define _CRT_SECURE_NO_WARNINGS
#include "exception.h"
#include <Windows.h>
#include <cstdarg>
#include <cstdio>
using namespace std;

Exception::Exception()
	:m_Time(0)
{}

Exception::Exception(std::string Src, const char* Desc, ...)
{
	char szText[512];
	va_list marker;
	va_start(marker, Desc);
	vsprintf_s(szText, Desc, marker);
	va_end(marker);

	m_Time = GetTickCount();
	m_ExcpSrc = Src;
	m_ExcpDesc = szText;
}

Exception::Exception(const Exception& Org)
{
	m_Time = Org.m_Time;
	m_ExcpSrc = Org.m_ExcpSrc;
	m_ExcpDesc = Org.m_ExcpDesc;
}


Exception::~Exception()
{
}


int Exception::getTime() const
{
	return m_Time;
}

std::string Exception::getSrc() const
{
	return m_ExcpSrc;
}

std::string Exception::getDesc() const
{
	return m_ExcpDesc;
}

///////////////////////////////////////////////////////////////////////////
ReaderException::ReaderException(std::string Src, const char* Desc, int Line, int Row)
	:Exception(Src, Desc), m_Line(Line), m_Row(Row)
{
	char buf[256];
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "at Line:%d Row:%d", Line, Row);
	m_ExcpDesc += buf;
}

ReaderException::~ReaderException()
{
}

int ReaderException::getLine()
{
	return m_Line;
}

int ReaderException::getRow()
{
	return m_Row;
}

