/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com
*/

#include "hqchart.complier.py.authorizeInfo.h"
#include <vector>
#include <sstream>
#include <fstream>


#include "HQChart.Log.h"
#include "HQChart.OutVarToJson.h"

//json 读取
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filereadstream.h"
using namespace rapidjson;

#include "userhelper.h"

#ifndef _WIN32
void GetLocalTime(SYSTEMTIME* pst)
{
	time_t st = time(NULL);
	tm* nt = localtime(&st);
	pst->wYear = nt->tm_year + 1900;
	pst->wMonth = nt->tm_mon + 1;
	pst->wDay = nt->tm_mday;
	pst->wDayOfWeek = nt->tm_yday;
	pst->wHour = nt->tm_hour;
	pst->wMinute = nt->tm_min;
	pst->wSecond = nt->tm_sec;
	pst->wMilliseconds = 0;
}
#endif


namespace HQChart { namespace Complier { namespace Py {


AuthorizeInfo::AuthorizeInfo()
{
	
}

AuthorizeInfo::~AuthorizeInfo()
{

}

const std::wstring& AuthorizeInfo::GetInfo() const
{
	return m_strInfo;
} 

const std::wstring& AuthorizeInfo::GetError() const
{
	return m_strError;
}

std::wstring AuthorizeInfo::ToString() const
{
	if (m_bVaild) return m_strInfo;

	std::wstring strValue = m_strError;
	strValue += L"HQChartPy2授权无效.请联系作者购买授权. QQ:48274798";

	return strValue;
}



void AuthorizeInfo::Load(const std::string& strJson)
{
	
}

bool AuthorizeInfo::IsVaild() const
{ 
	return true; 
}

AuthorizeInfo& AuthorizeInfo::GetInstance()
{
	static AuthorizeInfo instance;
	return instance;
}


}
}
}
