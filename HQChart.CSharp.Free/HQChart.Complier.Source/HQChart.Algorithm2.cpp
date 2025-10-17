/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

内置函数类 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.KLineData.h"
#include "HQChart.Execute.h"
#include <list>
#include<random>
#include<time.h>
#include <cfloat>

////////////////////////////////////////////////////////////////////////////////
//	内置函数2
//
///////////////////////////////////////////////////////////////////////////////

namespace HQChart { namespace Complier {


#ifdef _WIN32
#define VARIANT_TRIGONOMETRIC_FUNCTIONS(__funcname__, __op__)\
	Variant* VariantOperator::##__funcname__##(const Variant& data)\
	{\
		Variant* pResult = Create();\
		if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)\
				{\
			ARRAY_DOUBLE& dest = pResult->m_aryValue;\
			const ARRAY_DOUBLE& src = data.m_aryValue;\
			int nDataCount = (int)src.size();\
			dest.resize(nDataCount);\
			for (int i = 0; i < nDataCount; ++i)\
						{\
				if (!src[i].IsVaild()) continue;\
				dest[i].SetValue(##__op__##(src[i]._dValue));\
						}\
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);\
				}\
				else if (data.m_nType == Variant::DOUBLE_TYPE)\
		{\
			if (data.IsVaildDoulbe()) pResult->SetDoubleValue(##__op__##(data.GetDoubleValue()));\
		}\
		return pResult;\
	}
#else
#define VARIANT_TRIGONOMETRIC_FUNCTIONS(__funcname__, __op__)\
	Variant* VariantOperator::__funcname__(const Variant& data)\
	{\
		Variant* pResult = Create();\
		if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)\
				{\
			ARRAY_DOUBLE& dest = pResult->m_aryValue;\
			const ARRAY_DOUBLE& src = data.m_aryValue;\
			int nDataCount = (int)src.size();\
			dest.resize(nDataCount);\
			for (int i = 0; i < nDataCount; ++i)\
						{\
				if (!src[i].IsVaild()) continue;\
				dest[i].SetValue(__op__(src[i]._dValue));\
						}\
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);\
				}\
				else if (data.m_nType == Variant::DOUBLE_TYPE)\
		{\
			if (data.IsVaildDoulbe()) pResult->SetDoubleValue(__op__(data.GetDoubleValue()));\
		}\
		return pResult;\
	}
#endif


Variant* VariantOperator::AVEDEV(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			AVEDEV(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}


void VariantOperator::AVEDEV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount, VARIANT_ITEM());
	if (n <= 0) return;
	double dTotal = 0;
	std::vector<double> aryAverage;	//平均值
	aryAverage.resize(nDataCount, 0);
	for (int i = n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild()) dTotal += data[i - j]._dValue;
		}
		aryAverage[i] = dTotal / n;
	}

	for (int i = n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild()) dTotal += abs(data[i - j]._dValue - aryAverage[i]);
		}

		dest[i].SetValue(dTotal / n);
	}
}

Variant* VariantOperator::STD(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			STD(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::STD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	dest.resize(nDataCount, VARIANT_ITEM());
	if (n <= 0) return;
	int nStart = (long)GetFirstVaildIndex(data);
	double dTotal = 0;
	std::vector<double> aryAverage;	//平均值
	aryAverage.resize(nDataCount, 0);
	for (int i = nStart + n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild()) dTotal += data[i - j]._dValue;
		}
		aryAverage[i] = dTotal / n;
	}

	for (int i = nStart + n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild())
				dTotal += pow((data[i - j]._dValue - aryAverage[i]), 2);
		}

		dest[i].SetValue(sqrt(dTotal / (n-1)));
	}
}

/*
DEVSQ(X,N) 返回数据偏差平方和
*/
Variant* VariantOperator::DEVSQ(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			DEVSQ(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::DEVSQ(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	if (nDataCount <= 0) return;
	if (n <= 0) return;
	dest.resize(nDataCount, VARIANT_ITEM());
	if (n <= 0) return;
	int nStart = (long)GetFirstVaildIndex(data);
	double dTotal = 0;
	std::vector<double> aryAverage;	//平均值
	aryAverage.resize(nDataCount, 0);
	for (int i = nStart + n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild()) dTotal += data[i - j]._dValue;
		}
		aryAverage[i] = dTotal / n;
	}

	for (int i = nStart + n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild())
				dTotal += pow((data[i - j]._dValue - aryAverage[i]), 2);
		}

		dest[i].SetValue(dTotal);
	}
}

Variant* VariantOperator::FORCAST(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			FORCAST(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::FORCAST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	if (n < 1) return;
	long lDataCount = (long)data.size();
	long lStart = (long)GetFirstVaildIndex(data);
	dest.resize(lDataCount, VARIANT_ITEM());

	double dEx = 0, dEy = 0, dSxy = 0, dSxx = 0, dConst, dSlope;
	long x = 0;
	for (long j = lStart, i = lStart + n - 1; i < lDataCount; ++i)
	{
		dEx = dEy = dSxy = dSxx = 0;
		for (j = 0, x = n; j<n && j <= i; ++j, --x)
		{
			dEx += x;
			dEy += data[i - j]._dValue;
		}

		dEx /= n, dEy /= n;
		for (j = 0, x = n; j<n &&j <= i; ++j, --x)
		{
			dSxy += (x - dEx)*(data[i - j]._dValue - dEy);
			dSxx += (x - dEx)*(x - dEx);
		}

		dSlope = dSxy / dSxx;
		dConst = dEy - dEx*dSlope;
		dest[i].SetValue(dSlope * n + dConst);
	}

}

Variant* VariantOperator::SLOPE(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			SLOPE(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::SLOPE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	if (n < 1) return;
	long lDataCount = (long)data.size();
	long lStart = (long)GetFirstVaildIndex(data);
	dest.resize(lDataCount, VARIANT_ITEM());

	double dEx = 0, dEy = 0, dSxy = 0, dSxx = 0;
	for (long j = lStart, i = lStart + n - 1; i < lDataCount; ++i)
	{
		dEx = dEy = dSxy = dSxx = 0;
		for (j = 0; j<n && j <= i; ++j)
		{
			dEx += (i - j);
			dEy += data[i - j]._dValue;
		}

		dEx /= n, dEy /= n;
		for (j = 0; j<n &&j <= i; ++j)
		{
			dSxy += (i - j - dEx)*(data[i - j]._dValue - dEy);
			dSxx += (i - j - dEx)*(i - j - dEx);
		}

		if (dSxx) dest[i].SetValue(dSxy / dSxx);
		else if (i - 1 >= 0) dest[i] = dest[i - 1];
	}
}

//STDP(X,N) 返回总体标准差
Variant* VariantOperator::STDP(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			STDP(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::STDP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	if (n < 1) return;
	long lCount = (long)data.size();
	long lStart = (long)GetFirstVaildIndex(data);
	dest.resize(lCount, VARIANT_ITEM());

	long i = lStart, j = 0;
	double dSigmaPowerX = 0, dSigmaX = 0, dMidResult = 0;
	for (; i<lCount && j<n; ++i, ++j)
	{
		dSigmaPowerX += data[i]._dValue * data[i]._dValue;
		dSigmaX += data[i]._dValue;
	}

	if (j == n)
	{
		dMidResult = n*dSigmaPowerX - dSigmaX*dSigmaX;
		dest[i - 1].SetValue(sqrt(dMidResult) / n);
	}

	for (; i<lCount; ++i)
	{
		dSigmaPowerX += data[i]._dValue * data[i]._dValue - data[i - n]._dValue * data[i - n]._dValue;
		dSigmaX += data[i]._dValue - data[i - n]._dValue;
		dMidResult = n*dSigmaPowerX - dSigmaX*dSigmaX;
		dest[i].SetValue(sqrt(dMidResult) / n);
	}
}

Variant* VariantOperator::VAR(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			VAR(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::VAR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	if (n < 1) return;
	long lCount = (long)data.size();
	long lStart = (long)GetFirstVaildIndex(data);
	dest.resize(lCount, VARIANT_ITEM());

	double dSigmaPowerX = 0, dSigmaX = 0;
	for (long j = 0, i = lStart + n - 1; i<lCount; ++i)
	{
		dSigmaPowerX = dSigmaX = 0;
		for (j = 0; j<n && j <= i; ++j)
		{
			dSigmaPowerX += data[i - j]._dValue * data[i - j]._dValue;
			dSigmaX += data[i - j]._dValue;
		}
		dest[i].SetValue((n*dSigmaPowerX - dSigmaX*dSigmaX) / n*(n - 1));
	}
}

Variant* VariantOperator::VARP(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			VARP(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::VARP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	if (n < 1) return;
	long lCount = (long)data.size();
	long lStart = (long)GetFirstVaildIndex(data);
	dest.resize(lCount, VARIANT_ITEM());

	double dSigmaPowerX = 0, dSigmaX = 0;
	long i = lStart, j = 0;
	for (; i<lCount&&j<n; ++i, ++j)
	{
		dSigmaPowerX += data[i]._dValue * data[i]._dValue;
		dSigmaX += data[i]._dValue;
	}

	if (j == n) dest[i - 1].SetValue((n*dSigmaPowerX - dSigmaX*dSigmaX) / (n*n));
	for (; i<lCount; ++i)
	{
		dSigmaPowerX += data[i]._dValue * data[i]._dValue - data[i - n]._dValue * data[i - n]._dValue;
		dSigmaX += data[i]._dValue - data[i - n]._dValue;
		dest[i].SetValue((n*dSigmaPowerX - dSigmaX*dSigmaX) / (n*n));
	}
}

Variant* VariantOperator::COVAR(const Variant& data, const Variant& data2, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && data2.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			COVAR(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::COVAR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, long n)
{
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();
	if (nDataCount <= 0 || nData2Count <= 0) return;
	if (n <= 0) return;
	int nCount = std::max(nDataCount, nData2Count);
	dest.resize(nCount, VARIANT_ITEM());

	int nStart = (long)GetFirstVaildIndex(data);
	double dTotal = 0;
	std::vector<double> aryAverage;	//平均值
	aryAverage.resize(nDataCount, 0);
	for (int i = nStart + n - 1, j = 0; i < nDataCount; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data[i - j].IsVaild()) dTotal += data[i - j]._dValue;
		}
		aryAverage[i] = dTotal / n;
	}

	int nStart2 = (long)GetFirstVaildIndex(data2);
	dTotal = 0;
	std::vector<double> aryAverage2;	//平均值
	aryAverage2.resize(nData2Count, 0);
	for (int i = nStart2 + n - 1, j = 0; i < nData2Count; ++i)
	{
		dTotal = 0;
		for (j = 0; j < n; ++j)
		{
			if (data2[i - j].IsVaild()) dTotal += data2[i - j]._dValue;
		}
		aryAverage2[i] = dTotal / n;
	}

	double dAverage = 0, dAverage2 = 0;
	for (int i = std::max(nStart, nStart2), j = 0; i < nCount; ++i)
	{
		if (i >= nDataCount || i >= nData2Count) continue;

		dAverage = aryAverage[i];
		dAverage2 = aryAverage2[i];

		dTotal = 0;
		for (j = i - n + 1; j <= i; ++j)
		{
			dTotal += (data[j]._dValue - dAverage)*(data2[j]._dValue - dAverage2);
		}

		dest[i].SetValue(dTotal / n);
	}

}

Variant* VariantOperator::IFF(const Variant& data, const Variant& trueData, const Variant& falseData)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::DOUBLE_TYPE && data.IsVaildDoulbe()) //单数值
	{
		*pResult = data.GetDoubleValue() ? trueData : falseData;
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		bool isDouble = trueData.m_nType == Variant::DOUBLE_TYPE;
		double dTrueValue = 0;
		if (isDouble) dTrueValue = trueData.GetDoubleValue();

		bool isDouble2 = falseData.m_nType == Variant::DOUBLE_TYPE;
		double dFalseValue = 0;
		if (isDouble2) dFalseValue = falseData.GetDoubleValue();

		int nDataCount = (int)data.m_aryValue.size();
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		dest.resize(nDataCount, VARIANT_ITEM());
		for (int i = 0; i < nDataCount; ++i)
		{
			if (!data.m_aryValue[i].IsVaild()) continue;
			if (data.m_aryValue[i]._dValue)
			{
				if (isDouble)
				{
					dest[i].SetValue(dTrueValue);
				}
				else
				{
					if (i<(int)trueData.m_aryValue.size())
						dest[i] = trueData.m_aryValue[i];
				}
			}
			else
			{
				if (isDouble2)
				{
					dest[i].SetValue(dFalseValue);
				}
				else
				{
					if (i<(int)falseData.m_aryValue.size())
						dest[i] = falseData.m_aryValue[i];
				}
			}
		}

		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	return pResult;
}

Variant* VariantOperator::IFN(const Variant& data, const Variant& trueData, const Variant& falseData)
{
	return IFF(data, falseData, trueData);
}

Variant* VariantOperator::IFS(const Variant& data, const Variant& trueData, const Variant& falseData)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::DOUBLE_TYPE && data.IsVaildDoulbe()) //单数值
	{
		*pResult = data.GetDoubleValue() ? trueData : falseData;
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && trueData.GetType() == Variant::STRING_TYPE && falseData.GetType() == Variant::STRING_TYPE)
	{
		const ARRAY_DOUBLE src = data.m_aryValue;
		int nDataCount = (int)src.size();
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const std::wstring strTrue = trueData.GetStringValue();
		const std::wstring strFalse = falseData.GetStringValue();

		dest.resize(nDataCount, VARIANT_ITEM());
		for (int i = 0; i < nDataCount; ++i)
		{
			const auto& item = src[i];
			if (dest[i]._pString == NULL) dest[i]._pString = CreateString();
			if (!item.IsVaild())
			{
				dest[i].SetValue(strFalse);
			}
			else
			{
				if (item._dValue) dest[i].SetValue(strTrue);
				else dest[i].SetValue(strFalse);
			}
		}

		pResult->SetType(Variant::ARRAY_STRING_TYPE);
	}
	return pResult;
}

Variant* VariantOperator::NOT(const Variant& data)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nDataCount = (int)src.size();
		dest.resize(nDataCount, VARIANT_ITEM());
		for (int i = 0; i < nDataCount; ++i)
		{
			const auto& item = src[i];
			if (!item.IsVaild()) continue;
			dest[i].SetValue(item._dValue ? 0 : 1);
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			pResult->SetDoubleValue(data.GetDoubleValue() ? 0 : 1);
		}
	}

	return pResult;
}


