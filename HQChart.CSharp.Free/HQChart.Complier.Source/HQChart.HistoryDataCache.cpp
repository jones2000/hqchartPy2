/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	股票数据类 (c++)
*/


#include "HQChart.KLineData.h"
#include "HQChart.data.h"
#include "HQChart.Log.h"
#include "HQChart.Execute.h"
#include <sstream>


namespace HQChart {
	namespace Complier {
#ifdef _WIN32
#define HISTORYDATACACHE_FUNCTIONS(__funcname__, __field__)\
	Variant* HistoryDataCache::##__funcname__##() const \
	{ \
	Variant* pResult = Create(); \
	size_t nDataCount = m_aryData.size(); \
	auto& aryValue = pResult->GetArrayValue(); \
	aryValue.resize(nDataCount); \
	for (size_t i = 0; i < nDataCount; ++i) \
							{\
		const auto& item = m_aryData[i];\
		aryValue[i].SetValue(item.##__field__##);\
							}\
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);\
	return pResult;\
	}
#else
#define HISTORYDATACACHE_FUNCTIONS(__funcname__, __field__)\
	Variant* HistoryDataCache::__funcname__() const \
	{ \
	Variant* pResult = Create(); \
	size_t nDataCount = m_aryData.size(); \
	auto& aryValue = pResult->GetArrayValue(); \
	aryValue.resize(nDataCount); \
	for (size_t i = 0; i < nDataCount; ++i) \
							{\
		const auto& item = m_aryData[i];\
		aryValue[i].SetValue(item.__field__);\
							}\
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);\
	return pResult;\
	}
#endif


HistoryDataCache::HistoryDataCache(const std::wstring& strSymbol, long lPeriod, long lRight)
	:m_strSymbol(strSymbol), m_lPeriod(lPeriod), m_lRight(lRight)
{
	m_strClassName = L"HistoryDataCache";
	memset(&m_KDataInfo, 0, sizeof(m_KDataInfo));
}

HistoryDataCache::~HistoryDataCache()
{

}

HISTORYDATACACHE_FUNCTIONS(GetClose, _dClose);
HISTORYDATACACHE_FUNCTIONS(GetOpen, _dOpen);
HISTORYDATACACHE_FUNCTIONS(GetHigh, _dHigh);
HISTORYDATACACHE_FUNCTIONS(GetLow, _dLow);
HISTORYDATACACHE_FUNCTIONS(GetAmount, _dAmount);
HISTORYDATACACHE_FUNCTIONS(GetAvPrice, _dYClose);
HISTORYDATACACHE_FUNCTIONS(GetSettle, _dSettle);
HISTORYDATACACHE_FUNCTIONS(GetPosition, _dPosition);

/*
Variant* HistoryDataCache::GetClose() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		aryValue[i].SetValue(item._dClose);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}
*/

Variant* HistoryDataCache::GetKDataItem(const IHistoryData::ARRAY_KDATA& aryData, const std::wstring& strVarName) const
{
	Variant* pResult = NULL;
	if (strVarName == L"C" || strVarName==L"CLOSE")
	{
		pResult = Create();
		size_t nDataCount = aryData.size();
		auto& aryValue = pResult->GetArrayValue(); 
		aryValue.resize(nDataCount);
		for (size_t i = 0; i < nDataCount; ++i) 
		{
			const auto& item = aryData[i]; 
			aryValue[i].SetValue(item._dClose);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (strVarName == L"O" || strVarName == L"OPEN")
	{
		pResult = Create();
		size_t nDataCount = aryData.size();
		auto& aryValue = pResult->GetArrayValue();
		aryValue.resize(nDataCount);
		for (size_t i = 0; i < nDataCount; ++i)
		{
			const auto& item = aryData[i];
			aryValue[i].SetValue(item._dOpen);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (strVarName == L"H" || strVarName == L"HIGH")
	{
		pResult = Create();
		size_t nDataCount = aryData.size();
		auto& aryValue = pResult->GetArrayValue();
		aryValue.resize(nDataCount);
		for (size_t i = 0; i < nDataCount; ++i)
		{
			const auto& item = aryData[i];
			aryValue[i].SetValue(item._dHigh);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (strVarName == L"L" || strVarName == L"LOW")
	{
		pResult = Create();
		size_t nDataCount = aryData.size();
		auto& aryValue = pResult->GetArrayValue();
		aryValue.resize(nDataCount);
		for (size_t i = 0; i < nDataCount; ++i)
		{
			const auto& item = aryData[i];
			aryValue[i].SetValue(item._dLow);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (strVarName == L"AMO" || strVarName == L"AMOUNT")
	{
		pResult = Create();
		size_t nDataCount = aryData.size();
		auto& aryValue = pResult->GetArrayValue();
		aryValue.resize(nDataCount);
		for (size_t i = 0; i < nDataCount; ++i)
		{
			const auto& item = aryData[i];
			aryValue[i].SetValue(item._dAmount);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (strVarName == L"V" || strVarName == L"VOL")
	{
		pResult = Create();
		size_t nDataCount = m_aryData.size();
		auto& aryValue = pResult->GetArrayValue();
		aryValue.resize(nDataCount);
		for (size_t i = 0; i < nDataCount; ++i)
		{
			const auto& item = m_aryData[i];
			aryValue[i].SetValue(item._dVol / 100);	//转成手
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		return pResult;
	}

	return pResult;
}



/*
	取得该周期从1900以来的的年月日.
	用法:
	DATE
	例如函数返回1000101,表示2000年1月1日,DATE+19000000后才是真正的日期值
*/
Variant* HistoryDataCache::GetDate() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		aryValue[i].SetValue(item._nDate - 19000000);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}



Variant* HistoryDataCache::GetVol() const
{ 
	Variant* pResult = Create(); 
	size_t nDataCount = m_aryData.size(); 
	auto& aryValue = pResult->GetArrayValue(); 
	aryValue.resize(nDataCount); 
	for (size_t i = 0; i < nDataCount; ++i) 
	{
		const auto& item = m_aryData[i];
		aryValue[i].SetValue(item._dVol/100);	//转成手
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetTime() const 
{ 
	Variant* pResult = Create(); 
	size_t nDataCount = m_aryData.size(); 
	bool isMinutePeriod = IsMinutePeriod();
	auto& aryValue = pResult->GetArrayValue(); 
	aryValue.resize(nDataCount); 
	for (size_t i = 0; i < nDataCount; ++i) 
	{
		const auto& item = m_aryData[i];
		
		if (isMinutePeriod) aryValue[i].SetValue(item._nTime/100);	//分钟周期 去掉秒
		else aryValue[i].SetValue(item._nTime);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetMonth() const
{
	Variant* pResult = Create(); 
	size_t nDataCount = m_aryData.size(); 
	auto& aryValue = pResult->GetArrayValue(); 
	aryValue.resize(nDataCount); 
	long lMonth = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._nDate <= 0) continue;
		lMonth = (int)((item._nDate % 10000) / 100);
		aryValue[i].SetValue(lMonth);
	}
							
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetYear() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lYear = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._nDate <= 0) continue;
		lYear = (int)(item._nDate / 10000);
		aryValue[i].SetValue(lYear);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetDay() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lDay = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._nDate <= 0) continue;
		lDay = (int)(item._nDate % 100);
		aryValue[i].SetValue(lDay);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetWeekDay() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lWeek = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._nDate <= 0) continue;
		lWeek = GetWeek(item._nDate);
		aryValue[i].SetValue(lWeek);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetHour() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lHour = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._nTime < 0) continue;
		lHour = (int)(item._nTime / 100);
		aryValue[i].SetValue(lHour);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}


Variant* HistoryDataCache::GetMinute() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lMinute = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._nTime < 0) continue;
		lMinute = item._nTime % 100;
		aryValue[i].SetValue(lMinute);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetAdvance() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lMinute = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		aryValue[i].SetValue(item._nAdvance);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetDecline() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	long lMinute = 0;
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		aryValue[i].SetValue(item._nDecline);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetCurrBarsCount() const
{
	Variant* pResult = Create();
	long lDataCount = (long)m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(lDataCount);
	for (long i = lDataCount - 1, j=0; i >= 0;--i,++j)
	{
		aryValue[j].SetValue(i);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetBarPos() const
{
	Variant* pResult = Create();
	long lDataCount = (long)m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(lDataCount);
	for (long i = 0;i< lDataCount; ++i)
	{
		aryValue[i].SetValue(i+1);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetTotalBarsCount() const
{
	Variant* pResult = Create();
	long lDataCount = (long)m_aryData.size();
	pResult->SetDoubleValue(lDataCount);
	return pResult;
}

Variant* HistoryDataCache::GetFromOpen(Node* pNode) const
{
	Variant* pResult = Create();
	pResult->SetDoubleValue(0);
	return pResult;
}

Variant* HistoryDataCache::GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	Variant* pResult = Create();
	return pResult;
}

Variant* HistoryDataCache::GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	Variant* pResult = Create();
	return pResult;
}

Variant* HistoryDataCache::GetName() const
{
	Variant* pResult = Create();
	pResult->SetStringValue(m_strName);
	return pResult;
}

Variant* HistoryDataCache::GetSymbol(long lType) const
{
	Variant* pResult = Create();
	if (lType == 1)
	{
		std::wstring strSymbol;
		for (auto& item : m_strSymbol)
		{
			if (item == L'.') break;
			strSymbol.push_back(item);
		}

		pResult->SetStringValue(strSymbol);
	}
	else
	{
		pResult->SetStringValue(m_strSymbol);
	}
	return pResult;
}

Variant* HistoryDataCache::GetCategroyName(long lType) const
{
	if (m_aryCategory.empty())
	{
		LoadCategoryData();
	}

	std::wstring strCategroy;
	for (auto& item : m_aryCategory)
	{
		if (!strCategroy.empty()) strCategroy += L";";

		if (lType == -1 || lType == item._lType)
			strCategroy += item._strName;
	}

	Variant* pResult = Create();
	pResult->SetStringValue(strCategroy);
	return pResult;
}

Variant* HistoryDataCache::IsInCategroy(long lType, const std::wstring& strName) const
{
	if (m_aryCategory.empty())
	{
		LoadCategoryData();
	}

	bool bFind = false;
	for (auto& item : m_aryCategory)
	{
		if (lType == -1)
		{
			if (strName.compare(item._strName) == 0)
			{
				bFind = true;
				break;
			}
		}
		else if (lType == item._lType)
		{
			if (strName.compare(item._strName) == 0)
			{
				bFind = true;
				break;
			}
		}
	}

	Variant* pResult = Create();
	pResult->SetDoubleValue(bFind ? 1 : 0);
	return pResult;
}

Variant* HistoryDataCache::GetMarketName() const
{
	std::wstring strName = L"unknow";
	if (m_strSymbol.find(L".sh") || m_strSymbol.find(L".SH"))
	{
		strName = L"SH";
	}

	if (m_strSymbol.find(L".sz") || m_strSymbol.find(L".SZ"))
	{
		strName = L"SZ";
	}

	Variant* pResult = Create();
	pResult->SetStringValue(strName);
	return pResult;
}

Variant* HistoryDataCache::GetMarketCNName() const
{
	std::wstring strName = L"unknow";
	if (m_strSymbol.find(L".sh") || m_strSymbol.find(L".SH"))
	{
		strName = L"上海证券交易所";
	}

	if (m_strSymbol.find(L".sz") || m_strSymbol.find(L".SZ"))
	{
		strName = L"深圳证券交易所";
	}

	Variant* pResult = Create();
	pResult->SetStringValue(strName);
	return pResult;
}

Variant* HistoryDataCache::GetPeriodID() const
{
	//分时线=1,分笔成交=2, 1分钟线=3, 5分钟线=4, 15分钟线=5, 30分钟线=6, 60分钟线=7, 日线=8, 周线=9, 月线=10, 多日线=11, 年线=12, 季线=13, 半年线=14。
	const std::map<long,long> MAP_PERIOD=
	{
		{ PERIOD_TYPE_ID::PERIOD_TICK_ID ,2},
		{ PERIOD_TYPE_ID::PERIOD_MIN1_ID, 3 },
		{ PERIOD_TYPE_ID::PERIOD_MIN5_ID, 4 },
		{ PERIOD_TYPE_ID::PERIOD_MIN15_ID, 5 },
		{ PERIOD_TYPE_ID::PERIOD_MIN30_ID, 6 },
		{ PERIOD_TYPE_ID::PERIOD_MIN60_ID, 7 },
		{ PERIOD_TYPE_ID::PERIOD_DAY_ID, 8 }, 
		{ PERIOD_TYPE_ID::PERIOD_WEEK_ID, 9 },
		{ PERIOD_TYPE_ID::PERIOD_MONTH_ID, 10 },
		{ PERIOD_TYPE_ID::PERIOD_YEAR_ID, 12 },
		{ PERIOD_TYPE_ID::PERIOD_QUARTER_ID, 13 },
	};
	Variant* pResult = Create();
	auto find = MAP_PERIOD.find(m_lPeriod);
	if (find != MAP_PERIOD.end()) pResult->SetDoubleValue(find->second);

	return pResult;
}

//TYPE	股票类别
//阐释：TYPE，返回类别 : 指数为0；A股为1；B股为2；债券为3；基金为4；选择权为5; 外汇为6; 期货为7; 期指为8; 认购证为9; ETF为10; LOF为11; 可转债为12; 信托为13; 权证为14; 回购为15.
Variant* HistoryDataCache::GetSymbolTypeID() const
{
	Variant* pResult = Create();
	if (IsSHSZIndex()) pResult->SetDoubleValue(0);
	else if (IsSHSZA()) pResult->SetDoubleValue(1);
	
	return pResult;
}

Variant* HistoryDataCache::GetDrawNULL() const
{
	Variant* pResult = Create();
	if (m_aryData.empty()) return pResult;

	int nCount = (int)m_aryData.size();
	pResult->GetArrayValue().resize(nCount, VARIANT_ITEM());
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);

	return pResult;
}

bool HistoryDataCache::GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const
{
	return false;
}

Variant* HistoryDataCache::RPCCall(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	return NULL;
}


const HISTORY_ITEM* HistoryDataCache::GetKItem(int nIndex) const
{
	if (nIndex < 0 || nIndex >= (int)m_aryData.size()) return NULL;

	return &m_aryData[nIndex];
}

long HistoryDataCache::GetKCount() const
{
	return (long)m_aryData.size();
}

const bool HistoryDataCache::GetKMaxMin(double& dMax, double& dMin) const
{
	if (m_aryData.empty()) return false;

	dMax = m_aryData[0]._dHigh;
	dMin = m_aryData[0]._dLow;

	long lCount = (long)m_aryData.size();
	for (long i = 1; i < lCount; ++i)
	{
		const auto& item = m_aryData[i];
		dMax = std::max(dMax, item._dHigh);
		dMin = std::min(dMin, item._dLow);
	}

	return true;
}

const bool HistoryDataCache::GetKMaxMin(long lStart, long lEnd, double& dMax, double& dMin) const
{
	if (m_aryData.empty()) return false;
	if (lStart < 0 || lEnd < 0) return false;

	dMax = m_aryData[lStart]._dHigh;
	dMin = m_aryData[lStart]._dLow;

	long lCount = (long)m_aryData.size();
	for (long i = lStart + 1; i < lCount && i<lEnd; ++i)
	{
		const auto& item = m_aryData[i];
		dMax = std::max(dMax, item._dHigh);
		dMin = std::min(dMin, item._dLow);
	}

	return true;
}

long HistoryDataCache::FindByDate(long lDate) const
{
	long lResult = -1;

	for (long i = 0; i < (long)m_aryData.size();++i)
	{
		const auto& item = m_aryData[i];
		if (item._nDate == lDate)
		{
			lResult = i;
			break;
		}
	}

	return lResult;
}

Variant* HistoryDataCache::GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	if (args.size() <= 0 || !args[0])
	{
		std::wstringstream strDescription;
		strDescription << L"GetFinance() 参数错误";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCache::GetFinance", pNode);
		throw error;
	}

	Variant* pParam = args[0];
	if (!pParam->IsVaildDoulbe())
	{
		std::wstringstream strDescription;
		strDescription << L"GetFinance() 参数无效";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCache::GetFinance", pNode);
		throw error;
	}

	Variant* pResult = Create();
	long lValue = (long)pParam->GetDoubleValue();	//取对应的财务数据
	if (!LoadFinance(lValue)) return pResult;

	ARRAY_EXTEND_DATA aryExtendData;
	FitExtendData(aryExtendData, m_aryData, m_aryFinance);	//合并数据

	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();
	dest.resize(aryExtendData.size(), VARIANT_ITEM());
	for (long i = 0; i < (long)aryExtendData.size(); ++i)
	{
		const auto& item = aryExtendData[i];
		dest[i].SetValue(item._dValue);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

//加载财务数据
bool HistoryDataCache::LoadFinance(long lType) const	
{
	m_aryFinance.clear();
	return m_aryFinance.size() > 0;
}

bool HistoryDataCache::LoadDynainfo(long lType) const
{
	return false;
}

bool HistoryDataCache::LoadIndexData(const std::wstring& strName, Node* pNode) const
{
	return false;
}

bool HistoryDataCache::LoadCategoryData() const
{
	return false;
}

bool HistoryDataCache::LoadKData()
{
	return false;
}

bool HistoryDataCache::LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo)
{
	return false;
}

void HistoryDataCache::Initialization(void* pData)
{

}

Variant* HistoryDataCache::GetExchange(Node* pNode) const
{
	Variant* pResult = Create();
	if (!LoadFinance(FINANCE_ID::FLOW_EQUITY_ID)) return pResult;

	ARRAY_EXTEND_DATA aryExtendData;
	FitExtendData(aryExtendData, m_aryData, m_aryFinance);	//财务数据和K线数据拟合

	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();
	dest.resize(aryExtendData.size(), VARIANT_ITEM());
	for (int i = 0; i < (int)aryExtendData.size(); ++i)
	{
		const auto& kItem = m_aryData[i];
		const auto& item = aryExtendData[i];
		if (item._dValue != 0)
			dest[i].SetValue(kItem._dVol / item._dValue * 100);	//计算换手率 成交量/流通股本*100
		else
			dest[i].SetValue(0);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

//CAPITAL  当前流通股本 单位手
Variant* HistoryDataCache::GetCapital(Node* pNode) const
{
	Variant* pResult = Create();
	if (!LoadFinance(FINANCE_ID::FLOW_EQUITY_ID)) return pResult;

	auto& lastData = m_aryFinance.back();	//FINANCE(7) 单位股

	pResult->SetDoubleValue((long)(lastData._dValue / 100)); //和通达信保持一致 去掉小数部分.
	return pResult;
}

Variant* HistoryDataCache::GetTotalCaptialAll(Node* pNode) const
{
	Variant* pResult = Create();
	return pResult;
}

Variant* HistoryDataCache::GetHisCapital(Node* pNode) const
{
	Variant* pResult = Create();
	if (!LoadFinance(FINANCE_ID::FLOW_EQUITY_ID)) return pResult;

	ARRAY_EXTEND_DATA aryExtendData;
	FitExtendData(aryExtendData, m_aryData, m_aryFinance);	//合并数据

	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();
	dest.resize(aryExtendData.size(), VARIANT_ITEM());
	for (long i = 0; i < (long)aryExtendData.size(); ++i)
	{
		const auto& item = aryExtendData[i];
		dest[i].SetValue(item._dValue);
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const
{
	if (args.size() <= 0 || !args[0])
	{
		std::wstringstream strDescription;
		strDescription << L"DYNAINFO() 参数错误.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCache::GetDynaInfo", pNode);
		throw error;
	}

	Variant* pParam = args[0];
	if (!pParam->IsVaildDoulbe())
	{
		std::wstringstream strDescription;
		strDescription << L"DYNAINFO() 参数无效.";
		ExecuteExcept error(strDescription.str(), L"HistoryDataCache::GetDynaInfo", pNode);
		throw error;
	}

	Variant* pResult = Create();
	long lValue = (long)pParam->GetDoubleValue();
	auto find = m_mapDynainfo.find(lValue);
	if (find == m_mapDynainfo.end())
	{
		LoadDynainfo(lValue);
		find = m_mapDynainfo.find(lValue);
	}

	if (find != m_mapDynainfo.end())
		pResult->SetDoubleValue(find->second);
	
	return pResult;
}

Variant* HistoryDataCache::GetIndexData(const std::wstring& strName, Node* pNode) const
{
	if (m_aryIndex.empty()) LoadIndexData(strName,pNode);
	Variant* pResult = Create();
	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();

	long lCount = (long)m_aryIndex.size();
	dest.resize(lCount, VARIANT_ITEM());
	if (strName == L"INDEXA")
	{
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = m_aryIndex[i];
			dest[i].SetValue(item._dAmount);
		}
	}
	else if (strName == L"INDEXC")
	{
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = m_aryIndex[i];
			dest[i].SetValue(item._dClose);
		}
	}
	else if (strName == L"INDEXH")
	{
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = m_aryIndex[i];
			dest[i].SetValue(item._dHigh);
		}
	}
	else if (strName == L"INDEXL")
	{
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = m_aryIndex[i];
			dest[i].SetValue(item._dLow);
		}
	}
	else if (strName == L"INDEXO")
	{
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = m_aryIndex[i];
			dest[i].SetValue(item._dOpen);
		}
	}
	else if (strName == L"INDEXV")
	{
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = m_aryIndex[i];
			dest[i].SetValue(item._dVol);
		}
	}
	else
	{
		return pResult;
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE); 
	return pResult;
}

Variant* HistoryDataCache::GetCustomValue(const std::wstring& strName, Node* pNode) const
{
	if (strName == L"ISDOWN") return GetIsPriceDown();
	else if (strName == L"ISEQUAL") return GetIsPriceEqual();
	else if (strName == L"ISUP") return GetIsPriceUp();
	else if (strName == L"ISLASTBAR") return GetIsLastBar();
	else if (strName == L"BARSTATUS") return GetBarStatus();


	std::wstringstream strDescription;
	strDescription << L"变量不存在. name=" << strName;
	ExecuteExcept error(strDescription.str(), L"HistoryDataCache::GetCustomValue", pNode);
	throw error;

	return NULL;
}

Variant* HistoryDataCache::CallCustomFunction(const std::wstring& strName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode) const
{
	std::wstringstream strDescription;
	strDescription << L"函数不存在. name=" << strName;
	ExecuteExcept error(strDescription.str(), L"HistoryDataCache::CallCustomFunction", pNode);
	throw error;

	return NULL;
}

const IHistoryData::ARRAY_KDATA* HistoryDataCache::GetKData(const std::wstring& strSymbol, long lPeriod, long lRight) const
{
	if (m_mapKData.empty()) return NULL;

	std::wstringstream strBuffer;
	strBuffer << strSymbol << L"-" << lPeriod << L"-" << lRight;	//key=代码-周期-复权
	std::wstring strKey = strBuffer.str();

	auto find = m_mapKData.find(strKey);
	if (find == m_mapKData.end()) return NULL;

	return &find->second;
}

Variant* HistoryDataCache::GetIsPriceDown() const
{
	Variant* pResult = Create();
	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();

	long lCount = (long)m_aryData.size();
	dest.resize(lCount, VARIANT_ITEM());
	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._dClose <= 0 || item._dOpen <= 0) continue;

		if (item._dClose < item._dOpen) dest[i].SetValue(1);
		else dest[i].SetValue(0);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetIsPriceUp() const
{
	Variant* pResult = Create();
	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();

	long lCount = (long)m_aryData.size();
	dest.resize(lCount, VARIANT_ITEM());
	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._dClose <= 0 || item._dOpen <= 0) continue;

		if (item._dClose > item._dOpen) dest[i].SetValue(1);
		else dest[i].SetValue(0);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetIsPriceEqual() const
{
	Variant* pResult = Create();
	Variant::ARRAY_DOUBLE& dest = pResult->GetArrayValue();

	long lCount = (long)m_aryData.size();
	dest.resize(lCount, VARIANT_ITEM());
	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (item._dClose <= 0 || item._dOpen <= 0) continue;

		if (item._dClose == item._dOpen) dest[i].SetValue(1);
		else dest[i].SetValue(0);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

Variant* HistoryDataCache::GetIsLastBar() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (i == nDataCount - 1) aryValue[i].SetValue(1);
		else aryValue[i].SetValue(0);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

//BARSTATUS返回数据位置信息,1表示第一根K线,2表示最后一个数据,0表示中间位置.
//例如:BARSTATUS=2表示当天是该数据的最后一个周期.
Variant* HistoryDataCache::GetBarStatus() const
{
	Variant* pResult = Create();
	size_t nDataCount = m_aryData.size();
	auto& aryValue = pResult->GetArrayValue();
	aryValue.resize(nDataCount);
	for (size_t i = 0; i < nDataCount; ++i)
	{
		const auto& item = m_aryData[i];
		if (i == 0) aryValue[i].SetValue(1);
		else if (i == nDataCount - 1) aryValue[i].SetValue(2);
		else aryValue[i].SetValue(0);
	}
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

void HistoryDataCache::FitOveralyDayData(ARRAY_OVERLAY_DATA& dest, const ARRAY_KDATA& kData, const ARRAY_KDATA& overalyData)
{
	int nKDataCount = (long)kData.size();
	int nOverlayDataCount = (long)overalyData.size();

	dest.resize(nKDataCount, OVERLAY_HISTORY_ITEM());
	for (int i = 0, j = 0; i < nKDataCount;)
	{
		const auto& kItem = kData[i];
		if (j < nOverlayDataCount)
		{
			const auto& fItem = overalyData[j];
			if (fItem._nDate > kItem._nDate)
			{
				++i;
				continue;
			}
		}


		if (j + 1 < nOverlayDataCount)
		{
			const auto& fItem = overalyData[j];
			const auto& fItem2 = overalyData[j + 1];

			if (fItem._nDate < kItem._nDate && fItem2._nDate <= kItem._nDate)
			{
				++j;
				continue;
			}
		}

		OVERLAY_HISTORY_ITEM item = { };

		item._nDate = kItem._nDate;
		item._nTime = kItem._nTime;
		if (j < nOverlayDataCount)
		{
			const auto& overlayItem = overalyData[j];
			item._nExDate = overlayItem._nDate;
			item._nExTime = overlayItem._nTime;

			item._dClose = overlayItem._dClose;
			item._dHigh = overlayItem._dHigh;
			item._dLow = overlayItem._dLow;
			item._dOpen = overlayItem._dOpen;
			item._dYClose = overlayItem._dYClose;
			item._dAmount = overlayItem._dAmount;
			item._dVol = overlayItem._dVol;
		}
		else
		{
			const auto& overlayItem = overalyData[nOverlayDataCount - 1];
			item._nExDate = overlayItem._nDate;
			item._nExTime = overlayItem._nTime;

			item._dClose = overlayItem._dClose;
			item._dHigh = overlayItem._dHigh;
			item._dLow = overlayItem._dLow;
			item._dOpen = overlayItem._dOpen;
			item._dYClose = overlayItem._dYClose;
			item._dAmount = overlayItem._dAmount;
			item._dVol = overlayItem._dVol;
		}

		dest[i] = item;
		++i;
	}
}

void HistoryDataCache::FitOveralyMinuteData(ARRAY_OVERLAY_DATA& dest, const ARRAY_KDATA& kData, const ARRAY_KDATA& overalyData)
{
	int nKDataCount = (long)kData.size();
	int nOverlayDataCount = (long)overalyData.size();

	dest.resize(nKDataCount, OVERLAY_HISTORY_ITEM());
	for (int i = 0, j = 0; i < nKDataCount;)
	{
		const auto& kItem = kData[i];
		XINT64 lKDateTime = (XINT64)(kItem._nDate) * 10000 + kItem._nTime;

		if (j < nOverlayDataCount)
		{
			const auto& fItem = overalyData[j];
			XINT64 lfDateTime = (XINT64)(fItem._nDate) * 10000 + fItem._nTime;
			if (lfDateTime > lKDateTime)
			{
				++i;
				continue;
			}
		}

		if (j + 1 < nOverlayDataCount)
		{
			const auto& fItem = overalyData[j];
			const auto& fItem2 = overalyData[j + 1];

			XINT64 lfDateTime = (XINT64)(fItem._nDate) * 10000 + fItem._nTime;
			XINT64 lfDateTime2 = (XINT64)(fItem2._nDate) * 10000 + fItem2._nTime;

			if (lfDateTime < lKDateTime && lfDateTime2 <= lKDateTime)
			{
				++j;
				continue;
			}
		}

		OVERLAY_HISTORY_ITEM item = {};

		item._nDate = kItem._nDate;
		item._nTime = kItem._nTime;
		if (j < nOverlayDataCount)
		{
			const auto& overlayItem = overalyData[j];
			item._nExDate = overlayItem._nDate;
			item._nExTime = overlayItem._nTime;

			item._dClose = overlayItem._dClose;
			item._dHigh = overlayItem._dHigh;
			item._dLow = overlayItem._dLow;
			item._dOpen = overlayItem._dOpen;
			//item._dYClose = overlayItem._dYClose;
			item._dAmount = overlayItem._dAmount;
			item._dVol = overlayItem._dVol;
		}
		else
		{
			const auto& overlayItem = overalyData[nOverlayDataCount - 1];
			item._nExDate = overlayItem._nDate;
			item._nExTime = overlayItem._nTime;

			item._dClose = overlayItem._dClose;
			item._dHigh = overlayItem._dHigh;
			item._dLow = overlayItem._dLow;
			item._dOpen = overlayItem._dOpen;
			//item._dYClose = overlayItem._dYClose;
			item._dAmount = overlayItem._dAmount;
			item._dVol = overlayItem._dVol;
		}

		dest[i] = item;
		++i;
	}
}

void HistoryDataCache::FitExtendData(ARRAY_EXTEND_DATA& dest, const ARRAY_KDATA& kData, const ARRAY_FINANCE_DATA& fData)
{
	int nDataCount = (int)kData.size();
	int nFDataCount = (int)fData.size();
	if (nFDataCount <= 0 || nDataCount <= 0) return;

	dest.resize(nDataCount, HISTORY_EXTEND_ITEM());
	for (int i = 0, j = 0; i < nDataCount;)
	{
		const auto& kItem = kData[i];
		if (j < nFDataCount)
		{
			const auto& fItem = fData[j];
			if (kItem._nDate < fItem._nDate)
			{
				++i;
				continue;
			}
		}

		if (j + 1 < nFDataCount)
		{
			const auto& fItem = fData[j];
			const auto& fItem2 = fData[j + 1];

			if (fItem._nDate < kItem._nDate && fItem2._nDate <= kItem._nDate)
			{
				++j;
				continue;
			}
		}

		HISTORY_EXTEND_ITEM item = {};
		item._nDate = kItem._nDate;
		item._nTime = kItem._nTime;
		if (j < nFDataCount)
		{
			const auto& fItem = fData[j];
			item._nExDate = fItem._nDate;
			item._dValue = fItem._dValue;
		}
		else
		{
			const auto& fItem = fData[nFDataCount - 1];
			item._nExDate = fItem._nDate;
			item._dValue = fItem._dValue;
		}

		dest[i] = item;
		++i;
	}
}

void HistoryDataCache::FitExtendData(ARRAY_EXTEND_DATA& dest, const ARRAY_KDATA& kData, long lPeriod, const ARRAY_FINANCE_DATA& fData, double dNUll, bool bExactMatch)
{
	int nDataCount = (int)kData.size();
	int nFDataCount = (int)fData.size();
	if (nFDataCount <= 0 || nDataCount <= 0) return;

	HISTORY_EXTEND_ITEM defaultItem;
	defaultItem._dValue = dNUll;
	dest.resize(nDataCount, defaultItem);

	bool bMatch = false;
	for (int i = 0, j = 0; i < nDataCount;)
	{
		const auto& kItem = kData[i];

		if (j < nFDataCount)
		{
			const auto& fItem = fData[j];
			if (kItem._nDate < fItem._nDate)
			{
				++i;
				continue;
			}
		}

		if (j + 1 < nFDataCount)
		{
			const auto& fItem = fData[j];
			const auto& fItem2 = fData[j + 1];

			if (fItem._nDate < kItem._nDate && fItem2._nDate <= kItem._nDate)
			{
				++j;
				bMatch = false;
				continue;
			}
		}

		HISTORY_EXTEND_ITEM& item = dest[i];
		item._nDate = kItem._nDate;
		item._nTime = kItem._nTime;
		if (j < nFDataCount)
		{
			const auto& fItem = fData[j];
			if ((lPeriod == 0 || IHistoryData::IsMinutePeriod(lPeriod)) && bExactMatch ==true)	//日线,分钟K线完全匹配
			{
				if (fItem._nDate == item._nDate)
				{
					item._nExDate = fItem._nDate;
					item._dValue = fItem._dValue;
					bMatch = true;
				}
			}
			else
			{
				if (bMatch == false)
				{
					item._nExDate = fItem._nDate;
					item._dValue = fItem._dValue;
					bMatch = true;
				}
			}
			
		}

		++i;
	}
}


bool HistoryDataCache::IsMinutePeriod() const
{
	return IHistoryData::IsMinutePeriod(m_lPeriod);
}

bool HistoryDataCache::IsDayPeriod() const
{
	return IHistoryData::IsDayPeriod(m_lPeriod);
}

bool HistoryDataCache::IsTickPeriod() const
{
	return IHistoryData::IsTickPeriod(m_lPeriod);
}

bool HistoryDataCache::IsMinuteChartPeriod() const
{
	return IHistoryData::IsMinuteChartPeriod(m_lPeriod);
}


int HistoryDataCache::GetWeek(int nDate)
{
	int nWeek = 0;
	int nYear = 0, nMonth = 0, nDay = 0;
	nYear = (int)(nDate / 10000);
	nMonth =(int)((nDate % 10000) / 100);
	nDay = nDate % 100;

	const int DAYS[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int nAllDays = 0;
	for (int i=1980; i < nYear; ++i)
	{
		nAllDays += (((i % 400) == 0 || (i % 4) == 0 && (i % 100) != 0) ? 366 : 365);
	}

	for (int i = 1; i < nMonth; ++i)
	{
		nAllDays += DAYS[i - 1];
		if (i == 2 && ((nYear % 400) == 0 || ((nYear % 4) == 0 && (nYear % 100) != 0))) nAllDays++;
	}


	nWeek = ((nAllDays + nDay + 1) % 7);// 1980年1月1日为星期二,从1980年至当日共有多少天
	return nWeek;
}

bool HistoryDataCache::IsSH() const
{
	if (m_strSymbol.find(L"\\.SH") != std::wstring::npos && m_strSymbol.find(L"\\.sh") != std::wstring::npos) return false;

	return true;
}

bool HistoryDataCache::IsSZ() const
{
	if (m_strSymbol.find(L"\\.SZ") != std::wstring::npos && m_strSymbol.find(L"\\.sz") != std::wstring::npos) return false;

	return true;
}

bool HistoryDataCache::IsSHSZA() const
{
	if (m_strSymbol.size() != 9) return false;
	if (IsSH())
	{
		if (m_strSymbol[0] == L'6') return true;
	}
	else if (IsSZ())
	{
		if (m_strSymbol[0] == L'0')
		{
			if (m_strSymbol[1] == L'0' && m_strSymbol[2] == L'2') return true;  //002 中小板
			if (m_strSymbol[1] != L'7' && m_strSymbol[1] != L'8') return true;
		}
	}

	return false;
}

bool HistoryDataCache::IsSHSZIndex() const
{
	if (m_strSymbol.size() != 9) return false;
	if (IsSH())
	{
		std::wstring strValue = m_strSymbol.substr(0, 6);
		long lValue = _wtoi(strValue.c_str());
		if (m_strSymbol[0] == L'0' && lValue <= 3000) return true;
	}
	else if (IsSZ())
	{
		if (m_strSymbol[0] == L'3' && m_strSymbol[1] == '9') return true;
	}

	return false;
}





}
}