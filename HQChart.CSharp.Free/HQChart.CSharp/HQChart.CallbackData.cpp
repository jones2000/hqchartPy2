/*
Copyright (c) 2018 jones

http://www.apache.org/licenses/LICENSE-2.0

开源项目 https://github.com/jones2000/HQChart

jones_2000@163.com

股票数据回调接口类 (c++)
*/

#include "pch.h"
#include <fstream>
#include <codecvt>
#include <sstream>
#include <iostream>
#include "HQChart.CallbackData.h"
#include "HQChart.data.h"
#include "HQChart.Log.h"
#include "HQChart.Execute.h"
#include "HQChart.RunConfig.h"
#include "HQChart.Interface.Data.h"


namespace HQChart { namespace Complier {


REGISTER_HISTORYDATA(HistoryDataCallback)

HistoryDataCallback::HistoryDataCallback(const std::wstring& strSymbol, long lPeriod, long lRight)
	:HistoryDataCache(strSymbol, lPeriod, lRight)
{
	m_strClassName = L"HistoryDataCallback";
}

HistoryDataCallback::HistoryDataCallback()
	:HistoryDataCache(L"", 0, 0)
{
	m_strClassName = L"HistoryDataCallback";
}

HistoryDataCallback::~HistoryDataCallback()
{

}

void HistoryDataCallback::Initialization(void* pData)
{
	if (!pData) return;

	HistoryDataCallback* p = (HistoryDataCallback*)pData;
	m_pRunConfig = p->m_pRunConfig;
}

void HistoryDataCallback::SetSymbol(const std::wstring& strSymbol, long lPeriod, long lRight)
{
	m_strSymbol = strSymbol;
	m_lPeriod = lPeriod;
	m_lRight = lRight;
}

bool HistoryDataCallback::GetCacheKData(const std::wstring& strSymbol, long lPeriod, long lRight, std::wstring* pStrName)
{
	if (!GetCacheKData(strSymbol, lPeriod, lRight, m_aryData, pStrName)) return false;

	m_KDataInfo._lPeriod = m_lPeriod;
	m_KDataInfo._lRight = m_lRight;
	m_KDataInfo._lCount = (long)m_aryData.size();

	if (m_KDataInfo._lCount > 0)
	{
		auto start = m_aryData.begin();
		m_KDataInfo._lStartDate = start->_nDate;
		m_KDataInfo._lStartTime = start->_nTime;
		auto end = m_aryData.rbegin();
		m_KDataInfo._lEndDate = end->_nDate;
		m_KDataInfo._lEndTime = end->_nTime;
	}

	return true;
}

bool HistoryDataCallback::GetCacheKData(const std::wstring& strSymbol, long lPeriod, long lRight, ARRAY_KDATA& aryData, std::wstring* pStrName) const
{
	return false;
}

bool HistoryDataCallback::GetCacheDynaInfo(const std::wstring& strSymbol, long lId, double& dValue) const
{
	return false;
}

bool HistoryDataCallback::LoadKData()
{
	if (GetCacheKData(m_strSymbol, m_lPeriod, m_lRight, &m_strName)) return true;

	if (!m_pRunConfig || !m_pRunConfig->m_pLoadKData) return false;

	HQCHART_KDATA_RESULT result = { 0 };

	bool bResult = m_pRunConfig->m_pLoadKData(m_strSymbol.c_str(), m_lPeriod, m_lRight , &result, m_pRunConfig->m_strGuid.c_str());
	if (!bResult)
	{
		if (result._pszError)
		{
			TRACE_WARNING(L"[HistoryDataCallback::LoadKData] load [symbol=%ls, period=%d, right=%d] failed. error=%ls", m_strSymbol.c_str(), m_lPeriod, m_lRight, result._pszError);
		}
		return false;
	}

	m_KDataInfo._lPeriod = m_lPeriod;
	m_KDataInfo._lRight = m_lRight;
	m_KDataInfo._lCount = result._lCount;

	if (result._lCount > 0)
	{
		const HQCHART_KDATA* pData = result._pData;
		m_aryData.reserve(result._lCount);
		m_aryData.assign(&pData[0], &pData[result._lCount]);

		auto start = m_aryData.begin();
		m_KDataInfo._lStartDate = start->_nDate;
		m_KDataInfo._lStartTime = start->_nTime;
		auto end = m_aryData.rbegin();
		m_KDataInfo._lEndDate = end->_nDate;
		m_KDataInfo._lEndTime = end->_nTime;
	}
	
	if (result._pszName) m_strName = result._pszName;

	TRACE_DEBUG(L"[HistoryDataCallback::LoadKData] load [symbol=%ls, name=%ls, period=%d, right=%d] count=%d", m_strSymbol.c_str(), m_strName.c_str(), m_lPeriod, m_lRight, m_aryData.size());
	return true;
}

bool HistoryDataCallback::LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo)
{
	ARRAY_KDATA aryData, aryKData;
	if (!Invoke_LoadKData2(strSymbol, lPeriod, lRight, aryData, pNode, strCallInfo)) return false;
	
	if (!aryData.empty())
	{
		bool bMinute = IHistoryData::IsMinutePeriod(m_lPeriod);	//主数据K线周期
		bool bMinute2 = IHistoryData::IsMinutePeriod(lPeriod);	//跨周期
		bool bMimToMin = bMinute && bMinute2;
		bool bMinToDay = bMinute && !bMinute2;
		bool bDayToDay = !bMinute && !bMinute2;
		HISTORY_ITEM newItem;
		const HISTORY_ITEM* pFindItem=NULL;

		//拟合到主图K线上
		for (int i = 0, j = 0; i < (int)m_aryData.size(); ++i)
		{
			const auto& item = m_aryData[i];
			for (; j < (int)aryData.size(); ++j)
			{
				const auto& periodItem = aryData[j];
				if (bMimToMin)  //都是分钟数据
				{
					if ((periodItem._nDate > item._nDate) || (periodItem._nDate == item._nDate && periodItem._nTime >= item._nTime))
					{
						pFindItem = &periodItem;
						break;
					}
				}
				else if (bMinToDay || bDayToDay)    //分钟 => 日线, 日线 => 日线
				{
					if (periodItem._nDate >= item._nDate)
					{
						pFindItem = &periodItem;
						break;
					}
				}
			}

			if (pFindItem)
			{
				newItem = *pFindItem;
			}
			else
			{
				memset(&newItem, 0, sizeof(newItem));
			}

			newItem._nDate = item._nDate;
			newItem._nTime = item._nTime;
			aryKData.push_back(newItem);
		}
	}
	
	std::wstringstream strBuffer;
	strBuffer << strSymbol << L"-" << lPeriod << L"-" << lRight;	//key=代码-周期-复权
	std::wstring strKey = strBuffer.str();

	m_mapKData[strKey] = aryKData;
	return true;
}

Variant* HistoryDataCallback::Invoke_GetDataByNumber(const std::wstring& strSymbol, const std::wstring& strFunctionName, long lParam, long lKCount, Node* pNode) const
{
	std::wstringstream strBuffer;
	strBuffer << strFunctionName << L"(" << lParam << L")";
	std::wstring strCallInfo = strBuffer.str();

	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumber)
	{
		std::wstringstream strDescription;
		strDescription << strCallInfo << L": 读取失败. CallStack: HistoryDataCallback::Invoke_GetDataByName. Error: m_pGetDataByName is null.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByNumber", pNode);
		throw error;
	}

	HQCHART_VALUE_RESULT result = { 0 };
	bool bResult = m_pRunConfig->m_pGetDataByNumber(strSymbol.c_str(), strFunctionName.c_str(), lParam, m_lPeriod, m_lRight, &m_KDataInfo, &result, m_pRunConfig->m_strGuid.c_str());
	if (!bResult)
	{
		std::wstringstream strDescription;
		strDescription << strCallInfo << L": 读取失败. CallStack: HistoryDataCallback::Invoke_GetDataByName. Error: Call m_pGetDataByName() function failed.";
		if (result._pszError)
		{
			TRACE_WARNING(L"[HistoryDataCallback::Invoke_GetDataByNumber] load %ls failed. error=%ls. ", strCallInfo.c_str(), result._pszError);
			strDescription << L"Message: " << result._pszError;
		}
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByNumber", pNode);
		throw error;
	}

	return ValueResultToVariant(result, pNode, strCallInfo);
}

