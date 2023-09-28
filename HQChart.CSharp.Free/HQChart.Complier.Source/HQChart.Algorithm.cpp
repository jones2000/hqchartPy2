/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

内置函数类 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Execute.h"

////////////////////////////////////////////////////////////////////////////////
//	内置函数
//
///////////////////////////////////////////////////////////////////////////////

namespace HQChart { namespace Complier {

Variant* VariantOperator::MAX(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		MAX(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (data.IsVaildDoulbe())
			MAX(pResult->m_aryValue, data.GetDoubleValue(), data2.m_aryValue);
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (data2.IsVaildDoulbe())
			MAX(pResult->m_aryValue, data2.GetDoubleValue(), data.m_aryValue);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE && data2.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe() && data2.IsVaildDoulbe())
		{
			double dValue = std::max(data.GetDoubleValue(), data2.GetDoubleValue());
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::MAX(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	size_t nCount1 = data.size(), nCount2 = data.size();
	size_t nMaxCount = std::max(nCount1, nCount2);
	for (size_t i = 0; i < nMaxCount; ++i)
	{
		VARIANT_ITEM item;
		if (i < nCount1 && i < nCount2)
		{
			const auto& dataItem = data[i];
			const auto& data2Item = data2[i];
			if (dataItem.IsVaild() && data2Item.IsVaild())
				item.SetValue(std::max(dataItem._dValue, data2Item._dValue));
			else if (dataItem.IsVaild() && !data2Item.IsVaild())
				item.SetValue(dataItem._dValue);
			else if (!dataItem.IsVaild() && data2Item.IsVaild())
				item.SetValue(data2Item._dValue);
		}

		dest.push_back(item);
	}
}

void VariantOperator::MAX(ARRAY_DOUBLE& dest, const double dData, const ARRAY_DOUBLE& data2)
{
	for (auto data2Item : data2)
	{
		VARIANT_ITEM item;
		if (data2Item.IsVaild()) item.SetValue(std::max(dData, data2Item._dValue));
		else item.SetValue(dData);

		dest.push_back(item);
	}
}

Variant* VariantOperator::MIN(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		MIN(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (data.m_aryValue.size() == 1 && data.m_aryValue[0].IsVaild())
			MIN(pResult->m_aryValue, data.m_aryValue[0]._dValue, data2.m_aryValue);
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (data2.m_aryValue.size() == 1 && data2.m_aryValue[0].IsVaild())
			MIN(pResult->m_aryValue, data2.m_aryValue[0]._dValue, data.m_aryValue);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE && data2.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.m_aryValue.size() == 1 && data.m_aryValue[0].IsVaild() &&
			data2.m_aryValue.size() == 1 && data2.m_aryValue[0].IsVaild())
		{
			double dValue = std::min(data.m_aryValue[0]._dValue, data2.m_aryValue[0]._dValue);
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::MIN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	size_t nCount1 = data.size(), nCount2 = data.size();
	size_t nMaxCount = std::max(nCount1, nCount2);
	for (size_t i = 0; i < nMaxCount; ++i)
	{
		VARIANT_ITEM item;
		if (i < nCount1 && i < nCount2)
		{
			const auto& dataItem = data[i];
			const auto& data2Item = data2[i];
			if (dataItem.IsVaild() && data2Item.IsVaild())
				item.SetValue(std::min(dataItem._dValue, data2Item._dValue));
			else if (dataItem.IsVaild() && !data2Item.IsVaild())
				item.SetValue(dataItem._dValue);
			else if (!dataItem.IsVaild() && data2Item.IsVaild())
				item.SetValue(data2Item._dValue);
		}

		dest.push_back(item);
	}
}

void VariantOperator::MIN(ARRAY_DOUBLE& dest, const double dData, const ARRAY_DOUBLE& data2)
{
	for (auto data2Item : data2)
	{
		VARIANT_ITEM item;
		if (data2Item.IsVaild()) item.SetValue(std::min(dData, data2Item._dValue));
		else item.SetValue(dData);

		dest.push_back(item);
	}
}

Variant* VariantOperator::MA(const Variant& data, const Variant& dayCount)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::DOUBLE_TYPE)
	{
		if (dayCount.IsVaildDoulbe())
		{
			long lDayCount = (long)dayCount.GetDoubleValue();
			if (lDayCount > 0)
			{
				pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
				MA(pResult->m_aryValue, data.m_aryValue, lDayCount);
			}
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		MA(pResult->m_aryValue, data.m_aryValue, dayCount.m_aryValue);
	}

	return pResult;
}

void VariantOperator::MA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount)
{
	long nDataCount = (long)data.size();
	if (nDataCount <= 0) return;
	if (lDayCount <= 0) return;
	dest.resize(nDataCount, VARIANT_ITEM());
	if ((long)nDataCount < lDayCount) return;
	int nStart = GetFirstVaildIndex(data);
	int i = nStart;
	double dTotal = 0;
	for (; i < nStart + lDayCount && i<nDataCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild()) dTotal += item._dValue;
	}
	dest[i - 1].SetValue(dTotal / lDayCount);

	for (; i < nDataCount; ++i)
	{
		const auto& item = data[i];
		const auto& item2 = data[i - lDayCount];
		if (item2.IsVaild()) dTotal -= item2._dValue;
		if (item.IsVaild())
		{
			dTotal += item._dValue;
			dest[i].SetValue(dTotal / lDayCount);
		}
	}
}

void VariantOperator::MA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	long nDataCount = (long)data.size();
	long nData2Count = (long)data2.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount, VARIANT_ITEM());

	long nDayCount = 0, lTotalCount = 0;
	double dTotal = 0;
	for (long i = 0, j = 0, k = 0; i < nDataCount; ++i)
	{
		if (i >= nData2Count) continue;
		if (!data2[i].IsVaild()) continue;

		nDayCount = (long)data2[i]._dValue;
		if (nDayCount <= 0) continue;
		dTotal = 0;
		lTotalCount = 0;
		for (j = i, k = 0; j >= 0 && k < nDayCount; --j, ++k)
		{
			if (data[j].IsVaild()) dTotal += data[j]._dValue;
			++lTotalCount;
		}

		if (lTotalCount>0) dest[i].SetValue(dTotal / lTotalCount);
	}
}

long VariantOperator::GetFirstVaildIndex(const ARRAY_DOUBLE& data)
{
	long lCount = (long)data.size();
	for (long i = 0; i <lCount; ++i)
	{
		if (data[i].IsVaild())
		{
			return i;
		}
	}

	return lCount;
}

//指数平均数指标 EMA(close,10)
Variant* VariantOperator::EMA(const Variant& data, const Variant& dayCount)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::DOUBLE_TYPE)
	{
		if (dayCount.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			EMA(pResult->m_aryValue, data.m_aryValue, (long)(dayCount.GetDoubleValue()));
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		EMA(pResult->m_aryValue, data.m_aryValue, dayCount.m_aryValue);
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	}

	return pResult;
}

void VariantOperator::EMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	if (lDayCount < 1) return;
	int nStart = GetFirstVaildIndex(data);
	if (lDayCount + nStart>nDataCount) return;
	dest[nStart] = data[nStart + 1];

	for (long i = nStart + 1; i < nDataCount; ++i)
	{
		if (dest[i - 1].IsVaild() && data[i].IsVaild())
			dest[i].SetValue((2 * data[i]._dValue + (lDayCount - 1)*dest[i - 1]._dValue) / (lDayCount + 1));
		else if (i - 1 >-1 && dest[i - 1].IsVaild())
			dest[i].SetValue(dest[i - 1]._dValue);
	}
}

//EMA(N) = 2/(N+1)*C + (N-1)/(N+1)*EMA', EMA'为前一天的ema
void VariantOperator::EMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	int nData2Count = (int)data2.size();

	int nPeriod = 0, nIndex = 0;
	double dEMA = 0, dLastEMA = 0;
	bool bFind = false;
	double dEMAFactor[2] = { 0 };
	for (int i = 0, j = 0; i < nData2Count && i<nDataCount; ++i)
	{
		const auto& item = data2[i];
		if (!item.IsVaild()) continue;
		nPeriod = (int)item._dValue;
		if (nPeriod <= 0) continue;
		if (nPeriod > i + 1) nPeriod = i + 1;

		dEMA = 0, dLastEMA = 0;
		bFind = false;	//是否找到第1个有效数据
		dEMAFactor[0] = 2.0 / (nPeriod + 1);
		dEMAFactor[1] = (nPeriod - 1.0) / (nPeriod + 1.0);
		for (j = 0; j < nPeriod; ++j)
		{
			nIndex = i - (nPeriod - j - 1);
			const auto& dataItem = data[nIndex];
			if (!dataItem.IsVaild()) continue;
			if (!bFind)
			{
				dEMA = dLastEMA = dataItem._dValue;
				bFind = true;
			}
			else
			{
				dEMA = dEMAFactor[0] * dataItem._dValue + dEMAFactor[1] * dLastEMA;
				dLastEMA = dEMA;
			}
		}

		if (bFind) dest[i].SetValue(dEMA);
	}
}

//SMA 移动平均  用法：　SMA(X，N，M)　X的M日移动平均，M为权重，如Y=(X*M+Y'*(N-M))/N 
Variant* VariantOperator::SMA(const Variant& data, const Variant& n, const Variant& m)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE && m.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe() && m.IsVaildDoulbe())
		{
			const ARRAY_DOUBLE& aryData = data.m_aryValue;
			ARRAY_DOUBLE& aryDest = pResult->m_aryValue;
			aryDest.resize(aryData.size(), VARIANT_ITEM());
			long ln = (long)n.GetDoubleValue();
			double dm = m.GetDoubleValue();

			double dLastValue = 0, dValue = 0;
			long nStart = GetFirstVaildIndex(aryData);
			if (nStart>=(long)aryData.size()) return pResult;	//数据不够

			dLastValue = aryData[nStart]._dValue;
			aryDest[nStart].SetValue(dLastValue);

			for (long i = nStart + 1; i < (long)aryData.size(); ++i)
			{
				if (!aryData[i].IsVaild()) continue;
				dValue = (dm*aryData[i]._dValue + (ln - dm)*dLastValue) / ln;
				aryDest[i].SetValue(dValue);
				dLastValue = dValue;
			}

			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE && m.m_nType == Variant::DOUBLE_TYPE)
	{
		if (m.IsVaildDoulbe())
		{
			SMA(pResult->m_aryValue, data.m_aryValue, n.m_aryValue, m.GetDoubleValue());
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}

	return pResult;
}

