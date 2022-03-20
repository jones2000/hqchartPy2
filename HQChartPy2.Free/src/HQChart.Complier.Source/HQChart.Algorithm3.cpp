/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	内置函数类 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Execute.h"
#include "HQChart.KLineData.h"
#include <sstream>
#include <algorithm>
#include <ctime>
#include "CalendarHelper.h"


//////////////////////////////////////////////////////////////////
//	字符串操作
//
//

namespace HQChart { namespace Complier {

const wchar_t DOUBLE_FORMAT[][6] = { L"%.00f", L"%.01f", L"%.02f", L"%.03f", L"%.04f", L"%.05f", L"%.06f", L"%.07f", L"%.08f" };
const long BUFFER_LEN = 256;

std::wstring* VariantOperator::CreateDoubleString(bool bRemoveZero, const wchar_t* fmt, ...)
{
	wchar_t buffer[BUFFER_LEN] = { 0 };

	va_list vl;
	va_start(vl, fmt);
	int nsize = vswprintf(buffer, ARRAYSIZE(buffer), fmt, vl);
	if (bRemoveZero) RemoveStringZero(buffer);
	std::wstring* pStrValue = CreateString(buffer);
	va_end(vl);

	return pStrValue;
}

void VariantOperator::RemoveStringZero(wchar_t* lpStr)
{
	long lLen=(long)wcslen(lpStr);

	if (wcschr(lpStr, L'.') == NULL) return;

	for (long i = lLen - 1; i >= 0; --i)
	{
		if (lpStr[i] == L'.')
		{
			lpStr[i] = L'\0';
			break;
		}

		if (lpStr[i] != L'0') break;
		
		lpStr[i] = L'\0';
	}
}


Variant* VariantOperator::NUMTOSTR(const Variant& data)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_STRING_TYPE;
		NUMTOSTR(pResult->m_aryValue, data.m_aryValue,3,true,this);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			wchar_t szBuffer[256] = { 0 };
			wprintf(szBuffer, DOUBLE_FORMAT[3], dValue);
			RemoveStringZero(szBuffer);
			pResult->SetStringValue(szBuffer);
		}
	}

	return pResult;
}



void VariantOperator::NUMTOSTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long ldecimal, bool bRemoveZero, VariantOperator* pVariantOperator)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());

	std::wstringstream strValue;
	for (long i = 0; i < lDataCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		dest[i].SetValue(pVariantOperator->CreateDoubleString(bRemoveZero, DOUBLE_FORMAT[ldecimal], item._dValue));
	}
}

Variant* VariantOperator::NUMTOSTRN(const Variant& data, const Variant& m)
{
	Variant* pResult = Create();
	long ldecimal = 3;
	if (m.m_nType == Variant::DOUBLE_TYPE && m.IsVaildDoulbe()) ldecimal = (long)m.GetDoubleValue();

	if (ldecimal > ARRAYSIZE(DOUBLE_FORMAT)) ldecimal = ARRAYSIZE(DOUBLE_FORMAT) - 1;

	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_STRING_TYPE;
		NUMTOSTR(pResult->m_aryValue, data.m_aryValue, ldecimal, false, this);
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			double dValue = data.GetDoubleValue();
			wchar_t szBuffer[256] = { 0 };
			wprintf(szBuffer, DOUBLE_FORMAT[ldecimal], dValue);
			pResult->SetStringValue(szBuffer);
		}
	}

	return pResult;
}

Variant* VariantOperator::STRCMP(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_STRING_TYPE && data2.m_nType == Variant::ARRAY_STRING_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		STRCMP(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue);
	}
	else if (data.m_nType == Variant::ARRAY_STRING_TYPE && data2.m_nType == Variant::STRING_TYPE)
	{
		if (data2.IsVaildString())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			STRCMP(pResult->m_aryValue, data.m_aryValue, data2.GetStringValue());
		}
	}
	else if (data.m_nType == Variant::STRING_TYPE && data2.m_nType == Variant::ARRAY_STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			STRCMP(pResult->m_aryValue, data.GetStringValue(), data.m_aryValue);
		}
	}
	else if (data.m_nType == Variant::STRING_TYPE && data2.m_nType == Variant::STRING_TYPE)
	{
		if (data.IsVaildString() && data2.IsVaildString())
		{
			int nValue = data.GetStringValue().compare(data2.GetStringValue());
			pResult->SetDoubleValue(nValue);
		}
	}

	return pResult;
}

