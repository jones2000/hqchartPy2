/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

股票数据回调接口类 (c++)
*/

#include <fstream>
#include <codecvt>
#include <sstream>
#include <iostream>
#include "HQChart.data.h"
#include "HQChart.Log.h"
#include "HQChart.Execute.h"
#include "HQChart.OutVarToJson.h"
#include "hqchart.complier.py.callbackdata.h"
#include "hqchart.complier.py.runconfig.h"
#include "hqchart.complier.py.callbackfunction.h"
#include "HQChart.PyCache.h"

//json 读取
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filereadstream.h"
using namespace rapidjson;


namespace HQChart { namespace Complier { namespace Py {

//REGISTER_HISTORYDATA(HistoryDataCallback)

IHistoryData * Create_HistoryDataCallback(const std::wstring & strSymbol, long lPeriod, long lRight)
{
	return new HistoryDataCallback(strSymbol, lPeriod, lRight);
}

void Delete_HistoryDataCallback(IHistoryData*& pHistoryData)
{
	if (pHistoryData) 
	{ 
		delete pHistoryData; 
		pHistoryData = NULL; 
	}
}

const bool bRegisteredCreate_HistoryDataCallback= HQChart::Complier::DataCreateFactory::GetInstance().RegisterHistoryData(L"HistoryDataCallback", Create_HistoryDataCallback, Delete_HistoryDataCallback);


HistoryDataCallback::HistoryDataCallback(const std::wstring& strSymbol, long lPeriod, long lRight)
	:HistoryDataCache(strSymbol, lPeriod, lRight)
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
	ClearCache();

	m_lPeriod = 0;
	m_lRight = 0;
	m_lUpdateTime = 0;
}

bool HistoryDataCallback::LoadKData()
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetKData) return false;

	//读缓存
	const PyCache::KLineCache& cache = PyCache::KLineCache::GetInstance();
	PyCache::KLineCacheItem::REF_PTR pItem=cache.GetData(m_strSymbol, m_lPeriod, m_lRight);
	if (pItem)
	{
		m_aryData = pItem->GetData();
		m_strName = pItem->GetName();
		UpdateKDataInfo();
		return true;
	}

	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetKData);
	PyObject* arglist = Py_BuildValue("uiis", m_strSymbol.c_str(), m_lPeriod, m_lRight, m_pRunConfig->m_strGuid.c_str());

	PyObject* pResult = pyFunction.Call(arglist);
	std::wstring strName;
	if (!PyDataToKData(m_strSymbol, pResult, m_aryData, strName, m_lPeriod, m_lRight))
	{
		if (!m_strError.empty())
		{
			std::wstringstream strDescription;
			strDescription << L"(" << m_strSymbol << L") 第3方K线数据读取失败. 错误信息:" << m_strError;
			ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::LoadKData", NULL);
			throw error;
		}

		return false;
	}

	m_strName = strName;
	UpdateKDataInfo();

	return true;
}

void HistoryDataCallback::UpdateKDataInfo()
{
	m_KDataInfo._lPeriod = m_lPeriod;
	m_KDataInfo._lRight = m_lRight;
	m_KDataInfo._lCount = 0;

	if (!m_aryData.empty())
	{
		auto start = m_aryData.begin();
		m_KDataInfo._lStartDate = start->_nDate;
		m_KDataInfo._lStartTime = start->_nTime;
		auto end = m_aryData.rbegin();
		m_KDataInfo._lEndDate = end->_nDate;
		m_KDataInfo._lEndTime = end->_nTime;
		m_KDataInfo._lCount = (long)m_aryData.size();
	}
}