void VariantOperator::SMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, double m)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());
	long lData2Count = (long)data2.size();

	long lPeriod = 0, lIndex = 0;
	bool bFind = false;
	double dLastSMA = 0, dSMA = 0;
	for (long i = 0, j = 0; i < lDataCount && i < lData2Count; ++i)
	{
		const auto& periodItem = data2[i];
		if (!periodItem.IsVaild()) continue;

		lPeriod = (long)periodItem._dValue;
		if (lPeriod <= 0) continue;
		if (lPeriod > i + 1) lPeriod = i + 1;

		for (j = 0; j < lPeriod; ++j)
		{
			lIndex = i - (lPeriod - j - 1);
			const auto& item = data[lIndex];
			if (!item.IsVaild()) continue;

			if (!bFind)
			{
				dSMA = item._dValue;
				dLastSMA = dSMA;
				bFind = true;
			}
			else
			{
				dSMA = (m*item._dValue + (lPeriod - m)*dLastSMA) / lPeriod;
				dLastSMA = dSMA;
			}
		}

		if (bFind) dest[i].SetValue(dSMA);
	}
}


/*
求动态移动平均.
用法: DMA(X,A),求X的动态移动平均.
算法: 若Y=DMA(X,A)则 Y=A*X+(1-A)*Y',其中Y'表示上一周期Y值,A必须小于1.
例如:DMA(CLOSE,VOL/CAPITAL)表示求以换手率作平滑因子的平均价
*/
Variant* VariantOperator::DMA(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		DMA(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
	}

	return pResult;
}

void VariantOperator::DMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();
	int nCount = std::max(nData2Count, nDataCount);
	dest.resize(nCount, VARIANT_ITEM());

	long nStart = 1;
	double dLastValue = 0;
	for (long i = 0; i < nCount; ++i)
	{
		if (i > nDataCount || i > nData2Count) return;
		if (data[i].IsVaild() && data2[i].IsVaild())
		{
			dest[i].SetValue(data[i]._dValue);
			nStart = i;
			break;
		}
	}

	double dValue = 0, dValue2 = 0;
	for (long i = nStart + 1; i < nCount; ++i)
	{
		if (i > nDataCount || i > nData2Count) continue;
		if (!data[i].IsVaild() || !data2[i].IsVaild()) continue;

		dValue = data[i]._dValue;
		dValue2 = data2[i]._dValue;
		if (dValue2 < 1)
			dest[i].SetValue((dValue2*dValue) + (1 - dValue2)*dest[i - 1]._dValue);
		else
			dest[i].SetValue(dValue);
	}
}

/*
返回加权移动平均
用法:WMA(X,N):X的N日加权移动平均.
算法:Yn=(1*X1+2*X2+...+n*Xn)/(1+2+...+n)
*/
Variant* VariantOperator::WMA(const Variant& data, const Variant& dayCount)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::DOUBLE_TYPE)
	{
		if (dayCount.IsVaildDoulbe())
		{
			long lPeriod = (long)dayCount.GetDoubleValue();
			if (lPeriod > 0)
			{
				WMA(pResult->m_aryValue, data.m_aryValue, lPeriod);
				pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			}
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		WMA(pResult->m_aryValue, data.m_aryValue, dayCount.m_aryValue);
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	}

	return pResult;
}

/*
返回加权移动平均
用法:WMA(X,N):X的N日加权移动平均.
算法:Yn=(1*X1+2*X2+...+n*Xn)/(1+2+...+n)
*/
void VariantOperator::WMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	int nStart = GetFirstVaildIndex(data);
	ARRAY_DOUBLE tempData = data;
	double dValue = 0;
	for (long i = nStart, nDays = 0; i < nDataCount; ++i, ++nDays)
	{
		if ((long)nDays < lDayCount - 1) continue;

		double dPreValue = tempData[i - (lDayCount - 1)]._dValue;
		double dSum = 0;
		long nCount = 0;
		for (long j = lDayCount - 1; j >= 0; --j)
		{
			const auto& item = tempData[i - j];
			if (!item.IsVaild())
			{
				dValue = dPreValue;
				tempData[i - j].SetValue(dValue);
			}
			else
			{
				dValue = item._dValue;
				dPreValue = dValue;
			}

			nCount += lDayCount - j;
			dSum += dValue * (lDayCount - j);
		}

		dest[i].SetValue(dSum / nCount);
	}

}

void VariantOperator::WMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	double dPreValue = 0;
	double dSum = 0, dCount = 0, dValue = 0;
	long lPeriod = 0, lStart = 0, lIndex = 0;
	for (int i = 0, j = 0, k = 0; i < nDataCount && i < (long)data2.size(); ++i)
	{
		const auto& periodItem = data2[i];
		if (!periodItem.IsVaild()) continue;
		lPeriod = (long)periodItem._dValue;
		if (lPeriod <= 0) continue;
		if (lPeriod > i + 1) lPeriod = i + 1;
		lStart = 0;
		for (j = 0; j < lPeriod; ++j, ++lStart)
		{
			lIndex = i - (lPeriod - j - 1);
			const auto& item = data[lIndex];
			if (item.IsVaild())
			{
				dPreValue = item._dValue;
				break;
			}
		}

		if (lStart >= lPeriod) continue;
		dSum = 0;
		dCount = 0;
		for (j = lStart, k = 1; j < lPeriod; ++j, ++k)
		{
			lIndex = i - (lPeriod - j - 1);
			const auto& item = data[lIndex];
			if (item.IsVaild())
			{
				dValue = item._dValue;
				dPreValue = dValue;
			}
			else
			{
				dValue = dPreValue;
			}

			dCount += k;
			dSum += dValue * k;
		}

		dest[i].SetValue(dSum / dCount);
	}
}

/*
加权移动平均
返回加权移动平均
用法:EXPMA(X,M):X的M日加权移动平均
EXPMA[i]=buffer[i]*para+(1-para)*EXPMA[i-1] para=2/(1+__para)
*/
Variant* VariantOperator::EXPMA(const Variant& data, const Variant& dayCount)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::DOUBLE_TYPE)
	{
		if (dayCount.IsVaildDoulbe())
		{
			EXPMA(pResult->m_aryValue, data.m_aryValue, (long)(dayCount.GetDoubleValue()));
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}

	return pResult;
}

void VariantOperator::EXPMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	int nStart = GetFirstVaildIndex(data);
	if (nStart >= nDataCount) return;

	dest[nStart].SetValue(data[nStart]._dValue);
	for (long i = nStart + 1; i < nDataCount; ++i)
	{
		if (dest[i - 1].IsVaild() || data[i].IsVaild())
		{
			dest[i].SetValue((2 * data[i]._dValue + (lDayCount - 1)*dest[i - 1]._dValue) / (lDayCount + 1));
		}
		else if (dest[i - 1].IsVaild())
		{
			dest[i] = dest[i - 1];
		}
	}
}

//加权平滑平均,MEMA[i]=SMA[i]*para+(1-para)*SMA[i-1] para=2/(1+__para)
Variant* VariantOperator::EXPMEMA(const Variant& data, const Variant& dayCount)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && dayCount.m_nType == Variant::DOUBLE_TYPE)
	{
		if (dayCount.IsVaildDoulbe())
		{
			EXPMEMA(pResult->m_aryValue, data.m_aryValue, (long)(dayCount.GetDoubleValue()));
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}

	return pResult;
}

void VariantOperator::EXPMEMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	int nStart = GetFirstVaildIndex(data);
	if (lDayCount<1 || nStart >= nDataCount) return;

	double dSum = 0;
	int i = nStart;
	for (; i < nStart + lDayCount; ++i)
	{
		if (data[i].IsVaild()) dSum = data[i]._dValue;
	}
	dest[i - 1].SetValue(dSum / lDayCount);

	for (; i < nDataCount; ++i)
	{
		if (dest[i - 1].IsVaild() && data[i].IsVaild()) dest[i].SetValue((2 * data[i]._dValue + (lDayCount - 1)*dest[i - 1]._dValue) / (lDayCount + 1));
		else if (i - 1>-1 && dest[i - 1].IsVaild()) dest[i] = dest[i - 1];
	}
}
/*
向前累加到指定值到现在的周期数.
用法:
SUMBARS(X,A):将X向前累加直到大于等于A,返回这个区间的周期数
例如:SUMBARS(VOL,CAPITAL)求完全换手到现在的周期数
*/
Variant* VariantOperator::SUMBARS(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		int nCount = (int)data.m_aryValue.size();
		int nCount2 = (int)data2.m_aryValue.size();
		if (nCount2 != nCount) return pResult;
		pResult->m_aryValue.resize(nCount);
		int nStart = (int)GetFirstVaildIndex(data.m_aryValue);
		int i = 0, j = 0;
		double dSum = 0;
		for (i = nCount - 1; i >= nStart; --i)
		{
			if (data2.m_aryValue[i].IsVaild())
			{
				for (j = i, dSum = 0; j >= nStart; --j)
				{
					if (data.m_aryValue[j].IsVaild())
						dSum += data.m_aryValue[j]._dValue;
					if (dSum >= data2.m_aryValue[i]._dValue)
						break;
				}
				if (j >= nStart)
					pResult->m_aryValue[i].SetValue(i - j + 1);
			}
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}
/*
返回平滑移动平均
用法:
MEMA(X,N):X的N日平滑移动平均,如Y=(X+Y'*(N-1))/N
*/
Variant* VariantOperator::MEMA(const Variant& data, const Variant& dayCount)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && dayCount.GetType() == Variant::DOUBLE_TYPE)
	{
		int nCount = (int)data.m_aryValue.size();
		pResult->m_aryValue.resize(nCount);
		int nNum = (int)dayCount.m_aryValue[0]._dValue;
		int nStart = (int)GetFirstVaildIndex(data.m_aryValue);
		int i = nStart;
		if (nNum < 1 || i + nNum >= nCount) return pResult;
		double dSum = 0;
		for (; i < nStart + nNum; ++i)
		{
			if (data.m_aryValue[i].IsVaild())
				dSum += data.m_aryValue[i]._dValue;
			else
			{
				if (i - 1 >= 0 && data.m_aryValue[i - 1].IsVaild())
					dSum += data.m_aryValue[i - 1]._dValue;
			}
		}
		pResult->m_aryValue[i - 1].SetValue(dSum / nNum);
		for (; i < nCount; ++i)
		{
			if (pResult->m_aryValue[i - 1].IsVaild() && data.m_aryValue[i].IsVaild())
				pResult->m_aryValue[i].SetValue((data.m_aryValue[i]._dValue + (nNum - 1)*pResult->m_aryValue[i - 1]._dValue) / nNum);
			else if (i - 1 >= 0 && pResult->m_aryValue[i - 1].IsVaild())
				pResult->m_aryValue[i].SetValue(pResult->m_aryValue[i - 1]._dValue);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}
/*
LLV 最低值
求最低值。
用法：　LLV(X，N)　求N周期内X最低值，N=0则从第一个有效值开始。
例如：　LLV(LOW，0)　表示求历史最低价。
*/
Variant* VariantOperator::LLV(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			LLV(pResult->m_aryValue, data.m_aryValue, (long)(n.GetDoubleValue()));
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		LLV(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
	}

	return pResult;
}

void VariantOperator::LLV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lPeriod)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount);
	if (lPeriod <= 0) lPeriod = nDataCount;
	int nStart = GetFirstVaildIndex(data);
	int nMin = (int)nStart;
	if (nMin < nDataCount) dest[nMin].SetValue(data[nMin]._dValue);

	int i = 0, j = 2;
	for (i = nMin + 1; i<nDataCount && j<lPeriod; ++i, ++j)
	{
		if (!data[i].IsVaild()) continue;
		if (data[i]._dValue <= data[nMin]._dValue) nMin = i;
		dest[i].SetValue(data[nMin]._dValue);
	}

	for (; i<nDataCount; ++i)
	{
		if (!data[i].IsVaild()) continue;

		if (i - nMin<lPeriod)
		{
			nMin = data[i]._dValue>data[nMin]._dValue ? nMin : i;
		}
		else
		{
			for (j = (nMin = i - lPeriod + 1) + 1; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;
				nMin = data[j]._dValue > data[nMin]._dValue ? nMin : j;
			}
		}
		dest[i].SetValue(data[nMin]._dValue);
	}
}