void VariantOperator::STRCMP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& leftData, const ARRAY_DOUBLE& rightData)
{
	long lDataCount = (long)leftData.size();
	long lData2Count = (long)rightData.size();
	long lCount = std::max(lDataCount, lData2Count);

	dest.resize(lCount, VARIANT_ITEM());
	long lValue = 0;
	for (long i = 0; i < lCount; ++i)
	{
		if (i < lDataCount && i < lData2Count)
		{
			const auto& leftItem = leftData[i];
			const auto& rightItem = rightData[i];
			
			if (leftItem.IsVaild() && rightItem.IsVaild())
			{
				lValue = leftItem._pString->compare(*rightItem._pString);
				dest[i].SetValue(lValue);
			}
		}
	}
}

void VariantOperator::STRCMP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& leftData, const std::wstring& strRight)
{
	long lCount = (long)leftData.size();
	dest.resize(lCount, VARIANT_ITEM());
	long lValue = 0;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& leftItem = leftData[i];
		if (leftItem.IsVaild())
		{
			lValue = leftItem._pString->compare(strRight);
			dest[i].SetValue(lValue);
		}
	}
}

void VariantOperator::STRCMP(ARRAY_DOUBLE& dest, const std::wstring& strLeft, const ARRAY_DOUBLE& rightData)
{
	long lCount = (long)rightData.size();
	dest.resize(lCount, VARIANT_ITEM());
	long lValue = 0;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& rightItem = rightData[i];
		if (rightItem.IsVaild())
		{
			lValue = strLeft.compare(*(rightItem._pString));
			dest[i].SetValue(lValue);
		}
	}
}

Variant* VariantOperator::STRTONUM(const Variant& data)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_STRING_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		STRTONUM(pResult->m_aryValue, data.m_aryValue);
	}
	else if (data.m_nType == Variant::STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			double dValue = _wtof(data.GetStringValue().c_str());
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::STRTONUM(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());
	double dValue = 0;
	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			dValue = _wtof(item._pString->c_str());
			dest[i].SetValue(dValue);
		}
	}
}

Variant* VariantOperator::INBLOCK(const Variant& data, const IHistoryData* pHistory, Node* pNode)
{
	if (data.m_nType == Variant::STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			return pHistory->IsInCategroy(-1, data.GetStringValue());
		}
	}

	Variant* pResult = Create();
	return pResult;
}

Variant* VariantOperator::LOWERSTR(const Variant& data)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_STRING_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		LOWERSTR(pResult->m_aryValue, data.m_aryValue,this);
	}
	else if (data.m_nType == Variant::STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			std::wstring strValue = data.GetStringValue();
			std::transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
			pResult->SetStringValue(strValue);
		}
	}

	return pResult;
}

void VariantOperator::LOWERSTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, VariantOperator* pVariantOperator)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());
	std::wstring strValue;

	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			strValue = item._pString->c_str();
			std::transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
			dest[i].SetValue(pVariantOperator->CreateString(strValue));
		}
	}
}

Variant* VariantOperator::UPPERSTR(const Variant& data)
{
	Variant* pResult = Create();

	if (data.m_nType == Variant::ARRAY_STRING_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		UPPERSTR(pResult->m_aryValue, data.m_aryValue,this);
	}
	else if (data.m_nType == Variant::STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			std::wstring strValue = data.GetStringValue();
			std::transform(strValue.begin(), strValue.end(), strValue.begin(), ::toupper);
			pResult->SetStringValue(strValue);
		}
	}

	return pResult;
}

void VariantOperator::UPPERSTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, VariantOperator* pVariantOperator)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());
	std::wstring strValue;

	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			strValue = item._pString->c_str();
			std::transform(strValue.begin(), strValue.end(), strValue.begin(), ::toupper);
			dest[i].SetValue(pVariantOperator->CreateString(strValue));
		}
	}
}