Variant* HistoryDataCallback::Invoke_GetDataByNumbers(const std::wstring& strSymbol, const std::wstring& strFunctionName, const ARRAY_CALL_ARGUMENT& args, long lKCount, const IHistoryData* pHistoryData, Node* pNode) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumbers)
	{
		std::wstringstream strDescription;
		strDescription << strFunctionName << L": 读取失败. CallStack: HistoryDataCallback::Invoke_GetDataByNumbers. Error: m_pGetDataByNumbers is null.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByNumbers",pNode);
		throw error;
	}

	CUSTOM_FUNCTION_ARGUMENT argument = { 0 };
	long lArgumentCount = (long)args.size();
	if (lArgumentCount > ARRAYSIZE(argument._Args))
	{
		std::wstringstream strDescription;
		strDescription << strFunctionName << L": 参数错误.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByNumbers", pNode);
		throw error;
	}

	argument._lCount = lArgumentCount;
	long lType = 0;
	std::vector<REF_ARRAY_ARG_VALUE> aryCache;
	for (int i = 0; i < (int)args.size(); ++i)
	{
		const auto& item = args[i];
		auto& destItem = argument._Args[i];
		lType = item->GetType();
		if (lType == Variant::DOUBLE_TYPE)
		{
			destItem._lType = lType;
			destItem._dValue = item->GetDoubleValue();
		}
		else if (lType == Variant::ARRAY_DOUBLE_TYPE)
		{
			destItem._lType = lType;
			REF_ARRAY_ARG_VALUE pRef = REF_ARRAY_ARG_VALUE(new ARRAY_ARG_VALUE());
			const auto& aryValue = item->GetArrayValue();
			for (int i = 0; i < (int)aryValue.size(); ++i)
			{
				const auto& subItem = aryValue[i];
				ARG_ARRAY_ITEM valueItem = { 0 };

				valueItem._lType = subItem._sType;
				valueItem._dValue = subItem._dValue;
				pRef->push_back(valueItem);
			}

			if (!pRef->empty())
			{
				aryCache.push_back(pRef);
				destItem._lCount = (long)pRef->size();
				destItem._pAryValue = &(*pRef)[0];
			}
		}
	}

	HQCHART_VALUE_RESULT result = { 0 };
	bool bResult = m_pRunConfig->m_pGetDataByNumbers(strSymbol.c_str(), strFunctionName.c_str(), &argument, m_lPeriod, m_lRight, &m_KDataInfo, &result, m_pRunConfig->m_strGuid.c_str());
	if (!bResult)
	{
		std::wstringstream strDescription;
		strDescription << strFunctionName << L": 读取失败. CallStack: HistoryDataCallback::Invoke_GetDataByName. Error: Call m_pGetDataByName() function failed.";
		if (result._pszError)
		{
			TRACE_WARNING(L"[HistoryDataCallback::Invoke_GetDataByNumbers] load %ls failed. error=%ls. ", strFunctionName, result._pszError);
			strDescription << L"Message: " << result._pszError;
		}
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByNumbers", pNode);
		throw error;
	}

	return ValueResultToVariant(result, pNode, strFunctionName);
}