void VariantOperator::LLV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount);
	double dMin = 0;
	bool bMinVaild = false;
	for (int i = 0; i < nDataCount; ++i)
	{
		if (i >= nData2Count) continue;
		if (!data2[i].IsVaild()) continue;
		int nCount = (int)data2[i]._dValue;
		dMin = 0;
		bMinVaild = false;
		if (nCount > 0 && nCount <= i)
		{
			for (int j = i - nCount; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;

				if (bMinVaild == false)
				{
					dMin = data[j]._dValue;
					bMinVaild = true;
				}
				else if (dMin > data[j]._dValue)
				{
					dMin = data[j]._dValue;
				}
			}
		}
		else
		{
			nCount = i;
			for (int j = 0; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;
				if (bMinVaild == false)
				{
					dMin = data[j]._dValue;
					bMinVaild = true;
				}
				else if (dMin > data[j]._dValue)
				{
					dMin = data[j]._dValue;
				}
			}
		}

		if (bMinVaild) dest[i].SetValue(dMin);
	}
}

/*
	LV(X,N) 求X在N个周期内的最小值（不包含当前k线）

	注：
	1、若N为0则从第一个有效值开始算起;
	2、当N为有效值，但当前的k线数不足N根，按照实际的根数计算;
	3、N为空值时，返回空值。
	4、N可以是变量。

	例1：
	LL:LV(L,10);//求前面10根k线的最低点。（不包含当前k线）
	例2：
	N:=BARSLAST(DATE<>REF(DATE,1))+1;//分钟周期，日内k线根数
	ZL:VALUEWHEN(DATE<>REF(DATE,1),LV(L,N));//在分钟周期上，求昨天最低价。
	例3：
	LV(L,5) 和 REF(LLV(L,5),1) 的结果是一样的，用LV编写更加方便。
*/

Variant* VariantOperator::LV(const Variant& data, const Variant& n)
{
	Variant* pValue = LLV(data, n);
	Variant var_1;
	var_1.SetDoubleValue(1);
	return REF(*pValue, var_1);
}

/*
HHV 最高值
求最高值。
用法：　HHV(X，N)　求N周期内X最高值，N=0则从第一个有效值开始。
例如：　HHV(HIGH,30)　表示求30日最高价。
*/
Variant* VariantOperator::HHV(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			HHV(pResult->m_aryValue, data.m_aryValue, (long)(n.GetDoubleValue()));
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		HHV(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
	}

	return pResult;
}

void VariantOperator::HHV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lPeriod)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount);
	if (lPeriod <= 0) lPeriod = nDataCount;
	int nStart = GetFirstVaildIndex(data);
	int nMax = (int)nStart;
	if (nMax < nDataCount) dest[nMax].SetValue(data[nMax]._dValue);

	int i = 0, j = 2;
	for (i = nMax + 1; i<nDataCount && j<lPeriod; ++i, ++j)
	{
		if (!data[i].IsVaild()) continue;
		if (data[i]._dValue >= data[nMax]._dValue) nMax = i;
		dest[i].SetValue(data[nMax]._dValue);
	}

	for (; i<nDataCount; ++i)
	{
		if (!data[i].IsVaild()) continue;

		if (i - nMax<lPeriod)
		{
			nMax = data[i]._dValue<data[nMax]._dValue ? nMax : i;
		}
		else
		{
			for (j = (nMax = i - lPeriod + 1) + 1; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;
				nMax = data[j]._dValue < data[nMax]._dValue ? nMax : j;
			}
		}
		dest[i].SetValue(data[nMax]._dValue);
	}
}

void VariantOperator::HHV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount);
	double dMax = 0;
	bool bnMaxVaild = false;
	for (int i = 0; i < nDataCount; ++i)
	{
		if (i >= nData2Count) continue;
		if (!data2[i].IsVaild()) continue;
		int nCount = (int)data2[i]._dValue;
		dMax = 0;
		bnMaxVaild = false;
		if (nCount > 0 && nCount <= i)
		{
			for (int j = i - nCount; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;

				if (bnMaxVaild == false)
				{
					dMax = data[j]._dValue;
					bnMaxVaild = true;
				}
				else if (dMax < data[j]._dValue)
				{
					dMax = data[j]._dValue;
				}
			}
		}
		else
		{
			nCount = i;
			for (int j = 0; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;
				if (bnMaxVaild == false)
				{
					dMax = data[j]._dValue;
					bnMaxVaild = true;
				}
				else if (dMax < data[j]._dValue)
				{
					dMax = data[j]._dValue;
				}
			}
		}

		if (bnMaxVaild) dest[i].SetValue(dMax);
	}
}

/*
	HV(X,N)：求X在N个周期内（不包含当前k线）的最高值。

	注：
	1、若N为0则从第一个有效值开始算起(不包含当前K线)；
	2、当N为有效值，但当前的k线数不足N根，按照实际的根数计算，第一根k线返回空值；
	3、N为空值时，返回空值。
	4、N可以是变量。

	例1：
	HH:HV(H,10);//求前10根k线的最高点。
	例2：
	N:=BARSLAST(DATE<>REF(DATE,1))+1;
	ZH:VALUEWHEN(DATE<>REF(DATE,1),HV(H,N));//在分钟周期上，求昨天最高价。
	例3：
	HV(H,5) 和 REF(HHV(H,5),1) 的结果是一样的，用HV编写更加方便。
*/
Variant* VariantOperator::HV(const Variant& data, const Variant& n)
{
	Variant* pValue = HHV(data, n);
	Variant var_1;
	var_1.SetDoubleValue(1);
	return REF(*pValue, var_1);
}
/*
求上一低点到当前的周期数.
用法: LLVBARS(X,N):求N周期内X最低值到当前周期数,N=0表示从第一个有效值开始统计
例如: LLVBARS(HIGH,20)求得20日最低点到当前的周期数
*/

Variant* VariantOperator::LLVBARS(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			LLVBARS(pResult->m_aryValue, data.m_aryValue, (long)(n.GetDoubleValue()));
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		LLVBARS(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	}

	return pResult;
}

void VariantOperator::LLVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lPeriod)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount);
	int nStart = (int)GetFirstVaildIndex(data);
	int i = 0, nMin = 0, j = 2;
	for (i = nMin + 1; i<nDataCount && j<lPeriod; ++i, ++j)
	{
		if (!data[i].IsVaild()) continue;
		if (data[i]._dValue <= data[nMin]._dValue) nMin = i;
		if (lPeriod == nDataCount) dest[i].SetValue(nMin);
	}

	for (; i<nDataCount; ++i)
	{
		if (i - nMin<lPeriod)
		{
			if (data[i].IsVaild())
			{
				nMin = data[i]._dValue>data[nMin]._dValue ? nMin : i;
			}
		}
		else
		{
			for (j = (nMin = i - lPeriod + 1) + 1; j <= i; ++j)
			{
				if (!data[j].IsVaild()) continue;
				nMin = data[j]._dValue > data[nMin]._dValue ? nMin : j;
			}
		}

		dest[i].SetValue(i - nMin);
	}
}

void VariantOperator::LLVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nCount = (int)data2.size();
	int nDataCount = (int)data.size();
	if (nCount <= 0) return;
	dest.resize(nCount);

	int nPeriod = 0, nStart = 0, nMinIndex = -1;
	for (int i = 0,j=0,k=0; i < nCount; ++i)
	{
		dest[i].SetNull();
		if (!data2[i].IsVaild()) continue;
		if (i >= nDataCount) continue;

		nPeriod = (int)data2[i]._dValue;
		nStart = i - nPeriod;
		if (nStart < 0) nStart = 0;
		nMinIndex = -1;
		for (j = nStart; j < nDataCount; ++j)
		{
			if (data[j].IsVaild())
			{
				nMinIndex = j;
				break;
			}
		}

		for (k = 0; j < nDataCount && k < nPeriod; ++k, ++j)
		{
			if (data[j].IsVaild())
			{
				if (data[j]._dValue< data[nMinIndex]._dValue) 
					nMinIndex = j;
			}
		}

		if (nMinIndex>=0)
			dest[i].SetValue(i - nMinIndex);
	}
}

