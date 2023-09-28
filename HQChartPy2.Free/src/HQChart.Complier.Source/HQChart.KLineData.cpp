/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	股票数据对外接口类 (c++)
*/

#include <fstream>
#include "HQChart.KLineData.h"
#include "HQChart.data.h"
#include "HQChart.Complier.h"

namespace HQChart { namespace Complier {


IHistoryData::IHistoryData()
{

}

IHistoryData::~IHistoryData()
{
	for (auto item : m_VariantCache)
	{
		DELETE_PTR(item);
	}

	m_VariantCache.clear();
	m_VariantUsedCache.clear();
	m_VariantFreeCache.clear();
}

Variant* IHistoryData::Create() const
{
	Variant* pResult = NULL;

	if (m_VariantFreeCache.empty())
	{
		pResult = new Variant();
		m_VariantCache.push_back(pResult);
		m_VariantUsedCache.push_back(pResult);
	}
	else
	{
		pResult = m_VariantFreeCache.front();
		m_VariantFreeCache.pop_front();

		pResult->Clear();
		m_VariantUsedCache.push_back(pResult);
	}

	return pResult;
}

void IHistoryData::ClearCache()
{
	for (auto item : m_VariantUsedCache)
	{
		m_VariantFreeCache.push_back(item);
	}

	m_VariantUsedCache.clear();
}

void IHistoryData::Reset()
{

}

bool IHistoryData::ConvertToDayPeriod(const ARRAY_KDATA& aryDay, ARRAY_KDATA& dest, long lPeriod) const
{
	//0=日线 1=周线 2=月线 3=年线 9=季线 21=双周
	long lCount = (long)aryDay.size();
	bool bNewPeriod = false;
	long lDate = 0, lIndex = 0, lStartDate = 0, lValue = 0, lMonth = 0, lQuarter = 0;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = aryDay[i];
		bNewPeriod = false;
		lDate = item._nDate;
		switch (lPeriod)
		{
		case PERIOD_TYPE_ID::PERIOD_WEEK_ID:	//周线
			lValue = GetFriday(lDate);
			if (lStartDate != lValue)
			{
				bNewPeriod = true;
				lStartDate = lValue;
			}
			break;
		case PERIOD_TYPE_ID::PERIOD_MONTH_ID:	//月线
			if ( (lDate/100) != (lStartDate / 100))
			{
				bNewPeriod = true;
				lStartDate = lDate;
			}
			break;
		case PERIOD_TYPE_ID::PERIOD_YEAR_ID:	//年线
			if ((lDate / 10000L) != (lStartDate / 10000L))
			{
				bNewPeriod = 1;
				lStartDate = lDate;
			}
			break;
		case PERIOD_TYPE_ID::PERIOD_QUARTER_ID: //季线
			lMonth = (lDate / 100) % 100;
			lValue = ((lMonth&&lMonth<13) ? ((lMonth + 2) / 3) : 0);
			if (lQuarter != lValue)
			{
				bNewPeriod = 1;
				lStartDate = lDate;
				lQuarter = lValue;
			}
			break;
		case 21: //双周
			break;
		default:
			break;
		}

		if (bNewPeriod)
		{
			dest.push_back(item);
		}
		else
		{
			auto& periodItem=dest.back();

			if (periodItem._dHigh<item._dHigh) periodItem._dHigh = item._dHigh;
			if (periodItem._dLow > item._dLow) periodItem._dLow = item._dLow;

			periodItem._nDate = item._nDate;
			periodItem._dClose = item._dClose;
			periodItem._nAdvance = item._nAdvance;
			periodItem._nDecline = item._nDecline;

			periodItem._dVol += item._dVol;
			periodItem._dAmount += item._dAmount;
		}
	}