Variant* VariantOperator::STRFIND(const Variant& data, const Variant& findData, const Variant& n)
{
	Variant* pResult = Create();
	long ln = 0;
	if (n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe()) ln = (long)n.GetDoubleValue();
		if (ln < 0) ln = 0;
	}
	std::wstring strFind;
	if (findData.IsVaildString()) strFind = findData.GetStringValue();

	if (data.m_nType == Variant::ARRAY_STRING_TYPE && findData.m_nType == Variant::STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (findData.IsVaildString() && n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			STRFIND(pResult->m_aryValue, data.m_aryValue, strFind, ln);
		}
	}
	else if (data.m_nType == Variant::STRING_TYPE && findData.m_nType == Variant::STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (findData.IsVaildString() && n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::DOUBLE_TYPE;
			const std::wstring& strData = data.GetStringValue();
			auto pos=strData.find(strFind, ln);
			long lPos = 0;
			if (pos == std::wstring::npos) lPos = 0;
			else lPos = (long)(pos+1);

			pResult->SetDoubleValue(lPos);
		}
	}

	return pResult;
}

void VariantOperator::STRFIND(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const std::wstring strFind, long n)
{
	long lCount = (long)data.size();
	dest.resize(lCount, VARIANT_ITEM());

	for (long i = 0; i < lCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			auto pos = item._pString->find(strFind, n);
			long lPos = 0;
			if (pos == std::wstring::npos) lPos = 0;
			else lPos = (long)(pos+1);

			dest[i].SetValue(lPos);
		}
	}
}

Variant* VariantOperator::STRLEFT(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	long ln = 0;
	if (n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe()) ln = (long)n.GetDoubleValue();
		if (ln < 0) ln = 0;
	}

	if (data.m_nType == Variant::ARRAY_STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_STRING_TYPE;
			STRLEFT(pResult->m_aryValue, data.m_aryValue, ln,this);
		}
	}
	else if (data.m_nType == Variant::STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::STRING_TYPE;
			std::wstring strData = data.GetStringValue();
			StringLeft(strData, ln);
			pResult->SetStringValue(strData);
		}
	}

	return pResult;
}

void VariantOperator::STRLEFT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lCount, VariantOperator* pVariantOperator)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());

	for (long i = 0; i < lDataCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			std::wstring* pstrValue = pVariantOperator->CreateString(*item._pString);
			StringLeft(*pstrValue, lCount);
			dest[i].SetValue(pstrValue);
		}
	}
}

void VariantOperator::StringLeft(std::wstring& str, long lCount)
{
	auto strString = str.substr(0, lCount);
	str = strString;
}


Variant* VariantOperator::STRRIGHT(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	long ln = 0;
	if (n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe()) ln = (long)n.GetDoubleValue();
		if (ln < 0) ln = 0;
	}

	if (data.m_nType == Variant::ARRAY_STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_STRING_TYPE;
			STRRIGHT(pResult->m_aryValue, data.m_aryValue, ln, this);
		}
	}
	else if (data.m_nType == Variant::STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::STRING_TYPE;
			std::wstring strData = data.GetStringValue();
			StringRight(strData, ln);
			pResult->SetStringValue(strData);
		}
	}

	return pResult;
}

void VariantOperator::STRRIGHT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lCount, VariantOperator* pVariantOperator)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());

	for (long i = 0; i < lDataCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			std::wstring* pstrValue = pVariantOperator->CreateString(*item._pString);
			StringRight(*pstrValue, lCount);
			dest[i].SetValue(pstrValue);
		}
	}
}

void VariantOperator::StringRight(std::wstring& str, long lCount)
{
	long lLen = (long)str.size();
	long lFirst = lLen - lCount;
	if (lFirst <= 0)
	{
		str.clear();
		return;
	}

	auto strString = str.substr(lFirst);
	str = strString;
}

Variant* VariantOperator::STRMID(const Variant& data, const Variant& first, const Variant& n)
{
	Variant* pResult = Create();
	long ln = 0;
	if (n.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe()) ln = (long)n.GetDoubleValue();
		if (ln < 0) ln = 0;
	}

	long lFirst = 0;
	if (first.m_nType == Variant::DOUBLE_TYPE)
	{
		if (first.IsVaildDoulbe()) lFirst = (long)first.GetDoubleValue();
		if (lFirst < 0) lFirst = 0;
	}

	if (data.m_nType == Variant::ARRAY_STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE && first.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe() && first.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_STRING_TYPE;
			STRMID(pResult->m_aryValue, data.m_aryValue, lFirst, ln, this);
		}
	}
	else if (data.m_nType == Variant::STRING_TYPE && n.m_nType == Variant::DOUBLE_TYPE && first.m_nType == Variant::DOUBLE_TYPE)
	{
		if (n.IsVaildDoulbe() && first.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::STRING_TYPE;
			std::wstring strData = data.GetStringValue();
			StringMid(strData, lFirst, ln);
			pResult->SetStringValue(strData);
		}
	}

	return pResult;
}