/*
求上一高点到当前的周期数.
用法:
HHVBARS(X,N):求N周期内X最高值到当前周期数,N=0表示从第一个有效值开始统计
例如:
HHVBARS(HIGH,0)求得历史新高到到当前的周期数
*/
Variant* VariantOperator::HHVBARS(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			HHVBARS(pResult->m_aryValue, data.m_aryValue, (long)(n.GetDoubleValue()));
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		HHVBARS(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	}

	return pResult;
}

void VariantOperator::HHVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lPeriod)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount);	//初始化数据
	lPeriod = lPeriod < 1 ? nDataCount : lPeriod;
	int nStart = (int)GetFirstVaildIndex(data);
	int i = 0, j = 2, nMax = nStart;
	for (i = nMax + 1; i < nDataCount && j < lPeriod; ++i, ++j)
	{
		if (!data[i].IsVaild()) continue;
		if (data[i]._dValue > data[nMax]._dValue) nMax = i;
		if (lPeriod == nDataCount) dest[i].SetValue(nMax);
	}
	for (; i < nDataCount; ++i)
	{
		if (i - nMax < lPeriod)
		{
			if (data[i].IsVaild())
				nMax = data[i]._dValue < data[nMax]._dValue ? nMax : i;
		}
		else
		{
			nMax = i - lPeriod + 1;
			for (j = nMax + 1; j <= i; ++j)
			{
				if (data[j].IsVaild())
					nMax = data[j]._dValue < data[nMax]._dValue ? nMax : j;
			}
		}
		dest[i].SetValue(i - nMax);
	}
}

void VariantOperator::HHVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nCount = (int)data2.size();
	int nDataCount = (int)data.size();
	if (nCount <= 0) return;
	dest.resize(nCount);

	int nPeriod = 0, nStart = 0, nMaxIndex = -1;
	for (int i = 0, j = 0, k = 0; i < nCount; ++i)
	{
		dest[i].SetNull();
		if (!data2[i].IsVaild()) continue;
		if (i >= nDataCount) continue;

		nPeriod = (int)data2[i]._dValue;
		nStart = i - nPeriod;
		if (nStart < 0) nStart = 0;
		nMaxIndex = -1;
		for (j = nStart; j < nDataCount; ++j)
		{
			if (data[j].IsVaild())
			{
				nMaxIndex = j;
				break;
			}
		}

		for (k = 0; j < nDataCount && k < nPeriod; ++k, ++j)
		{
			if (data[j].IsVaild())
			{
				if (data[j]._dValue > data[nMaxIndex]._dValue)
					nMaxIndex = j;
			}
		}

		if (nMaxIndex >= 0)
			dest[i].SetValue(i - nMaxIndex);
	}
}

/*
两条线维持一定周期后交叉.
用法:LONGCROSS(A,B,N)表示A在N周期内都小于B,本周期从下方向上穿过B时返回1,否则返回0
*/
Variant* VariantOperator::LONGCROSS(const Variant& data, const Variant& data2, const Variant& n)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		int nSize = (int)data.m_aryValue.size();
		int nSize2 = (int)data2.m_aryValue.size();
		int nCount = std::max(nSize, nSize2);
		int nNum = (int)n.m_aryValue[0]._dValue;
		pResult->m_aryValue.resize(nCount);
		int i = 0;
		for (; i < nCount && (!data.m_aryValue[i].IsVaild() || !data2.m_aryValue[i].IsVaild()); ++i);

		int nStart = i;
		for (++i; i < nCount; ++i)
		{
			if (i >= nSize || i >= nSize2) continue;

			const auto& item = data.m_aryValue[i];
			const auto& item2 = data2.m_aryValue[i];

			const auto& itemPre = data.m_aryValue[i - 1];
			const auto& item2Pre = data2.m_aryValue[i - 1];

			if (!item.IsVaild() || !item2.IsVaild() || !itemPre.IsVaild() || !item2Pre.IsVaild()) continue;

			if (item._dValue > item2._dValue &&  itemPre._dValue < item2Pre._dValue)
				pResult->m_aryValue[i].SetValue(1);
			else
				pResult->m_aryValue[i].SetValue(0);
		}

		i = nStart;
		for (++i; i < nCount; ++i)
		{
			if (pResult->m_aryValue[i].IsVaild() && pResult->m_aryValue[i]._dValue > 0)
			{
				for (long j = 1; j <= nNum && i - j >= 0; ++j)
				{
					if (data.m_aryValue[i - j].IsVaild() && data2.m_aryValue[i - j].IsVaild() && data.m_aryValue[i - j]._dValue >= data2.m_aryValue[i - j]._dValue)
					{
						pResult->m_aryValue[i].SetValue(0);
						break;
					}
				}
			}
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}

