/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	日志输出类 (c++)
*/

#include "HQChart.Log.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <wchar.h>
#include <ctime>


namespace HQChart { namespace Complier {

const wchar_t LOG_TITLE[][32] =
{
	L"[Error]",
	L"[Normal]",
	L"[Debug]",
	L"[Warning]"
};

#ifdef _DEBUG
long Log::s_lLogStatus = 1;
#else
long Log::s_lLogStatus = 0;	//默认禁止日志
#endif


bool Log::SetLogStatus(long lType)
{
	s_lLogStatus = lType;
	return true;
}

void Log::LogTraceFormatV(LOG_LEVEL_ID LogType, LPCWSTR pszFormat, va_list args, va_list args2)
{
#ifdef _WIN32
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	wchar_t strTimeBuffer[256] = { 0 };
	GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &st, NULL, strTimeBuffer, ARRAYSIZE(strTimeBuffer));
	TCHAR strDateBuffer[256] = { 0 };
	::GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &st, NULL, strDateBuffer, ARRAYSIZE(strDateBuffer));
	DWORD dwThreadId = GetCurrentThreadId();
	DWORD dwProcessID = GetCurrentProcessId();

	std::wstringstream  strLogBuffer;
	strLogBuffer <<LOG_TITLE[LogType] << L"[" << strDateBuffer << L" " << strTimeBuffer << L":" << st.wMilliseconds << L" " << dwProcessID << L":" << dwThreadId <<L"] ";

	size_t len = std::vswprintf(NULL, 0, pszFormat, args);
	va_end(args);
	std::vector<wchar_t> aryBuffer(len + 1);

	std::vswprintf(&aryBuffer[0], len + 1, pszFormat, args2);
	va_end(args2);

	strLogBuffer << aryBuffer.data() << L"\r\n";
	std::wstring strLog = strLogBuffer.str();

	{
		//AutoLock lock(&g_csTrace);
		::OutputDebugStringW(strLog.c_str());
		//wprintf_s(L"%s", strLog.c_str());
		//WriteLogFileW(strLog);
		std::wcout << strLog;
		if (LogType == LOG_LEVEL_ERROR)
		{
			_RPTW0(_CRT_ASSERT, strLog);
		}
	}
#else
	{
		std::wstringstream  strLogBuffer;
		std::time_t tmCurrent = std::time(0);
		wchar_t strTimeBuffer[256] = { 0 };
		std::wcsftime(strTimeBuffer, ARRAYSIZE(strTimeBuffer)-1, L"%Y-%m-%d %H:%M:%S", std::localtime(&tmCurrent));
		
		size_t len = std::vswprintf(NULL, 0, pszFormat, args);
		wchar_t szBuffer[1024*2] = { 0 };
		std::vswprintf(szBuffer, ARRAYSIZE(szBuffer)-1, pszFormat, args2);

		va_end(args);
		va_end(args2);

		strLogBuffer << LOG_TITLE[LogType] << L"[" << strTimeBuffer << L"]" << szBuffer << L"\r\n";
		std::wstring strLog = strLogBuffer.str();
		std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
		std::string strUtf8Log = myconv.to_bytes(strLog);
		std::cout << strUtf8Log;
	}
#endif
}

void Log::Trace(LOG_LEVEL_ID LogType, LPCWSTR pszFormat, ...)
{
	if (s_lLogStatus == 0) return;

	va_list args;
	va_list args2;
	va_start(args, pszFormat);
	va_copy(args2, args);

	LogTraceFormatV(LogType, pszFormat, args, args2);
}

void Log::TraceError(LPCWSTR pszFormat, ...)
{
	if (s_lLogStatus == 0) return;

	va_list args;
	va_list args2;
	va_start(args, pszFormat);
	va_copy(args2, args);

	LogTraceFormatV(LOG_LEVEL_ERROR, pszFormat, args, args2);
}

void Log::TraceWarning(LPCWSTR pszFormat, ...)
{
	va_list args;
	va_list args2;
	va_start(args, pszFormat);
	va_copy(args2, args);

	LogTraceFormatV(LOG_LEVEL_WARNING, pszFormat, args, args2);
}

void Log::TraceNormal(LPCWSTR pszFormat, ...)
{
	if (s_lLogStatus == 0) return;

	va_list args;
	va_list args2;
	va_start(args, pszFormat);
	va_copy(args2, args);

	LogTraceFormatV(LOG_LEVEL_NORMAL, pszFormat, args, args2);
}

void Log::TraceDebug(LPCWSTR pszFormat, ...)
{
	if (s_lLogStatus == 0) return;

	va_list args;
	va_list args2;
	va_start(args, pszFormat);
	va_copy(args2, args);
	
	LogTraceFormatV(LOG_LEVEL_DEBUG, pszFormat, args, args2);
}



#if (defined WIN32) || (defined WIN64)
void Log::WriteLogFile(const char* szStr)
{

}
#else
void Log::WriteLogFile(const char* szStr)
{
	FILE* fp;
	fp = fopen("run.log", "at+");
	if (fp != NULL) {
		fwrite(szStr, strlen(szStr), 1, fp); //
		fwrite("\r\n", 2, 1, fp); //
		fclose(fp);
		fp = NULL;
	}
}
#endif


const std::string HQCHART_LOGO =
"********************************************************************************************\n"
"*********11*******222***********************************************************************\n"
"*********11******22**22**********************************************************************\n"
"*********11*********22************************************************************************\n"
"*********11******222*************************************************************************\n"
"********1111****22******************************************************************************\n"
"***************2222222222**************************************************************************\n"
"********************************************************************************************\n"
"********************************************************************************************\n"
"********************************************************************************************\n"
"********************************************************************************************\n"
"********************************************************************************************\n"
"********************************************************************************************\n";

}
}