/*
Copyright (c) 2018 jones

http://www.apache.org/licenses/LICENSE-2.0

开源项目 https://github.com/jones2000/HQChart

jones_2000@163.com

指标调用配置类 (c++)
*/

#include "pch.h"
#include "hqchart.csharp.h"
#include "HQChart.RunConfig.h"
#include <locale>
#include <codecvt>
#include <sstream>
#include <memory>
#include "HQChart.Log.h"
#include "HQChart.CallbackData.h"
#include "HQChart.OutVarToJson.h"





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


namespace HQChart { namespace Complier {

RunConfig::RunConfig()
{

}

RunConfig::~RunConfig()
{

}

void RunConfig::SetCallbackProc(HQCHART_CALLBACK_PTR callback)
{
	m_pFailed = callback._pFailed;
	m_pSuccess = callback._pSuccess;
	m_pLoadKData = callback._pLoadKData;
	m_pLoadKData2 = callback._pLoadKData2;
	m_pGetDataByName = callback._pGetDataByName;
	m_pGetDataByNumbers = callback._pGetDataByNumbers;
	m_pGetDataByNumber = callback._pGetDataByNumber;
	m_pGetIndexScript = callback._pGetIndexScript;

	m_pTest = callback._pTest;
}

bool RunConfig::LoadJsonConfig(const wchar_t* pJsonConfig)
{
	std::string strUtf8Json = WStringToUTF8(pJsonConfig);
	rapidjson::Document root;
	root.Parse(strUtf8Json.c_str());
	if (root.IsNull())
	{
		TRACE_WARNING(L"[RunConfig::LoadJsonConfig] config string error.");
		return false;
	}

	std::wstring strValue;

	//脚本
	if (!root.HasMember("Script") || !root["Script"].IsString())
	{
		TRACE_WARNING(L"[RunConfig::LoadJsonConfig] config 'Script' error.");
		return false;
	}
	m_strCode = UTF8ToWString(root["Script"].GetString());

	//参数
	if (root.HasMember("Args") && root["Args"].IsArray())
	{
		const Value& jsArgs = root["Args"];

		long lCount = (long)jsArgs.Size();

		double dValue = 0;
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = jsArgs[i];
			strValue = UTF8ToWString(item["Name"].GetString());
			dValue = item["Value"].GetDouble();

			HQChart::Complier::ARGUMENT_ITEM argItem;
			argItem._dValue = dValue;
			argItem._strName = strValue;

			m_aryArgs.push_back(argItem);
		}
	}

	//周期
	if (root.HasMember("Period") && root["Period"].IsNumber())
		m_lPeriod = root["Period"].GetInt();

	//复权
	if (root.HasMember("Right") && root["Right"].IsNumber())
		m_lRight = root["Right"].GetInt();

	if (!root.HasMember("Symbol"))
	{
		TRACE_WARNING(L"[RunConfig::LoadJsonConfig] config 'Symbol' not exist.");
		return false;
	}

	//股票代码
	const Value& jsSymbol = root["Symbol"];
	if (jsSymbol.IsString())
	{
		m_arySymbol.push_back(UTF8ToWString(root["Symbol"].GetString()));
	}
	else if (jsSymbol.IsArray())
	{
		long lCount = (long)jsSymbol.Size();
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = jsSymbol[i];
			if (item.IsString())
				m_arySymbol.push_back(UTF8ToWString(item.GetString()));
		}
	}
	else
	{
		TRACE_WARNING(L"[RunConfig::LoadJsonConfig] config 'Symbol' type error.");
		return false;

	}
	if (m_arySymbol.empty())
	{
		TRACE_WARNING(L"[RunConfig::LoadJsonConfig] config 'Symbol' empty.");
		return false;
	}

	if (root.HasMember("JobID") && root["JobID"].IsString())
		m_strGuid = UTF8ToWString(root["JobID"].GetString()).c_str();

	if (root.HasMember("OutCount") && root["OutCount"].IsInt())
	{
		m_lOutCount = root["OutCount"].GetInt();
	}

	if (root.HasMember("KLineCount") && root["KLineCount"].IsInt())
	{
		m_lKLineCount= root["KLineCount"].GetInt();
	}

	if (root.HasMember("OutRange") && root["OutRange"].IsObject())
	{
		auto& jsNode = root["OutRange"];
		if (jsNode.HasMember("Start") && jsNode["Start"].IsInt() && jsNode.HasMember("End") && jsNode["End"].IsInt())
		{
			long lStart = jsNode["Start"].GetInt();
			long lEnd = jsNode["End"].GetInt();

			if (lStart <= lEnd && lStart >= 0 && lEnd >= 0)
			{
				m_lOutStartIndex = lStart;
				m_lOutEndIndex = lEnd;
			}
		}
	}