/*
求相反数.
用法:REVERSE(X)返回-X.
例如:REVERSE(CLOSE)返回-CLOSE
*/
Variant* VariantOperator::REVERSE(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->m_aryValue.resize(1);
		pResult->m_aryValue[0].SetValue(0 - data.m_aryValue[0]._dValue);
		pResult->SetType(Variant::DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		size_t nSize = data.m_aryValue.size();
		pResult->m_aryValue.resize(nSize);
		for (size_t i = 0; i < nSize; ++i)
		{
			if (data.m_aryValue[i].IsVaild())
				pResult->m_aryValue[i].SetValue(0 - data.m_aryValue[i]._dValue);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}

/*
统计满足条件的周期数.
用法:
COUNT(X, N), 统计N周期中满足X条件的周期数, 若N<0则从第一个有效值开始.
例如 :COUNT(CLOSE>OPEN, 20)表示统计20周期内收阳的周期数
N支持数组
*/
Variant* VariantOperator::COUNT(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		long lCount = (long)data.m_aryValue.size();
		long lPeriod = (long)n.m_aryValue[0]._dValue;
		lPeriod = lPeriod < 1 ? lCount : lPeriod;
		long i = 0, j = 0, days = 0;
		for (; i < lCount && !data.m_aryValue[i].IsVaild(); ++i);
		pResult->m_aryValue.resize(lCount);
		for (; i < lCount && j < lPeriod; ++i, ++j)
		{
			if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
				days += 1;
			pResult->m_aryValue[i].SetValue(days);
		}
		for (; i < lCount; ++i)
		{
			if (data.m_aryValue[i - lPeriod].IsVaild() && data.m_aryValue[i - lPeriod]._dValue && days > 0)
				days--;
			if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
				days++;
			pResult->m_aryValue[i].SetValue(days);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		COUNT(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::COUNT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	long lCount = (long)data.size();
	VARIANT_ITEM ZeroItem;
	ZeroItem.SetValue(0);
	dest.resize(lCount, ZeroItem);

	long lPeriod = 0, lIndex = 0, lTotal = 0;
	for (long i = 0, j = 0; i < (long)n.size(); ++i)
	{
		if (!n[i].IsVaild())
		{
			lPeriod = i + 1;
		}
		else
		{
			lPeriod = (long)n[i]._dValue;
			if (lPeriod < 0) lPeriod = i + 1;
		}
		

		lTotal = 0;
		for (long j = 0; j < lPeriod; ++j)
		{
			lIndex = i - j;
			if (lIndex < 0) break;
			const auto& item = data[lIndex];
			if (!item.IsVaild()) continue;
			if (item._dValue) ++lTotal;
		}

		dest[i].SetValue(lTotal);
	}
}

//上穿
Variant* VariantOperator::CROSS(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		CROSS(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data2.IsVaildDoulbe())
		{
			double dData2 = data2.GetDoubleValue();
			CROSS(pResult->m_aryValue, data.m_aryValue, dData2);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dData = data.GetDoubleValue();
			CROSS(pResult->m_aryValue, dData, data2.m_aryValue);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)	//两个单值
	{
		pResult->SetDoubleValue(0);
	}
	return pResult;
}

void VariantOperator::CROSS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	long nSize = (long)data.size();
	long nSize2 = (long)data2.size();
	long nCount = std::max(nSize, nSize2);
	dest.resize(nCount);
	long i = 0;
	for (; i < nCount && (!data[i].IsVaild() || !data2[i].IsVaild()); ++i);

	for (++i; i < nCount; ++i)
	{
		if (i >= nSize || i >= nSize2) continue;

		const auto& item = data[i];
		const auto& item2 = data2[i];

		const auto& itemPre = data[i - 1];
		const auto& item2Pre = data2[i - 1];

		if (!item.IsVaild() || !item2.IsVaild() || !itemPre.IsVaild() || !item2Pre.IsVaild()) continue;

		if (item._dValue > item2._dValue && itemPre._dValue < item2Pre._dValue)
			dest[i].SetValue(1);
		else
			dest[i].SetValue(0);
	}
}

void VariantOperator::CROSS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dData2)
{
	int nCount = (int)data.size();
	dest.resize(nCount);

	int i = 0;
	for (; i < nCount && !data[i].IsVaild(); ++i);

	for (++i; i < nCount; ++i)
	{
		const auto& item = data[i];
		const auto& itemPre = data[i - 1];

		if (!item.IsVaild() || !itemPre.IsVaild()) continue;

		if (item._dValue > dData2 && itemPre._dValue < dData2)
			dest[i].SetValue(1);
		else
			dest[i].SetValue(0);
	}
}

void VariantOperator::CROSS(ARRAY_DOUBLE& dest, double dData, const ARRAY_DOUBLE& data2)
{
	int nCount = (int)data2.size();
	dest.resize(nCount);

	int i = 0;
	for (; i < nCount && !data2[i].IsVaild(); ++i);

	for (++i; i < nCount; ++i)
	{
		const auto& item2 = data2[i];
		const auto& item2Pre = data2[i - 1];

		if (!item2.IsVaild() || !item2Pre.IsVaild()) continue;

		if (dData > item2._dValue && dData < item2Pre._dValue)
			dest[i].SetValue(1);
		else
			dest[i].SetValue(0);
	}
}

Variant* VariantOperator::SUM(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
	{
		long lCount = (long)data.m_aryValue.size();
		pResult->m_aryValue.resize(lCount);
		long lNum = (long)data2.m_aryValue[0]._dValue;
		lNum = lNum > 0 ? std::min(lNum, lCount) : lCount;
		long i = 0, j = 0;
		double dSum = 0;
		for (; i < lCount && !data.m_aryValue[i].IsVaild(); ++i);

		for (; i < lCount && j < lNum; ++i, ++j)
		{
			if (data.m_aryValue[i].IsVaild())
				dSum += data.m_aryValue[i]._dValue;
			pResult->m_aryValue[i].SetValue(dSum);
		}
		for (; i < lCount; ++i)
		{
			if (data.m_aryValue[i - lNum].IsVaild())
				dSum -= data.m_aryValue[i - lNum]._dValue;
			if (data.m_aryValue[i].IsVaild())
				dSum += data.m_aryValue[i]._dValue;
			pResult->m_aryValue[i].SetValue(dSum);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		SUM(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}

void VariantOperator::SUM(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());

	long lPeriod = 0, lIndex = 0, lSumCount = 0;
	double dSum = 0;
	for (long i = 0, j = 0; i < (long)n.size() && i < lDataCount; ++i)
	{
		const auto& periodItem = n[i];
		if (!periodItem.IsVaild()) continue;

		lPeriod = (long)periodItem._dValue;
		if (lPeriod <= 0) continue;

		dSum = 0;
		for (j = 0; j < lPeriod; ++j)
		{
			lIndex = i - j;
			if (lIndex < 0) break;
			const auto& item = data[lIndex];
			if (item.IsVaild()) dSum += item._dValue;
		}

		dest[i].SetValue(dSum);
	}
}
/*
求累乘.
用法:
MULAR(X,N),统计N周期中X的乘积.N=0则从第一个有效值开始.
例如:
MULAR(C/REF(C,1),0)表示统计从上市第一天以来的复利
*/
Variant* VariantOperator::MULAR(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
	{
		int nCount = (int)data.m_aryValue.size();
		int nPeriod = (int)data2.m_aryValue[0]._dValue;
		if (nPeriod <= 0 || nPeriod > nCount)
			nPeriod = nCount;
		pResult->m_aryValue.resize(nCount);
		int nStart = (int)GetFirstVaildIndex(data.m_aryValue);
		if (nPeriod == 1)
		{
			for (int i = nStart; i < nCount; ++i)
			{
				if (data.m_aryValue[i].IsVaild())
					pResult->m_aryValue[i].SetValue(data.m_aryValue[i]._dValue);
			}
		}
		else
		{
			int i = nStart;
			for (; i < nPeriod; ++i)
			{
				if (i == nStart)
					pResult->m_aryValue[i].SetValue(data.m_aryValue[i]._dValue);
				else
				{
					if (pResult->m_aryValue[i - 1].IsVaild() && data.m_aryValue[i].IsVaild())
						pResult->m_aryValue[i].SetValue(pResult->m_aryValue[i - 1]._dValue * data.m_aryValue[i]._dValue);
					else if (pResult->m_aryValue[i - 1].IsVaild())
						pResult->m_aryValue[i].SetValue(pResult->m_aryValue[i - 1]._dValue);
					else if (data.m_aryValue[i].IsVaild())
						pResult->m_aryValue[i].SetValue(data.m_aryValue[i]._dValue);
				}
			}
			double dData = 0;
			for (; i < nCount; ++i)
			{
				if (pResult->m_aryValue[i - 1].IsVaild() && data.m_aryValue[i].IsVaild())
				{
					if (data.m_aryValue[i - 1 - (nPeriod - 1)].IsVaild())
						dData = pResult->m_aryValue[i - 1]._dValue / data.m_aryValue[i - 1 - (nPeriod - 1)]._dValue;
					else
						dData = pResult->m_aryValue[i - 1]._dValue;
					pResult->m_aryValue[i].SetValue(dData*data.m_aryValue[i]._dValue);
				}
				else if (pResult->m_aryValue[i - 1].IsVaild())
				{
					if (data.m_aryValue[i - 1 - (nPeriod - 1)].IsVaild())
						dData = pResult->m_aryValue[i - 1]._dValue / data.m_aryValue[i - 1 - (nPeriod - 1)]._dValue;
					else
						dData = pResult->m_aryValue[i - 1]._dValue;
					pResult->m_aryValue[i].SetValue(dData);
				}
				else if (data.m_aryValue[i].IsVaild())
				{
					pResult->m_aryValue[i].SetValue(data.m_aryValue[i]._dValue);
				}
			}
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

/*
BARSCOUNT 有效数据周期数
求总的周期数。
用法：　BARSCOUNT(X)　第一个有效数据到当前的天数。
例如：　BARSCOUNT(CLOSE)　对于日线数据取得上市以来总交易日数，对于分笔成交取得当日成交笔数，对于1分钟线取得当日交易分钟数。
*/
Variant* VariantOperator::BARSCOUNT(const Variant& data)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nDataCount = (int)src.size();
		VARIANT_ITEM zeorItem;
		zeorItem.SetValue(0);
		dest.resize(nDataCount, zeorItem);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);

		int nStart = (int)GetFirstVaildIndex(src);
		int nCount = 0;
		for (int i = nStart; i < nDataCount; ++i, ++nCount)
		{
			dest[i].SetValue(nCount);
		}
	}
	return pResult;
}

/*
统计连续满足条件的周期数.
用法: BARSLASTCOUNT(X),统计连续满足X条件的周期数.
例如: BARSLASTCOUNT(CLOSE>OPEN)表示统计连续收阳的周期数
*/
Variant* VariantOperator::BARSLASTCOUNT(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nDataCount = (int)src.size();
		VARIANT_ITEM zeorItem;
		zeorItem.SetValue(0);
		dest.resize(nDataCount, zeorItem);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		int nCount = 0;
		for (int i = nDataCount - 1, j = 0; i >= 0; --i)
		{
			nCount = 0;
			for (j = i; j >= 0; --j)
			{
				const auto& item = src[j];
				if (item.IsVaild() && item._dValue)
					++nCount;
				else
					break;
			}
			dest[i].SetValue(nCount);
		}
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe()) pResult->SetDoubleValue(1);
		else pResult->SetDoubleValue(0);
	}
	else
	{
		pResult->SetDoubleValue(0);
	}

	return pResult;
}

/*
属于未来函数,将当前位置到若干周期前的数据设为1.
用法:
BACKSET(X,N),若X非0,则将当前位置到N周期前的数值设为1.
例如:
BACKSET(CLOSE>OPEN,2)若收阳则将该周期及前一周期数值设为1,否则为0
*/
Variant* VariantOperator::BACKSET(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		int nNum = (int)n.m_aryValue[0]._dValue;
		int nCount = (int)data.m_aryValue.size();
		int nStart = (int)GetFirstVaildIndex(data.m_aryValue);
		if (nStart == nCount) return pResult;
		nNum = std::min(nCount - nStart, std::max(1, nNum));
		VARIANT_ITEM zeorItem;
		zeorItem.SetValue(0);
		pResult->m_aryValue.resize(nCount, zeorItem);
		int i = 0, j = 0, end = 0;
		for (i = nCount - 1, j = 0, end = nStart + nNum; i >= end; --i)
		{
			if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
			{
				for (j = i; j > i - nNum; --j)
					pResult->m_aryValue[j].SetValue(1);
			}
		}
		if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
		{
			for (j = i; j >= nStart; --j)
				pResult->m_aryValue[j].SetValue(1);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		BACKSET(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}

void VariantOperator::BACKSET(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nCount = (int)data2.size();
	int nDataCount = (int)data.size();
	VARIANT_ITEM zeorItem;
	zeorItem.SetValue(0);
	dest.resize(nDataCount, zeorItem);

	int nPeriod = 0;
	for (int i = 0, j = 0, k = 0; i < nDataCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild() || !item._dValue) continue;
		if (i >= nCount) continue;
		const auto& nItem = data2[i];
		if (!item.IsVaild()) continue;

		nPeriod = (int)item._dValue;
		for (j = i, k = 0; j >= 0 && k < nPeriod; --j,++k)
		{
			dest[i].SetValue(1);
		}
	}
}

/*
上一次条件成立到当前的周期数.
用法:
BARSLAST(X):上一次X不为0到现在的天数
例如:
BARSLAST(CLOSE/REF(CLOSE,1)>=1.1)表示上一个涨停板到当前的周期数
*/
Variant* VariantOperator::BARSLAST(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		int nCount = (int)data.m_aryValue.size();
		pResult->m_aryValue.resize(nCount);
		int i = 0;
		for (; i < nCount; ++i)
		{
			if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
				break;
		}
		for (int days = 0; i < nCount; ++i, ++days)
		{
			if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
				days = 0;
			pResult->m_aryValue[i].SetValue(days);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}
/*
第一个条件成立到当前的周期数.
用法:
BARSSINCE(X):第一次X不为0到现在的天数
例如:
BARSSINCE(HIGH>10)表示股价超过10元时到当前的周期数
*/
Variant* VariantOperator::BARSSINCE(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		int nCount = (int)data.m_aryValue.size();
		pResult->m_aryValue.resize(nCount);
		int i = 0;
		for (; i < nCount; ++i)
		{
			if (data.m_aryValue[i].IsVaild() && data.m_aryValue[i]._dValue)
				break;
		}
		for (int days = 0; i < nCount; ++i, ++days)
		{
			pResult->m_aryValue[i].SetValue(days);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}


/*
RANGE(A,B,C):A在B和C范围之间,B<A<C.
用法:RANGE(A,B,C)表示A大于B同时小于C时返回1,否则返回0
*/
Variant* VariantOperator::RANGE(const Variant& a, const Variant& b, const Variant& c)
{
	Variant* pResult = Create();
	if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::ARRAY_DOUBLE_TYPE && c.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		RANGE(pResult->m_aryValue, a.m_aryValue, b.m_aryValue, c.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::DOUBLE_TYPE && c.GetType() == Variant::DOUBLE_TYPE)
	{
		if (b.IsVaildDoulbe() && c.IsVaildDoulbe())
		{
			RANGE(pResult->m_aryValue, a.m_aryValue, b.GetDoubleValue(), c.GetDoubleValue());
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::ARRAY_DOUBLE_TYPE && c.GetType() == Variant::DOUBLE_TYPE)
	{
		if (c.IsVaildDoulbe())
		{
			RANGE(pResult->m_aryValue, a.m_aryValue, b.m_aryValue, c.GetDoubleValue());
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::DOUBLE_TYPE && c.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (b.IsVaildDoulbe())
		{
			RANGE(pResult->m_aryValue, a.m_aryValue, b.GetDoubleValue(), c.m_aryValue);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, const ARRAY_DOUBLE& b, const ARRAY_DOUBLE& c)
{
	long lACount = (long)a.size();
	long lBCount = (long)b.size();
	long lCCount = (long)c.size();

	long lCount = std::max(lACount, lBCount);
	lCount = std::max(lCount, lCCount);
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	bool bValue = false;
	for (long i = 0; i < lCount; ++i)
	{
		if (i >= lACount || i >= lBCount || i >= lCCount) continue;

		const auto& aItem = a[i];
		const auto& bItem = b[i];
		const auto& cItem = c[i];

		if (!aItem.IsVaild() || !bItem.IsVaild() || !cItem.IsVaild()) continue;

		bValue = DoubleGT(aItem._dValue, bItem._dValue) && DoubleLT(aItem._dValue, cItem._dValue);
		dest[i].SetValue(bValue ? 1 : 0);
	}
}

void VariantOperator::RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, double dB, double dC)
{
	long lCount = (long)a.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	bool bValue = false;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = a[i];
		if (!item.IsVaild()) continue;

		bValue = DoubleGT(item._dValue, dB) && DoubleLT(item._dValue, dC);
		dest[i].SetValue(bValue ? 1 : 0);
	}
}

void VariantOperator::RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, const ARRAY_DOUBLE& b, double dC)
{
	long lACount = (long)a.size();
	long lBCount = (long)b.size();

	long lCount = std::max(lACount, lBCount);
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	bool bValue = false;
	for (long i = 0; i < lCount; ++i)
	{
		if (i >= lACount || i >= lBCount) continue;

		const auto& aItem = a[i];
		const auto& bItem = b[i];

		if (!aItem.IsVaild() || !bItem.IsVaild()) continue;

		bValue = DoubleGT(aItem._dValue, bItem._dValue) && DoubleLT(aItem._dValue, dC);
		dest[i].SetValue(bValue ? 1 : 0);
	}
}

void VariantOperator::RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, double dB, const ARRAY_DOUBLE& c)
{
	long lACount = (long)a.size();
	long lCCount = (long)c.size();

	long lCount = std::max(lACount, lCCount);
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	bool bValue = false;
	for (long i = 0; i < lCount; ++i)
	{
		if (i >= lACount || i >= lCCount) continue;

		const auto& aItem = a[i];
		const auto& cItem = c[i];

		if (!aItem.IsVaild() || !cItem.IsVaild()) continue;

		bValue = DoubleGT(aItem._dValue, dB) && DoubleLT(aItem._dValue, cItem._dValue);
		dest[i].SetValue(bValue ? 1 : 0);
	}
}

Variant* VariantOperator::XMA(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			long lN = (long)n.GetDoubleValue();
			XMA(pResult->m_aryValue, data.m_aryValue, lN);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::XMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lDataCount = (long)data.size();
	long lDataStart = GetFirstVaildIndex(data);
	long p = (long)((n - 2) / 2);
	double dSum = 0;
	long lCount = 0, lStart = 0, lEnd = 0;
	dest.resize(lDataCount);

	for (long i = lDataStart, j = 0; i < lDataCount; ++i)
	{
		lStart = i - p - 1;
		lEnd = i + (n - p) - 1;
		for (j = lStart; j < lEnd; ++j)
		{
			if (j >= 0 && j<lDataCount)
			{
				const auto& item = data[j];
				if (item.IsVaild())
				{
					dSum += item._dValue;
					++lCount;
				}
			}
		}

		if (lCount != 0) dest[i].SetValue(dSum / lCount);
		dSum = 0;
		lCount = 0;
	}
}


//当前值是近多少周期内的最大值.
//用法: TOPRANGE(X):X是近多少周期内X的最大值
//例如: TOPRANGE(HIGH)表示当前最高价是近多少周期内最高价的最大值
Variant* VariantOperator::TOPRANGE(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nCount = (int)src.size();
		VARIANT_ITEM zeroItem;
		zeroItem.SetValue(0);
		dest.resize(nCount, zeroItem);
		
		int nIndex = 0;
		for(int i=nCount-1, j=0;i>=0;--i)
		{
			const auto& item = src[i];
			if (!item.IsVaild()) continue;

			nIndex = 0;
			for (j = i - 1; j >= 0; --j)
			{
				const auto& item2 = src[j];
				if (!item2.IsVaild()) continue;

				if (DoubleGT(item2._dValue,item._dValue))
					break;

				++nIndex;
			}

			dest[i].SetValue(nIndex);
		}

		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(0);
	}

	return pResult;
}

//当前值是近多少周期内的最小值.
//用法:LOWRANGE(X):X是近多少周期内X的最小值
//例如:LOWRANGE(LOW)表示当前最低价是近多少周期内最低价的最小值
Variant* VariantOperator::LOWRANGE(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nCount = (int)src.size();
		VARIANT_ITEM zeroItem;
		zeroItem.SetValue(0);
		dest.resize(nCount, zeroItem);

		int nIndex = 0;
		for (int i = nCount - 1, j = 0; i >= 0; --i)
		{
			const auto& item = src[i];
			if (!item.IsVaild()) continue;

			nIndex = 0;
			for (j = i - 1; j >= 0; --j)
			{
				const auto& item2 = src[j];
				if (!item2.IsVaild()) continue;

				if (DoubleLT(item2._dValue,item._dValue))
					break;

				++nIndex;
			}

			dest[i].SetValue(nIndex);
		}

		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(0);
	}

	return pResult;
}

//属于未来函数,下一次条件成立到当前的周期数.
//用法:BARSNEXT(X):下一次X不为0到现在的天数
//例如:BARSNEXT(CLOSE/REF(CLOSE,1)>=1.1)表示下一个涨停板到当前的周期数
Variant* VariantOperator::BARSNEXT(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nCount = (int)src.size();
		VARIANT_ITEM zeroItem;
		zeroItem.SetValue(0);
		dest.resize(nCount, zeroItem);

		for (int i=0,j=0, k=0;i<nCount;++i)
		{
			for (j = i, k=0 ; j<nCount; ++j,++k)
			{
				const auto& item = src[j];
				if (!item.IsVaild()) continue;

				if (item._dValue > 0)
				{
					dest[i].SetValue(k);
					break;
				}
			}
		}

		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(0);
	}

	return pResult;
}

//求低值名次.
//用法:LOD(X,N):求当前X数据是N周期内的第几个低值,N=0则从第一个有效值开始.
//例如:LOD(LOW,20)返回是20日的第几个低价
Variant* VariantOperator::LOD(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(1);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		LOD(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			long ln = (long)n.GetDoubleValue();
			LOD(pResult->m_aryValue, data.m_aryValue, ln);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::LOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nCount = (int)data.size();
	int nCount2 = (int)n.size();
	int nSubCount = 0, nIndex = 0;
	double dValue = 0;
	dest.resize(nCount, VARIANT_ITEM());
	for (int i = nCount - 1, j=0, k=0; i >= 0; --i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		dValue = item._dValue;

		if (i >= nCount2) continue;
		const auto& nItem = n[i];
		if (!nItem.IsVaild()) continue;

		nSubCount = (long)nItem._dValue;
		if (nSubCount <= 0) nSubCount = i;

		nIndex = 1;

		for (j = i - 1, k = 1; j >= 0 && k < nSubCount; --j, ++k)
		{
			const auto& item2 = data[j];
			if (item2.IsVaild() && DoubleLT(item2._dValue, dValue)) ++nIndex;
		}

		dest[i].SetValue(nIndex);
	}
}

void VariantOperator::LOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nCount = (int)data.size();
	int nSubCount = 0, nIndex = 0;
	double dValue = 0;
	dest.resize(nCount, VARIANT_ITEM());
	for (int i = nCount - 1, j = 0, k = 0; i >= 0; --i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		dValue = item._dValue;

		nSubCount = n;
		if (nSubCount <= 0) nSubCount = i;

		nIndex = 1;
		for (j = i - 1, k = 1; j >= 0 && k < nSubCount; --j, ++k)
		{
			const auto& item2 = data[j];
			if (item2.IsVaild() && DoubleLT(item2._dValue, dValue)) ++nIndex;
		}

		dest[i].SetValue(nIndex);
	}
}

//求高值名次.
//用法:HOD(X,N):求当前X数据是N周期内的第几个高值,N=0则从第一个有效值开始.
//例如:HOD(HIGH,20)返回是20日的第几个高价
Variant* VariantOperator::HOD(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(1);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		HOD(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			long ln = (long)n.GetDoubleValue();
			HOD(pResult->m_aryValue, data.m_aryValue, ln);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::HOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nCount = (int)data.size();
	int nCount2 = (int)n.size();
	int nSubCount = 0, nIndex = 0;
	double dValue = 0;
	dest.resize(nCount, VARIANT_ITEM());
	for (int i = nCount - 1, j = 0, k = 0; i >= 0; --i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		dValue = item._dValue;

		if (i >= nCount2) continue;
		const auto& nItem = n[i];
		if (!nItem.IsVaild()) continue;

		nSubCount = (long)nItem._dValue;
		if (nSubCount <= 0) nSubCount = i;

		nIndex = 1;

		for (j = i - 1, k = 1; j >= 0 && k < nSubCount; --j, ++k)
		{
			const auto& item2 = data[j];
			if (item2.IsVaild() && DoubleGT(item2._dValue, dValue)) ++nIndex;
		}

		dest[i].SetValue(nIndex);
	}
}

void VariantOperator::HOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nCount = (int)data.size();
	int nSubCount = 0, nIndex = 0;
	double dValue = 0;
	dest.resize(nCount, VARIANT_ITEM());
	for (int i = nCount - 1, j = 0, k = 0; i >= 0; --i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		dValue = item._dValue;

		nSubCount = n;
		if (nSubCount <= 0) nSubCount = i;

		nIndex = 1;
		for (j = i - 1, k = 1; j >= 0 && k < nSubCount; --j, ++k)
		{
			const auto& item2 = data[j];
			if (item2.IsVaild() && DoubleGT(item2._dValue, dValue)) ++nIndex;
		}

		dest[i].SetValue(nIndex);
	}
}

//N周期前的M周期内的第T个最小值.
//用法:FINDLOW(VAR,N,M,T):VAR在N日前的M天内第T个最低价
Variant* VariantOperator::FINDLOW(const Variant& data, const Variant& n, const Variant& m, const Variant& t)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(data.GetDoubleValue());
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe() && m.IsVaildDoulbe() && t.IsVaildDoulbe())
		{
			FINDLOW(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue(), (long)m.GetDoubleValue(), (long)t.GetDoubleValue());
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	return pResult;
}

void VariantOperator::FINDLOW(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	std::vector<double> aryValue;
	int nIndex = 0;
	for (int i = nCount - 1, j=0; i >= 0; --i)
	{
		aryValue.clear();
		for (j = n; j < m; ++j)
		{
			nIndex = i - j;
			if (nIndex < 0) break;
			const auto& item = data[nIndex];
			if (item.IsVaild()) aryValue.push_back(item._dValue);
		}

		if (!aryValue.empty())
		{
			std::sort(aryValue.begin(), aryValue.end());
			nIndex = t - 1;
			if (nIndex < 0) nIndex = 0;
			else if (nIndex >= (int)aryValue.size()) nIndex = (int)aryValue.size() - 1;
			dest[i].SetValue(aryValue[nIndex]);
		}
	}
}

Variant* VariantOperator::FINDHIGH(const Variant& data, const Variant& n, const Variant& m, const Variant& t)
{
	Variant* pResult = Create();

	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->SetDoubleValue(data.GetDoubleValue());
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe() && m.IsVaildDoulbe() && t.IsVaildDoulbe())
		{
			FINDHIGH(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue(), (long)m.GetDoubleValue(), (long)t.GetDoubleValue());
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::FINDHIGH(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	std::vector<double> aryValue;
	int nIndex = 0;
	for (int i = nCount - 1, j = 0; i >= 0; --i)
	{
		aryValue.clear();
		for (j = n; j < m; ++j)
		{
			nIndex = i - j;
			if (nIndex < 0) break;
			const auto& item = data[nIndex];
			if (item.IsVaild()) aryValue.push_back(item._dValue);
		}

		if (!aryValue.empty())
		{
			std::sort(aryValue.begin(), aryValue.end(), std::greater<double>());
			nIndex = t - 1;
			if (nIndex < 0) nIndex = 0;
			else if (nIndex >= (int)aryValue.size()) nIndex = (int)aryValue.size() - 1;
			dest[i].SetValue(aryValue[nIndex]);
		}
	}
}

Variant* VariantOperator::FINDHIGHBARS(const Variant& data, const Variant& n, const Variant& m, const Variant& t)
{
	Variant* pResult = Create();

	if (n.IsVaildDoulbe() && m.IsVaildDoulbe() && t.IsVaildDoulbe())
	{
		long ln = (long)n.GetDoubleValue(), lm = (long)m.GetDoubleValue(), lt = (long)t.GetDoubleValue();
		if (data.GetType() == Variant::DOUBLE_TYPE)
		{
			pResult->SetDoubleValue(ln - lm - lt);
		}
		else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			FINDHIGHBARS(pResult->m_aryValue, data.m_aryValue, ln, lm, lt);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::FINDHIGHBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	std::vector<std::pair<double,int>> aryValue;
	int nIndex = 0;
	for (int i = nCount - 1, j = 0; i >= 0; --i)
	{
		aryValue.clear();
		for (j = n; j < m; ++j)
		{
			nIndex = i - j;
			if (nIndex < 0) break;
			const auto& item = data[nIndex];
			if (item.IsVaild()) aryValue.push_back(std::pair<double, int>(item._dValue,j));
		}

		if (!aryValue.empty())
		{
			std::sort(aryValue.begin(), aryValue.end(), 
				[](const std::pair<double, int>& a, const std::pair<double, int>& b)
				{
					return a.first > b.first;
				});

			nIndex = t - 1;
			if (nIndex < 0) nIndex = 0;
			else if (nIndex >= (int)aryValue.size()) nIndex = (int)aryValue.size() - 1;
			dest[i].SetValue(aryValue[nIndex].second);
		}
	}
}

Variant* VariantOperator::FINDLOWBARS(const Variant& data, const Variant& n, const Variant& m, const Variant& t)
{
	Variant* pResult = Create();

	if (n.IsVaildDoulbe() && m.IsVaildDoulbe() && t.IsVaildDoulbe())
	{
		long ln = (long)n.GetDoubleValue(), lm = (long)m.GetDoubleValue(), lt = (long)t.GetDoubleValue();
		if (data.GetType() == Variant::DOUBLE_TYPE)
		{
			pResult->SetDoubleValue(ln - lm - lt);
		}
		else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			FINDLOWBARS(pResult->m_aryValue, data.m_aryValue, ln, lm, lt);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::FINDLOWBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	std::vector<std::pair<double, int>> aryValue;
	int nIndex = 0;
	for (int i = nCount - 1, j = 0; i >= 0; --i)
	{
		aryValue.clear();
		for (j = n; j < m; ++j)
		{
			nIndex = i - j;
			if (nIndex < 0) break;
			const auto& item = data[nIndex];
			if (item.IsVaild()) aryValue.push_back(std::pair<double, int>(item._dValue, j));
		}

		if (!aryValue.empty())
		{
			std::sort(aryValue.begin(), aryValue.end(),
				[](const std::pair<double, int>& a, const std::pair<double, int>& b)
				{
					return a.first < b.first;
				});

			nIndex = t - 1;
			if (nIndex < 0) nIndex = 0;
			else if (nIndex >= (int)aryValue.size()) nIndex = (int)aryValue.size() - 1;
			dest[i].SetValue(aryValue[nIndex].second);
		}
	}
}

//求自适应均线值.
//用法:AMA(X,A),A为自适应系数,必须小于1.
//算法:Y=Y'+A*(X-Y').初值为X
Variant* VariantOperator::AMA(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType()==Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			double dN = n.GetDoubleValue();
			if (dN <= 0)
			{
				AMA(pResult->m_aryValue, data.m_aryValue, dN);
				pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
			}
		}
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			pResult->SetDoubleValue(dValue);
		}
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		const auto& aryA = n.m_aryValue;
		if (!aryA.empty())
		{
			const auto last = aryA.rbegin();
			if (last->IsVaild() && last->_dValue <= 1)
			{
				AMA(pResult->m_aryValue, data.m_aryValue, last->_dValue);
				pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
			}
		}
	}

	return pResult;
}


void VariantOperator::AMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dA)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());

	bool bFirstFind = false;
	double dPreValue = 0, dValue = 0;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		if (bFirstFind == false)
		{
			dest[i].SetValue(item._dValue);
			dPreValue = item._dValue;
			bFirstFind = true;
		}
		else
		{
			dValue = dPreValue + dA * (item._dValue - dPreValue);
			dest[i].SetValue(dValue);
			dPreValue = dValue;
		}
	}
}