/*
引用若干周期前的数据(平滑处理).
用法: REF(X,A),引用A周期前的X值.A可以是变量.
平滑处理:当引用不到数据时进行的操作.此函数中,平滑时使用上一个周期的引用值.
例如: REF(CLOSE,BARSCOUNT(C)-1)表示第二根K线的收盘价.
*/
Variant* VariantOperator::REF(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			REF(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		REF(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
	}

	return pResult;
}

void VariantOperator::REF(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);

	if (nDataCount > 0)
	{
		const auto& firstData = data[0];	//引用不到数据平滑处理
		for (int i = 0; i < n && i< nDataCount; ++i)
		{
			dest[i] = firstData;
		}
	}

	for (int i = n; i < nDataCount; ++i)
	{
		dest[i] = data[i - n];
	}
}

void VariantOperator::REF(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);
	int nValue = 0;
	for (int i = 0; i < nDataCount; ++i)
	{
		if (i >= (long)n.size()) continue;

		const auto& itemN = n[i];
		if (!itemN.IsVaild()) continue;
		nValue = (int)itemN._dValue;

		if (nValue >= 0 && nValue <= i) dest[i] = data[i - nValue];
		else if (i>0) dest[i] = dest[i - 1];	//引用不到数据平滑处理
		else dest[i] = data[i];
	}
}

/*
引用若干周期前的数据(未作平滑处理).
用法: REFV(X,A),引用A周期前的X值.A可以是变量.
平滑处理:当引用不到数据时进行的操作.
例如:REFV(CLOSE,BARSCOUNT(C)-1)表示第二根K线的收盘价.
*/
Variant* VariantOperator::REFV(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			REFV(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		REFV(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
	}

	return pResult;
}

void VariantOperator::REFV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);
	for (int i = n; i < nDataCount; ++i)
	{
		dest[i] = data[i - n];
	}
}

void VariantOperator::REFV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);
	int nValue = 0;
	for (int i = 0; i < nDataCount; ++i)
	{
		if (i >= (long)n.size()) continue;

		const auto& itemN = n[i];
		if (!itemN.IsVaild()) continue;
		nValue = (int)itemN._dValue;

		if (nValue >= 0 && nValue <= i) dest[i] = data[i - nValue];
	}
}

/*
属于未来函数,引用若干周期后的数据(平滑处理).
用法: REFX(X,A),引用A周期后的X值.A可以是变量.
平滑处理:当引用不到数据时进行的操作.此函数中,平滑时使用上一个周期的引用值.
例如:
TT:=IF(C>O,1,2);
REFX(CLOSE,TT);表示阳线引用下一周期的收盘价,阴线引用日后第二周期的收盘价.
*/
Variant* VariantOperator::REFX(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			REFX(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		REFX(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
	}

	return pResult;
}

void VariantOperator::REFX(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);

	int nIndex = 0;
	for (nIndex = 0; nIndex + n<nDataCount; ++nIndex)
	{
		dest[nIndex] = data[nIndex + n];
	}

	if (nDataCount > 0)
	{
		const auto& lastData = data[nDataCount - 1];	//引用不到数据平滑处理
		for (; nIndex < nDataCount; ++nIndex)
		{
			dest[nIndex] = lastData;
		}
	}
}

void VariantOperator::REFX(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);
	int nValue = 0;
	for (int i = 0; i < nDataCount; ++i)
	{
		if (i >= (long)n.size()) continue;

		const auto& itemN = n[i];
		if (!itemN.IsVaild()) continue;
		nValue = (int)itemN._dValue;

		if (nValue >= 0 && nValue + i <nDataCount) dest[i] = data[i + nValue];
		else if (i>0) dest[i] = dest[i - 1];
		else dest[i] = data[i];
	}
}

/*
属于未来函数,引用若干周期后的数据(未作平滑处理).
用法: REFXV(X,A),引用A周期后的X值.A可以是变量.
平滑处理:当引用不到数据时进行的操作.
例如: REFXV(CLOSE,1)表示下一周期的收盘价,在日线上就是明天收盘价
*/

Variant* VariantOperator::REFXV(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			REFXV(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}
	else if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && n.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		REFXV(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
	}

	return pResult;
}

void VariantOperator::REFXV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);

	int nIndex = 0;
	for (nIndex = 0; nIndex + n<nDataCount; ++nIndex)
	{
		dest[nIndex] = data[nIndex + n];
	}
}

void VariantOperator::REFXV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount);
	int nValue = 0;
	for (int i = 0; i < nDataCount; ++i)
	{
		if (i >= (long)n.size()) continue;

		const auto& itemN = n[i];
		if (!itemN.IsVaild()) continue;
		nValue = (int)itemN._dValue;

		if (nValue >= 0 && nValue + i <nDataCount) dest[i] = data[i + nValue];
	}
}

/*
引用自1900年以来指定日期的数据.
用法: REFDATE(X, A), 引用A日期的X值.
例如 : REFDATE(CLOSE, 1011208)表示2001年12月08日的收盘价
*/
Variant* VariantOperator::REFDATE(const Variant& data, const Variant& n, const IHistoryData* pHistoryData, Node* pNode)
{
	if (!pHistoryData)
	{
		ExecuteExcept error(L"pHistoryData is null", L"VariantOperator::REFDATE", pNode);
		throw error;
	}

	Variant* pResult = Create();
	if (data.GetType() != Variant::ARRAY_DOUBLE_TYPE) return pResult;

	long lDate = 0;
	if (n.GetType() == Variant::DOUBLE_TYPE)
	{
		lDate = (long)n.GetDoubleValue();
	}
	else if (n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (n.m_aryValue.empty()) return pResult;
		const auto& item = n.m_aryValue[n.m_aryValue.size()-1];
		if (!item.IsVaild()) return pResult;

		lDate = (long)item._dValue;
	}

	if (lDate <= 0) return pResult;
	if (lDate < 5000000) lDate += 19000000;
	if (lDate < 19900101) return pResult;


	long lID = pHistoryData->FindByDate(lDate);
	if (lID < 0) return pResult;

	const ARRAY_DOUBLE& src = data.m_aryValue;
	if (lID < (long)src.size())
	{
		const auto& item = src[lID];
		if (item.IsVaild()) pResult->SetDoubleValue(item._dValue);
	}

	return pResult;
}

Variant* VariantOperator::ACOS(const Variant& data)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nDataCount = (int)src.size();
		dest.resize(nDataCount);
		double dValue = 0;
		for (int i = 0; i < nDataCount; ++i)
		{
			if (!src[i].IsVaild()) continue;

			dValue = src[i]._dValue;
			if (dValue >= -1 && dValue <= 1)
			{
				dest[i].SetValue(acos(dValue));
			}
			else if (i - 1 >= 0)	//使用上一个有效数据
			{
				const auto& preItem = dest[i - 1];
				if (preItem.IsVaild()) dest[i].SetValue(preItem._dValue);
			}

		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			if (dValue >= -1 && dValue <= 1) pResult->SetDoubleValue(acos(dValue));
		}
	}

	return pResult;
}

VARIANT_TRIGONOMETRIC_FUNCTIONS(ATAN, atan);
VARIANT_TRIGONOMETRIC_FUNCTIONS(COS, cos);
VARIANT_TRIGONOMETRIC_FUNCTIONS(SIN, sin);
VARIANT_TRIGONOMETRIC_FUNCTIONS(TAN, tan);
VARIANT_TRIGONOMETRIC_FUNCTIONS(EXP, exp);
VARIANT_TRIGONOMETRIC_FUNCTIONS(LN, log);
VARIANT_TRIGONOMETRIC_FUNCTIONS(LOG, log10);
VARIANT_TRIGONOMETRIC_FUNCTIONS(ABS, abs);

Variant* VariantOperator::ASIN(const Variant& data)
{
	Variant* pResult = Create();
	double dValue = 0;
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nDataCount = (int)src.size();
		dest.resize(nDataCount);
		for (int i = 0; i < nDataCount; ++i)
		{
			if (!src[i].IsVaild()) continue;
			dValue = src[i]._dValue;
			if (dValue >= -1 && dValue <= 1)
			{
				dest[i].SetValue(asin(dValue));
			}
			else if (i - 1 >= 0)
			{
				const auto& preItem = dest[i - 1];
				if (preItem.IsVaild()) dest[i].SetValue(preItem._dValue);
			}
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			dValue = data.GetDoubleValue();
			if (dValue>-1 && dValue<1) pResult->SetDoubleValue(asin(dValue));
		}
	}
	return pResult;
}


Variant* VariantOperator::SQRT(const Variant& data)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		int nDataCount = (int)src.size();
		dest.resize(nDataCount);
		double dValue = 0;
		for (int i = 0; i < nDataCount; ++i)
		{
			if (!src[i].IsVaild()) continue;
			dValue = src[i]._dValue;
			if (dValue >= 0) dest[i].SetValue(sqrt(dValue));
		}
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			if (dValue >= 0) pResult->SetDoubleValue(sqrt(dValue));
		}
	}

	return pResult;
}


/*
乘幂.
用法:POW(A,B)返回A的B次幂
例如:POW(CLOSE,3)求得收盘价的3次方
*/
Variant* VariantOperator::POW(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();
	if (ln < 1) return pResult;

	if (data.GetType() == Variant::DOUBLE_TYPE && data.IsVaildDoulbe())
	{
		pResult->SetDoubleValue(pow(data.GetDoubleValue(), ln));
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lCount = (long)src.size();
		dest.resize(lCount, VARIANT_ITEM());
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		for (long i = 0; i < lCount; ++i)
		{
			const auto& dataItem = src[i];
			if (!dataItem.IsVaild()) continue;

			dest[i].SetValue(pow(dataItem._dValue, ln));
		}
	}

	return pResult;
}

/*
向上舍入.
用法:CEILING(A)返回沿A数值增大方向最接近的整数
例如:CEILING(12.3)求得13,CEILING(-3.5)求得-3
*/
Variant* VariantOperator::CEILING(const Variant& data)
{
	Variant* pResult = Create();
	double dValue = 0;
	if (data.GetType() == Variant::DOUBLE_TYPE && data.IsVaildDoulbe())
	{
		dValue = data.GetDoubleValue();
		if (dValue > 0) pResult->SetDoubleValue((int)(dValue + 1));
		else if (dValue < 0) pResult->SetDoubleValue((int)(dValue));
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lCount = (long)src.size();
		dest.resize(lCount, VARIANT_ITEM());

		for (long i = 0; i < lCount; ++i)
		{
			const auto& dataItem = src[i];
			if (!dataItem.IsVaild()) continue;

			dValue = dataItem._dValue;
			if (dValue > 0) pResult->SetDoubleValue((int)(dValue + 1));
			else if (dValue < 0) pResult->SetDoubleValue((int)(dValue));
		}
	}

	return pResult;
}

/*
向下舍入.
用法:FLOOR(A)返回沿A数值减小方向最接近的整数
例如:FLOOR(12.3)求得12,FLOOR(-3.5)求得-4
*/

Variant* VariantOperator::FLOOR(const Variant& data)
{
	Variant* pResult = Create();
	double dValue = 0;
	if (data.GetType() == Variant::DOUBLE_TYPE && data.IsVaildDoulbe())
	{
		dValue = data.GetDoubleValue();
		if (dValue > 0) pResult->SetDoubleValue((int)(dValue));
		else if (dValue < 0) pResult->SetDoubleValue((int)(dValue - 1));
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lCount = (long)src.size();
		dest.resize(lCount, VARIANT_ITEM());

		for (long i = 0; i < lCount; ++i)
		{
			const auto& dataItem = src[i];
			if (!dataItem.IsVaild()) continue;

			dValue = dataItem._dValue;
			if (dValue > 0) pResult->SetDoubleValue((int)(dValue));
			else if (dValue < 0) pResult->SetDoubleValue((int)(dValue - 1));
		}
	}

	return pResult;
}

/*
取整.
用法:INTPART(A)返回沿A绝对值减小方向最接近的整数
例如:INTPART(12.3)求得12,INTPART(-3.5)求得-3
*/

Variant* VariantOperator::INTPART(const Variant& data)
{
	Variant* pResult = Create();

	double dValue = 0;
	if (data.GetType() == Variant::DOUBLE_TYPE && data.IsVaildDoulbe())
	{
		dValue = data.GetDoubleValue();
		pResult->SetDoubleValue((int)(dValue));
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lCount = (long)src.size();
		dest.resize(lCount, VARIANT_ITEM());

		for (long i = 0; i < lCount; ++i)
		{
			const auto& dataItem = src[i];
			if (!dataItem.IsVaild()) continue;

			dValue = dataItem._dValue;
			pResult->SetDoubleValue((int)(dValue));
		}
	}

	return pResult;
}