void VariantOperator::STRMID(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lFirst, long lCount, VariantOperator* pVariantOperator)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());

	for (long i = 0; i < lDataCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			std::wstring* pstrValue = pVariantOperator->CreateString(*item._pString);
			StringMid(*pstrValue, lFirst, lCount);
			dest[i].SetValue(pstrValue);
		}
	}
}

void VariantOperator::StringMid(std::wstring& str, long lFirst, long lCount)
{
	long lLen = (long)str.size();
	if (lFirst>=lLen)
	{
		str.clear();
		return;
	}

	if (lCount <= 0)
	{
		str.clear();
		return;
	}

	auto strString = str.substr(lFirst,lCount);
	str = strString;
}

Variant* VariantOperator::ZTPRICE(const Variant& data, const Variant& rate)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && rate.m_nType == Variant::DOUBLE_TYPE)
	{
		if (rate.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			ZTPRICE(pResult->m_aryValue, data.m_aryValue, rate.GetDoubleValue());
		}
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE && rate.m_nType == Variant::DOUBLE_TYPE)
	{
		if (rate.IsVaildDoulbe() && data.IsVaildDoulbe())
		{
			double dValue = (1 + rate.GetDoubleValue())*data.GetDoubleValue();
			pResult->SetDoubleValue(dValue);
		}
	}
	return pResult;
}

void VariantOperator::ZTPRICE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dRate)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());
	double dValue = 0;
	for (long i = 0; i < lDataCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			dValue = (1 + dRate)*item._dValue;
			dest[i].SetValue(dValue);
		}
	}
}

Variant* VariantOperator::DTPRICE(const Variant& data, const Variant& rate)
{
	Variant* pResult = Create();
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE && rate.m_nType == Variant::DOUBLE_TYPE)
	{
		if (rate.IsVaildDoulbe())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			DTPRICE(pResult->m_aryValue, data.m_aryValue, rate.GetDoubleValue());
		}
	}
	else if (data.m_nType == Variant::DOUBLE_TYPE && rate.m_nType == Variant::DOUBLE_TYPE)
	{
		if (rate.IsVaildDoulbe() && data.IsVaildDoulbe())
		{
			double dValue = (1 - rate.GetDoubleValue())*data.GetDoubleValue();
			pResult->SetDoubleValue(dValue);
		}
	}
	return pResult;
}

void VariantOperator::DTPRICE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dRate)
{
	long lDataCount = (long)data.size();
	dest.resize(lDataCount, VARIANT_ITEM());
	double dValue = 0;
	for (long i = 0; i < lDataCount; ++i)
	{
		const auto& item = data[i];
		if (item.IsVaild())
		{
			dValue = (1 - dRate) * item._dValue;
			dest[i].SetValue(dValue);
		}
	}
}

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

Variant* VariantOperator::GetCurrentTime()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	long lTime = st.wHour * 10000 + st.wMinute * 100 + st.wSecond;
	Variant* pResult = Create();
	pResult->SetDoubleValue(lTime);
	return pResult;
}

Variant* VariantOperator::GetCurrentDate()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	long lYear = st.wYear * 10000 + st.wMonth * 100 + st.wDay;
	Variant* pResult = Create();
	pResult->SetDoubleValue(lYear);
	return pResult;
}

Variant* VariantOperator::DATESTR(const Variant& data)
{
	Variant* pResult = Create();
	wchar_t szValue[32] = { 0 };
	long lValue = 0;
	std::wstringstream strBuffer;
	if (data.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		const auto& src = data.m_aryValue;
		long lCount = (long)src.size();
		auto& dest = pResult->m_aryValue;
		dest.resize(lCount, VARIANT_ITEM());
		pResult->m_nType = Variant::ARRAY_STRING_TYPE;
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = src[i];
			if (!item.IsVaild()) continue;
			lValue = (long)item._dValue;
			if (lValue > 17900101 && lValue <= 99991231)
			{
				dest[i].SetValue(CreateString(std::to_wstring(lValue)));
			}
		}

	}
	else if (data.m_nType == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			lValue = (long)data.GetDoubleValue();
			if (lValue > 17900101 && lValue <= 99991231)
			{
				pResult->SetStringValue(std::to_wstring(lValue));
			}
		}
	}
	return pResult;
}