//返回移动平均
//用法:TMA(X,A,B),A和B必须小于1,算法	Y=(A*Y'+B*X),其中Y'表示上一周期Y值.初值为X
Variant* VariantOperator::TMA(const Variant& data, const Variant& a, const Variant& b, const IHistoryData* pHistoryData)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (a.GetType() == Variant::DOUBLE_TYPE && b.GetType() == Variant::DOUBLE_TYPE)
		{
			if (a.IsVaildDoulbe() && b.IsVaildDoulbe())
			{
				double dA = a.GetDoubleValue();
				double dB = b.GetDoubleValue();
				if (dA <= 0 && dB <= 0)
				{
					TMA(pResult->m_aryValue, data.m_aryValue, dA, dB);
					pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
				}
			}
		}
		else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			const auto& aryA = a.m_aryValue;
			const auto& aryB = b.m_aryValue;
			if (!aryA.empty() && !aryB.empty())
			{
				const auto lastA = aryA.rbegin();
				const auto lastB = aryB.rbegin();
				if (lastA->IsVaild() && lastA->_dValue <= 1 && lastB->IsVaild() && lastB->_dValue <= 1)
				{
					TMA(pResult->m_aryValue, data.m_aryValue, lastA->_dValue, lastB->_dValue);
					pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
				}
			}
		}
		else if (a.GetType() == Variant::DOUBLE_TYPE && b.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			const auto& aryB = b.m_aryValue;
			if (a.IsVaildDoulbe() && !aryB.empty())
			{
				double dA = a.GetDoubleValue();
				const auto lastB = aryB.rbegin();
				if (dA <= 1 && lastB->IsVaild() && lastB->_dValue <= 1)
				{
					TMA(pResult->m_aryValue, data.m_aryValue, dA, lastB->_dValue);
					pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
				}
			}
		}
		else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::DOUBLE_TYPE)
		{
			const auto& aryA = a.m_aryValue;
			if (!aryA.empty() && b.IsVaildDoulbe())
			{
				const auto lastA = aryA.rbegin();
				double dB = b.GetDoubleValue();
				if (lastA->IsVaild() && lastA->_dValue <= 1 && dB <= 1)
				{
					TMA(pResult->m_aryValue, data.m_aryValue, lastA->_dValue, dB);
					pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
				}
			}
		}
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		int nKCount = pHistoryData->GetKCount();
		if (data.IsVaildDoulbe() && nKCount>0)
		{
			double dData = data.GetDoubleValue();
			if (a.GetType() == Variant::DOUBLE_TYPE && b.GetType() == Variant::DOUBLE_TYPE)
			{
				if (a.IsVaildDoulbe() && b.IsVaildDoulbe())
				{
					double dA = a.GetDoubleValue();
					double dB = b.GetDoubleValue();
					if (dA <= 0 && dB <= 0)
					{
						TMA(pResult->m_aryValue, dData, dA, dB, nKCount);
						pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
					}
				}
			}
			else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::ARRAY_DOUBLE_TYPE)
			{
				const auto& aryA = a.m_aryValue;
				const auto& aryB = b.m_aryValue;
				if (!aryA.empty() && !aryB.empty())
				{
					const auto lastA = aryA.rbegin();
					const auto lastB = aryB.rbegin();
					if (lastA->IsVaild() && lastA->_dValue <= 1 && lastB->IsVaild() && lastB->_dValue <= 1)
					{
						TMA(pResult->m_aryValue, dData, lastA->_dValue, lastB->_dValue, nKCount);
						pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
					}
				}
			}
			else if (a.GetType() == Variant::DOUBLE_TYPE && b.GetType() == Variant::ARRAY_DOUBLE_TYPE)
			{
				const auto& aryB = b.m_aryValue;
				if (a.IsVaildDoulbe() && !aryB.empty())
				{
					double dA = a.GetDoubleValue();
					const auto lastB = aryB.rbegin();
					if (dA <= 1 && lastB->IsVaild() && lastB->_dValue <= 1)
					{
						TMA(pResult->m_aryValue, dData, dA, lastB->_dValue, nKCount);
						pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
					}
				}
			}
			else if (a.GetType() == Variant::ARRAY_DOUBLE_TYPE && b.GetType() == Variant::DOUBLE_TYPE)
			{
				const auto& aryA = a.m_aryValue;
				if (!aryA.empty() && b.IsVaildDoulbe())
				{
					const auto lastA = aryA.rbegin();
					double dB = b.GetDoubleValue();
					if (lastA->IsVaild() && lastA->_dValue <= 1 && dB <= 1)
					{
						TMA(pResult->m_aryValue, dData, lastA->_dValue, dB, nKCount);
						pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
					}
				}
			}
		}
	}

	return pResult;
}