bool HistoryDataCallback::PyDataToKData(const std::wstring& strSymbol, PyObject* pResult, ARRAY_KDATA& aryData, std::wstring& strName, long lPeriod, long lRight) const
{
	if (!pResult || !PyDict_Check(pResult)) return false;

	PyObject* pError= PyDict_GetItemString(pResult, "error");
	if (pError)
	{
		m_strError = PyUnicode_AsWideCharString(pError, NULL);
		return false;
	}

	PyObject* pCount = PyDict_GetItemString(pResult, "count");
	PyObject* pName = PyDict_GetItemString(pResult, "name");

	PyObject* pDate = PyDict_GetItemString(pResult, "date");
	PyObject* pTime = PyDict_GetItemString(pResult, "time");
	PyObject* pYClose = PyDict_GetItemString(pResult, "yclose");
	PyObject* pOpen = PyDict_GetItemString(pResult, "open");
	PyObject* pHigh = PyDict_GetItemString(pResult, "high");
	PyObject* pLow = PyDict_GetItemString(pResult, "low");
	PyObject* pClose = PyDict_GetItemString(pResult, "close");

	PyObject* pVol = PyDict_GetItemString(pResult, "vol");
	PyObject* pAmount = PyDict_GetItemString(pResult, "amount");

	PyObject* pAdvance = PyDict_GetItemString(pResult, "advance");
	PyObject* pDecline = PyDict_GetItemString(pResult, "decline");

	PyObject* pPosition = PyDict_GetItemString(pResult, "position");	//持仓
	PyObject* pSettle = PyDict_GetItemString(pResult, "settle");		//结算价

	if (!pDate || !pYClose || !pOpen || !pHigh || !pLow || !pClose || !pVol || !pAmount || !pCount) return false;
	if (!PyList_Check(pDate) || !PyList_Check(pYClose) || !PyList_Check(pOpen) || !PyList_Check(pHigh) || !PyList_Check(pLow) || !PyList_Check(pClose) ||
		!PyList_Check(pVol) || !PyList_Check(pAmount) ) return false;

	//可选数据
	if (!pTime || !PyList_Check(pTime)) pTime = NULL;
	if (!pAdvance || !PyList_Check(pAdvance)) pAdvance = NULL;
	if (!pDecline || !PyList_Check(pDecline)) pDecline = NULL;
	if (!pName || !PyUnicode_Check(pName)) pName = NULL;
	if (!pPosition || !PyList_Check(pPosition)) pPosition = NULL;
	if (!pSettle || !PyList_Check(pSettle)) pSettle = NULL;

	int nCount = PyLong_AsLong(pCount);

	//设置K线
	HISTORY_ITEM item;
	aryData.reserve(nCount);
	PyObject* pValue = NULL;
	for (int i = 0; i < nCount; ++i)
	{
		memset(&item, 0, sizeof(item));
		pValue = PyList_GetItem(pDate, i);
		item._nDate = PyLong_AsLong(pValue);

		if (pTime)
		{
			pValue = PyList_GetItem(pTime, i);
			item._nTime = PyLong_AsLong(pValue);
		}

		pValue = PyList_GetItem(pYClose, i);
		if (pValue == Py_None)continue;
		item._dYClose = PyFloat_AsDouble(pValue);

		pValue = PyList_GetItem(pOpen, i);
		if (pValue == Py_None)continue;
		item._dOpen = PyFloat_AsDouble(pValue);

		pValue = PyList_GetItem(pHigh, i);
		if (pValue == Py_None)continue;
		item._dHigh = PyFloat_AsDouble(pValue);

		pValue = PyList_GetItem(pLow, i);
		if (pValue == Py_None)continue;
		item._dLow = PyFloat_AsDouble(pValue);

		pValue = PyList_GetItem(pClose, i);
		if (pValue == Py_None)continue;
		item._dClose = PyFloat_AsDouble(pValue);

		pValue = PyList_GetItem(pVol, i);
		if(pValue != Py_None) item._dVol = PyFloat_AsDouble(pValue);

		pValue = PyList_GetItem(pAmount, i);
		if (pValue != Py_None) item._dAmount = PyFloat_AsDouble(pValue);

		if (pAdvance)
		{
			pValue = PyList_GetItem(pAdvance, i);
			if (pValue != Py_None) item._nAdvance = PyLong_AsLong(pValue);
		}

		if (pDecline)
		{
			pValue = PyList_GetItem(pDecline, i);
			if (pValue != Py_None) item._nDecline = PyLong_AsLong(pValue);
		}

		if (pPosition)
		{
			pValue = PyList_GetItem(pPosition, i);
			if (pValue != Py_None) item._dPosition = PyFloat_AsDouble(pValue);
		}

		if (pSettle)
		{
			pValue = PyList_GetItem(pSettle, i);
			if (pValue != Py_None) item._dSettle = PyFloat_AsDouble(pValue);
		}

		aryData.push_back(item);
	}


	if (pName) strName = PyUnicode_AsWideCharString(pName, NULL);

	PyObject* pPeriod = PyDict_GetItemString(pResult, "period");	//周期
	PyObject* pRight = PyDict_GetItemString(pResult, "right");		//复权

	if (pRight)
	{
		long lSrcRight = PyLong_AsLong(pRight);
		if (lSrcRight != lRight)	//计算复权
		{
			if (!ConvertToRight(aryData, m_lRight))
			{
				TRACE_WARNING(L"[HistoryDataCallback::PyDataToKData] [%ls] ConvertToRight(out,%d) failed", strSymbol.c_str(), m_lRight);
				return false;
			}
		}
	}

	if (pPeriod)
	{
		long lSrcPeriod = PyLong_AsLong(pPeriod);
		if (lSrcPeriod != lPeriod)	//计算周期
		{
			if (lSrcPeriod == PERIOD_TYPE_ID::PERIOD_DAY_ID && IHistoryData::IsDayPeriod(m_lPeriod))
			{
				HistoryDataCache::ARRAY_KDATA dest;
				ConvertToDayPeriod(aryData, dest, m_lPeriod);
				aryData = dest;
			}
			else if (lSrcPeriod == PERIOD_TYPE_ID::PERIOD_MIN1_ID && IHistoryData::IsMinutePeriod(m_lPeriod))
			{
				HistoryDataCache::ARRAY_KDATA dest;
				ConvertToMinutePeriod(aryData, dest, m_lPeriod);
				aryData = dest;
			}
			else
			{
				TRACE_WARNING(L"[HistoryDataCallback::LoadKData] [%ls] Convert to period failed, lSrcPeriod=%d, lPeriod=%d", strSymbol.c_str(), lSrcPeriod, m_lPeriod);
				return false;
			}
		}
	}

	TRACE_DEBUG(L"[HistoryDataCallback::PyDataToKData] load [%ls] success count=%d", strSymbol.c_str(), aryData.size());
	return true;
}