/*
CON2STR(A,N):取A最后的值(非序列值)转为字符串,小数位数N.
用法:CON2STR(FINANCE(20),3)表示取营业收入,以3位小数转为字符串
*/
Variant* VariantOperator::CON2STR(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe())
		return pResult;

	int lDecimal = (int)n.GetDoubleValue();
	if (lDecimal < 0) lDecimal = 0;
	else if (lDecimal > 20) lDecimal = 20;

	double dValue = 0;
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		const auto& aryData = data.m_aryValue;
		if (aryData.empty()) return pResult;

		const auto item = aryData.rbegin();
		if (!item->IsVaild()) return pResult;

		dValue = item->_dValue;
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return pResult;

		dValue = data.GetDoubleValue();
	}
	else
	{
		return pResult;
	}

	std::wstringstream strBuffer;
	strBuffer.precision(lDecimal);
	strBuffer.setf(std::ios::fixed);
	strBuffer << dValue;
	pResult->SetStringValue(strBuffer.str());

	return pResult;
}

/*
VAR2STR(A,N):取A的每一个值转为字符串,小数位数N.
用法:VAR2STR(C,3)表示取收盘价,以3位小数转为字符串
*/
Variant* VariantOperator::VAR2STR(const Variant& data, const Variant& n)
{
	Variant* pResult = Create();
	if (n.GetType() != Variant::DOUBLE_TYPE || !n.IsVaildDoulbe())
		return pResult;

	int lDecimal = (int)n.GetDoubleValue();
	if (lDecimal < 0) lDecimal = 0;
	else if (lDecimal > 20) lDecimal = 20;

	double dValue = 0;
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		VAR2STR(pResult->m_aryValue, data.m_aryValue, lDecimal, this);
		pResult->SetType(Variant::ARRAY_STRING_TYPE);
		return pResult;
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return pResult;

		dValue = data.GetDoubleValue();

		std::wstringstream strBuffer;
		strBuffer.precision(lDecimal);
		strBuffer.setf(std::ios::fixed);
		strBuffer << dValue;
		pResult->SetStringValue(strBuffer.str());

		return pResult;
	}
	else
	{
		return pResult;
	}
}

void VariantOperator::VAR2STR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long ln, VariantOperator* pVariantOperator)
{
	std::wstringstream strBuffer;
	strBuffer.precision(ln);
	strBuffer.setf(std::ios::fixed);

	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	double dValue = 0;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		dValue = item._dValue;
		strBuffer.str(L"");
		strBuffer << dValue;
		std::wstring* pstr = pVariantOperator->CreateString(strBuffer.str());
		dest[i].SetValue(pstr);
	}
}

/*
STRCAT(A,B):将两个字符串A,B(非序列化)相加成一个字符串C.
用法: STRCAT('多头','开仓')将两个字符串'多头','开仓'相加成一个字符串'多头开仓'
*/
Variant* VariantOperator::STRCAT(const Variant& data, const Variant& data2)
{
	Variant* pResult = Create();
	if (data.GetType() != Variant::STRING_TYPE || data2.GetType() != Variant::STRING_TYPE) return pResult;
	if (!data.IsVaildString() || !data2.IsVaildString()) return pResult;

	std::wstring strResult(data.GetStringValue());
	strResult.append(data2.GetStringValue());

	pResult->SetStringValue(strResult);
	return pResult;
}


/*
VARCAT(A,B):将两个字符串A,B相加成一个字符串C.
用法: DRAWTEXT(CLOSE>OPEN,LOW,VARCAT('多头',VAR2STR(C,2))) 将两个字符串相加成一个字符串并按条件显示出来
*/
Variant* VariantOperator::VARCAT(const Variant& data, const Variant& data2)
{
	if (data.GetType() == Variant::STRING_TYPE && data2.GetType() == Variant::STRING_TYPE)
		return STRCAT(data, data2);

	Variant* pResult = Create();
	if (data.GetType() == Variant::STRING_TYPE && data2.GetType() == Variant::ARRAY_STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			VARCAT(pResult->m_aryValue, data.GetStringValue(), data2.m_aryValue, this);
			pResult->SetType(Variant::ARRAY_STRING_TYPE);
		}
			
	}
	else if (data.GetType() == Variant::ARRAY_STRING_TYPE && data2.GetType() == Variant::STRING_TYPE)
	{
		if (data2.IsVaildString())
		{
			VARCAT(pResult->m_aryValue, data.m_aryValue, data2.GetStringValue(), this);
			pResult->SetType(Variant::ARRAY_STRING_TYPE);
		}
			
	}
	else if (data.GetType() == Variant::ARRAY_STRING_TYPE && data2.GetType() == Variant::ARRAY_STRING_TYPE)
	{
		VARCAT(pResult->m_aryValue, data.m_aryValue, data2.m_aryValue, this);
		pResult->SetType(Variant::ARRAY_STRING_TYPE);
	}

	return pResult;
}