Variant* HistoryDataCallback::Invoke_GetDataByName(const std::wstring& strSymbol, const std::wstring& strFunctionName, long lKCount, Node* pNode) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByName)
	{
		std::wstringstream strDescription;
		strDescription << strFunctionName << L": 读取失败. CallStack: HistoryDataCallback::Invoke_GetDataByName. Error: m_pGetDataByName is null.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByName", pNode);
		throw error;
	}

	HQCHART_VALUE_RESULT result = { 0 };
	bool bResult = m_pRunConfig->m_pGetDataByName(strSymbol.c_str(), strFunctionName.c_str(), m_lPeriod, m_lRight, &m_KDataInfo, &result, m_pRunConfig->m_strGuid.c_str());
	if (!bResult)
	{
		std::wstringstream strDescription;
		strDescription << strFunctionName << L": 读取失败. CallStack: HistoryDataCallback::Invoke_GetDataByName. Error: Call m_pGetDataByName() function failed.";
		if (result._pszError)
		{
			TRACE_WARNING(L"[HistoryDataCallback::Invoke_GetDataByName] load %ls failed. error=%ls. ", strFunctionName.c_str(), result._pszError);
			strDescription << L"Message: " << result._pszError;
		}
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::Invoke_GetDataByName", pNode);
		throw error;
	}

	return ValueResultToVariant(result, pNode, strFunctionName);
}