bool HistoryDataCallback::LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo)
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetKData2) return false;

	PyObject* pPyDictDataInfo = PyDict_New();	//字典类型
	PyDict_SetItemString(pPyDictDataInfo, "Period", PyLong_FromLong(m_KDataInfo._lPeriod));
	PyDict_SetItemString(pPyDictDataInfo, "Right", PyLong_FromLong(m_KDataInfo._lRight));
	PyDict_SetItemString(pPyDictDataInfo, "KCount", PyLong_FromLong(m_KDataInfo._lCount));
	PyDict_SetItemString(pPyDictDataInfo, "StartDate", PyLong_FromLong(m_KDataInfo._lStartDate));
	PyDict_SetItemString(pPyDictDataInfo, "StartTime", PyLong_FromLong(m_KDataInfo._lStartTime));
	PyDict_SetItemString(pPyDictDataInfo, "EndDate", PyLong_FromLong(m_KDataInfo._lEndDate));
	PyDict_SetItemString(pPyDictDataInfo, "EndTime", PyLong_FromLong(m_KDataInfo._lEndTime));

	PyObject* pArgs = PyTuple_New(6);
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("u", strSymbol.c_str()));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", lPeriod));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", lRight));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("u", strCallInfo.c_str()));
	PyTuple_SetItem(pArgs, 4, pPyDictDataInfo);
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("s", m_pRunConfig->m_strGuid.c_str()));

	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetKData2);
	PyObject* pResult = pyFunction.Call(pArgs);
	ARRAY_KDATA aryData, aryKData;
	std::wstring strName;
	if (!PyDataToKData(strSymbol, pResult, aryData, strName, lPeriod, lRight))
	{
		if (!m_strError.empty())
		{
			std::wstringstream strDescription;
			strDescription << L"(" << m_strSymbol << L") 第3方K线数据读取失败. 错误信息:" << m_strError;
			ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::LoadKData", pNode);
			throw error;
		}

		return false;
	}


	TRACE_DEBUG(L"[HistoryDataCallback::LoadKData] load [symbol=%ls, period=%d, right=%d] count=%d", strSymbol.c_str(), lPeriod, lRight, aryData.size());

	if (!aryData.empty())
	{
		bool bMinute = IHistoryData::IsMinutePeriod(m_lPeriod);	//主数据K线周期
		bool bMinute2 = IHistoryData::IsMinutePeriod(lPeriod);	//跨周期
		bool bMimToMin = bMinute && bMinute2;
		bool bMinToDay = bMinute && !bMinute2;
		bool bDayToDay = !bMinute && !bMinute2;
		HISTORY_ITEM newItem;
		const HISTORY_ITEM* pFindItem = NULL;

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

bool HistoryDataCallback::PyDataToVariant(const std::wstring& strInfo, PyObject* pResult, Variant& out) const
{
	if (!pResult || !PyDict_Check(pResult)) return false;

	PyObject* pType = PyDict_GetItemString(pResult, "type");
	if (!pType) return false;
	int nType = PyLong_AsLong(pType);

	if (nType == 0)	//double 
	{
		PyObject* pValue = PyDict_GetItemString(pResult, "data");
		if (!pValue || pValue== Py_None) return false;
		double dValue = PyFloat_AsDouble(pValue);
		out.SetDoubleValue(dValue);
		TRACE_DEBUG(L"[HistoryDataCallback::PyDataToVariant] strInfo. value=%.04f", strInfo.c_str(), dValue);
	}
	else if (nType == 1)	//array double
	{
		PyObject* pAryValue = PyDict_GetItemString(pResult, "data");
		if (!pAryValue || !PyList_Check(pAryValue)) return false;
		int nCount = (int)PyList_Size(pAryValue);
		double dValue = 0;
		PyObject* pValue = NULL;
		Variant::ARRAY_DOUBLE& aryValue = out.GetArrayValue();
		aryValue.resize(nCount, VARIANT_ITEM());
		for (long i = 0; i < nCount; ++i)
		{
			pValue = PyList_GetItem(pAryValue, i);
			if (pValue && Py_None!= pValue)
			{
				dValue = PyFloat_AsDouble(pValue);
				aryValue[i].SetValue(dValue);
			}
		}
		out.SetType(Variant::ARRAY_DOUBLE_TYPE);
		TRACE_DEBUG(L"[HistoryDataCallback::PyDataToVariant] %ls. array count=%d", strInfo.c_str(), nCount);
	}
	else if (nType == 2)	//数组类型, 带日期 需要自己拟合数据 "data"=数据  "date"=日期
	{
		PyObject* pAryValue = PyDict_GetItemString(pResult, "data");
		if (!pAryValue || !PyList_Check(pAryValue)) return false;
		PyObject* pAryDate = PyDict_GetItemString(pResult, "date");
		if (!pAryDate || !PyList_Check(pAryDate)) return false;

		int nCount = (int)PyList_Size(pAryValue);
		int nCount2= (int)PyList_Size(pAryDate);
		if (nCount != nCount2) return false;

		HistoryDataCache::ARRAY_FINANCE_DATA aryData;
		PyObject* pValue = NULL;
		PyObject* pDate = NULL;
		double dValue = 0;
		int nDate = 0;
		for (long i = 0; i < nCount; ++i)
		{
			pValue = PyList_GetItem(pAryValue, i);
			pDate = PyList_GetItem(pAryDate, i);
			if (!pValue || !pDate) continue;

			dValue = PyFloat_AsDouble(pValue);
			nDate = PyLong_AsLong(pDate);
			if (nDate <= 19700101) continue;
			FINANCE_ITEM fitem = { nDate, dValue };
			aryData.push_back(fitem);
		}
		
		if (!aryData.empty()) FitFinanceData(out, aryData);	//合并数据
		TRACE_DEBUG(L"[HistoryDataCallback::PyDataToVariant] %ls. array src=%d, dest=%d ", strInfo, nCount, out.GetArrayValue().size());
	}
	else if (nType == 3) //数组类型 数组类型, 带日期,时间 需要自己拟合数据 K线数据 "data"= , "date"= , "time"=
	{
		HistoryDataCache::ARRAY_KDATA aryData;
		if (IHistoryData::IsDayPeriod(m_lPeriod))
		{
			PyObject* pAryValue = PyDict_GetItemString(pResult, "data");
			if (!pAryValue || !PyList_Check(pAryValue)) return false;
			PyObject* pAryDate = PyDict_GetItemString(pResult, "date");
			if (!pAryDate || !PyList_Check(pAryDate)) return false;

			int nCount = (int)PyList_Size(pAryValue);
			int nCount2 = (int)PyList_Size(pAryDate);
			if (nCount != nCount2) return false;

			PyObject* pValue = NULL;
			PyObject* pDate = NULL;
			double dValue = 0;
			int nDate = 0;
			for (long i = 0; i < nCount; ++i)
			{
				pValue = PyList_GetItem(pAryValue, i);
				pDate = PyList_GetItem(pAryDate, i);
				if (!pValue || !pDate) continue;

				dValue = PyFloat_AsDouble(pValue);
				nDate = PyLong_AsLong(pDate);
				if (nDate <= 19700101) continue;
				HISTORY_ITEM kitem;
				kitem._nDate=nDate, kitem._nTime=0, kitem._dYClose=dValue, kitem._dOpen=dValue, kitem._dHigh=dValue ,kitem._dLow=dValue ,kitem._dClose=dValue ,kitem._dVol=dValue ,kitem._dAmount=dValue,kitem._nAdvance=0,kitem._nDecline=0;
				aryData.push_back(kitem);
			}
		}
		else if (IHistoryData::IsMinutePeriod(m_lPeriod))
		{
			PyObject* pAryValue = PyDict_GetItemString(pResult, "data");
			if (!pAryValue || !PyList_Check(pAryValue)) return false;
			PyObject* pAryDate = PyDict_GetItemString(pResult, "date");
			if (!pAryDate || !PyList_Check(pAryDate)) return false;
			PyObject* pAryTime = PyDict_GetItemString(pResult, "time");
			if (!pAryTime || !PyList_Check(pAryTime)) return false;

			int nCount = (int)PyList_Size(pAryValue);
			int nCount2 = (int)PyList_Size(pAryDate);
			int nCount3 = (int)PyList_Size(pAryDate);
			if (nCount != nCount2 || nCount!= nCount3) return false;

			PyObject* pValue = NULL;
			PyObject* pDate = NULL;
			PyObject* pTime = NULL;
			double dValue = 0;
			int nDate = 0, nTime = 0;
			for (long i = 0; i < nCount; ++i)
			{
				pValue = PyList_GetItem(pAryValue, i);
				pDate = PyList_GetItem(pAryDate, i);
				pTime= PyList_GetItem(pAryTime, i);
				if (!pValue || !pDate || !pTime) continue;

				dValue = PyFloat_AsDouble(pValue);
				nDate = PyLong_AsLong(pDate);
				nTime = PyLong_AsLong(pTime);
				if (nDate <= 19700101) continue;
				//struct HISTORY_ITEM kitem = { nDate, nTime, dValue, dValue,dValue ,dValue ,dValue ,dValue ,dValue,0,0  };
				HISTORY_ITEM kitem;
				kitem._nDate=nDate, kitem._nTime=nTime, kitem._dYClose=dValue, kitem._dOpen=dValue, kitem._dHigh=dValue ,kitem._dLow=dValue ,kitem._dClose=dValue ,kitem._dVol=dValue ,kitem._dAmount=dValue,kitem._nAdvance=0,kitem._nDecline=0;
				aryData.push_back(kitem);
			}
		}
		else
		{
			return false;
		}
		
		if (!aryData.empty()) FitKData(out, aryData, m_lPeriod);
		TRACE_DEBUG(L"[HistoryDataCallback::PyDataToVariant] %ls. array src=%d, dest=%d", strInfo.c_str(), aryData.size(), out.GetArrayValue().size());
	}
	else if (nType == 4)	//数组类型, 带日期 需要自己拟合数据 (不做平滑处理)
	{
		PyObject* pAryValue = PyDict_GetItemString(pResult, "data");
		if (!pAryValue || !PyList_Check(pAryValue)) return false;
		PyObject* pAryDate = PyDict_GetItemString(pResult, "date");
		if (!pAryDate || !PyList_Check(pAryDate)) return false;

		int nCount = (int)PyList_Size(pAryValue);
		int nCount2 = (int)PyList_Size(pAryDate);
		if (nCount != nCount2) return false;

		HistoryDataCache::ARRAY_FINANCE_DATA aryData;
		PyObject* pValue = NULL;
		PyObject* pDate = NULL;
		double dValue = 0;
		int nDate = 0;
		for (long i = 0; i < nCount; ++i)
		{
			pValue = PyList_GetItem(pAryValue, i);
			pDate = PyList_GetItem(pAryDate, i);
			if (!pValue || !pDate) continue;

			dValue = PyFloat_AsDouble(pValue);
			nDate = PyLong_AsLong(pDate);
			if (nDate <= 19700101) continue;
			FINANCE_ITEM fitem = { nDate, dValue };
			aryData.push_back(fitem);
		}

		if (!aryData.empty()) FitFinanceData(out, aryData, 0, false);	//合并数据
		TRACE_DEBUG(L"[HistoryDataCallback::PyDataToVariant] %ls. array src=%d, dest=%d ", strInfo, nCount, out.GetArrayValue().size());
	}

	return true;
}

Variant* HistoryDataCallback::Invoke_GetDataByNumber(const std::wstring& strFunctionName) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumber) return NULL;

	long lKCount = GetKCount();
	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetDataByNumber);
	PyObject* arglist = Py_BuildValue("uuiiiis", m_strSymbol.c_str(), strFunctionName.c_str(), -1, m_lPeriod, m_lRight, lKCount, m_pRunConfig->m_strGuid.c_str());

	PyObject* pResult = pyFunction.Call(arglist);
	if (!pResult || !PyDict_Check(pResult)) return NULL;

	Variant* pVariant = Create();
	if (!PyDataToVariant(strFunctionName, pResult, *pVariant)) return NULL;

	return pVariant;
}

