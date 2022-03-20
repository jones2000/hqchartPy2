/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	股票数据创建类 (c++)
*/

#include <fstream>
#include "HQChart.KLineData.h"
#include "HQChart.data.h"
#include "HQChart.Complier.h"

namespace HQChart { namespace Complier {


DataCreateFactory::DataCreateFactory()
{

}

DataCreateFactory::~DataCreateFactory()
{

}

DataCreateFactory& DataCreateFactory::GetInstance()
{
	static DataCreateFactory instance;
	return instance;
}


bool DataCreateFactory::RegisterHistoryData(const std::wstring& strClassName, pCreateHistoryData pProcCreate, pDeleteHistoryData pProcDelete)
{
	CREATE_ITEM item;
	item._strName = strClassName;
	item._pProcCreate = pProcCreate;
	item._pProcDelete = pProcDelete;

	m_mapCreateData[item._strName] = item;

	return true;
}

IHistoryData* DataCreateFactory::CreateHistoryData(const std::wstring& strClassName, const std::wstring& strSymbol, long lPeriod, long lRight)
{
	auto find=m_mapCreateData.find(strClassName);
	if (find == m_mapCreateData.end()) return NULL;

	return find->second._pProcCreate(strSymbol, lPeriod, lRight);
}

bool DataCreateFactory::DeleteHistoryData(const std::wstring& strClassName, IHistoryData*& pHistoryData)
{
	auto find = m_mapCreateData.find(strClassName);
	if (find == m_mapCreateData.end()) return false;

	find->second._pProcDelete(pHistoryData);
	return true;
}




////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

AutoPtrHistoryData::AutoPtrHistoryData(const std::wstring& strClassName)
	:m_strClassName(strClassName), m_pHistoryData(NULL)
{
	
}

AutoPtrHistoryData::~AutoPtrHistoryData()
{
	Release();
}

void AutoPtrHistoryData::Release()
{
	if (m_pHistoryData) DataCreateFactory::GetInstance().DeleteHistoryData(m_strClassName, m_pHistoryData);
}

IHistoryData* AutoPtrHistoryData::Create(const std::wstring& strSymbol, long lPeriod, long lRight)
{
	Release();
	m_pHistoryData = DataCreateFactory::GetInstance().CreateHistoryData(m_strClassName, strSymbol, lPeriod, lRight);
	return m_pHistoryData;
}

}
}