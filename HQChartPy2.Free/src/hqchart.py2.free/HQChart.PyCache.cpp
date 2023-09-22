/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

数据缓存 (c++)
*/

#include "HQChart.PyCache.h"
#include <locale>
#include <codecvt>
#include "HQChart.Log.h"



namespace HQChart { namespace PyCache {


KLineCacheItem::KLineCacheItem()
{

}

KLineCacheItem::~KLineCacheItem()
{

}

bool KLineCacheItem::SetPyData(PyObject* pData)
{
	if (!pData || !PyDict_Check(pData)) return false;

	PyObject* pError = PyDict_GetItemString(pData, "error");
	if (pError)
	{
		std::wstring strError = PyUnicode_AsWideCharString(pError, NULL);
		return false;
	}

	PyObject* pCount = PyDict_GetItemString(pData, "count");
	PyObject* pSymbol = PyDict_GetItemString(pData, "symbol");
	PyObject* pName = PyDict_GetItemString(pData, "name");
	PyObject* pPeriod = PyDict_GetItemString(pData, "period");
	PyObject* pRight = PyDict_GetItemString(pData, "right");

	PyObject* pDate = PyDict_GetItemString(pData, "date");
	PyObject* pTime = PyDict_GetItemString(pData, "time");
	PyObject* pYClose = PyDict_GetItemString(pData, "yclose");
	PyObject* pOpen = PyDict_GetItemString(pData, "open");
	PyObject* pHigh = PyDict_GetItemString(pData, "high");
	PyObject* pLow = PyDict_GetItemString(pData, "low");
	PyObject* pClose = PyDict_GetItemString(pData, "close");

	PyObject* pVol = PyDict_GetItemString(pData, "vol");
	PyObject* pAmount = PyDict_GetItemString(pData, "amount");

	PyObject* pAdvance = PyDict_GetItemString(pData, "advance");
	PyObject* pDecline = PyDict_GetItemString(pData, "decline");

	PyObject* pPosition = PyDict_GetItemString(pData, "position");	//持仓
	PyObject* pSettle = PyDict_GetItemString(pData, "settle");		//结算价

	if (!pDate || !pYClose || !pOpen || !pHigh || !pLow || !pClose || !pVol || !pAmount || !pCount || !pPeriod || !pRight) return false;
	if (!PyList_Check(pDate) || !PyList_Check(pYClose) || !PyList_Check(pOpen) || !PyList_Check(pHigh) || !PyList_Check(pLow) || !PyList_Check(pClose) ||
		!PyList_Check(pVol) || !PyList_Check(pAmount)) return false;

	//可选数据
	if (!pTime || !PyList_Check(pTime)) pTime = NULL;
	if (!pAdvance || !PyList_Check(pAdvance)) pAdvance = NULL;
	if (!pDecline || !PyList_Check(pDecline)) pDecline = NULL;
	if (!pName || !PyUnicode_Check(pName)) pName = NULL;
	if (!pSymbol || !PyUnicode_Check(pSymbol)) pSymbol = NULL;
	if (!pPosition || !PyList_Check(pPosition)) pPosition = NULL;
	if (!pSettle || !PyList_Check(pSettle)) pSettle = NULL;

	m_lPeriod = PyLong_AsLong(pPeriod);
	m_lRight = PyLong_AsLong(pRight);
	if (pName) m_strName = PyUnicode_AsWideCharString(pName, NULL);
	m_strSymbol = PyUnicode_AsWideCharString(pSymbol, NULL);

	int nCount = PyLong_AsLong(pCount);
	//设置K线
	HISTORY_ITEM item;
	m_aryData.reserve(nCount);
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
		if (pValue != Py_None) item._dVol = PyFloat_AsDouble(pValue);

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

		m_aryData.push_back(item);
	}


	TRACE_DEBUG(L"[KLineCacheItem::SetPyData] [Symbol=%s] [Period=%d] [Right=%d] [Count=%d] ", m_strSymbol.c_str(),m_lPeriod, m_lRight, m_aryData.size());