Variant* HistoryDataCallback::Invoke_GetDataByNumber(const std::wstring& strFunctionName, long lValue) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumber) return NULL;

	long lKCount = GetKCount();
	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetDataByNumber);
	PyObject* arglist = Py_BuildValue("uuiiiis", m_strSymbol.c_str(), strFunctionName.c_str(), lValue, m_lPeriod, m_lRight, lKCount, m_pRunConfig->m_strGuid.c_str());

	PyObject* pResult = pyFunction.Call(arglist);
	if (!pResult || !PyDict_Check(pResult)) return NULL;

	std::wstringstream strBuffer;
	strBuffer << strFunctionName << L"(" << lValue << L")";
	std::wstring strInfo = strBuffer.str();

	//wchar_t szInfo[256] = { 0 };
	//wsprintfW(szInfo,256,L"%s(%d)", strFunctionName.c_str(), lValue);

	Variant* pVariant = Create();
	if (!PyDataToVariant(strInfo, pResult, *pVariant)) return NULL;

	return pVariant;
}

Variant* HistoryDataCallback::Invoke_GetDataByName(const std::wstring& strFunctionName) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByName) return NULL;

	long lKCount = GetKCount();
	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetDataByName);
	PyObject* arglist = Py_BuildValue("uuiiis", m_strSymbol.c_str(), strFunctionName.c_str(), m_lPeriod, m_lRight, lKCount, m_pRunConfig->m_strGuid.c_str());
	PyObject* pResult = pyFunction.Call(arglist);
	if (!pResult || !PyDict_Check(pResult)) return NULL;

	Variant* pVariant = Create();
	if (!PyDataToVariant(strFunctionName.c_str(), pResult, *pVariant)) return NULL;

	return pVariant;
}