void VariantOperator::TMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dA, double dB)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());

	bool bFirstFind = false;
	double dPreValue = 0, dValue = 0;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		if (bFirstFind == false)
		{
			dest[i].SetValue(item._dValue);
			dPreValue = item._dValue;
			bFirstFind = true;
		}
		else
		{
			dValue = dA * dPreValue + dB * item._dValue;
			dest[i].SetValue(dValue);
			dPreValue = dValue;
		}
	}
}

void VariantOperator::TMA(ARRAY_DOUBLE& dest, double dData, double dA, double dB, int nCount)
{
	dest.resize(nCount, VARIANT_ITEM());
	double dPreValue = 0, dValue = 0;
	for (int i = 0; i < nCount; ++i)
	{
		if (i == 0)
		{
			dest[i].SetValue(dData);
			dPreValue = dData;
		}
		else
		{
			dValue = dA * dPreValue + dB * dData;
			dest[i].SetValue(dValue);
		}
	}
}

/*
TRMA(X,N)： 求X在N个周期的三角移动平均值。

算法：三角移动平均线公式，是采用算数移动平均，并且对第一个移动平均线再一次应用算数移动平均。
 TRMA(X,N) 算法如下
 ma_half= MA(X,N/2)
 trma=MA(ma_half,N/2)

注：
1、N包含当前k线。
2、当N为有效值，但当前的k线数不足N根，函数返回空值。
3、N为0或空值的情况下，函数返回空值。

例1：
 TRMA5:TRMA(CLOSE,5);//计算5个周期内收盘价的三角移动平均。(N不能被2整除)
 //TRMA(CLOSE,5)=MA(MA(CLOSE,(5+1)/2)),(5+1)/2);
例2:
 TRMA10:TRMA(CLOSE,10);// 计算10个周期内收盘价的三角移动平均。(N能被2整除)
 TRMA(CLOSE,10)=MA(MA(CLOSE,10/2),(10/2)+1));
 */