//用法:BETWEEN(A,B,C)表示A处于B和C之间时返回1,否则返回0 
//例如:BETWEEN(CLOSE,MA(CLOSE,20),MA(CLOSE,10))表示收盘价介于10日均线和20日均线之间
Variant* VariantOperator::BETWEEN(const Variant& data, const Variant& startData, const Variant& endData)
{
	Variant* pResult = Create();
	bool bIsNumber = data.GetType() == Variant::DOUBLE_TYPE;
	bool bIsNumber2 = startData.GetType() == Variant::DOUBLE_TYPE;
	bool bIsNumber3 = endData.GetType() == Variant::DOUBLE_TYPE;

	if (bIsNumber && bIsNumber2 && bIsNumber3)	//单数值
	{
		if (data.IsVaildDoulbe() && startData.IsVaildDoulbe() && endData.IsVaildDoulbe())
		{
			double dCondition = data.GetDoubleValue();
			double dStart = startData.GetDoubleValue();
			double dEnd = endData.GetDoubleValue();
			pResult->SetDoubleValue(DoubleGTE(dCondition, dStart) && DoubleLTE(dCondition, dEnd) ? 1 : 0);
		}
		return pResult;
	}

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		int nDataCount = (int)data.m_aryValue.size();
		VARIANT_ITEM falseItem;
		falseItem.SetValue(0);
		dest.resize(nDataCount, falseItem);

		double dCondition = 0, dStart = 0, dEnd = 0;

		for (int i = 0; i < nDataCount; ++i)
		{
			const auto& item = data.m_aryValue[i];
			if (!item.IsVaild()) continue;
			dCondition = item._dValue;
			if (bIsNumber2)
			{
				dStart = startData.GetDoubleValue();
			}
			else
			{
				if (i >= (int)startData.m_aryValue.size()) continue;
				if (!startData.m_aryValue[i].IsVaild()) continue;
				dStart = startData.m_aryValue[i]._dValue;
			}

			if (bIsNumber3)
			{
				dEnd = endData.GetDoubleValue();
			}
			else
			{
				if (i >= (long)endData.m_aryValue.size()) continue;
				if (!endData.m_aryValue[i].IsVaild()) continue;
				dEnd = endData.m_aryValue[i]._dValue;
			}

			if (dStart > dEnd)
			{
				if (DoubleGTE(dCondition, dEnd) && DoubleLTE(dCondition, dStart)) dest[i].SetValue(1);
			}
			else
			{
				if (DoubleGTE(dCondition, dStart) && DoubleLTE(dCondition, dEnd)) dest[i].SetValue(1);
			}
		}
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

/*
SGN	符号值
阐释:SGN(X)，当X>0,X=0,X<0分别返回1,0,-1
*/
Variant* VariantOperator::SGN(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			if (dValue > 0) pResult->SetDoubleValue(1);
			else if (dValue == 0) pResult->SetDoubleValue(0);
			else pResult->SetDoubleValue(-1);
		}
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lCount = (long)src.size();
		dest.resize(lCount, VARIANT_ITEM());

		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = src[i];
			if (!item.IsVaild()) continue;
			if (item._dValue > 0) dest[i].SetValue(1);
			else if (item._dValue == 0) dest[i].SetValue(0);
			else dest[i].SetValue(-1);
		}

		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);

	}

	return pResult;
}

/*
FRACPART(A)	 取得小数部分
含义:FRACPART(A)返回数值的小数部分
阐释:例如FRACPART(12.3)求得0.3,FRACPART(-3.5)求得-0.5
*/
Variant* VariantOperator::FRACPART(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		long lInteger = 0;//	整数部分
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			lInteger = (long)dValue;
			pResult->SetDoubleValue(dValue - lInteger);
		}
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lCount = (long)src.size();
		dest.resize(lCount, VARIANT_ITEM());

		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = src[i];
			if (!item.IsVaild()) continue;
			if (item._dValue > 0) dest[i].SetValue(1);
			else if (item._dValue == 0) dest[i].SetValue(0);
			else dest[i].SetValue(-1);
		}

		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

Variant* VariantOperator::MOD(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		MOD(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data2.IsVaildDoulbe())
		{
			MOD(pResult->m_aryValue, data.m_aryValue, data2.GetDoubleValue());
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe() && data2.IsVaildDoulbe())
		{
			pResult->SetDoubleValue(MOD(data.GetDoubleValue(), data2.GetDoubleValue()));
		}
	}

	return pResult;
}

double VariantOperator::MOD(double dA, double dB)
{
	double dResult = 0;
	if ((dA > 0 && dB > 0) || (dA < 0 || dB < 0))	//同号
	{
		long lA = (long)(dA), lB = (long)(dB);
		if (lA == dA && lB == dB)  //全为整数
		{
			return lA%lB;
		}
		else  //被除数-(整商×除数)之后在第一位小数位进行四舍五入
		{
			dResult = dA - (floor(dA / dB) * dB);
			return dResult;
		}
	}
	else
	{
		double dA2 = abs(dA);        //绝对值
		double dB2 = abs(dB);		 //绝对值
		dResult = abs(dB2 * (floor(dA2 / dB2) + 1) - dA2);
		if (dB < 0) dResult = -dResult;
		return dResult;
	}
}

void VariantOperator::MOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, const ARRAY_DOUBLE& b)
{
	long lACount = (long)a.size();
	long lBCount = (long)b.size();
	long lCount = std::max(lACount, lBCount);
	dest.resize(lCount, VARIANT_ITEM());

	for (long i = 0; i < lCount; ++i)
	{
		if (i >= lACount || i >= lBCount) continue;
		const auto& aItem = a[i];
		const auto& bItem = b[i];

		if (!aItem.IsVaild() || !bItem.IsVaild()) continue;

		dest[i].SetValue(MOD(aItem._dValue, bItem._dValue));
	}
}

void VariantOperator::MOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, double dB)
{
	long lCount = (long)a.size();
	dest.resize(lCount, VARIANT_ITEM());

	for (long i = 0; i < lCount; ++i)
	{
		const auto& aItem = a[i];

		if (!aItem.IsVaild()) continue;

		dest[i].SetValue(MOD(aItem._dValue, dB));
	}
}

/*
获利盘比例.
用法:
WINNER(CLOSE), 表示以当前收市价卖出的获利盘比例, 例如返回0.1表示10%获利盘; WINNER(10.5)表示10.5元价格的获利盘比例
该函数仅对日线分析周期有效
*/
Variant* VariantOperator::WINNER(const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	std::wstring strError;
	Variant* pHisCapital = GetHisCapital(pHistoryData, pNode, strError);
	if (!strError.empty())
	{
		ExecuteExcept error(strError,L"VariantOperator::WINNER", pNode);
		throw error;
	}

	Variant* pResult = Create();

	double dMinPrice = 0, dMaxPrice = 0;
	if (!pHistoryData->GetKMaxMin(dMaxPrice, dMinPrice))
	{
		ExecuteExcept error(L"WINNER() 获取历史K线最大最小值失败", L"VariantOperator::WINNER", pNode);
		throw error;
	}

	if (dMinPrice > 8000 || dMinPrice < 0 || dMaxPrice>8000 || dMinPrice < 0)
	{
		ExecuteExcept error(L"WINNER() 历史K线最大最小值错误, 超出(0,8000)范围", L"VariantOperator::WINNER", pNode);
		throw error;
	}

	long lMaxPrice = (long)(dMaxPrice * 100 + 1);
	long lMinPrice = (long)(dMinPrice * 100 - 1);
	long lLow = 0, lHigh = 0, lClose = 0;
	dMaxPrice = lMaxPrice / 100.0;
	dMinPrice = lMinPrice / 100.0;
	long lSpeed = lMaxPrice - lMinPrice + 1;
	if (lSpeed < 1) return pResult;

	std::vector<double> aryVolPrice(lSpeed, 0);
	std::vector<double> aryPerVol(lSpeed, 0);

	const ARRAY_DOUBLE& aryCapital = pHisCapital->m_aryValue;
	int nCapitalCount = (int)aryCapital.size();
	int nKDataCount = pHistoryData->GetKCount();

	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(nKDataCount, VARIANT_ITEM());

	bool bDouble = false;
	double dValue = 0;
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		bDouble = true;
		dValue = data.GetDoubleValue();
	}

	double dHSL = 0, dTotalVol = 0, dCost = 0, dVol = 0;
	for (long i = 0, j = 0, k = 0; i < nKDataCount; ++i)
	{
		if (i >= nCapitalCount) continue;

		const auto& capitalItem = aryCapital[i];
		if (!capitalItem.IsVaild() || capitalItem._dValue <= 1) continue;

		auto pKItem = pHistoryData->GetKItem(i);
		dHSL = pKItem->_dVol / capitalItem._dValue;

		for (j = 0; j < lSpeed; j++)
		{
			aryVolPrice[j] *= (1.0 - dHSL);
		}

		lLow = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dLow * 100))) - lMinPrice;
		lHigh = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dHigh * 100))) - lMinPrice;
		lClose = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dClose * 100))) - lMinPrice;

		aryPerVol.assign(lSpeed, 0);
		long  lHalf = (lLow + lHigh + 2 * lClose) / 4;

		if (lHalf == lHigh || lHalf == lLow)
		{
			aryPerVol[lHalf] += pKItem->_dVol;
		}
		else
		{
			double	dVH = pKItem->_dVol / (lHalf - lLow);
			for (k = lLow; k<lHalf; ++k)
			{
				aryPerVol[k] += (k - lLow)*(dVH / (lHalf - lLow));
			}
			for (k; k <= lHigh; ++k)
			{
				aryPerVol[k] += (k - lHigh)*(dVH / (lHalf - lHigh));
			}
		}

		dTotalVol = 0;
		for (j = lLow; j <= lHigh; j++)
		{
			aryVolPrice[j] += aryPerVol[j];
		}

		for (j = 0; j < lSpeed; j++)
		{
			dTotalVol += aryVolPrice[j];
		}

		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE)
		{
			if (bDouble)
			{
				lHigh = (long)std::min((long)(dValue * 100) - lMinPrice, lSpeed - 1);
			}
			else
			{
				const auto& item = data.m_aryValue[i];
				lHigh = (long)std::min((long)(item._dValue * 100) - lMinPrice, lSpeed - 1);
			}

			for (j = 0, dVol = 0; j <= lHigh; j++)
			{
				dVol += aryVolPrice[j];
			}

			if (dTotalVol > 0) dest[i].SetValue(dVol / dTotalVol);
			else if (i - 1 >= 0) dest[i] = dest[i - 1];
		}

	}

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	return pResult;

}

Variant* VariantOperator::WINNERB(const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	return WINNER(data, pHistoryData, pNode);
}


/*
成本分布情况.
用法: COST(10),表示10%获利盘的价格是多少,即有10%的持仓量在该价格以下,其余90%在该价格以上,为套牢盘
该函数仅对日线分析周期有效
*/
Variant* VariantOperator::COST(const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	std::wstring strError;
	Variant* pHisCapital = GetHisCapital(pHistoryData, pNode, strError);
	if (!strError.empty())
	{
		ExecuteExcept error(strError, L"VariantOperator::COST", pNode);
		throw error;
	}

	Variant* pResult = Create();

	if (data.GetType() != Variant::DOUBLE_TYPE || !data.IsVaildDoulbe())
	{
		ExecuteExcept error(L"COST()参数错误", L"VariantOperator::COST", pNode);
		throw error;
	}

	double dRate = data.GetDoubleValue() / 100;
	if (dRate<0.000001 || dRate>1) return pResult;

	double dMinPrice = 0, dMaxPrice = 0;
	if (!pHistoryData->GetKMaxMin(dMaxPrice, dMinPrice))
	{
		ExecuteExcept error(L"COST() 获取历史K线最大最小值失败", L"VariantOperator::COST", pNode);
		throw error;
	}

	const long MAX_PRICE = 8000;
	if (dMinPrice > MAX_PRICE || dMinPrice < 0 || dMaxPrice>MAX_PRICE || dMinPrice < 0)
	{
		ExecuteExcept error(L"COST() 历史K线最大最小值错误, 超出(0,8000)范围", L"VariantOperator::COST", pNode);
		throw error;
	}

	long lMaxPrice = (long)(dMaxPrice * 100 + 1);
	long lMinPrice = (long)(dMinPrice * 100 - 1);
	long lLow = 0, lHigh = 0, lClose = 0;
	dMaxPrice = lMaxPrice / 100.0;
	dMinPrice = lMinPrice / 100.0;
	long lSpeed = lMaxPrice - lMinPrice + 1;
	if (lSpeed < 1) return pResult;

	std::vector<double> aryVolPrice(lSpeed, 0);
	std::vector<double> aryPerVol(lSpeed, 0);

	const ARRAY_DOUBLE& aryCapital = pHisCapital->m_aryValue;
	int nCapitalCount = (int)aryCapital.size();
	int nKDataCount = pHistoryData->GetKCount();

	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(nKDataCount, VARIANT_ITEM());

	double dHSL = 0, dTotalVol = 0, dCost = 0, dVol = 0;
	for (long i = 0, j = 0, k = 0; i < nKDataCount; ++i)
	{
		if (i >= nCapitalCount) continue;

		const auto& capitalItem = aryCapital[i];
		if (!capitalItem.IsVaild() || capitalItem._dValue <= 1) continue;

		auto pKItem = pHistoryData->GetKItem(i);
		dHSL = pKItem->_dVol / capitalItem._dValue;

		for (j = 0; j < lSpeed; j++)
		{
			aryVolPrice[j] *= (1.0 - dHSL);
		}

		lLow = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dLow * 100))) - lMinPrice;
		lHigh = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dHigh * 100))) - lMinPrice;
		lClose = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dClose * 100))) - lMinPrice;

		aryPerVol.assign(lSpeed, 0);
		long  lHalf = (lLow + lHigh + 2 * lClose) / 4;

		if (lHalf == lHigh || lHalf == lLow)
		{
			aryPerVol[lHalf] += pKItem->_dVol;
		}
		else
		{
			double	dVH = pKItem->_dVol / (lHalf - lLow);
			for (k = lLow; k<lHalf; ++k)
			{
				aryPerVol[k] += (k - lLow)*(dVH / (lHalf - lLow));
			}
			for (k; k <= lHigh; ++k)
			{
				aryPerVol[k] += (k - lHigh)*(dVH / (lHalf - lHigh));
			}
		}

		dTotalVol = 0;
		for (j = lLow; j <= lHigh; j++)
		{
			aryVolPrice[j] += aryPerVol[j];
		}

		for (j = 0; j < lSpeed; j++)
		{
			dTotalVol += aryVolPrice[j];
		}

		for (j = 0, dCost = dVol = 0; j<lSpeed; ++j)
		{
			dVol += aryVolPrice[j];
			if (dVol >= dTotalVol*dRate)
			{
				dCost = (dMaxPrice - dMinPrice)*j / lSpeed + dMinPrice;
				break;
			}
		}

		dest[i].SetValue(dCost);
	}

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	return pResult;
}


