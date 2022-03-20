/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	日志输出类 (c++)
*/


#pragma once
#include "zwldef.h"
#include <stdio.h>
#ifdef _WIN32

	#ifndef NO_TRACE
		#define TRACE_ERROR(__Format__,...) HQChart::Complier::Log::TraceError(__Format__,__VA_ARGS__)
		#define TRACE_WARNING(__Format__,...) HQChart::Complier::Log::TraceWarning(__Format__,__VA_ARGS__)
		#define TRACE_NORMAL(__Format__,...) HQChart::Complier::Log::TraceNormal(__Format__,__VA_ARGS__)
		#define TRACE_DEBUG(__Format__,...) HQChart::Complier::Log::TraceDebug(__Format__,__VA_ARGS__)
	#else
		#define TRACE_ERROR(__Format__,...) {}
		#define TRACE_WARNING(__Format__,...) {}
		#define TRACE_NORMAL(__Format__,...) {}
		#define TRACE_DEBUG(__Format__,...) {}
	#endif
#else
	#ifndef NO_TRACE
		#define TRACE_ERROR(__Format__,...) HQChart::Complier::Log::TraceError(__Format__,##__VA_ARGS__)
		#define TRACE_WARNING(__Format__,...) HQChart::Complier::Log::TraceWarning(__Format__,##__VA_ARGS__)
		#define TRACE_NORMAL(__Format__,...) HQChart::Complier::Log::TraceNormal(__Format__,##__VA_ARGS__)
		#define TRACE_DEBUG(__Format__,...) HQChart::Complier::Log::TraceDebug(__Format__,##__VA_ARGS__)
	#else
		#define TRACE_ERROR(__Format__,...) {}
		#define TRACE_WARNING(__Format__,...) {}
		#define TRACE_NORMAL(__Format__,...) {}
		#define TRACE_DEBUG(__Format__,...) {}
	#endif
#endif

namespace HQChart { namespace Complier {

class  Log
{
public:
	enum LOG_LEVEL_ID
	{
		LOG_LEVEL_ERROR = 0,
		LOG_LEVEL_NORMAL,
		LOG_LEVEL_DEBUG,
		LOG_LEVEL_WARNING,
	};

	static void Trace(LOG_LEVEL_ID lLogType, LPCWSTR pszFormat, ...);

	static void TraceError(LPCWSTR pszFormat, ...);
	static void TraceWarning(LPCWSTR pszFormat, ...);
	static void TraceNormal(LPCWSTR pszFormat, ...);
	static void TraceDebug(LPCWSTR pszFormat, ...);
	static bool SetLogStatus(long lType);

	//linux开发调试用
	static void WriteLogFile(const char* szStr);


private:

	static void LogTraceFormatV(LOG_LEVEL_ID LogType, LPCWSTR pszFormat, va_list args, va_list args2);

	static long s_lLogStatus;	//0 禁用 1=开启
};


}
}