Variant* HistoryDataCallback::ValueResultToVariant(const HQCHART_VALUE_RESULT& result, Node* pNode, const std::wstring& strCallInfo) const
{
	if (result._lType == 0)	//单值
	{
		Variant* pResult = Create();
		pResult->SetDoubleValue(result._dValue);
		TRACE_DEBUG(L"[HistoryDataCallback::ValueResultToVariant] %ls, value=%.02f", strCallInfo.c_str(), result._dValue);
		return pResult;
	}
	else if (result._lType == 1)	//数组类型, 数据个数和K线个数一致,不需要内部拟合数据
	{
		double dValue = 0;
		Variant* pResult = Create();
		Variant::ARRAY_DOUBLE& aryValue = pResult->GetArrayValue();
		aryValue.resize(result._lCount, VARIANT_ITEM());
		for (long i = 0; i < result._lCount; ++i)
		{
			dValue = result._pData[i]._dValue;
			aryValue[i].SetValue(dValue);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		TRACE_DEBUG(L"[HistoryDataCallback::ValueResultToVariant] %ls . array count=%d", strCallInfo.c_str(), result._lCount);
		return pResult;
	}
	else if (result._lType == 2)	//数组类型, 带日期 需要自己拟合数据 (做平滑处理,没有数据的周期返回上一周期的值)
	{
		HistoryDataCache::ARRAY_FINANCE_DATA aryData;
		for (long i = 0; i < result._lCount; ++i)
		{
			const auto& item = result._pData[i];
			if (item._lDate <= 19700101) continue;
			FINANCE_ITEM fitem = { item._lDate, item._dValue };
			aryData.push_back(fitem);
		}
		Variant* pResult = Create();
		if (!aryData.empty())
			FitStockData(*pResult, aryData);	//合并数据

		TRACE_DEBUG(L"[HistoryDataCallback::ValueResultToVariant] %ls . array count=%d", strCallInfo.c_str(), result._lCount);
		return pResult;
	}
	else if (result._lType == 3)	//数组类型, 带日期,时间 需要自己拟合数据 K线数据
	{
		HistoryDataCache::ARRAY_KDATA aryData;
		for (long i = 0; i < result._lCount; ++i)
		{
			const auto& item = result._pData[i];
			if (item._lDate <= 19700101) continue;
			HISTORY_ITEM kitem;
			kitem._nDate = item._lDate;
			kitem._nTime = item._lTime;
			kitem._dOpen = kitem._dHigh = kitem._dLow = kitem._dClose = kitem._dYClose = kitem._dVol = kitem._dAmount = item._dValue;
			aryData.push_back(kitem);
		}

		Variant* pResult = Create();
		if (!aryData.empty())
			FitKData(*pResult, aryData, m_lPeriod);

		TRACE_DEBUG(L"[HistoryDataCallback::ValueResultToVariant] %ls. array count=%d", strCallInfo.c_str(),  result._lCount);
		return pResult;
	}
	else if (result._lType == 4) //数组类型, 带日期 需要自己拟合数据 (不做平滑处理)
	{
		HistoryDataCache::ARRAY_FINANCE_DATA aryData;
		for (long i = 0; i < result._lCount; ++i)
		{
			const auto& item = result._pData[i];
			if (item._lDate <= 19700101) continue;
			FINANCE_ITEM fitem = { item._lDate, item._dValue };
			aryData.push_back(fitem);
		}
		Variant* pResult = Create();
		if (!aryData.empty())
			FitStockData(*pResult, aryData, 0, false);	//合并数据

		TRACE_DEBUG(L"[HistoryDataCallback::ValueResultToVariant] %ls . array count=%d", strCallInfo.c_str(), result._lCount);
		return pResult;
	}

	std::wstringstream strDescription;
	strDescription << strCallInfo << L": 读取失败. CallStack: HistoryDataCallback::ValueResultToVariant. DataType: "<< result._lType;
	ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::ValueResultToVariant", pNode);
	throw error;
}

bool HistoryDataCallback::Invoke_LoadKData2(const std::wstring& strSymbol, long lPeriod, long lRight, ARRAY_KDATA& aryData, Node* pNode, const std::wstring& strCallInfo) const
{
	aryData.clear();

	if (!m_pRunConfig || !m_pRunConfig->m_pLoadKData2)
	{
		std::wstringstream strDescription;
		strDescription <<strCallInfo << L": K线读取失败. CallStack: HistoryDataCallback::Invoke_LoadKData2. Error: m_pLoadKData2 is null.";
		ExecuteExcept error(strDescription.str(),L"HistoryDataCallback::Invoke_LoadKData2", pNode);
		throw error;
	}

	HQCHART_KDATA_RESULT result = { 0 };
	bool bResult = m_pRunConfig->m_pLoadKData2(strSymbol.c_str(), lPeriod, lRight, strCallInfo.c_str(), &m_KDataInfo, &result, m_pRunConfig->m_strGuid.c_str());
	if (!bResult)
	{
		std::wstringstream strDescription;
		strDescription << strCallInfo << L": K线读取失败. CallStack: HistoryDataCallback::Invoke_LoadKData2. Error: Call m_pLoadKData2() function failed.";
		if (result._pszError)
		{
			TRACE_WARNING(L"[HistoryDataCallback::Invoke_LoadKData2] load [symol=%ls, period=%d, right=%d] failed. error=%ls", strSymbol.c_str(), lPeriod, lRight, result._pszError);
			strDescription << L"Message: " << result._pszError;
		}
		ExecuteExcept error(strDescription.str(),L"HistoryDataCallback::Invoke_LoadKData2", pNode);
		throw error;
	}

	if (result._lCount > 0)
	{
		const HQCHART_KDATA* pData = result._pData;
		aryData.reserve(result._lCount);
		aryData.assign(&pData[0], &pData[result._lCount]);
	}

	TRACE_DEBUG(L"[HistoryDataCallback::Invoke_LoadKData2] load [symbol=%ls, period=%d, right=%d] count=%d", strSymbol.c_str(), lPeriod, lRight, aryData.size());
	return true;
}

void HistoryDataCallback::FitStockData(Variant& out, const ARRAY_FINANCE_DATA& fData) const
{
	ARRAY_EXTEND_DATA dest;
	HistoryDataCache::FitExtendData(dest, m_aryData, fData);

	int nCount = (int)dest.size();
	Variant::ARRAY_DOUBLE& aryValue = out.GetArrayValue();
	aryValue.resize(nCount, VARIANT_ITEM());
	for (long i = 0; i < (long)dest.size(); ++i)
	{
		const auto& item = dest[i];
		aryValue[i].SetValue(item._dValue);
	}

	out.SetType(Variant::ARRAY_DOUBLE_TYPE);
}

void HistoryDataCallback::FitStockData(Variant& out, const ARRAY_FINANCE_DATA& fData, double dDefault, bool bExactMatch) const
{
	ARRAY_EXTEND_DATA dest;
	HistoryDataCache::FitExtendData(dest, m_aryData, m_lPeriod, fData, dDefault, bExactMatch);

	int nCount = (int)dest.size();
	Variant::ARRAY_DOUBLE& aryValue = out.GetArrayValue();
	aryValue.resize(nCount, VARIANT_ITEM());
	for (long i = 0; i < (long)dest.size(); ++i)
	{
		const auto& item = dest[i];
		aryValue[i].SetValue(item._dValue);
	}

	out.SetType(Variant::ARRAY_DOUBLE_TYPE);
}

void HistoryDataCallback::FitKData(Variant& out, const ARRAY_KDATA& aryData, long lPeriod) const
{
	ARRAY_OVERLAY_DATA dest;
	if (IHistoryData::IsDayPeriod(lPeriod))
		FitOveralyDayData(dest, m_aryData, aryData);
	else if (IHistoryData::IsMinutePeriod(lPeriod))
		FitOveralyMinuteData(dest, m_aryData, aryData);
	else
		return;

	int nCount = (int)dest.size();
	Variant::ARRAY_DOUBLE& aryValue = out.GetArrayValue();
	aryValue.resize(nCount, VARIANT_ITEM());
	for (long i = 0; i < (long)dest.size(); ++i)
	{
		const auto& item = dest[i];
		aryValue[i].SetValue(item._dClose);
	}
	out.SetType(Variant::ARRAY_DOUBLE_TYPE);
}

void HistoryDataCallback::FitOverlayKData(ARRAY_OVERLAY_DATA& out, const ARRAY_KDATA& aryData, long lPeriod) const
{
	if (IHistoryData::IsDayPeriod(lPeriod))
		FitOveralyDayData(out, m_aryData, aryData);
	else if (IHistoryData::IsMinutePeriod(lPeriod))
		FitOveralyMinuteData(out, m_aryData, aryData);
	else
		return;
}

Variant* HistoryDataCallback::GetFromOpen(Node* pNode) const
{
	long lKCount = GetKCount();
	return Invoke_GetDataByName(m_strSymbol, L"FROMOPEN", lKCount, pNode);
}


Variant* HistoryDataCallback::GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	return NULL;
}