/*
区间成本.
用法: 例如COSTEX(CLOSE,REF(CLOSE,1)),表示近两日收盘价格间筹码的成本
该函数仅对日线分析周期有效
*/
Variant* VariantOperator::COSTEX(const Variant& data, const Variant& data2, const IHistoryData* pHistoryData, Node* pNode)
{
	std::wstring strError;
	Variant* pHisCapital = GetHisCapital(pHistoryData, pNode, strError);
	if (!strError.empty())
	{
		ExecuteExcept error(strError, L"VariantOperator::COSTEX", pNode);
		throw error;
	}

	Variant* pResult = Create();

	double dMinPrice = 0, dMaxPrice = 0;
	if (!pHistoryData->GetKMaxMin(dMaxPrice, dMinPrice))
	{
		ExecuteExcept error(L"COSTEX() 获取历史K线最大最小值失败", L"VariantOperator::COSTEX", pNode);
		throw error;
	}

	if (dMinPrice > 8000 || dMinPrice < 0 || dMaxPrice>8000 || dMinPrice < 0)
	{
		ExecuteExcept error(L"COSTEX() 历史K线最大最小值错误, 超出(0,8000)范围", L"VariantOperator::COSTEX", pNode);
		throw error;
	}

	long lMaxPrice = (long)(dMaxPrice * 100 + 1);
	long lMinPrice = (long)(dMinPrice * 100 - 1);
	long lLow = 0, lHigh = 0, lClose = 0;
	dMaxPrice = lMaxPrice / 100.0;
	dMinPrice = lMinPrice / 100.0;
	long lSpeed = lMaxPrice - lMinPrice + 1;
	if (lSpeed < 1) return pResult;

	std::vector<double> aryVolPrice(lSpeed, 0);
	std::vector<double> aryPerVol(lSpeed, 0);

	const ARRAY_DOUBLE& aryCapital = pHisCapital->m_aryValue;
	int nCapitalCount = (int)aryCapital.size();
	int nKDataCount = pHistoryData->GetKCount();

	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(nKDataCount, VARIANT_ITEM());

	double dHSL = 0, dTotalVol = 0, dVola = 0, dPerVola = 0, dVolb = 0, dPerVolb = 0, dVol = 0, dPerVolRange = 0;
	for (long i = 0, j = 0, k = 0; i < nKDataCount; ++i)
	{
		if (i >= nCapitalCount) continue;

		const auto& capitalItem = aryCapital[i];
		if (!capitalItem.IsVaild() || capitalItem._dValue <= 1) continue;

		auto pKItem = pHistoryData->GetKItem(i);
		dHSL = pKItem->_dVol / capitalItem._dValue;

		for (j = 0; j < lSpeed; j++)
		{
			aryVolPrice[j] *= (1.0 - dHSL);
		}

		lLow = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dLow * 100))) - lMinPrice;
		lHigh = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dHigh * 100))) - lMinPrice;
		lClose = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dClose * 100))) - lMinPrice;

		aryPerVol.assign(lSpeed, 0);
		long  lHalf = (lLow + lHigh + 2 * lClose) / 4;

		if (lHalf == lHigh || lHalf == lLow)
		{
			aryPerVol[lHalf] += pKItem->_dVol;
		}
		else
		{
			double	dVH = pKItem->_dVol / (lHalf - lLow);
			for (k = lLow; k<lHalf; ++k)
			{
				aryPerVol[k] += (k - lLow)*(dVH / (lHalf - lLow));
			}
			for (k; k <= lHigh; ++k)
			{
				aryPerVol[k] += (k - lHigh)*(dVH / (lHalf - lHigh));
			}
		}

		dTotalVol = 0;
		for (j = lLow; j <= lHigh; j++)
		{
			aryVolPrice[j] += aryPerVol[j];
		}

		for (j = 0; j < lSpeed; j++)
		{
			dTotalVol += aryVolPrice[j];
		}

		if ((data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE) &&
			(data2.GetType() == Variant::ARRAY_DOUBLE_TYPE || data2.GetType() == Variant::DOUBLE_TYPE))
		{
			if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
			{
				const auto& aryStart = data.m_aryValue;
				const auto& startItem = aryStart[i];
				lHigh = (long)std::min((long)(startItem._dValue * 100) - lMinPrice, lSpeed - 1);
			}
			else if (data.GetType() == Variant::DOUBLE_TYPE)
			{
				lHigh = (long)std::min((long)(data.GetDoubleValue() * 100) - lMinPrice, lSpeed - 1);
			}

			for (j = 0, dVola = 0, dPerVola = 0; j <= lHigh; j++)
			{
				dVola += aryVolPrice[j];
				dPerVola += (0.01*(j + lMinPrice))*aryVolPrice[j];
			}

			if (data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
			{
				const auto& aryEnd = data2.m_aryValue;
				const auto& endItem = aryEnd[i];
				lHigh = (long)std::min((long)(endItem._dValue * 100) - lMinPrice, lSpeed - 1);
			}
			else if (data2.GetType() == Variant::DOUBLE_TYPE)
			{
				lHigh = (long)std::min((long)(data2.GetDoubleValue() * 100) - lMinPrice, lSpeed - 1);
			}

			for (j = 0, dVolb = 0.0f, dPerVolb = 0.0f; j <= lHigh; j++)
			{
				dVolb += aryVolPrice[j];
				dPerVolb += (0.01f*(j + lMinPrice))*aryVolPrice[j];
			}

			dVol = dVola - dVolb;
			dPerVolRange = dPerVola - dPerVolb;
			if (abs(dPerVolRange) > 0.001 && dVol != 0) dest[i].SetValue(dPerVolRange / dVol);
			else if (i - 1 >= 0) dest[i] = dest[i - 1];
		}

	}

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	return pResult;
}




/*
近期获利盘比例.
用法: LWINNER(5,CLOSE),表示最近5天的那部分成本以当前收市价卖出的获利盘比例
例如:返回0.1表示10%获利盘
*/
Variant* VariantOperator::LWINNER(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	if (!n.IsVaildDoulbe())
	{
		ExecuteExcept error(L"LWINNER() 第1个参数错误", L"VariantOperator::LWINNER", pNode);
		throw error;
	}

	bool bDouble = false;
	double dValue = 0;
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		bDouble = true;
		dValue = data.GetDoubleValue();
	}

	std::wstring strError;
	Variant* pHisCapital = GetHisCapital(pHistoryData, pNode, strError);
	if (!strError.empty())
	{
		ExecuteExcept error(strError, L"VariantOperator::LWINNER", pNode);
		throw error;
	}

	Variant* pResult = Create();

	const ARRAY_DOUBLE& aryCapital = pHisCapital->m_aryValue;
	int nCapitalCount = (int)aryCapital.size();
	int nKDataCount = pHistoryData->GetKCount();
	long lStartDay = (long)n.GetDoubleValue();
	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(nKDataCount, VARIANT_ITEM());

	long lIndex = 0, lMaxPrice = 0, lMinPrice = 0, lLow = 0, lHigh = 0, lClose = 0;
	double dMinPrice = 0, dMaxPrice = 0, dHSL = 0, dTotalVol = 0, dVol = 0;
	for (long i = lStartDay - 1, j = 0, k = 0; i < nKDataCount; ++i)
	{
		lIndex = i - lStartDay + 1;

		dMinPrice = dMaxPrice = 0;
		if (!pHistoryData->GetKMaxMin(lIndex, lIndex + lStartDay, dMaxPrice, dMinPrice))
		{
			ExecuteExcept error(L"LWINNER() 获取历史K线最大最小值失败", L"VariantOperator::LWINNER", pNode);
			throw error;
		}

		if (dMinPrice > 8000 || dMinPrice < 0 || dMaxPrice>8000 || dMinPrice < 0)
		{
			ExecuteExcept error(L"LWINNER() 历史K线最大最小值错误, 超出(0,8000)范围", L"VariantOperator::LWINNER", pNode);
			throw error;
		}

		lMaxPrice = (long)(dMaxPrice * 100 + 1);
		lMinPrice = (long)(dMinPrice * 100 - 1);
		dMaxPrice = lMaxPrice / 100.0;
		dMinPrice = lMinPrice / 100.0;
		long lSpeed = lMaxPrice - lMinPrice + 1;
		if (lSpeed < 1) return pResult;

		std::vector<double> aryVolPrice(lSpeed, 0);
		std::vector<double> aryPerVol(lSpeed, 0);

		for (j = 0; j<lStartDay; ++j)
		{
			const auto& capitalItem = aryCapital[lIndex + j];
			if (!capitalItem.IsVaild() || capitalItem._dValue<1) continue;
			const auto pKItem = pHistoryData->GetKItem(lIndex + j);

			dHSL = pKItem->_dVol / capitalItem._dValue;
			for (k = 0; k < lSpeed; k++)
			{
				aryVolPrice[k] *= (1 - dHSL);
			}

			lLow = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dLow * 100))) - lMinPrice;
			lHigh = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dHigh * 100))) - lMinPrice;
			lClose = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dClose * 100))) - lMinPrice;

			aryPerVol.assign(lSpeed, 0);

			long  lHalf = (lLow + lHigh + 2 * lClose) / 4;
			if (lHalf == lHigh || lHalf == lLow)
			{
				aryPerVol[lHalf] += pKItem->_dVol;
			}
			else
			{
				double	dVH = pKItem->_dVol / (lHalf - lLow);
				for (k = lLow; k<lHalf; ++k)
				{
					aryPerVol[k] += (k - lLow)*(dVH / (lHalf - lLow));
				}
				for (k; k <= lHigh; ++k)
				{
					aryPerVol[k] += (k - lHigh)*(dVH / (lHalf - lHigh));
				}
			}

			dTotalVol = 0;
			for (k = lLow; k <= lHigh; k++)
			{
				aryVolPrice[k] += aryPerVol[k];
			}
		}

		for (k = 0; k < lSpeed; k++)
		{
			dTotalVol += aryVolPrice[k];
		}

		dVol = 0;
		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE)
		{
			if (bDouble)
			{
				lHigh = (long)std::min((long)(dValue * 100) - lMinPrice, lSpeed - 1);
			}
			else
			{
				const auto& item = data.m_aryValue[i];
				lHigh = (long)std::min((long)(item._dValue * 100) - lMinPrice, lSpeed - 1);
			}
			for (k = 0; k <= lHigh; k++)
			{
				dVol += aryVolPrice[k];
			}
		}

		if (dTotalVol > 0 && dVol > 0 && dVol <= dTotalVol) dest[i].SetValue(dVol / dTotalVol);
		else if (i - 1 >= 0) dest[i] = dest[i - 1];
	}

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	return pResult;
}

Variant* VariantOperator::LWINNERB(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	return LWINNER(n, data, pHistoryData, pNode);
}

/*
远期获利盘比例.
用法: PWINNER(5,CLOSE),表示5天前的那部分成本以当前收市价卖出的获利盘比例
例如:
返回0.1表示10%获利盘
*/
Variant* VariantOperator::PWINNER(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	if (!n.IsVaildDoulbe())
	{
		ExecuteExcept error(L"PWINNER() 第1个参数错误", L"VariantOperator::PWINNER", pNode);
		throw error;
	}

	bool bDouble = false;
	double dValue = 0;
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		bDouble = true;
		dValue = data.GetDoubleValue();
	}

	std::wstring strError;
	Variant* pHisCapital = GetHisCapital(pHistoryData, pNode, strError);
	if (!strError.empty())
	{
		ExecuteExcept error(strError, L"VariantOperator::PWINNER", pNode);
		throw error;
	}

	Variant* pResult = Create();

	const ARRAY_DOUBLE& aryCapital = pHisCapital->m_aryValue;
	int nCapitalCount = (int)aryCapital.size();
	int nKDataCount = pHistoryData->GetKCount();
	long lStartDay = (long)n.GetDoubleValue();
	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(nKDataCount, VARIANT_ITEM());

	long lIndex = 0, lMaxPrice = 0, lMinPrice = 0, lLow = 0, lHigh = 0, lClose = 0;
	double dMinPrice = 0, dMaxPrice = 0, dHSL = 0, dTotalVol = 0, dVol = 0;
	std::map<long, double> aryVolPrice;
	long lSubIndex = 0, lPrice = 0;
	for (long i = lStartDay - 1, j = 0, k = 0; i < nKDataCount; ++i)
	{
		dMinPrice = dMaxPrice = 0;
		if (!pHistoryData->GetKMaxMin(0, i - lStartDay + 2, dMaxPrice, dMinPrice))
		{
			ExecuteExcept error(L"PWINNER() 获取历史K线最大最小值失败", L"VariantOperator::PWINNER", pNode);
			throw error;
		}

		if (dMinPrice > 8000 || dMinPrice < 0 || dMaxPrice>8000 || dMinPrice < 0)
		{
			ExecuteExcept error(L"PWINNER() 历史K线最大最小值错误, 超出(0,1000)范围", L"VariantOperator::PWINNER", pNode);
			throw error;
		}

		lMaxPrice = (long)(dMaxPrice * 100 + 1);
		lMinPrice = (long)(dMinPrice * 100 - 1);
		//去掉小数
		dMaxPrice = lMaxPrice / 100.0;
		dMinPrice = lMinPrice / 100.0;
		long lSpeed = lMaxPrice - lMinPrice + 1;
		if (lSpeed < 1) return pResult;

		std::vector<double> aryPerVol(lSpeed, 0);
		for (j = lSubIndex; j<i - lStartDay + 2; ++j, ++lSubIndex)
		{
			const auto& capitalItem = aryCapital[j];
			if (!capitalItem.IsVaild() || capitalItem._dValue<1) continue;
			const auto pKItem = pHistoryData->GetKItem(j);

			dHSL = pKItem->_dVol / capitalItem._dValue;
			for (k = 0; k < lSpeed; k++)
			{
				lPrice = k + lMinPrice;
				auto find = aryVolPrice.find(lPrice);
				if (find != aryVolPrice.end())
					find->second *= (1 - dHSL);
			}

			lLow = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dLow * 100))) - lMinPrice;
			lHigh = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dHigh * 100))) - lMinPrice;
			lClose = (long)std::min(lMaxPrice, std::max(lMinPrice, (long)(pKItem->_dClose * 100))) - lMinPrice;

			aryPerVol.assign(lSpeed, 0);

			long  lHalf = (lLow + lHigh + 2 * lClose) / 4;
			if (lHalf == lHigh || lHalf == lLow)
			{
				aryPerVol[lHalf] += pKItem->_dVol;
			}
			else
			{
				double	dVH = pKItem->_dVol / (lHalf - lLow);
				for (k = lLow; k<lHalf; ++k)
				{
					aryPerVol[k] += (k - lLow)*(dVH / (lHalf - lLow));
				}
				for (k; k <= lHigh; ++k)
				{
					aryPerVol[k] += (k - lHigh)*(dVH / (lHalf - lHigh));
				}
			}

			dTotalVol = 0;
			for (k = lLow; k <= lHigh; k++)
			{
				lPrice = k + lMinPrice;
				auto find = aryVolPrice.find(lPrice);
				if (find == aryVolPrice.end())
					aryVolPrice[lPrice] = aryPerVol[k];
				else
					find->second += aryPerVol[k];
			}
		}

		for (k = 0; k < lSpeed; k++)
		{
			lPrice = k + lMinPrice;
			dTotalVol += aryVolPrice[lPrice];
		}

		dVol = 0;
		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE)
		{
			if (bDouble)
			{
				lHigh = (long)std::min((long)(dValue * 100) - lMinPrice, lSpeed - 1);
			}
			else
			{
				const auto& item = data.m_aryValue[i];
				lHigh = (long)std::min((long)(item._dValue * 100) - lMinPrice, lSpeed - 1);
			}
			for (k = 0; k <= lHigh; k++)
			{
				lPrice = k + lMinPrice;
				dVol += aryVolPrice[lPrice];
			}
		}

		if (dTotalVol > 0 && dVol > 0 && dVol <= dTotalVol) dest[i].SetValue(dVol / dTotalVol);
		else if (i - 1 >= 0) dest[i] = dest[i - 1];
	}

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	return pResult;
}