	return true;
}

bool KLineCacheItem::UpdatePyData(PyObject* pData)
{
	if (!pData || !PyDict_Check(pData)) return false;

	PyObject* pError = PyDict_GetItemString(pData, "error");
	if (pError)
	{
		std::wstring strError = PyUnicode_AsWideCharString(pError, NULL);
		return false;
	}

	PyObject* pCount = PyDict_GetItemString(pData, "count");

	PyObject* pDate = PyDict_GetItemString(pData, "date");
	PyObject* pTime = PyDict_GetItemString(pData, "time");
	PyObject* pYClose = PyDict_GetItemString(pData, "yclose");
	PyObject* pOpen = PyDict_GetItemString(pData, "open");
	PyObject* pHigh = PyDict_GetItemString(pData, "high");
	PyObject* pLow = PyDict_GetItemString(pData, "low");
	PyObject* pClose = PyDict_GetItemString(pData, "close");

	PyObject* pVol = PyDict_GetItemString(pData, "vol");
	PyObject* pAmount = PyDict_GetItemString(pData, "amount");

	PyObject* pAdvance = PyDict_GetItemString(pData, "advance");
	PyObject* pDecline = PyDict_GetItemString(pData, "decline");

	PyObject* pPosition = PyDict_GetItemString(pData, "position");	//持仓
	PyObject* pSettle = PyDict_GetItemString(pData, "settle");		//结算价

	if (!pDate || !pYClose || !pOpen || !pHigh || !pLow || !pClose || !pVol || !pAmount || !pCount) return false;
	if (!PyList_Check(pDate) || !PyList_Check(pYClose) || !PyList_Check(pOpen) || !PyList_Check(pHigh) || !PyList_Check(pLow) || !PyList_Check(pClose) ||
		!PyList_Check(pVol) || !PyList_Check(pAmount)) return false;

	//可选数据
	if (!pTime || !PyList_Check(pTime)) pTime = NULL;
	if (!pAdvance || !PyList_Check(pAdvance)) pAdvance = NULL;
	if (!pDecline || !PyList_Check(pDecline)) pDecline = NULL;
	if (!pPosition || !PyList_Check(pPosition)) pPosition = NULL;
	if (!pSettle || !PyList_Check(pSettle)) pSettle = NULL;

	int nCount = PyLong_AsLong(pCount);

	HISTORY_ITEM item;
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
		if (pValue != Py_None) item._dVol = PyFloat_AsDouble(pValue);

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

		UpdateData(item);
	}

	return true;
}



//不支持更新历史的,查询耗时间, 只更新最后一条数据和最新数据
void KLineCacheItem::UpdateData(const HISTORY_ITEM& item)
{
	if (m_aryData.empty())
	{
		m_aryData.push_back(item);
		return;
	}

	auto& lastItem = m_aryData[m_aryData.size() - 1];

	if (IHistoryData::IsDayPeriod(m_lPeriod))
	{
		if (lastItem._nDate == item._nDate)
		{
			lastItem = item;
			return;
		}

		if (lastItem._nDate < item._nDate)
		{
			m_aryData.push_back(item);
			return;
		}
	}
	else if (IHistoryData::IsMinutePeriod(m_lPeriod))
	{
		if (lastItem._nDate == item._nDate && lastItem._nTime == item._nTime)
		{
			lastItem = item;
			return;
		}

		if (lastItem._nDate < item._nDate || (lastItem._nDate == item._nDate && lastItem._nTime < item._nTime))
		{
			m_aryData.push_back(item);
			return;
		}
	}
}

KLineCache::KLineCache()
{

}

KLineCache::~KLineCache()
{

}

KLineCache& KLineCache::GetInstance()
{
	static KLineCache instance;
	return instance;
}

KLineCacheItem::REF_PTR KLineCache::GetData(const std::wstring& strSymbol, long lPeriod, long lRight) const
{
	wchar_t szKey[128] = { 0 };
	wprintf(szKey, L"%s-%02d-%02d", strSymbol.c_str(), lPeriod, lRight);

	auto find = m_mapData.find(szKey);
	if (find == m_mapData.end()) return NULL;

	return find->second;
}

bool KLineCache::AddData(PyObject* pData)
{
	KLineCacheItem::REF_PTR pRefKLineData = KLineCacheItem::REF_PTR(new KLineCacheItem);
	if (!pRefKLineData->SetPyData(pData)) return false;

	wchar_t szKey[128] = { 0 };
	wprintf(szKey, L"%s-%02d-%02d", pRefKLineData->GetSymbol().c_str(), pRefKLineData->GetPeriod(), pRefKLineData->GetRight());

	m_mapData[szKey] = pRefKLineData;


	return true;
}

bool KLineCache::UpdateData(PyObject* pData)
{
	if (!pData || !PyDict_Check(pData)) return false;

	PyObject* pError = PyDict_GetItemString(pData, "error");
	if (pError)
	{
		std::wstring strError = PyUnicode_AsWideCharString(pError, NULL);
		return false;
	}

	PyObject* pSymbol = PyDict_GetItemString(pData, "symbol");
	PyObject* pPeriod = PyDict_GetItemString(pData, "period");
	PyObject* pRight = PyDict_GetItemString(pData, "right");

	if (!pRight || !pSymbol || !pPeriod) return false;

	long lPeriod = PyLong_AsLong(pPeriod);
	long lRight = PyLong_AsLong(pRight);
	std::wstring strSymbol = PyUnicode_AsWideCharString(pSymbol, NULL);

	wchar_t szKey[128] = { 0 };
	wprintf(szKey, L"%s-%02d-%02d", strSymbol.c_str(), lPeriod, lRight);

	auto find = m_mapData.find(szKey);
	if (find == m_mapData.end()) return false;

	return find->second->UpdatePyData(pData);
}


}
}