Variant* HistoryDataCallback::Invoke_GetDataByNumbers(const std::wstring& strFunctionName, const std::vector<double>& aryArgs, const IHistoryData* pHistoryData) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumbers) return NULL;

	long lKCount = GetKCount();
	PyObject* pPyList = PyList_New(0);
	for (int i = 0; i < (int)aryArgs.size(); ++i)
	{
		PyList_Append(pPyList, Py_BuildValue("d", aryArgs[i]));
	}

	PyObject* pPyAryDate = PyList_New(0);
	PyObject* pPyAryTime = PyList_New(0);
	if (pHistoryData)
	{
		const auto& aryKLine= pHistoryData->GetData();
		long lKDataCount = (int)aryKLine.size();
		for (int i = 0; i < lKDataCount; ++i)
		{
			const auto& item = aryKLine[i];

			PyList_Append(pPyAryDate, Py_BuildValue("i", item._nDate));
			PyList_Append(pPyAryTime, Py_BuildValue("i", item._nTime));
		}
	}


	PyObject* pArgs = PyTuple_New(9);
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("u", m_strSymbol.c_str()));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("u", strFunctionName.c_str()));
	PyTuple_SetItem(pArgs, 2, pPyList);
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("i", m_lPeriod));
	PyTuple_SetItem(pArgs, 4, Py_BuildValue("i", m_lRight));
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("i", lKCount));
	PyTuple_SetItem(pArgs, 6, Py_BuildValue("s", m_pRunConfig->m_strGuid.c_str()));
	PyTuple_SetItem(pArgs, 7, pPyAryDate);
	PyTuple_SetItem(pArgs, 8, pPyAryTime);

	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetDataByNumbers);
	PyObject* pResult = pyFunction.Call(pArgs);

	//释放内存
	Py_CLEAR(pPyList);
	Py_CLEAR(pPyAryDate);
	Py_CLEAR(pPyAryTime);
	Py_CLEAR(pArgs);

	Variant* pVariant = Create();
	if (!PyDataToVariant(strFunctionName.c_str(), pResult, *pVariant)) return NULL;

	return pVariant;
}