Variant* HistoryDataCallback::GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	return NULL;
}

Variant* HistoryDataCallback::GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	if (args.size() <= 0 || !args[0])
	{
		std::wstringstream strDescription;
		strDescription << L"FINANCE() 参数错误";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::GetFinance", pNode);
		throw error;
	}

	Variant* pParam = args[0];
	if (!pParam->IsVaildDoulbe())
	{
		std::wstringstream strDescription;
		strDescription << L"FINANCE() 参数无效";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::GetFinance", pNode);
		throw error;
	}

	long lValue = (long)pParam->GetDoubleValue();	//取对应的财务数据
	long lKCount = GetKCount();
	Variant* pResult = Invoke_GetDataByNumber(m_strSymbol, L"FINANCE", lValue, lKCount, pNode);
		
	return pResult;
}

Variant* HistoryDataCallback::CallCustomFunction(const std::wstring& strName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode) const
{
	long lKCount = GetKCount();
	Variant* pResult = Invoke_GetDataByNumbers(m_strSymbol, strName, args, lKCount, pHistoryData, pNode);
	return pResult;
}

Variant* HistoryDataCallback::GetCustomValue(const std::wstring& strName, Node* pNode) const
{
	if (CustomVariant::GetInstance().IsExist(strName))
	{
		long lKCount = GetKCount();
		Variant* pResult = Invoke_GetDataByName(m_strSymbol, strName, lKCount, pNode);
		return pResult;
	}

	return HistoryDataCache::GetCustomValue(strName,pNode);
}