Variant* VariantOperator::PWINNERB(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode)
{
	return PWINNER(n, data, pHistoryData, pNode);
}

/*
远期成本分布比例.
用法:PPART(10),表示10前的成本占总成本的比例,0.2表示20%
*/
Variant* VariantOperator::PPART(const Variant& n, const IHistoryData* pHistoryData, Node* pNode)
{
	std::wstring strError;
	Variant* pHisCapital = GetHisCapital(pHistoryData, pNode, strError);
	if (!strError.empty())
	{
		ExecuteExcept error(strError, L"VariantOperator::PPART", pNode);
		throw error;
	}

	Variant* pResult = Create();

	if (n.GetType() != Variant::DOUBLE_TYPE) return pResult;
	if (!n.IsVaildDoulbe()) return pResult;
	long lStartDay = (long)n.GetDoubleValue();

	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	const ARRAY_DOUBLE& aryCapital = pHisCapital->m_aryValue;
	int nKDataCount = pHistoryData->GetKCount();
	int nCapitalCount = (int)aryCapital.size();
	dest.resize(nKDataCount, VARIANT_ITEM());

	double dPartVol = 0, dValue = 0;
	long lStart = 0;
	for (long i = lStartDay, j = 0; i < nKDataCount; ++i)
	{
		lStart = i - lStartDay;
		if (lStart < 0) continue;

		dPartVol = 0;
		for (j = 0; j < lStartDay; ++j)	//前n日成交量和
		{
			auto pKItem = pHistoryData->GetKItem(j + lStart);
			dPartVol += pKItem->_dVol;
		}

		if (i < nCapitalCount)
		{
			const auto& capitalItem = aryCapital[i];
			if (capitalItem.IsVaild() && capitalItem._dValue>0)
			{
				dValue = 1 - (dPartVol / capitalItem._dValue);
				dest[i].SetValue(dValue);
			}
		}
	}

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;

	return pResult;
}

Variant* VariantOperator::GetHisCapital(const IHistoryData* pHistoryData, Node* pNode, std::wstring& strError)
{
	if (!pHistoryData)
	{
		strError = L"[VariantOperator::GetHisCapital] Error: pHistoryData is null";
		return NULL;
	}

	Variant* pHisCapital = pHistoryData->GetHisCapital(pNode);
	if (!pHisCapital)
	{
		strError = L"[VariantOperator::GetHisCapital] Error:历史流通股本读取失败";
		return NULL;
	}

	return pHisCapital;
}

bool VariantOperator::CalculateLChip(ChipData& chipData, int nIndex, const ARRAY_DOUBLE& exchangeData, const IHistoryData* pHistory, double dRate, long ln)
{
	chipData.Clear();
	if (nIndex < (ln - 1)) return false;

	double dSeed = 1;	//筹码历史衰减换手系数
	double dMax = 0, dMin = 999999999;
	for (int i = nIndex, j = 0; i >= 0 && j<ln; --i, ++j)
	{
		auto pKItem = pHistory->GetKItem(i);
		if (!pKItem) continue;
		CHIP_ITEM item = {};
		if (i == nIndex) item._dVol = pKItem->_dVol*exchangeData[i]._dValue;
		else item._dVol = pKItem->_dVol*dSeed;

		item._nDate = pKItem->_nDate;
		item._nTime = pKItem->_nTime;
		item._dHigh = pKItem->_dHigh;
		item._dLow = pKItem->_dLow;

		if (dMax < item._dHigh) dMax = item._dHigh;
		if (dMin > item._dLow) dMin = item._dLow;

		chipData.Add(item);

		dSeed *= (1 - (exchangeData[i]._dValue / 100 * dRate));		//换手率累乘
	}

	chipData.SetMax(dMax);
	chipData.SetMin(dMin);

	return true;
}

bool VariantOperator::CalculatePChip(ChipData& chipData, int nIndex, const ARRAY_DOUBLE& exchangeData, const IHistoryData* pHistory, double dRate, long ln)
{
	chipData.Clear();
	if (nIndex - ln < 0) return false;

	double dSeed = 1;	//筹码历史衰减换手系数
	double dMax = 0, dMin = 999999999;
	for (int i = nIndex, j = 1; i >= 0; --i, ++j)
	{
		auto pKItem = pHistory->GetKItem(i);
		if (!pKItem) continue;
		CHIP_ITEM item = {};
		if (i == nIndex) item._dVol = pKItem->_dVol*exchangeData[i]._dValue;
		else item._dVol = pKItem->_dVol*dSeed;

		item._nDate = pKItem->_nDate;
		item._nTime = pKItem->_nTime;
		item._dHigh = pKItem->_dHigh;
		item._dLow = pKItem->_dLow;

		if (dMax < item._dHigh) dMax = item._dHigh;
		if (dMin > item._dLow) dMin = item._dLow;

		if (j>ln) chipData.Add(item);

		dSeed *= (1 - (exchangeData[i]._dValue / 100 * dRate));		//换手率累乘
	}

	chipData.SetMax(dMax);
	chipData.SetMin(dMin);

	return true;
}

bool VariantOperator::CalculateChip(ChipData& chipData, int nIndex, const ARRAY_DOUBLE& exchangeData, const IHistoryData* pHistory, double dRate)
{
	chipData.Clear();
	double dSeed = 1;	//筹码历史衰减换手系数
	double dMax = 0, dMin = 999999999;
	for (int i = nIndex; i >= 0; --i)
	{
		auto pKItem = pHistory->GetKItem(i);
		if (!pKItem) continue;
		CHIP_ITEM item = {};
		if (i == nIndex) item._dVol = pKItem->_dVol*exchangeData[i]._dValue;
		else item._dVol = pKItem->_dVol*dSeed;

		item._nDate = pKItem->_nDate;
		item._nTime = pKItem->_nTime;
		item._dHigh = pKItem->_dHigh;
		item._dLow = pKItem->_dLow;

		if (dMax < item._dHigh) dMax = item._dHigh;
		if (dMin > item._dLow) dMin = item._dLow;

		chipData.Add(item);

		dSeed *= (1 - (exchangeData[i]._dValue / 100 * dRate));		//换手率累乘
	}

	chipData.SetMax(dMax);
	chipData.SetMin(dMin);

	return true;
}


/*
属于未来函数, 前M个ZIG转向波峰值.
用法: PEAK(K, N, M)表示之字转向ZIG(K, N)的前M个波峰的数值, M必须大于等于1
例如 :PEAK(1, 5, 1)表示 % 5最高价ZIG转向的上一个波峰的数值
*/
Variant* VariantOperator::PEAK(const Variant& k, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (m.GetType() != Variant::DOUBLE_TYPE || !m.IsVaildDoulbe()) return pResult;
	Variant* pZig = ZIG(k, n, pHistory, pNode);
	if (!pZig || pZig->GetType() != Variant::ARRAY_DOUBLE_TYPE) return pResult;

	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	const ARRAY_DOUBLE& data = pZig->m_aryValue;
	int nDataCount = (long)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	long lEnd = (long)m.GetDoubleValue();
	if (lEnd < 1) return pResult;
	long lFlag = 0;
	long *peak = new long[lEnd];
	memset(peak, 0, sizeof(long)*lEnd);
	long i = GetFirstVaildIndex(data) + 1;
	for (lEnd--; i<nDataCount && data[i]._dValue<data[i - 1]._dValue; ++i);

	for (; i<nDataCount && data[i]._dValue>data[i - 1]._dValue; ++i);

	for (peak[0] = --i; i<nDataCount - 1; ++i)
	{
		if (data[i]._dValue>data[i + 1]._dValue)
		{
			if (lFlag)
			{
				if (lEnd)
					memmove(peak + 1, peak, lEnd*sizeof(long));
				peak[lFlag = 0] = i;
			}
		}
		else lFlag = 1;
		if (peak[lEnd]) dest[i].SetValue(data[peak[lEnd]]._dValue);
	}
	if (peak[lEnd])dest[i].SetValue(data[peak[lEnd]]._dValue);
	delete[] peak;
	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
	return pResult;
}

/*
属于未来函数,前M个ZIG转向波峰到当前距离.
用法:PEAKBARS(K,N,M)表示之字转向ZIG(K,N)的前M个波峰到当前的周期数,M必须大于等于1
例如:PEAKBARS(0,5,1)表示%5开盘价ZIG转向的上一个波峰到当前的周期数
*/
Variant* VariantOperator::PEAKBARS(const Variant& k, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (m.GetType() != Variant::DOUBLE_TYPE || !m.IsVaildDoulbe()) return pResult;
	Variant* pZig = ZIG(k, n, pHistory, pNode);
	if (!pZig || pZig->GetType() != Variant::ARRAY_DOUBLE_TYPE) return pResult;

	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	const ARRAY_DOUBLE& data = pZig->m_aryValue;
	int nDataCount = (long)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	long lEnd = (long)m.GetDoubleValue();
	if (lEnd < 1) return pResult;

	long *peak = new long[lEnd];
	memset(peak, 0, sizeof(long)*lEnd);
	long lFlag = 0;

	long i = GetFirstVaildIndex(data) + 1;
	for (lEnd--; i<nDataCount && data[i]._dValue<data[i - 1]._dValue; ++i);

	for (; i<nDataCount && data[i]._dValue>data[i - 1]._dValue; ++i);

	for (peak[0] = --i; i<nDataCount - 1; ++i)
	{
		if (data[i]._dValue>data[i + 1]._dValue)
		{
			if (lFlag)
			{
				if (lEnd) memmove(peak + 1, peak, lEnd*sizeof(long));
				peak[lFlag = 0] = i;
			}
		}
		else lFlag = 1;
		if (peak[lEnd]) dest[i].SetValue(i - peak[lEnd]);
	}
	if (peak[lEnd])dest[i].SetValue(i - peak[lEnd]);
	delete[] peak;

	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;

	return pResult;
}


/*
属于未来函数,之字转向.
用法: ZIG(K,N),当价格变化量超过N%时转向,K表示0:开盘价,1:最高价,2:最低价,3:收盘价,其余:数组信息
例如: ZIG(3,5)表示收盘价的5%的ZIG转向
*/
Variant* VariantOperator::ZIG(const Variant& k, const Variant& n, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (k.GetType() == Variant::DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (k.IsVaildDoulbe() && n.IsVaildDoulbe() && n.GetDoubleValue()>0)
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			ZIG(pResult->m_aryValue, (long)k.GetDoubleValue(), n.GetDoubleValue(), pHistory);
		}
	}
	else if (k.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe() && n.GetDoubleValue()>0)
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			ZIG(pResult->m_aryValue, k.m_aryValue, n.GetDoubleValue());
		}
	}

	return pResult;
}

void VariantOperator::ZIG(ARRAY_DOUBLE& dest, const long k, double n, const IHistoryData* pHistory)
{
	Variant* pData = NULL;
	switch (k)
	{
	case 0:
		pData = pHistory->GetOpen();
		break;
	case 1:
		pData = pHistory->GetHigh();
		break;
	case 2:
		pData = pHistory->GetLow();
		break;
	case 3:
		pData = pHistory->GetClose();
		break;
	default:
		pData = pHistory->GetClose();
		break;
	}

	ZIG(dest, pData->m_aryValue, n);
}