bool HistoryDataCallback::ArgumentToDoubleArray(const ARRAY_CALL_ARGUMENT& args, std::vector<double>& aryArgs, long lCount) const
{
	if (args.empty()) return false;
	if (lCount != (long)args.size()) return false;

	for (int i = 0; i < lCount; ++i)
	{
		Variant* pParam = args[i];
		if (!pParam->IsVaildDoulbe()) return false;

		aryArgs.push_back(pParam->GetDoubleValue());
	}
	
	return true;
}

Variant* HistoryDataCallback::GetFromOpen() const
{
	return Invoke_GetDataByName(L"FROMOPEN");
}

Variant* HistoryDataCallback::GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	return NULL;
}

Variant* HistoryDataCallback::GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	return NULL;
}

Variant* HistoryDataCallback::CallCustomFunction(const std::wstring& strName, const std::vector<double>& args, const IHistoryData* pHistoryData, Node* pNode) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumbers) return NULL;

	return Invoke_GetDataByNumbers(strName, args, pHistoryData);
}

Variant* HistoryDataCallback::GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumber) return NULL;

	std::vector<double> aryArgs;
	if (!ArgumentToDoubleArray(args, aryArgs, 1))
	{
		std::wstringstream strDescription;
		strDescription << L"GetFinance() 参数错误";
		ExecuteExcept error(strDescription.str(),L"HistoryDataCallback::GetFinance", pNode);
		throw error;
	}

	return Invoke_GetDataByNumber(L"FINANCE", (long)aryArgs[0]);
}