void VariantOperator::VARCAT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const std::wstring& str2, VariantOperator* pVariantOperator)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	std::wstring strValue;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;

		strValue = item._pString->c_str();
		strValue.append(str2);

		std::wstring* pStr = pVariantOperator->CreateString(strValue);
		dest[i].SetValue(pStr);
	}
}

void VariantOperator::VARCAT(ARRAY_DOUBLE& dest, const std::wstring& str, const ARRAY_DOUBLE& data2, VariantOperator* pVariantOperator)
{
	int nCount = (int)data2.size();
	dest.resize(nCount, VARIANT_ITEM());
	std::wstring strValue;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data2[i];
		if (!item.IsVaild()) continue;

		strValue = str; ;
		strValue.append(item._pString->c_str());

		std::wstring* pStr = pVariantOperator->CreateString(strValue);
		dest[i].SetValue(pStr);
	}
}

void VariantOperator::VARCAT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, VariantOperator* pVariantOperator)
{
	int nCount1 = (int)data.size();
	int nCount2 = (int)data2.size();
	int nCount = std::max(nCount1, nCount2);
	dest.resize(nCount, VARIANT_ITEM());
	std::wstring strValue = L"";
	for (int i = 0; i < nCount; ++i)
	{
		strValue = L"";
		if (i < nCount1)
		{
			const auto& item = data[i];
			if (item.IsVaild())
				strValue.append(item._pString->c_str());
		}

		if (i < nCount2)
		{
			const auto& item = data[i];
			if (item.IsVaild())
				strValue.append(item._pString->c_str());
		}

		if (!strValue.empty())
		{
			std::wstring* pStr = pVariantOperator->CreateString(strValue);
			dest[i].SetValue(pStr);
		}
	}
}

/*
STRSPACE(A):字符串附带一空格
*/
Variant* VariantOperator::STRSPACE(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::STRING_TYPE)
	{
		if (data.IsVaildString())
		{
			std::wstring strValue = data.GetStringValue();
			strValue.append(L" ");
			pResult->SetStringValue(strValue);
		}
	}
	else if (data.GetType() == Variant::ARRAY_STRING_TYPE)
	{
		const auto& aryData = data.m_aryValue;
		auto& dest = pResult->m_aryValue;
		std::wstring strValue;
		if (!aryData.empty())
		{
			int nCount = (int)aryData.size();
			dest.resize(nCount, VARIANT_ITEM());
			for (int i = 0; i < nCount; ++i)
			{
				const auto& item = aryData[i];
				if (!item.IsVaild()) continue;
				strValue = item._pString->c_str();
				strValue.append(L" ");

				std::wstring* pStr = CreateString(strValue);
				dest[i].SetValue(pStr);
			}

			pResult->SetType(Variant::ARRAY_STRING_TYPE);
		}
	}

	return pResult;
}

Variant* VariantOperator::GetLunarCalendar(const IHistoryData* pHistoryData, long lType) //获取农历 lType 0=全部 1=年 2=月 3=日
{
	Variant* pResult = Create();
	long lCount = pHistoryData->GetKCount();
	if (lCount <= 0) return pResult;

	auto& dest = pResult->m_aryValue;
	dest.resize(lCount, VARIANT_ITEM());
	pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;

	long lDate = 0, lYear = 0, lMonth = 0, lDay = 0, lNewDate = 0;
	for (long i = 0; i < lCount; ++i)
	{
		auto pItem = pHistoryData->GetKItem(i);
		if (!pItem) continue;
		lDate = pItem->_nDate;
		lYear = (long)(lDate / 10000);
		lMonth = (long)((lDate % 10000) / 100);
		lDay = lDate % 100;

		lNewDate = Helper::CalendarHelper::GetLunarDate((unsigned int)lYear, (unsigned char)lMonth, (unsigned char)lDay);
		lYear = (long)(lNewDate / 10000);
		lMonth = (long)((lNewDate % 10000) / 100);
		lDay = lNewDate % 100;

		switch (lType)
		{
		case 1:
			dest[i].SetValue(lYear);
			break;
		case 2:
			dest[i].SetValue(lMonth);
			break;
		case 3:
			dest[i].SetValue(lDay);
			break;
		default:
			dest[i].SetValue(lNewDate);
			break;
		}
	}


	return pResult;
}