bool HistoryDataCallback::GetSystemIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const
{
	const ScriptLibrary& sysLibrary = ScriptLibrary::GetInstance();	//系统指标库
	const ScriptIndex* pFind = sysLibrary.Get(strName);
	if (!pFind) return false;

	pFind->CopyTo(script);

	return true;
}

bool HistoryDataCallback::GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const
{
	if (GetSystemIndexScript(strName, strCallInfo, script, pNode)) return true;

	if (!m_pRunConfig || !m_pRunConfig->m_pGetIndexScript)
		return false;

	const wchar_t* pJson = m_pRunConfig->m_pGetIndexScript(strName.c_str(), strCallInfo.c_str(), m_pRunConfig->m_strGuid.c_str());
	if (!pJson) return false;

	std::wstring strJson = pJson;
	std::wstring strValue;

	std::string strUtf8Json = WStringToUTF8(strJson);
	rapidjson::Document root;
	root.Parse(strUtf8Json.c_str());
	if (root.IsNull())
	{
		TRACE_WARNING(L"[HistoryDataCallback::GetIndexScript] config string error.");
		return false;
	}

	if (!root.HasMember("Script") || !root["Script"].IsString())
	{
		TRACE_WARNING(L"[HistoryDataCallback::GetIndexScript] config 'Script' error.");
		return false;
	}
	script.SetCode(UTF8ToWString(root["Script"].GetString()));

	if (!root.HasMember("Name") || !root["Name"].IsString())
	{
		TRACE_WARNING(L"[HistoryDataCallback::GetIndexScript] config 'Name' error.");
		return false;
	}
	script.SetName(UTF8ToWString(root["Name"].GetString()));

	//参数
	if (root.HasMember("Args") && root["Args"].IsArray())
	{
		const Value& jsArgs = root["Args"];
		long lCount = (long)jsArgs.Size();
		Execute::ARRAY_ARGUMENT aryArgs;
		double dValue = 0;
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = jsArgs[i];
			strValue = UTF8ToWString(item["Name"].GetString());
			dValue = item["Value"].GetDouble();

			ARGUMENT_ITEM argItem;
			argItem._dValue = dValue;
			argItem._strName = strValue;
			aryArgs.push_back(argItem);
		}

		script.SetArgument(aryArgs);
	}

	return true;
}