bool HistoryDataCallback::GetSystemIndex(const std::wstring& strName, ScriptIndex& script) const
{
	const auto& system = ScriptLibrary::GetInstance();
	auto* pFind = system.Get(strName);
	if (!pFind)
	{
		TRACE_WARNING(L"[HistoryDataCallback::GetSystemIndex] can't find '%ls' in system index libary ", strName.c_str());
		return false;
	}

	TRACE_DEBUG(L"[HistoryDataCallback::GetSystemIndex] find '%ls' in system index libary ", strName.c_str());
	script = *pFind;
	return true;
}

bool HistoryDataCallback::GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const
{
	if (!m_pRunConfig) return false;
	if (!m_pRunConfig->m_pGetIndexScript) return GetSystemIndex(strName, script);

	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetIndexScript);
	PyObject* arglist = Py_BuildValue("uus", strName.c_str(), strCallInfo.c_str(), m_pRunConfig->m_strGuid.c_str());
	PyObject* pResult = pyFunction.Call(arglist);

	if (!pResult || pResult == Py_None) return GetSystemIndex(strName, script);

	if (!PyUnicode_Check(pResult)) return false;

	const char* pStrConfig = PyUnicode_AsUTF8AndSize(pResult, NULL);
	rapidjson::Document root;
	root.Parse(pStrConfig);
	if (root.IsNull()) return false;

	std::wstring strValue;
	if (!root.HasMember("Name") || !root["Name"].IsString()) return false;
	if (!root.HasMember("Script") || !root["Script"].IsString()) return false;

	strValue = UTF8ToWString(root["Name"].GetString());	//指标名称
	script.SetName(strValue);

	strValue = UTF8ToWString(root["Script"].GetString());	//指标脚本
	script.SetCode(strValue);
	
	//参数
	if (root.HasMember("Args") && root["Args"].IsArray())
	{
		const Value& jsArgs = root["Args"];
		long lCount = (long)jsArgs.Size();
		double dValue = 0;
		Execute::ARRAY_ARGUMENT aryArgs;
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

	/*
	if (!pResult || !PyDict_Check(pResult)) return false;

	PyObject* pName = PyDict_GetItemString(pResult, "Name");
	PyObject* pScript = PyDict_GetItemString(pResult, "Script");

	if (!pName || !PyUnicode_Check(pName)) return false;
	if (!pScript || !PyUnicode_Check(pScript)) return false;

	std::wstring strValue;
	strValue = PyUnicode_AsWideCharString(pName, NULL);
	script.SetName(strValue);
	strValue= PyUnicode_AsWideCharString(pScript, NULL);
	script.SetCode(strValue);

	PyObject* pAryValue = PyDict_GetItemString(pResult, "Args");
	if (pAryValue && PyList_Check(pAryValue))
	{
		Execute::ARRAY_ARGUMENT aryArgs;
		int nCount = (int)PyList_Size(pAryValue);

		PyObject* pItem = NULL;
		PyObject* pValue = NULL;
		PyObject* pName = NULL;
		double dValue = 0;
		for (long i = 0; i < nCount; ++i)
		{
			pItem = PyList_GetItem(pAryValue, i);
			if (!pItem || !PyDict_Check(pItem)) return false;

			pName = PyDict_GetItemString(pItem, "Name");
			pValue= PyDict_GetItemString(pItem, "Value");

			if (!pValue || !pName) return false;

			strValue = PyUnicode_AsWideCharString(pName, NULL);
			dValue = PyFloat_AsDouble(pValue);

			ARGUMENT_ITEM argItem;
			argItem._dValue = dValue;
			if (strValue.size() >= ARRAYSIZE(argItem._szName))
			{
				std::wstringstream strBuffer;
				strBuffer << L"the length of name > 32.  var:" << strValue;
				//if (pFailed) pFailed(L"", L"", strBuffer.str().c_str(), runConfig._strGuid.c_str());
				return false;
			}

			wcscpy(argItem._szName, strValue.c_str());
			aryArgs.push_back(argItem);
		}

		script.SetArgument(aryArgs);
	}
	*/

	return true;
}

void HistoryDataCallback::FitFinanceData(Variant& out, const ARRAY_FINANCE_DATA& fData) const
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

	return;
}

void HistoryDataCallback::FitFinanceData(Variant& out, const ARRAY_FINANCE_DATA& fData, double dDefault, bool bExactMatch) const
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