void VariantOperator::ZIG(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dRate)
{
	long nDataCount = (long)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	long m = GetFirstVaildIndex(data);
	long i = 0, lLastPos = 0, lState = 0, j = 0;
	double dif = 0;;
	for (i = m + 1, lLastPos = lState = m; i<nDataCount - 1 && lState == m; ++i)
	{
		lState = fabs(data[i]._dValue - data[m]._dValue) * 100 >= dRate*data[m]._dValue ? (data[i]._dValue>data[m]._dValue ? i : -i) : m;
	}

	for (; i<nDataCount - 1; ++i)
	{
		if (data[i]._dValue >= data[i - 1]._dValue && data[i]._dValue >= data[i + 1]._dValue)
		{
			if (lState<0)
			{
				if ((data[i]._dValue - data[-lState]._dValue) * 100<dRate*data[-lState]._dValue) continue;
				else
				{
					dif = (data[lLastPos]._dValue - data[j = -lState]._dValue) / (-lState - lLastPos);
					dest[j--].SetValue(data[-lState]._dValue);
					for (; j >= lLastPos; j--)
						dest[j].SetValue(data[-lState]._dValue + (-lState - j)*dif);
					lLastPos = -lState;
					lState = i;
				}
			}
			else if (data[i]._dValue>data[lState]._dValue)  lState = i;
		}
		else if (data[i]._dValue <= data[i - 1]._dValue && data[i]._dValue <= data[i + 1]._dValue)
		{
			if (lState>0)
			{
				if ((data[lState]._dValue - data[i]._dValue) * 100<dRate*data[lState]._dValue) continue;
				else
				{
					dif = (data[lState]._dValue - data[j = lLastPos]._dValue) / (lState - lLastPos);
					dest[j++].SetValue(data[lLastPos]._dValue);
					for (; j <= lState; ++j)
						dest[j].SetValue(data[lLastPos]._dValue + (j - lLastPos)*dif);
					lLastPos = lState;
					lState = -i;
				}
			}
			else if (data[i]._dValue<data[-lState]._dValue) lState = -i;
		}
	}

	if (abs(lState) >= nDataCount - 2)
	{
		if (lState>0 && data[nDataCount - 1]._dValue >= data[lState]._dValue) lState = nDataCount - 1;
		if (lState<0 && data[nDataCount - 1]._dValue <= data[-lState]._dValue) lState = 1 - nDataCount;
	}

	if (lState>0)
	{
		dif = (data[lState]._dValue - data[j = lLastPos]._dValue) / (lState - lLastPos);
		dest[j++].SetValue(data[lLastPos]._dValue);
		for (; j <= lState; ++j)
			dest[j].SetValue(data[lLastPos]._dValue + (j - lLastPos)*dif);
	}
	else
	{
		dif = (data[lLastPos]._dValue - data[j = -lState]._dValue) / (-lState - lLastPos);
		dest[j--].SetValue(data[-lState]._dValue);
		for (; j >= lLastPos; j--)
			dest[j].SetValue(data[-lState]._dValue + (-lState - j)*dif);
	}
	if ((lState = abs(lState))<nDataCount - 1)
	{
		if (data[nDataCount - 1]._dValue >= data[lState]._dValue)
		{
			dif = (data[nDataCount - 1]._dValue - data[j = lState]._dValue) / (nDataCount - lState);
			dest[j++].SetValue(data[lState]._dValue);
			for (; j<nDataCount; ++j)
				dest[j].SetValue(data[lState]._dValue + (j - lState)*dif);
		}
		else
		{
			dif = (data[lState]._dValue - data[j = nDataCount - 1]._dValue) / (nDataCount - lState);
			dest[j--].SetValue(data[nDataCount - 1]._dValue);
			for (; j >= lState; j--)
				dest[j].SetValue(data[nDataCount - 1]._dValue + (nDataCount - j)*dif);
		}
	}
}

/*
void VariantOperator::ZIG(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double n)
{
long nDataCount = (long)data.size();
dest.resize(nDataCount, VARIANT_ITEM());

bool bFirstPoint = false, bSecondPont = false;
ZIG_ITEM firstData = {}, secodeData = {}, thridData = {}, lastData = {};

for (long i = 0; i < nDataCount; ++i)
{
const auto& item = data[i];
if (!item.IsVaild())
continue;

if (bFirstPoint==false)	//第1个点
{
bFirstPoint = true;
firstData._nID = i;
firstData._Value = item;
}
else if (bFirstPoint && bSecondPont == false)
{
double dRate = (item._dValue - firstData._Value._dValue) / firstData._Value._dValue * 100;
if (dRate > n)
{
secodeData._nID = i;
secodeData._Value = item;
secodeData._bUp = true;
lastData._nID = i;
lastData._Value = item;
bSecondPont = true;
}
else if (dRate < -n)
{
secodeData._nID = i;
secodeData._Value = item;
secodeData._bUp = false;
lastData._nID = i;
lastData._Value = item;
bSecondPont = true;
}
}
else if (bFirstPoint && bSecondPont)
{
double dRate = (item._dValue - lastData._Value._dValue) / lastData._Value._dValue * 100;
if (dRate < 0)
int nnnnn = 10;
if (secodeData._bUp) //找下跌的点
{
if (dRate < 0)
{
thridData._nID = i;
thridData._Value = item;
thridData._bUp = false;
CalculateZIGLine(dest, data, firstData, secodeData, thridData);
lastData._nID = i;
lastData._Value = item;
}
else
{
if (item._dValue>lastData._Value._dValue)
{
lastData._nID = i;
lastData._Value = item;
}
}
}
else
{
if (dRate > n)
{
thridData._nID = i;
thridData._Value = item;
thridData._bUp = true;
CalculateZIGLine(dest, data, firstData, secodeData, thridData);
lastData._nID = i;
lastData._Value = item;
}
else
{
if (item._dValue < lastData._Value._dValue)
{
lastData._nID = i;
lastData._Value = item;
}
}
}
}
}

thridData._nID = nDataCount-1;
thridData._Value = data[nDataCount - 1];
thridData._bUp = !secodeData._bUp;

CalculateZIGLine(dest, data, firstData, secodeData, thridData);
}
*/


Variant* VariantOperator::EVERY(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			EVERY(pResult->m_aryValue, data.m_aryValue, (long)n.GetDoubleValue());
		}
	}

	return pResult;
}


void VariantOperator::EVERY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	int nDataCount = (int)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	if (n < 1) return;

	int nStart = (int)GetFirstVaildIndex(data);
	int nFlag = 0;
	for (int i = nStart; i < nDataCount; ++i)
	{
		if (data[i].IsVaild() && data[i]._dValue>0) nFlag += 1;
		else nFlag = 0;

		if (nFlag == n)
		{
			dest[i].SetValue(1);
			--nFlag;
		}
		else
		{
			dest[i].SetValue(0);
		}
	}


}


/*
返回是否持续存在X>Y
用法: NDAY(CLOSE,OPEN,3)
表示连续3日收阳线
*/

Variant* VariantOperator::NDAY(const Variant& data, const Variant& data2, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;

	long ln = (long)n.GetDoubleValue();
	if (ln < 1) return pResult;

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		NDAY(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE && data2.IsVaildDoulbe())
	{
		NDAY(pResult->m_aryValue, data.m_aryValue, data2.GetDoubleValue(), ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE && data.IsVaildDoulbe() && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		NDAY(pResult->m_aryValue, data.GetDoubleValue(), data2.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::NDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, long n)
{
	long lDataCount = (long)data.size();
	long lData2Count = (long)data2.size();
	long lCount = std::max(lDataCount, lData2Count);
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	long lFlag = 0;
	for (long i = 0; i < lCount; ++i)
	{
		if (i >= lDataCount || i >= lData2Count)
		{
			lFlag = 0;
			continue;
		}

		const auto& dataItem = data[i];
		const auto& data2Item = data2[i];
		if (!dataItem.IsVaild() || !data2Item.IsVaild())
		{
			lFlag = 0;
			continue;
		}

		if (dataItem._dValue > data2Item._dValue) lFlag += 1;
		else lFlag = 0;

		if (lFlag == n)
		{
			dest[i].SetValue(1);
			--lFlag;
		}
	}
}

void VariantOperator::NDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dData2, long n)
{
	long lCount = (long)data.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	long lFlag = 0;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& dataItem = data[i];
		if (!dataItem.IsVaild())
		{
			lFlag = 0;
			continue;
		}

		if (dataItem._dValue > dData2) lFlag += 1;
		else lFlag = 0;

		if (lFlag == n)
		{
			dest[i].SetValue(1);
			--lFlag;
		}
	}
}

void VariantOperator::NDAY(ARRAY_DOUBLE& dest, double dData, const ARRAY_DOUBLE& data2, long n)
{
	long lCount = (long)data2.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	long lFlag = 0;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& dataItem = data2[i];
		if (!dataItem.IsVaild())
		{
			lFlag = 0;
			continue;
		}

		if (dData> dataItem._dValue) lFlag += 1;
		else lFlag = 0;

		if (lFlag == n)
		{
			dest[i].SetValue(1);
			--lFlag;
		}
	}
}


/*
返回是否连涨周期数.
用法: UPNDAY(CLOSE, M)
表示连涨M个周期, M为常量
*/
Variant* VariantOperator::UPNDAY(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();
	if (ln < 1) return pResult;

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		UPNDAY(pResult->m_aryValue, data.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::UPNDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lCount = (long)data.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	long lFlag = 0;
	for (long i = 1; i < lCount; ++i)
	{
		const auto& dataItem = data[i];
		const auto& data2Item = data[i - 1];

		if (!dataItem.IsVaild() || !data2Item.IsVaild())
		{
			lFlag = 0;
			continue;
		}

		if (dataItem._dValue > data2Item._dValue) lFlag += 1;
		else lFlag = 0;

		if (lFlag == n)
		{
			dest[i].SetValue(1);
			--lFlag;
		}
	}
}

/*
返回是否连跌周期.
用法: DOWNNDAY(CLOSE,M)
表示连跌M个周期,M为常量
*/

Variant* VariantOperator::DOWNNDAY(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();
	if (ln < 1) return pResult;

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		DOWNNDAY(pResult->m_aryValue, data.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::DOWNNDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lCount = (long)data.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	long lFlag = 0;
	for (long i = 1; i < lCount; ++i)
	{
		const auto& dataItem = data[i];
		const auto& data2Item = data[i - 1];

		if (!dataItem.IsVaild() || !data2Item.IsVaild())
		{
			lFlag = 0;
			continue;
		}

		if (dataItem._dValue < data2Item._dValue) lFlag += 1;
		else lFlag = 0;

		if (lFlag == n)
		{
			dest[i].SetValue(1);
			--lFlag;
		}
	}
}


/*
是否存在.
例如:EXIST(CLOSE>OPEN,10)
表示10日内存在着阳线
*/

Variant* VariantOperator::EXIST(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!n.IsVaildDoulbe()) return pResult;
		long ln = (long)n.GetDoubleValue();
		if (ln < 1) return pResult;

		EXIST(pResult->m_aryValue, data.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		EXIST(pResult->m_aryValue, data.m_aryValue, n.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}


	return pResult;
}

void VariantOperator::EXIST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lCount = (long)data.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	long lFlag = -1;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& dataItem = data[i];
		if (!dataItem.IsVaild()) continue;
		if (dataItem._dValue) lFlag = i;
		if (i == -1) continue;
		if (lFlag > i - n) dest[i].SetValue(1);
	}
}

void VariantOperator::EXIST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	long lCount = (long)data.size();
	VARIANT_ITEM zeroItem;
	zeroItem.SetValue(0);
	dest.resize(lCount, zeroItem);
	long lData2Count = (long)data2.size();
	long lPeriod = 0, lIndex = 0;
	double dValue = 0;
	bool bFind = false;
	for (long i = 0, j = 0; i < lCount && i < lData2Count; ++i)
	{
		const auto& periodItem = data2[i];
		if (!periodItem.IsVaild()) continue;
		lPeriod = (long)periodItem._dValue;
		if (lPeriod <= 0) continue;
		if (lPeriod > i + 1) lPeriod = i + 1;

		bFind = false;
		for (j = 0; j < lPeriod; ++j)
		{
			lIndex = i - (lPeriod - j - 1);
			const auto& item = data[lIndex];
			if (!item.IsVaild()) continue;
			if (item._dValue > 0)
			{
				bFind = true;
				break;
			}
		}

		dest[i].SetValue(bFind ? 1 : 0);
	}
}

/*
LAST(X,A,B):持续存在.
例如: LAST(CLOSE>OPEN,10,5)
表示从前10日到前5日内一直阳线
若A为0,表示从第一天开始,B为0,表示到最后日止
*/

Variant* VariantOperator::LAST(const Variant& data, const Variant& n, const Variant& n2)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	if (n2.GetType() != Variant::DOUBLE_TYPE || !n2.IsVaildDoulbe()) return pResult;
	long lStart = (long)n.GetDoubleValue();
	long lEnd = (long)n2.GetDoubleValue();

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		LAST(pResult->m_aryValue, data.m_aryValue, lStart, lEnd);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::LAST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lStart, long lEnd)
{
	long lCount = (long)data.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);


	int nStart = lStart;
	int nEnd = lEnd, k = 0, i = 0, j = 0, t = 0;
	for (; k < lCount; ++k)
	{
		const auto& item = data[k];
		if (item.IsVaild() && item._dValue) break;
	}

	for (i = k, t = k - nEnd + 1; i<lCount; ++i, ++t)
	{
		j = (nStart == 0) ? k : std::max(k, i - nStart);
		for (; j < t; ++j)
		{
			const auto& item = data[j];
			if (!item.IsVaild() || item._dValue <= 0)
			{
				break;
			}
		}

		dest[i].SetValue(j<t ? 0 : 1);
	}
}