Variant* VariantOperator::TRMA(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			int nPeriod = (int)n.GetDoubleValue();
			if (nPeriod <= 0) return pResult;
			Variant::ARRAY_DOUBLE aryMAFalf;
			int nFalfPeriod = 1, nFalfPeriod2 = 0;
			if ((nPeriod % 2) == 0)
			{
				nFalfPeriod = nPeriod / 2;
				nFalfPeriod2 = nPeriod / 2 + 1;
			}
			else
			{
				nFalfPeriod = nFalfPeriod2 = (nPeriod + 1) / 2;
			}

			MA(aryMAFalf, data.m_aryValue, nFalfPeriod);
			MA(pResult->m_aryValue, aryMAFalf, nFalfPeriod2);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

/*
	HARMEAN(X,N) 求X在N个周期内的调和平均值。

	算法举例：HARMEAN(X,5)=1/[(1/X1+1/X2+1/X3+1/X4+1/X5)/5]

	注：
	1、N包含当前k线。
	2、调和平均值与倒数的简单平均值互为倒数。
	3、当N为有效值，但当前的k线数不足N根，函数返回空值。
	4、N为0或空值的情况下，函数返回空值。
	5、X为0或空值的情况下，函数返回空值。
	6、N可以为变量。

	例：
	HM5:=HARMEAN(C,5);//求5周期收盘价的调和平均值。
*/
Variant* VariantOperator::HARMEAN(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		HARMEAN(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!n.IsVaildDoulbe()) return pResult;
		int nPeriod = (int)n.GetDoubleValue();
		if (nPeriod <= 0) return pResult;
		HARMEAN(pResult->m_aryValue, data.m_aryValue, nPeriod);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::HARMEAN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, int n)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	double dSum = 0;
	bool isInvaild = false;
	for (int i = 0,j=0; i < nCount; ++i)
	{
		if (n > i) continue;

		dSum = 0;
		isInvaild = false;
		for (j = 0; j < n; ++j)
		{
			const auto& item = data[i-j];
			if (!item.IsVaild() || item._dValue == 0)
			{
				isInvaild = true;
				break;
			}

			dSum += 1 / item._dValue;
		}

		if (isInvaild || dSum == 0) continue;
		dest[i].SetValue(1.0 / (dSum / n));
	}
}

void VariantOperator::HARMEAN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	double dSum = 0;
	int nPeriod = 0;
	bool isInvaild = false;
	int nPeriodCount = (int)n.size();
	
	for (int i = 0, j = 0; i < nCount; ++i)
	{
		if (i >= nPeriodCount) break;

		const auto& nItem = n[i];
		if (!nItem.IsVaild()) continue;
		nPeriod = (int)nItem._dValue;
		if (nPeriod <= 0) continue;
		if (nPeriod > i) continue;

		dSum = 0;
		isInvaild = false;
		for (j = 0; j < nPeriod; ++j)
		{
			const auto& item = data[i - j];
			if (!item.IsVaild() || item._dValue == 0)
			{
				isInvaild = true;
				break;
			}

			dSum += 1 / item._dValue;
		}

		if (isInvaild || dSum == 0) continue;
		dest[i].SetValue(1.0 / (dSum / nPeriod));
	}
}

Variant* VariantOperator::VALUEWHEN(const Variant& cond, const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && cond.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		VALUEWHEN(pResult->m_aryValue, cond.m_aryValue, data.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return pResult;
		double dValue = data.GetDoubleValue();
		VALUEWHEN(pResult->m_aryValue, data.m_aryValue, dValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::VALUEWHEN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, double dValue)
{
	int nCount = (int)cond.size();
	dest.resize(nCount, VARIANT_ITEM());

	bool bFind = false;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = cond[i];
		if (bFind)
		{
			dest[i].SetValue(dValue);
		}
		else
		{
			if (!item.IsVaild()) continue;
			if (!item._dValue) continue;
			bFind = true;
			dest[i].SetValue(dValue);
		}
	}

}

void VariantOperator::VALUEWHEN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data)
{
	int nCount = (int)cond.size();
	int nDataCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());

	VARIANT_ITEM preItem;
	for (int i = 0; i < nCount; ++i)
	{
		if (i >= nDataCount)
		{
			if (preItem.IsVaild()) dest[i].SetValue(preItem._dValue);
			continue;
		}

		const auto& item = cond[i];
		const auto& dataItem = data[i];
		if (!item.IsVaild() || !item._dValue || !dataItem.IsVaild())
		{
			if (preItem.IsVaild()) dest[i].SetValue(preItem._dValue);
		}
		else
		{
			dest[i].SetValue(dataItem._dValue);
			preItem.SetValue(dataItem._dValue);
		}
	}
}


Variant* VariantOperator::BARSSINCEN(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			BARSSINCEN(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	return pResult;
}

void VariantOperator::BARSSINCEN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());

	int nStart = GetFirstVaildIndex(data);
	if (nStart >= nCount) return;
	int i = nStart;
	for (; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		if (DoubleGTE(item._dValue,0)) 
			break;
	}

	if (i >= nCount) return;

	int j = 0, nPeriod = n;
	if (nPeriod > nCount) nPeriod = nCount;
	if (i <= nPeriod - 1) j = nPeriod - 1;
	else j = i;

	dest[j].SetValue(j - i); 
	++j;

	for (; j < nCount; ++j)
	{
		if (dest[j - 1].IsVaild())
		{
			if (dest[j - 1]._dValue + 1 < nPeriod)
			{
				dest[j].SetValue(dest[j - 1]._dValue + 1);
			}
			else
			{
				for (int k = j - nPeriod + 1; k <= j; ++k)
				{
					const auto& item = data[k];
					if (item.IsVaild() && !(fabs(item._dValue) < 0.000001))
					{
						dest[j].SetValue(j - k);
						break;
					}
				}
			}
		}
		else
		{
			const auto& item = data[i];
			if (item.IsVaild() && !(abs(item._dValue) < 0.000001))
				dest[j].SetValue(0);
		}
	}
}


// dValue>dValue2
bool VariantOperator::DoubleGT(const double& dValue, const double& dValue2)
{
	return dValue - dValue2 > 0.000001;
}

// dValue<dValue2
bool VariantOperator::DoubleLT(const double& dValue, const double& dValue2)
{
	return dValue - dValue2 < 0.000001;
}

bool VariantOperator::DoubleGTE(const double& dValue, const double& dValue2)
{
	return dValue - dValue2 >= 0.000001;
}

bool VariantOperator::DoubleLTE(const double& dValue, const double& dValue2)
{
	return dValue - dValue2 <= 0.000001;
}


// dValue==dValue2
bool VariantOperator::DobuleEQ(const double& dValue, const double& dValue2)
{
	return abs(dValue - dValue2) < 0.000001;
}

}
}