bool HistoryDataCallback::GetBlockMember(const std::wstring& strBlockID, long lDate, std::vector<std::wstring>& arySymbol, Node* pNode) const
{
	return false;
}

Variant* HistoryDataCallback::GetExchange(Node* pNode) const
{
	long lKCount = GetKCount();
	Variant* pResult = Invoke_GetDataByName(m_strSymbol, L"EXCHANGE", lKCount, pNode);
	return pResult;
}

//最新流通股
Variant* HistoryDataCallback::GetCapital(Node* pNode) const
{
	long lKCount = GetKCount();
	Variant* pResult = Invoke_GetDataByName(m_strSymbol, L"CAPITAL", lKCount, pNode);
	return pResult;
}

Variant* HistoryDataCallback::GetTotalCaptialAll(Node* pNode) const
{
	long lKCount = GetKCount();
	Variant* pResult = Invoke_GetDataByName(m_strSymbol, L"TOTALCAPITAL", lKCount, pNode);
	return pResult;
}

//历史所有的流通股
Variant* HistoryDataCallback::GetHisCapital(Node* pNode) const
{
	long lKCount = GetKCount();
	Variant* pResult = Invoke_GetDataByName(m_strSymbol, L"GetHisCapital", lKCount, pNode);
	return pResult;
}

Variant* HistoryDataCallback::GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	if (args.size() <= 0 || !args[0])
	{
		std::wstringstream strDescription;
		strDescription << L"DYNAINFO() 参数错误";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::GetDynaInfo", pNode);
		throw error;
	}

	Variant* pParam = args[0];
	if (!pParam->IsVaildDoulbe())
	{
		std::wstringstream strDescription;
		strDescription << L"DYNAINFO() 参数无效";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::GetDynaInfo", pNode);
		throw error;
	}

	long lValue = (long)pParam->GetDoubleValue();
	long lKCount = GetKCount();

	Variant* pResult = Invoke_GetDataByNumber(m_strSymbol, L"DYNAINFO", lValue, lKCount, pNode);
	return pResult;
}

bool HistoryDataCallback::LoadIndexData(const std::wstring& strName, Node* pNode) const
{
	std::wstring strIndexSymbol(L"000001.sh");
	if (m_strSymbol.find(L".SZ") != std::wstring::npos || m_strSymbol.find(L".sz") != std::wstring::npos) strIndexSymbol = L"399001.sz";

	ARRAY_KDATA aryData;
	if (GetCacheKData(strIndexSymbol, m_lPeriod, m_lRight, aryData, NULL))
	{
		FitOverlayKData(m_aryIndex, aryData, m_lPeriod);
		return true;
	}
	
	if (!Invoke_LoadKData2(strIndexSymbol, m_lPeriod, m_lRight, aryData, pNode, strName)) return false;

	FitOverlayKData(m_aryIndex, aryData, m_lPeriod);

	return true;
}

void HistoryDataCallback::Reset()
{
	m_aryData.clear();
	m_strSymbol.clear();
	m_strName.clear();
	memset(&m_KDataInfo, 0, sizeof(m_KDataInfo));
	m_aryFinance.clear();
	m_mapDynainfo.clear();
	m_aryIndex.clear();
	m_aryCategory.clear();
	m_mapKData.clear();
	m_aryTempCacheData.clear();
	ClearCache();

	m_lPeriod = 0;
	m_lRight = 0;
	m_lUpdateTime = 0;
}

}
}