/*
EXISTR(X,A,B):是否存在(前几日到前几日间).
例如:EXISTR(CLOSE>OPEN,10,5)
表示从前10日内到前5日内存在着阳线
若A为0,表示从第一天开始,B为0,表示到最后日止
*/
Variant* VariantOperator::EXISTR(const Variant& data, const Variant& n, const Variant& n2)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	if (n2.GetType() != Variant::DOUBLE_TYPE || !n2.IsVaildDoulbe()) return pResult;
	long lStart = (long)n.GetDoubleValue();
	long lEnd = (long)n2.GetDoubleValue();

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		EXISTR(pResult->m_aryValue, data.m_aryValue, lStart, lEnd);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::EXISTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lEnd, long lStart)
{
	long lCount = (long)data.size();
	VARIANT_ITEM falseItem;
	falseItem.SetValue(0);
	dest.resize(lCount, falseItem);

	if (lEnd == 0) lEnd = lCount;
	if (lStart == 0) lStart = 1;

	bool bFlag = true;
	for (long i = 0, j = 0; i < lCount; ++i)
	{
		bFlag = false;
		for (j = lStart; j <= lEnd; ++j)
		{
			if (i - j - 1 < 0) break;

			const auto& dataItem = data[i - j - 1];
			if (dataItem.IsVaild() && dataItem._dValue>0)
			{
				bFlag = true;
				break;
			}
		}

		if (bFlag) dest[i].SetValue(1);
	}
}