Variant* HistoryDataCallback::GetExchange(Node* pNode) const
{
	return Invoke_GetDataByName(L"EXCHANGE");
}

//最新流通股
Variant* HistoryDataCallback::GetCapital(Node* pNode) const
{
	return Invoke_GetDataByName(L"CAPITAL");
}

//历史所有的流通股
Variant* HistoryDataCallback::GetHisCapital(Node* pNode) const
{
	return Invoke_GetDataByName(L"GetHisCapital");
}

Variant* HistoryDataCallback::GetTotalCaptialAll(Node* pNode) const
{
	return Invoke_GetDataByName(L"TOTALCAPITAL");
}

Variant* HistoryDataCallback::GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetDataByNumber) return NULL;

	std::vector<double> aryArgs;
	if (!ArgumentToDoubleArray(args, aryArgs, 1))
	{
		std::wstringstream strDescription;
		strDescription << L"DYNAINFO() 参数错误.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::GetDynaInfo", pNode);
		throw error;
	}

	return Invoke_GetDataByNumber(L"DYNAINFO", (long)aryArgs[0]);
}

bool HistoryDataCallback::LoadIndexData(const std::wstring& strCallInfo, Node* pNode) const
{
	if (!m_pRunConfig || !m_pRunConfig->m_pGetKData2) return false;

	const std::wstring strIndexSymbol(L"000001.Sh");
	PyObject* pPyDictDataInfo = PyDict_New();	//字典类型
	PyDict_SetItemString(pPyDictDataInfo, "Period", PyLong_FromLong(m_KDataInfo._lPeriod));
	PyDict_SetItemString(pPyDictDataInfo, "Right", PyLong_FromLong(m_KDataInfo._lRight));
	PyDict_SetItemString(pPyDictDataInfo, "KCount", PyLong_FromLong(m_KDataInfo._lCount));
	PyDict_SetItemString(pPyDictDataInfo, "StartDate", PyLong_FromLong(m_KDataInfo._lStartDate));
	PyDict_SetItemString(pPyDictDataInfo, "StartTime", PyLong_FromLong(m_KDataInfo._lStartTime));
	PyDict_SetItemString(pPyDictDataInfo, "EndDate", PyLong_FromLong(m_KDataInfo._lEndDate));
	PyDict_SetItemString(pPyDictDataInfo, "EndTime", PyLong_FromLong(m_KDataInfo._lEndTime));

	PyObject* pArgs = PyTuple_New(6);
	PyTuple_SetItem(pArgs, 0, Py_BuildValue("u", strIndexSymbol.c_str()));
	PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", m_lPeriod));
	PyTuple_SetItem(pArgs, 2, Py_BuildValue("i", 0));
	PyTuple_SetItem(pArgs, 3, Py_BuildValue("u", strCallInfo.c_str()));
	PyTuple_SetItem(pArgs, 4, pPyDictDataInfo);
	PyTuple_SetItem(pArgs, 5, Py_BuildValue("s", m_pRunConfig->m_strGuid.c_str()));

	PyCallbackFunction pyFunction(m_pRunConfig->m_pGetKData2);
	PyObject* pResult = pyFunction.Call(pArgs);
	ARRAY_KDATA aryData, aryKData;
	std::wstring strName;
	if (!PyDataToKData(strIndexSymbol, pResult, aryData, strName, m_lPeriod, 0))
	{
		if (!m_strError.empty())
		{
			std::wstringstream strDescription;
			strDescription << L"(" << strIndexSymbol << L") 第3方大盘指数K线数据读取失败. 错误信息:" << m_strError;
			ExecuteExcept error(strDescription.str(), L"HistoryDataCallback::LoadIndexData", pNode);
			throw error;
		}

		return false;
	}

	TRACE_DEBUG(L"[HistoryDataCallback::LoadIndexData] load [symbol=%ls, period=%d, right=%d] count=%d", strIndexSymbol.c_str(), m_lPeriod, 0, aryData.size());

	if (IHistoryData::IsDayPeriod(m_lPeriod))
		FitOveralyDayData(m_aryIndex, m_aryData, aryData);
	else if (IHistoryData::IsMinutePeriod(m_lPeriod))
		FitOveralyMinuteData(m_aryIndex, m_aryData, aryData);
	else
		return false;

	return true;

}

Variant* HistoryDataCallback::GetCustomValue(const std::wstring& strName, Node* pNode) const
{
	if (CustomVariant::GetInstance().IsExist(strName))
	{
		return Invoke_GetDataByName(strName);
	}

	return HistoryDataCache::GetCustomValue(strName, pNode);
}

}
}
}