//指定日期到1990.12.19的天数.
//用法: DATETODAY(date)
//DATETODAY(date).返回date到1990.12.19的天数.有效日期为(901219 - 1341231)
//例如 : DATETODAY(901219)返回0.
const int DATETODAY_STARTDATE = 19901219;
const int DATE_STARTDATE = 19000000;
Variant* VariantOperator::DATETODAY(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		DATETODAY(pResult->m_aryValue, data.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			int nDateItem = (int)(data.GetDoubleValue()) + DATE_STARTDATE;
			if (nDateItem >= DATETODAY_STARTDATE)
			{
				struct tm  tmItem = { 0,0,0,19,(12 - 1),(1990 - 1900) };
				time_t timeStart = mktime(&tmItem);

				tmItem.tm_year = (int)(nDateItem / 10000) - 1900;
				tmItem.tm_mon = (int)((nDateItem % 10000) / 100) - 1;
				tmItem.tm_mday = (int)(nDateItem % 100);
				time_t timeItem = mktime(&tmItem);

				double dValue = std::difftime(timeItem, timeStart) / (60.0 * 60 * 24);
				pResult->SetDoubleValue((int)(dValue));
			}
		}
	}
	return pResult;
}

void VariantOperator::DATETODAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	int nDateItem = 0;
	struct tm  tmItem = { 0,0,0,19,(12-1),(1990 - 1900) };
	time_t timeStart = mktime(&tmItem);
	time_t timeItem = 0, timeValue = 0;
	double dValue = 0;
	
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		nDateItem = (int)(item._dValue) + DATE_STARTDATE;
		if (nDateItem < DATETODAY_STARTDATE) continue;

		tmItem.tm_year = (int)(nDateItem / 10000) - 1900;
		tmItem.tm_mon = (int)((nDateItem % 10000) / 100) - 1;
		tmItem.tm_mday = (int)(nDateItem % 100);

		timeItem = mktime(&tmItem);

		dValue = std::difftime(timeItem, timeStart)/(60.0 * 60 * 24);
		dest[i].SetValue((int)(dValue));
	}
}

Variant* VariantOperator::DAYTODATE(const Variant& data)
{
	Variant* pResult = Create();
	if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		DAYTODATE(pResult->m_aryValue, data.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (data.IsVaildDoulbe())
		{
			struct tm  tmItem = { 0,0,0,19,(12 - 1),(1990 - 1900) };
			time_t timeStart = mktime(&tmItem);

			XINT64 iSecond = 60 * 60 * 24 * (XINT64)(data.GetDoubleValue());
			timeStart += iSecond;

			auto* pDay = std::localtime(&timeStart);
			int nValue = ((pDay->tm_year + 1900) * 10000 + (pDay->tm_mon + 1) * 100 + pDay->tm_mday) - DATE_STARTDATE;
			pResult->SetDoubleValue(nValue);
			
		}
	}
	return pResult;
}

void VariantOperator::DAYTODATE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data)
{
	int nCount = (int)data.size();
	dest.resize(nCount, VARIANT_ITEM());
	int nDateItem = 0;
	struct tm  tmItem = { 0,0,0,19,(12 - 1),(1990 - 1900) };
	time_t timeStart = mktime(&tmItem);
	time_t timeItem = 0;
	XINT64 iSecond = 0;
	int nValue = 0;
	for (int i = 0; i < nCount; ++i)
	{
		const auto& item = data[i];
		if (!item.IsVaild()) continue;
		iSecond = 60 * 60 * 24 * (XINT64)(item._dValue);
		timeItem = timeStart + iSecond;
		auto* pDay = std::localtime(&timeItem);
		nValue = ((pDay->tm_year + 1900) * 10000 + (pDay->tm_mon + 1) * 100 + pDay->tm_mday) - DATE_STARTDATE;
		dest[i].SetValue(nValue);
	}
}

}
}