	return true;
}

bool IHistoryData::ConvertToMinutePeriod(const ARRAY_KDATA& aryOneMinute, ARRAY_KDATA& dest, long lPeriod) const
{
	long lDataCount = 5;
	if (lPeriod == PERIOD_TYPE_ID::PERIOD_MIN5_ID) lDataCount = 5;
	else if (lPeriod == PERIOD_TYPE_ID::PERIOD_MIN15_ID) lDataCount = 15;
	else if(lPeriod == PERIOD_TYPE_ID::PERIOD_MIN30_ID) lDataCount = 30;
	else if (lPeriod == PERIOD_TYPE_ID::PERIOD_MIN60_ID) lDataCount = 60;

	bool bFirstData = false;
	long lCount = (long)aryOneMinute.size();
	long lPreTime = 0;
	for (long i = 0, j=0 ; i < lCount; )
	{
		bFirstData = true;
		for (j = 0; j < lDataCount && i<lCount; ++i)
		{
			const auto& item = aryOneMinute[i];

			// 9：25, 9:30 不连续就不算个数
			// 1点的数据 如果不是连续的 就不算个数
			if ((item._nTime == 92500 && lPreTime != 92400) || (item._nTime == 130000 && lPreTime != 125900) || (item._nTime == 93000 && lPreTime != 92900))
			{

			}
			else
			{
				++j;
			}

			lPreTime = item._nTime;

			if (bFirstData)
			{
				dest.push_back(item);
				bFirstData = false;
			}
			else
			{
				auto& periodItem = dest.back();

				if (periodItem._dHigh<item._dHigh) periodItem._dHigh = item._dHigh;
				if (periodItem._dLow > item._dLow) periodItem._dLow = item._dLow;

				periodItem._nDate = item._nDate;
				periodItem._nTime = item._nTime;
				periodItem._dClose = item._dClose;
				periodItem._nAdvance = item._nAdvance;
				periodItem._nDecline = item._nDecline;

				periodItem._dVol += item._dVol;
				periodItem._dAmount += item._dAmount;
			}

			if (i + 1 < lDataCount)
			{
				const auto& nextItem = aryOneMinute[i+1];
				if (nextItem._nDate != item._nDate)
				{
					++i;
					break;
				}
			}
			
		}
	}

	return true;
}

//复权 使用简单复权计算
bool IHistoryData::ConvertToRight(ARRAY_KDATA& aryData, long lRight) const
{
	if (aryData.empty()) return false;
	long lCount = (long)aryData.size();
	
	double dSeed(1), dYClose = 0, dClose = 0;
	if (lRight == RIGHT_TYPE_ID::RIGHT_BEFORE_ID)	//前复权
	{
		long i = lCount - 1;
		double dYClose;
		for (; i > 0; --i)
		{
			if (aryData[i]._dYClose != aryData[i - 1]._dClose) break;
		}

		for (; i >=0; --i)
		{
			auto& item = aryData[i];
			item._dClose *= dSeed;
			item._dHigh *= dSeed;
			item._dLow *= dSeed;
			item._dOpen *= dSeed;

			dYClose = item._dYClose;
			item._dYClose *= dSeed;

			if (i - 1 >= 0)
			{
				dClose = aryData[i - 1]._dClose;
				if (dYClose != dClose) dSeed = dYClose / dClose;
			}
		}
	}
	else if (lRight==RIGHT_TYPE_ID::RIGHT_AFTER_ID) //后复权
	{
		long i = 0;
		double dClose = aryData[0]._dClose;
		for (i=1 ; i < lCount; ++i)
		{
			if (aryData[i]._dYClose != dClose) break;
			dClose = aryData[i]._dClose;
		}

		for (; i < lCount; ++i)
		{
			auto& item = aryData[i];
			if (item._dYClose != dClose) dSeed *= dClose / item._dYClose;
			dClose = item._dClose;

			item._dClose *= dSeed;
			item._dHigh *= dSeed;
			item._dLow *= dSeed;
			item._dOpen *= dSeed;
			item._dYClose *= dSeed;
		}
	}

	return true;
}

long IHistoryData::GetFriday(long lDate) // 得到某日的星期五
{
	const long MONTH_DAYS[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	long days = 0, i = 1980, year(lDate / 10000), mon((lDate / 100) % 100), day(lDate % 100);
	for (; i<year; ++i)
	{
		days += 365;
		if (((i & 3) == 0) && ((i % 100) || !(i % 400))) ++days; //闰年
	}
	for (i = 1; i<mon; ++i)
	{
		days += MONTH_DAYS[i];
		if (i == 2 && (((year & 3) == 0) && ((year % 100) || !(year % 400)))) ++days;
	}
	days += day - 1; // 从1980年至当日共有多少天
	days -= 3; // 1980年1月1日为星期二，则离星期五的天数还要减去3
	i = days % 7;
	if (i > 2) // 得到当日所在星期的星期五。若为星期五、六、日，则为当日
		day += (7 - i) % 7;
	i = MONTH_DAYS[mon];
	if (mon == 2 && (((year & 3) == 0) && ((year % 100) || !(year % 400)))) ++i;
	if (day>i)
	{
		day -= i;
		++mon;
	}
	if (mon>12)
	{
		mon = 1;
		++year;
	}

	return (long)year * 10000L + (long)mon * 100L + (long)day;
}

long IHistoryData::TransDayToSeaon(long lMonth)
{
	return((lMonth && lMonth < 13) ? ((lMonth + 2) / 3) : 0);
}


void IHistoryData::FitDateTime(const IHistoryData* pDestHistoryData, const IHistoryData* pHistoryData, ARRAY_FIT_DATETIME_DATA& aryDateTime)
{
	const ARRAY_KDATA& dest = pDestHistoryData->GetData();
	const ARRAY_KDATA& kData = pHistoryData->GetData();
	long lCount = (long)kData.size();
	long lIndexCount = (long)dest.size();
	bool bMinutePeriod[2] = { pHistoryData->IsMinutePeriod(), pDestHistoryData->IsMinutePeriod(), }; //0 = 原始K线 1 = 需要拟合的K线
	bool bDayPeriod[2] = { pHistoryData->IsDayPeriod(), pDestHistoryData->IsDayPeriod(), };	//0 = 原始K线 1 = 需要拟合的K线
	aryDateTime.resize(lCount, FIT_DATETIME_ITEM());

	long lIndexStart = lIndexCount;
	const auto& firstItem = kData[0];
	for (long i = 0; i < lIndexCount; ++i)
	{
		const auto& item = dest[i];
		if ((bDayPeriod[0] && bDayPeriod[1]) || (bMinutePeriod[0] && bDayPeriod[1]))   //日线(拟合) => 日线(原始)    日线(拟合 => 分钟(原始)
		{
			if (item._nDate >= firstItem._nDate)
			{
				lIndexStart = i;
				break;
			}
		}
		else if (bMinutePeriod[0] && bMinutePeriod[1])	//分钟(拟合 => 分钟(原始)
		{
			if (item._nDate > firstItem._nDate)
			{
				lIndexStart = i;
				break;
			}

			if (item._nDate == firstItem._nDate && item._nTime >= firstItem._nTime)
			{
				lIndexStart = i;
				break;
			}
		}
	}

	for (long i = 0, j = lIndexStart; i < lCount;)
	{
		const auto& item = kData[i];
		if (j >= lIndexCount)
		{
			auto& fitItem = aryDateTime[i];
			fitItem._lDate[0] = item._nDate;
			fitItem._lTime[0] = item._nTime;
			fitItem._lIndex = -1;
			++i;
			continue;
		}

		const auto& destItem = dest[j];
		if ((bDayPeriod[0] && bDayPeriod[1]) || (bMinutePeriod[0] && bDayPeriod[1]))   //日线(拟合) => 日线(原始)    日线(拟合 => 分钟(原始)
		{
			if (destItem._nDate == item._nDate)
			{
				auto& fitItem = aryDateTime[i];
				fitItem._lDate[0] = item._nDate;
				fitItem._lTime[0] = item._nTime;
				fitItem._lDate[1] = destItem._nDate;
				fitItem._lTime[1] = destItem._nTime;
				fitItem._lIndex = j;
				++i;
			}
			else
			{
				if (j + 1 < lIndexCount)
				{
					const auto& nextDestItem = dest[j + 1];
					if (destItem._nDate <= item._nDate && nextDestItem._nDate>item._nDate)
					{
						auto& fitItem = aryDateTime[i];
						fitItem._lDate[0] = item._nDate;
						fitItem._lTime[0] = item._nTime;
						fitItem._lDate[1] = nextDestItem._nDate;
						fitItem._lTime[1] = nextDestItem._nTime;
						fitItem._lIndex = j+1;
						++i;
					}
					else if (nextDestItem._nDate <= item._nDate)
					{
						++j;
					}
					else
					{
						auto& fitItem = aryDateTime[i];
						fitItem._lDate[0] = item._nDate;
						fitItem._lTime[0] = item._nTime;
						fitItem._lIndex = -1;
						++i;
					}
				}
				else
				{
					++j;
				}
			}
		}
		else if (bMinutePeriod[0] && bMinutePeriod[1])	//分钟(拟合 => 分钟(原始)
		{
			if (destItem._nDate == item._nDate && destItem._nTime == item._nTime)
			{
				auto& fitItem = aryDateTime[i];
				fitItem._lDate[0] = item._nDate;
				fitItem._lTime[0] = item._nTime;
				fitItem._lDate[1] = destItem._nDate;
				fitItem._lTime[1] = destItem._nTime;
				fitItem._lIndex = j;
				++i;
			}
			else
			{
				if (j + 1 < lIndexCount)
				{
					const auto& nextDestItem = dest[j + 1];
					if ((destItem._nDate<item._nDate && nextDestItem._nDate>item._nDate) ||
						(destItem._nDate == item._nDate && destItem._nTime < item._nTime && nextDestItem._nDate == item._nDate && nextDestItem._nTime > item._nTime) ||
						(destItem._nDate == item._nDate && destItem._nTime < item._nTime && nextDestItem._nDate > item._nDate) ||
						(destItem._nDate < item._nDate && nextDestItem._nDate == item._nDate && nextDestItem._nTime > item._nTime))
					{
						auto& fitItem = aryDateTime[i];
						fitItem._lDate[0] = item._nDate;
						fitItem._lTime[0] = item._nTime;
						fitItem._lDate[1] = destItem._nDate;
						fitItem._lTime[1] = destItem._nTime;
						fitItem._lIndex = j+1;
						++i;
					}
					else if (nextDestItem._nDate < item._nDate || (nextDestItem._nDate == item._nDate && nextDestItem._nTime <= item._nTime))
					{
						++j;
					}
					else
					{
						auto& fitItem = aryDateTime[i];
						fitItem._lDate[0] = item._nDate;
						fitItem._lTime[0] = item._nTime;
						fitItem._lIndex = -1;
						++i;
					}
				}
				else
				{
					++j;
				}
			}
		}
	}
}

bool IHistoryData::IsMinutePeriod(long lPeriod)
{
	//4=1分钟 5=5分钟 6=15分钟 7=30分钟 8=60分钟
	switch (lPeriod)
	{
	case PERIOD_TYPE_ID::PERIOD_MIN1_ID:
	case PERIOD_TYPE_ID::PERIOD_MIN5_ID:
	case PERIOD_TYPE_ID::PERIOD_MIN15_ID:
	case PERIOD_TYPE_ID::PERIOD_MIN30_ID:
	case PERIOD_TYPE_ID::PERIOD_MIN60_ID:
	case PERIOD_TYPE_ID::PERIOD_MIN120_ID:
	case PERIOD_TYPE_ID::PERIOD_MIN240_ID:
		return true;
	default:
		if (lPeriod >= 20001 && lPeriod < 30000) return true;	//自定义日线周期
		return false;
	}
}

bool IHistoryData::IsMinuteChartPeriod(long lPeriod)
{
	switch (lPeriod)
	{
	case PERIOD_TYPE_ID::DAY_MINUTE_ID:
	case PERIOD_TYPE_ID::MULTIDAY_MINUTE_ID:
		return true;
	default:
		return false;
	}
}

bool IHistoryData::IsDayPeriod(long lPeriod)
{
	//0=日线 1=周线 2=月线 3=年线 9=季线 21=双周
	//[40001-50000) 自定义日线
	switch (lPeriod)
	{
	case PERIOD_TYPE_ID::PERIOD_DAY_ID:
	case PERIOD_TYPE_ID::PERIOD_WEEK_ID:
	case PERIOD_TYPE_ID::PERIOD_MONTH_ID:
	case PERIOD_TYPE_ID::PERIOD_YEAR_ID:
	case PERIOD_TYPE_ID::PERIOD_QUARTER_ID:
	case PERIOD_TYPE_ID::PERIOD_TWO_WEEK_ID:
	case PERIOD_TYPE_ID::PERIOD_HALF_YEAR_ID:
		return true;
	default:
		if (lPeriod >= 40001 && lPeriod < 50000) return true;	//自定义日线周期
		return false;
	}
}

bool IHistoryData::IsTickPeriod(long lPeriod)
{
	if (lPeriod == PERIOD_TYPE_ID::PERIOD_TICK_ID) return true;

	return false;
}

bool IHistoryData::IsSZSHStock(const std::wstring& strSymbol)
{
	if (strSymbol.empty()) return false;
	if (strSymbol.size() != 9) return false;

	std::wstring strUpperSymbol(strSymbol);
	std::transform(strUpperSymbol.begin(), strUpperSymbol.end(), strUpperSymbol.begin(), ::toupper);
	if (strUpperSymbol.find(L".SH",6) != std::wstring::npos)
	{
		if (strUpperSymbol.at(0) == L'6') return true;
	}
	else if (strUpperSymbol.find(L".SZ",6) != std::wstring::npos)
	{
		if (strUpperSymbol.at(0) == L'0') return true;
		if (strUpperSymbol.at(0) == L'3' || strUpperSymbol.at(1) != L'9') return true;	//创业版
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VariantCacheManage::VariantCacheManage()
{
	m_aryFreeVariant.reserve(200);
	m_aryUsedVariant.reserve(200);
}

VariantCacheManage::~VariantCacheManage()
{
	FreeVariant();
	Clear();
}

Variant* VariantCacheManage::Create()
{
	if (m_aryFreeVariant.empty())
	{
		Variant* pResult = new Variant();
		m_aryUsedVariant.push_back(pResult);
		return pResult;
	}
	else
	{
		Variant* pResult = m_aryFreeVariant.back();
		m_aryFreeVariant.pop_back();
		pResult->Clear();
		m_aryUsedVariant.push_back(pResult);
		return pResult;
	}
}

void VariantCacheManage::FreeVariant()
{
	for (auto& item : m_aryUsedVariant)
	{
		m_aryFreeVariant.push_back(item);
	}

	m_aryUsedVariant.clear();
}

void VariantCacheManage::Clear()
{
	for (auto& item : m_aryFreeVariant)
	{
		DELETE_PTR(item);
	}

	m_aryFreeVariant.clear();
}


}
}