Variant* VariantOperator::SAR(const Variant& n, const Variant& s, const Variant& m, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();

	if (s.GetType() != Variant::DOUBLE_TYPE || !s.IsVaildDoulbe()) return pResult;
	if (m.GetType() != Variant::DOUBLE_TYPE || !m.IsVaildDoulbe()) return pResult;

	SAR(pResult->m_aryValue, ln, s.GetDoubleValue(), m.GetDoubleValue(), pHistory);
	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

void VariantOperator::SAR(ARRAY_DOUBLE& dest, long n, double dStep, double dExValue, const IHistoryData* pHistory)
{
	long lCount = pHistory->GetKCount();
	dest.resize(lCount, VARIANT_ITEM());
	double dMax = 0, dMin = 0;
	for (long i = 0; i < n; ++i)
	{
		auto item = pHistory->GetKItem(i);
		if (i == 0)
		{
			dMax = item->_dHigh;
			dMin = item->_dLow;
		}
		else
		{
			if (dMax < item->_dHigh) dMax = item->_dHigh;
			if (dMin > item->_dLow) dMin = item->_dLow;
		}
	}

	const long SAR_LONG = 0, SAR_SHORT = 1;
	long lPosition = SAR_LONG;
	dest[n - 1].SetValue(dMin);
	double dNextSar = dMin, dSIP = pHistory->GetKItem(0)->_dHigh, dAF = dStep / 100, dYSIP = 0;
	for (long i = n; i < lCount; ++i)
	{
		dYSIP = dSIP;
		auto item = pHistory->GetKItem(i);
		auto yitem = pHistory->GetKItem(i - 1);

		if (lPosition == SAR_LONG)
		{
			if (item->_dLow<dest[i - 1]._dValue)
			{
				lPosition = SAR_SHORT;
				dSIP = item->_dLow;
				dAF = dStep / 100;
				dNextSar = std::max(item->_dHigh, yitem->_dHigh);
				dNextSar = std::max(dNextSar, dYSIP + dAF*(dSIP - dYSIP));
			}
			else
			{
				lPosition = SAR_LONG;
				if (item->_dHigh>dYSIP)
				{
					dSIP = item->_dHigh;
					dAF = std::min(dAF + dStep / 100, dExValue / 100);
				}
				dNextSar = std::min(item->_dLow, yitem->_dLow);
				dNextSar = std::min(dNextSar, dest[i - 1]._dValue + dAF*(dSIP - dest[i - 1]._dValue));
			}
		}
		else if (lPosition == SAR_SHORT)
		{
			if (item->_dHigh>dest[i - 1]._dValue)
			{
				lPosition = SAR_LONG;
				dSIP = item->_dHigh;
				dAF = dStep / 100;
				dNextSar = std::min(item->_dLow, yitem->_dLow);
				dNextSar = std::min(dNextSar, dest[i - 1]._dValue + dAF*(dSIP - dYSIP));
			}
			else
			{
				lPosition = SAR_SHORT;
				if (item->_dLow<dYSIP)
				{
					dSIP = item->_dLow;
					dAF = std::min(dAF + dStep / 100, dExValue / 100);
				}
				dNextSar = std::max(item->_dHigh, yitem->_dHigh);
				dNextSar = std::max(dNextSar, dest[i - 1]._dValue + dAF*(dSIP - dest[i - 1]._dValue));
			}
		}
		dest[i].SetValue(dNextSar);
	}
}


Variant* VariantOperator::SARTURN(const Variant& n, const Variant& s, const Variant& m, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();

	if (s.GetType() != Variant::DOUBLE_TYPE || !s.IsVaildDoulbe()) return pResult;
	if (m.GetType() != Variant::DOUBLE_TYPE || !m.IsVaildDoulbe()) return pResult;

	ARRAY_DOUBLE sarData;
	SAR(sarData, ln, s.GetDoubleValue(), m.GetDoubleValue(), pHistory);
	long lCount = pHistory->GetKCount();
	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(lCount, VARIANT_ITEM());

	long lStart = (long)GetFirstVaildIndex(sarData);
	bool bFlag = false;
	if (lStart<lCount) bFlag = pHistory->GetKItem(lStart)->_dClose>sarData[lStart]._dValue;

	for (long i = lStart + 1; i < lCount; ++i)
	{
		auto item = pHistory->GetKItem(i);

		if (item->_dClose<sarData[i]._dValue && bFlag) dest[i].SetValue(-1);
		else dest[i].SetValue((item->_dClose>sarData[i]._dValue && !bFlag) ? 1 : 0);

		bFlag = item->_dClose>sarData[i]._dValue;
	}

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

/*
属于未来函数,前M个ZIG转向波谷到当前距离.
用法:TROUGHBARS(K,N,M)表示之字转向ZIG(K,N)的前M个波谷到当前的周期数,M必须大于等于1
例如:TROUGHBARS(2,5,2)表示%5最低价ZIG转向的前2个波谷到当前的周期数
*/

Variant* VariantOperator::TROUGHBARS(const Variant& k, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (m.GetType() != Variant::DOUBLE_TYPE || !m.IsVaildDoulbe()) return pResult;
	long lm = (long)m.GetDoubleValue();
	if (lm < 1) return pResult;

	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();
	if (ln < 1) return pResult;

	Variant* pZig = ZIG(k, n, pHistory, pNode);
	if (!pZig || pZig->GetType() != Variant::ARRAY_DOUBLE_TYPE) return pResult;

	long lEnd = (long)m.GetDoubleValue();
	if (lEnd<1) return pResult;
	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	const ARRAY_DOUBLE& data = pZig->m_aryValue;
	int nDataCount = (long)data.size();
	dest.resize(nDataCount, VARIANT_ITEM());
	long *trough = new long[lEnd];
	long lFlag = 0;
	memset(trough, 0, sizeof(long)*lEnd);
	long i = GetFirstVaildIndex(data) + 1;
	for (lEnd--; i<nDataCount && data[i]._dValue>data[i - 1]._dValue; ++i);

	for (; i<nDataCount && data[i]._dValue<data[i - 1]._dValue; ++i);

	for (trough[0] = --i; i<nDataCount - 1; ++i)
	{
		if (data[i]._dValue<data[i + 1]._dValue)
		{
			if (lFlag)
			{
				if (lEnd) memmove(trough + 1, trough, lEnd*sizeof(long));
				trough[lFlag = 0] = i;
			}
		}
		else lFlag = 1;
		if (trough[lEnd]) dest[i].SetValue(i - trough[lEnd]);
	}
	if (trough[lEnd]) dest[i].SetValue(i - trough[lEnd]);

	delete[]	trough;

	pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	return pResult;
}

/*
属于未来函数, 前M个ZIG转向波谷值.
用法:TROUGH(K, N, M)表示之字转向ZIG(K, N)的前M个波谷的数值, M必须大于等于1
例如 :TROUGH(2, 5, 2)表示 % 5最低价ZIG转向的前2个波谷的数值
*/

Variant* VariantOperator::TROUGH(const Variant& k, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode)
{
	Variant* pResult = Create();
	if (m.GetType() != Variant::DOUBLE_TYPE || !m.IsVaildDoulbe()) return pResult;
	long lm = (long)m.GetDoubleValue();
	if (lm < 1) return pResult;

	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();
	if (ln < 1) return pResult;

	ARRAY_DOUBLE zigData;
	if (k.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		ZIG(zigData, (long)k.GetDoubleValue(), ln, pHistory);

	}
	else if (k.GetType() == Variant::ARRAY_DOUBLE_TYPE && n.GetType() == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		ZIG(zigData, k.m_aryValue, ln);
	}
	else
	{
		return pResult;
	}

	std::list<double> listTrough;
	long lStart = (long)GetFirstVaildIndex(zigData);
	long i = lStart, j = 0;
	long lCount = (long)zigData.size();
	ARRAY_DOUBLE& dest = pResult->m_aryValue;
	dest.resize(lCount, VARIANT_ITEM());
	for (; i < lCount; ++i)
	{
		if (i + 1<lCount && i - 1 >= 0 && zigData[i]._dValue<zigData[i - 1]._dValue && zigData[i]._dValue<zigData[i + 1]._dValue) //波谷
		{
			listTrough.push_back(zigData[i]._dValue);
			break;
		}
	}

	for (i += 1; i<lCount; ++i)
	{
		if (i + 1<lCount && i - 1 >= 0 && zigData[i]._dValue<zigData[i - 1]._dValue && zigData[i]._dValue<zigData[i + 1]._dValue) //波谷
		{
			++j;
			listTrough.push_back(zigData[i]._dValue);
			if (j + 1 == lm)
			{
				dest[i].SetValue(listTrough.front());
			}
			else if (j + 1>lm)
			{
				listTrough.pop_front(); //大于计算的波谷数,去掉第1个波谷
				--j;
				dest[i].SetValue(listTrough.front());
			}
		}
		else
		{
			if (j + 1 == lm) dest[i].SetValue(listTrough.front());
		}
	}

	return pResult;
}


/*
过滤连续出现的信号.
用法:FILTER(X,N):X满足条件后,将其后N周期内的数据置为0,N为常量.
例如:FILTER(CLOSE>OPEN,5)查找阳线,5天内再次出现的阳线不被记录在内
*/
Variant* VariantOperator::FILTER(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		FILTER(pResult->m_aryValue, data.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::FILTER(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());

	for (long i = 0, j = 0; i < lCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		if (!item._dValue)
		{
			dest[i].SetValue(0);
			continue;
		}

		dest[i].SetValue(1);
		for (j = 0; j < n && i + j + 1<lCount; ++j)
		{
			dest[i + j + 1].SetValue(0);
		}

		i += n;
	}
}


/*
过滤连续出现的信号.
用法:TFILTER(买入条件,卖出条件,N);过滤掉买入(卖出)信号发出后,下一个反向信号发出前的所有买入(卖出)信号.

N=1表示仅对买入信号过滤;
N=2表示仅对卖出信号过滤;
N=0表示对买入和卖出信号都过滤,返回1,2表示买入或卖出条件成立;
同一K线上只能有一个信号;

例如:
ENTERLONG:TFILTER(买入,卖出,1);
EXITLONG:TFILTER(买入,卖出,2);
*/
Variant* VariantOperator::TFILTER(const Variant& buy, const Variant& sell, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe()) return pResult;
	long ln = (long)n.GetDoubleValue();

	if (buy.GetType() == Variant::ARRAY_DOUBLE_TYPE && sell.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		TFILTER(pResult->m_aryValue, buy.m_aryValue, sell.m_aryValue, ln);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::TFILTER(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& buy, const ARRAY_DOUBLE& sell, long n)
{
	long lBuyCount = (long)buy.size();
	long lSellCount = (long)sell.size();
	long lCount = std::max(lBuyCount, lSellCount);
	dest.resize(lCount, VARIANT_ITEM());

	int nFlag = n;
	if (nFlag == 1)
	{
		int nEnable = 1;
		for (int i = 0; i < lCount; ++i)
		{
			if (nEnable)
			{
				dest[i].SetValue(buy[i]._dValue);
				if (dest[i].IsVaild() && dest[i]._dValue)
					nEnable = 0;
			}
			else
				dest[i].SetValue(0);

			if (sell[i].IsVaild() && sell[i]._dValue) nEnable = 1;
		}
	}
	else if (nFlag == 2)
	{
		int nEnable = 1;
		for (int i = 0; i < lCount; ++i)
		{
			if (nEnable)
			{
				dest[i].SetValue(sell[i]._dValue);
				if (dest[i].IsVaild() && dest[i]._dValue)
					nEnable = 0;
			}
			else
				dest[i].SetValue(0);

			if (buy[i].IsVaild() && buy[i]._dValue)
				nEnable = 1;
		}
	}
	else if (nFlag == 0)
	{
		int nEnable = 1;
		for (int i = 0; i < lCount; ++i)
		{
			if (nEnable)
			{
				dest[i].SetValue(buy[i]._dValue);
				if (dest[i].IsVaild() && dest[i]._dValue)
					nEnable = 0;
			}
			else
				dest[i].SetValue(0);
			if (sell[i].IsVaild() && sell[i]._dValue)
				nEnable = 1;
		}
		nEnable = 1;
		for (int i = 0; i < lCount; ++i)
		{
			if (nEnable && dest[i]._dValue == 0)
			{
				dest[i].SetValue(sell[i]._dValue);
				if (dest[i].IsVaild() && dest[i]._dValue)
					nEnable = 0;
			}
			if (buy[i].IsVaild() && buy[i]._dValue)
				nEnable = 1;
		}
	}
}

/*
CONST(A):取A最后的值为常量.
用法:CONST(INDEXC)表示取指数现价
*/
Variant* VariantOperator::CONST_(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE && data.IsVaildDoulbe())
	{
		pResult->SetDoubleValue(data.GetDoubleValue());
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ARRAY_DOUBLE& dest = pResult->m_aryValue;
		const ARRAY_DOUBLE& src = data.m_aryValue;
		long lIndex = (long)src.size() - 1;
		if (lIndex >= 0 && src[lIndex].IsVaild())
		{
			pResult->SetDoubleValue(src[lIndex]._dValue);
		}

	}

	return pResult;
}

/*
ALL(X,N)一直满足条件
含义:是否一直满足条件。
阐释:ALL(X,N),统计N周期中是否一直都满足X条件,若N=0则从第一个有效值开始。例如：ALL(CLOSE>OPEN,20)表示是否20周期内全部都收阳线。
*/
Variant* VariantOperator::ALL(const Variant& data, const Variant& dayCount, const IHistoryData* pHistoryData)
{
	Variant* pResult = Create();

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && dayCount.GetType() == Variant::DOUBLE_TYPE)
	{
		ALL(pResult->m_aryValue, data.m_aryValue, (long)dayCount.GetDoubleValue());
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && dayCount.GetType() == Variant::DOUBLE_TYPE && pHistoryData)
	{
		if (!data.IsVaildDoulbe() ||!dayCount.IsVaildDoulbe()) return pResult;
		double dValue = data.GetDoubleValue();
		long n = (long)dayCount.GetDoubleValue();

		ALL(pResult->m_aryValue, dValue, n, pHistoryData);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::ALL(ARRAY_DOUBLE& dest, double dValue, long n, const IHistoryData* pHistory)
{
	if (!pHistory) return;

	long lCount = pHistory->GetKCount();
	dest.resize(lCount, VARIANT_ITEM());

	double dItem = dValue > 0 ? 1 : 0;
	for (int i = n; i < lCount; ++i)
	{
		dest[i].SetValue(dItem);
	}
}

void VariantOperator::ALL(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());
	if (n > lCount) n = lCount;

	long lValue = 0;
	for (long i = n, j = 0; i < lCount; ++i)
	{
		lValue = 1;
		for (j = 0; j < n; ++j)
		{
			const auto& item = data[i];
			if (!item.IsVaild() || !item._dValue)
			{
				lValue = 0;
				break;
			}
		}

		dest[i].SetValue(lValue);
	}
}

/*
ANY(X,N)  至少一次满足条件。
含义:ANY(X,N),统计N周期中是否至少有一次满足X条件,若N=0则从第一个有效值开始。
阐释:例如：ANY(CLOSE>OPEN,20)表示是否20周期内是否存在一根阳线。
*/
Variant* VariantOperator::ANY(const Variant& data, const Variant& dayCount, const IHistoryData* pHistoryData)
{
	Variant* pResult = Create();

	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && dayCount.GetType() == Variant::DOUBLE_TYPE)
	{
		ANY(pResult->m_aryValue, data.m_aryValue, (long)dayCount.GetDoubleValue());
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && dayCount.GetType() == Variant::DOUBLE_TYPE && pHistoryData)
	{
		if (!data.IsVaildDoulbe() || !dayCount.IsVaildDoulbe()) return pResult;
		double dValue = data.GetDoubleValue();
		long n = (long)dayCount.GetDoubleValue();

		ALL(pResult->m_aryValue, dValue, n, pHistoryData);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::ANY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());
	if (n > lCount) n = lCount;

	long lValue = 0;
	for (long i = n, j = 0; i < lCount; ++i)
	{
		lValue = 0;
		for (j = 0; j < n; ++j)
		{
			const auto& item = data[i];
			if (item.IsVaild() && item._dValue)
			{
				lValue = 1;
				break;
			}
		}

		dest[i].SetValue(lValue);
	}
}

Variant* VariantOperator::RAND(const Variant& data, const IHistoryData* pHistoryData)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		int nCount = pHistoryData->GetKCount();
		if (data.IsVaildDoulbe() && nCount>0)
		{
			double dData = data.GetDoubleValue();
			RAND(pResult->m_aryValue, dData, nCount);
			pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		RAND(pResult->m_aryValue, data.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::RAND(ARRAY_DOUBLE& dest, double dData, int nCount)
{
	dest.resize(nCount, VARIANT_ITEM());

	std::default_random_engine random((unsigned int)time(NULL));
	std::uniform_real_distribution<double> distribution(0, std::nextafter(std::abs(dData), DBL_MAX));

	for (int i = 0; i < nCount; ++i)
	{
		dest[i].SetValue(distribution(random));
	}
}

void VariantOperator::RAND(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data)
{
	int nCount = (long)data.size();
	dest.resize(nCount, VARIANT_ITEM());

	std::default_random_engine random((unsigned int)time(NULL));
	std::uniform_real_distribution<double> distribution(0, std::nextafter(100, DBL_MAX));

	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			std::uniform_real_distribution<double> distribution2(0, std::nextafter(std::abs(item._dValue), DBL_MAX));
			dest[i].SetValue(distribution2(random));
		}
		else
		{
			dest[i].SetValue(distribution(random));
		}
	}
}

Variant* VariantOperator::ROUND(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			pResult->SetDoubleValue((int)(dValue + 0.5));
		}
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ROUND(pResult->m_aryValue, data.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::ROUND(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data)
{
	int nCount = (long)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		dest[i].SetValue((int)(item._dValue + 0.5));
	}
}


const double ROUND2_SEED[] = { 1,10,100,1000, 10000,100000,1000000,10000000, 100000000, 1000000000, 10000000000 };

Variant* VariantOperator::ROUND2(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	int nDecimal = 0;
	if (n.IsVaildDoulbe()) nDecimal = (int)n.GetDoubleValue();
	if (nDecimal < 0) nDecimal = 0;
	else if (nDecimal >= ARRAYSIZE(ROUND2_SEED)) nDecimal = ARRAYSIZE(ROUND2_SEED) - 1;

	if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			XINT64 iValue = (XINT64)((data.GetDoubleValue() * ROUND2_SEED[nDecimal]) + 0.5);
			pResult->SetDoubleValue(iValue/ ROUND2_SEED[nDecimal]);
		}
	}
	else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		ROUND2(pResult->m_aryValue, data.m_aryValue, nDecimal);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::ROUND2(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, int nDecimal)
{
	int nCount = (long)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	XINT64 iValue = 0;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		iValue = (XINT64)((item._dValue * ROUND2_SEED[nDecimal]) + 0.5);
		dest[i].SetValue(iValue/ ROUND2_SEED[nDecimal]);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
ChipData::ChipData()
{

}

void ChipData::Clear()
{
	m_aryData.clear();
}

void ChipData::Add(const CHIP_ITEM& item)
{
	m_aryData.push_back(item);
}

double ChipData::GetProfit(double dPrice, long lCalculateType) const
{
	const int PRICE_AMPLIFICATION = 100;	//价格放大倍速
	double dTotalVol = 0, dProfitVol = 0, dProfitTotalVol = 0;

	const double PI = 3.1415926535;
	const long ANGLE = 45;	//三角分布计算的角度
	double dY = 0, dYTotal = 0, dYProfot = 0;

	long lPrice = (long)(dPrice*PRICE_AMPLIFICATION);
	for (const auto& item : m_aryData)
	{
		long lStart = (long)(item._dLow*PRICE_AMPLIFICATION);
		long lEnd = (long)(item._dHigh*PRICE_AMPLIFICATION);
		long lCount = lEnd - lStart + 1;
		if (lCount <= 0) continue;

		dProfitVol = 0;
		if (lCalculateType == 1)	//三角分布
		{
			if (lPrice > lEnd)
			{
				dProfitVol = item._dVol;
			}
			else if (lPrice < lStart)
			{
				dProfitVol = 0;
			}
			else
			{
				double dMiddle = (lEnd - lStart) / 2.0 + lStart;
				long lValue = 0;
				dYTotal = dYProfot = 0;
				for (long j = 0; j < lCount; ++j)
				{
					if (j + lStart > dMiddle) break;
					if (j + lStart<dMiddle)
						dY = tan(ANGLE* PI / 180)*j;
					else
						dY = tan(ANGLE* PI / 180)*(lCount - j);

					lValue = j + lStart;
					if (lValue < lPrice) dYProfot += dY;
					dYTotal += dY;
				}

				dProfitVol = dYProfot / dYTotal*item._dVol;
			}
		}
		else   //平均分布
		{
			if (lPrice > lEnd) dProfitVol = item._dVol;
			else if (lPrice < lStart) dProfitVol = 0;
			else dProfitVol = item._dVol*(lPrice - lStart + 1) / lCount;
		}

		dProfitTotalVol += dProfitVol;
		dTotalVol += item._dVol;
	}

	if (dTotalVol == 0) return 0;

	return dProfitTotalVol / dTotalVol;
}

double ChipData::GetProfitPrice(double dValue, long lCalculateType) const
{
	const int PRICE_AMPLIFICATION = 100;	//价格放大倍速
	double dTotalVol = 0;
	const double PI = 3.1415926535;
	const long ANGLE = 45;	//三角分布计算的角度

	long lMin = (long)(m_dMin*PRICE_AMPLIFICATION);
	long lMax = (long)(m_dMax*PRICE_AMPLIFICATION);
	std::vector<double> aryVol;
	aryVol.resize(lMax - lMin + 1, 0);

	std::vector<std::pair<long, double>> aryY;
	for (const auto& item : m_aryData)
	{
		long lStart = (long)(item._dLow*PRICE_AMPLIFICATION);
		long lEnd = (long)(item._dHigh*PRICE_AMPLIFICATION);
		long lCount = lEnd - lStart + 1;
		if (lCount <= 0) continue;

		if (lCalculateType == 1)
		{
			double dTotalValue = 0, y = 0;
			double dMiddle = (lEnd - lStart) / 2.0 + lStart;
			aryY.clear();

			for (long i = lStart + 1, j = 1; i <= dMiddle; ++i, ++j)
			{
				y = tan(ANGLE* PI / 180)*j;

				dTotalValue += y;
				aryY.push_back(std::pair<long, double>(i - lMin, y));
			}

			for (long i = lEnd - 1, j = 1; i>dMiddle; --i, ++j)
			{
				y = tan(ANGLE* PI / 180)*j;

				dTotalValue += y;
				aryY.push_back(std::pair<long, double>(i - lMin, y));
			}

			if (dTotalValue>0)
			{
				for (const auto& yItem : aryY)
				{
					aryVol[yItem.first] += yItem.second*item._dVol / dTotalValue;
				}
			}
		}
		else
		{
			double dAverageVol = item._dVol / lCount;	//平均分布
			if (dAverageVol <= 0) continue;

			for (int i = lStart; i <= lEnd && i <= lMax; ++i)
			{
				aryVol[i - lMin] += dAverageVol;
			}
		}
		dTotalVol += item._dVol;
	}

	double dVol = 0;
	double dPrice = 0;
	for (int i = 0; i < (int)aryVol.size(); ++i)
	{
		dVol += aryVol[i];
		dPrice = (i + lMin)*1.0 / PRICE_AMPLIFICATION;
		if (dVol / dTotalVol * 100 > dValue)
			break;
	}

	return dPrice;
}

double ChipData::GetProfitPrice(double dStart, double dEnd, long lCalculateType) const
{
	const int PRICE_AMPLIFICATION = 100;	//价格放大倍速
	double dTotalVol = 0;
	const double PI = 3.1415926535;
	const long ANGLE = 45;	//三角分布计算的角度

	long lMin = (long)(m_dMin*PRICE_AMPLIFICATION);
	long lMax = (long)(m_dMax*PRICE_AMPLIFICATION);
	std::vector<double> aryVol;
	aryVol.resize(lMax - lMin + 1, 0);
	std::vector<std::pair<long, double>> aryY;

	for (const auto& item : m_aryData)
	{
		long lStart = (long)(item._dLow*PRICE_AMPLIFICATION);
		long lEnd = (long)(item._dHigh*PRICE_AMPLIFICATION);
		long lCount = lEnd - lStart + 1;
		if (lCount <= 0) continue;

		if (lCalculateType == 1)
		{
			double dTotalValue = 0, y = 0;
			double dMiddle = (lEnd - lStart) / 2.0 + lStart;
			aryY.clear();

			for (long i = lStart + 1, j = 1; i <= dMiddle; ++i, ++j)
			{
				y = tan(ANGLE* PI / 180)*j;

				dTotalValue += y;
				aryY.push_back(std::pair<long, double>(i - lMin, y));
			}

			for (long i = lEnd - 1, j = 1; i>dMiddle; --i, ++j)
			{
				y = tan(ANGLE* PI / 180)*j;

				dTotalValue += y;
				aryY.push_back(std::pair<long, double>(i - lMin, y));
			}

			if (dTotalValue>0)
			{
				for (const auto& yItem : aryY)
				{
					aryVol[yItem.first] += yItem.second*item._dVol / dTotalValue;
				}
			}
		}
		else
		{
			double dAverageVol = item._dVol / lCount;	//平均分布
			if (dAverageVol <= 0) continue;

			for (int i = lStart; i <= lEnd && i <= lMax; ++i)
			{
				aryVol[i - lMin] += dAverageVol;
			}
		}

		dTotalVol += item._dVol;
	}

	double dVol = 0, dPrice = 0;
	double dAmount = 0;
	for (int i = 0; i < (int)aryVol.size(); ++i)
	{
		dPrice = (i + lMin)*1.0 / PRICE_AMPLIFICATION;
		if (dPrice >= dStart && dPrice <= dEnd)
		{
			dVol += aryVol[i];
			dAmount += dVol*dPrice;
		}
	}

	return dAmount / dVol;
}

double ChipData::GetPPartRate(long ln)
{
	double dRate = 0;

	double dTotalVol = 0;
	double dPartVol = 0;

	long lCount = (long)m_aryData.size();
	for (long i = lCount - 1, j = 1; i >= 0; --i, ++j)
	{
		const auto& item = m_aryData[i];

		dTotalVol += item._dVol;
		if (j>ln) dPartVol += item._dVol;
	}

	if (dTotalVol > 0) dRate = dPartVol / dTotalVol;

	return dRate;
}


}
}