	return true;
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
bool RunConfig::RunScript(RunConfig& runConfig)
{
	try
	{
		if (!VerifyAuthorizeInfo(runConfig)) return false;

		HQChart::Complier::Pool pool;
		HQChart::Complier::Program* pProgram = HQChart::Complier::Parse(pool, runConfig.m_strCode);	//脚本编译

		const auto& arySymbol = runConfig.m_arySymbol;
		TRACE_DEBUG(L"[RunConfig::RunScript] start (count=%d)......", arySymbol.size());
		long lFinished = 0;
		std::string strJson;
		std::wstring strJsonW;

		HQChart::Complier::HistoryDataCallback* pHistoryDataCallback = new HQChart::Complier::HistoryDataCallback();
		std::shared_ptr< HQChart::Complier::HistoryDataCallback> ptrHistoryDataCallback = std::shared_ptr< HQChart::Complier::HistoryDataCallback>(pHistoryDataCallback);
		pHistoryDataCallback->SetRunConfig(&runConfig);
		HQChart::Complier::IHistoryData* pHistoryData = pHistoryDataCallback;

		HQChart::Complier::Execute exec;
		exec.SetProgram(pProgram);
		if (!runConfig.m_aryArgs.empty()) exec.SetArguments(runConfig.m_aryArgs);

		for (const auto& strSymbol : arySymbol)
		{
			try
			{
				//HQChart::Complier::IHistoryData* pHistoryData = NULL;
				//HQChart::Complier::HistoryDataCallback*  pHistoryDataCallback = new HQChart::Complier::HistoryDataCallback(strSymbol, runConfig.m_lPeriod, runConfig.m_lRight);
				//std::shared_ptr< HQChart::Complier::HistoryDataCallback> ptrHistoryDataCallback = std::shared_ptr< HQChart::Complier::HistoryDataCallback>(pHistoryDataCallback);

				//设置配置信息
				//pHistoryDataCallback->SetRunConfig(&runConfig);
				//pHistoryData = pHistoryDataCallback;
				pHistoryDataCallback->Reset();
				pHistoryDataCallback->SetSymbol(strSymbol, runConfig.m_lPeriod, runConfig.m_lRight);
				pHistoryData->LoadKData();	//加载K线数据
				
				//执行
				exec.SetHistoryData(pHistoryData);
				exec.Run();	//执行脚本
				
				if (runConfig.m_pSuccess)
				{
					OutVarToJsonHelper::ToJson(exec.GetOutVarInfo(), pHistoryDataCallback, strJson, runConfig.m_lOutCount, runConfig.m_lOutStartIndex, runConfig.m_lOutEndIndex,&runConfig.m_aryArgs);
					strJsonW = UTF8ToWString(strJson);
					runConfig.m_pSuccess(strSymbol.c_str(), strJsonW.c_str(), runConfig.m_strGuid.c_str());
				}

				++lFinished;
				if (lFinished== arySymbol.size() && lFinished==1)
					TRACE_NORMAL(L"[RunScript] %ls jobID=%ls run success. out var count=%d, out count=%d, kline count=%d", strSymbol.c_str(), runConfig.m_strGuid.c_str(), exec.GetOutVar().size(), runConfig.m_lOutCount, pHistoryData->GetKCount());
				else if (lFinished % 1000 == 0 || lFinished == arySymbol.size()) 
					TRACE_DEBUG(L"[RunScript] progress %d/%d", lFinished, arySymbol.size());
			}
			catch (const HQChart::Complier::ExecuteExcept& e)	//执行失败
			{
				std::wstring strError;
				std::wstringstream strStream;
				strStream << L"execute error:" << e.GetDescription().c_str();
				strError = strStream.str();
				if (runConfig.m_pFailed) runConfig.m_pFailed(runConfig.m_strCode.c_str(), strSymbol.c_str(), strError.c_str(), runConfig.m_strGuid.c_str());
				return false;
			}
		}

		return true;
	}
	catch (const HQChart::Complier::ParseExcept& e)	//编译失败
	{
		std::wstring strError;
		std::wstringstream strStream;
		strStream << L"build error:" << e.GetDescription().c_str();
		strError = strStream.str();
		if (runConfig.m_pFailed) runConfig.m_pFailed(runConfig.m_strCode.c_str(), L"", strError.c_str(), runConfig.m_strGuid.c_str());
		return false;
	}
}

bool RunConfig::VerifyAuthorizeInfo(RunConfig& runConfig)
{
	return true;
}


}
}

