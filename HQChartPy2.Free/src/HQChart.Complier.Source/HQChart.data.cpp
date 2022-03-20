/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	内置函数类, 类型定义 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Complier.h"
#include "HQChart.Execute.h"
#include <sstream>

namespace HQChart { namespace Complier {

#ifdef _WIN32
#define VARIANT_COMPARE_FUNCTIONS(__funcname__, __op__)\
	Variant* VariantOperator::##__funcname__##(const Variant& left, const Variant& right)\
	{\
		Variant* pResult =  Create();\
		if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)\
		{\
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;\
			__funcname__##(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);\
		}\
		else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)\
		{\
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;\
			if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())\
				__funcname__##(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);\
		}\
		else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)\
		{\
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;\
			if (right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())\
				__funcname__##(pResult->m_aryValue, left.m_aryValue, right.GetDoubleValue());\
		}\
		else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)\
		{\
			if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild() && \
				right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())\
			{\
				double dValue = (left.m_aryValue[0]._dValue __op__ right.m_aryValue[0]._dValue) ? 1 : 0;\
				pResult->SetDoubleValue(dValue);\
			}\
		}\
		return pResult;\
	}\
	void VariantOperator::##__funcname__##(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)\
	{\
		size_t nLeftCount = left.size(), nRightCount = right.size();\
		size_t nCount = std::max(nLeftCount, nRightCount);\
		for (size_t i = 0; i < nCount; ++i)\
		{\
			VARIANT_ITEM item;\
			if (i<nLeftCount || i<nRightCount)\
			{\
				const auto& leftItem = left[i];\
				const auto& rightItem = right[i];\
				if (leftItem._sType == VARIANT_ITEM::VAILD_ID && rightItem._sType == VARIANT_ITEM::VAILD_ID)\
				{\
					item.SetValue((leftItem._dValue __op__ rightItem._dValue) ? 1 : 0);\
				}\
			}\
			dest.push_back(item);\
		}\
	}\
	void VariantOperator::##__funcname__##(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)\
	{\
		for (auto rightItem : right)\
		{\
			VARIANT_ITEM item;\
			if (rightItem._sType == VARIANT_ITEM::VAILD_ID)\
			{\
				item.SetValue((dLeft __op__ rightItem._dValue) ? 1 : 0);\
			}\
			dest.push_back(item);\
		}\
	}\
	void VariantOperator::##__funcname__##(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight)\
	{\
		for (auto leftItem : left)\
		{\
			VARIANT_ITEM item;\
			if (leftItem._sType == VARIANT_ITEM::VAILD_ID)\
			{\
				item.SetValue((leftItem._dValue __op__ dRight) ? 1 : 0);\
			}\
			dest.push_back(item);\
		}\
	}
#else
#define VARIANT_COMPARE_FUNCTIONS(__funcname__, __op__)\
	Variant* VariantOperator::__funcname__(const Variant& left, const Variant& right)\
	{\
		Variant* pResult =  Create();\
		if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)\
		{\
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;\
			__funcname__(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);\
		}\
		else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)\
		{\
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;\
			if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())\
				__funcname__(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);\
		}\
		else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)\
		{\
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;\
			if (right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())\
				__funcname__(pResult->m_aryValue, left.m_aryValue, right.GetDoubleValue());\
		}\
		else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)\
		{\
			if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild() && \
				right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())\
			{\
				double dValue = (left.m_aryValue[0]._dValue __op__ right.m_aryValue[0]._dValue) ? 1 : 0;\
				pResult->SetDoubleValue(dValue);\
			}\
		}\
		return pResult;\
	}\
	void VariantOperator::__funcname__(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)\
	{\
		size_t nLeftCount = left.size(), nRightCount = right.size();\
		size_t nCount = std::max(nLeftCount, nRightCount);\
		for (size_t i = 0; i < nCount; ++i)\
		{\
			VARIANT_ITEM item;\
			if (i<nLeftCount || i<nRightCount)\
			{\
				const auto& leftItem = left[i];\
				const auto& rightItem = right[i];\
				if (leftItem._sType == VARIANT_ITEM::VAILD_ID && rightItem._sType == VARIANT_ITEM::VAILD_ID)\
				{\
					item.SetValue((leftItem._dValue __op__ rightItem._dValue) ? 1 : 0);\
				}\
			}\
			dest.push_back(item);\
		}\
	}\
	void VariantOperator::__funcname__(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)\
	{\
		for (auto rightItem : right)\
		{\
			VARIANT_ITEM item;\
			if (rightItem._sType == VARIANT_ITEM::VAILD_ID)\
			{\
				item.SetValue((dLeft __op__ rightItem._dValue) ? 1 : 0);\
			}\
			dest.push_back(item);\
		}\
	}\
	void VariantOperator::__funcname__(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight)\
	{\
		for (auto leftItem : left)\
		{\
			VARIANT_ITEM item;\
			if (leftItem._sType == VARIANT_ITEM::VAILD_ID)\
			{\
				item.SetValue((leftItem._dValue __op__ dRight) ? 1 : 0);\
			}\
			dest.push_back(item);\
		}\
	}
#endif
//内置函数判断
#define SWITCH_CALL_FUNCTIONS(__funcname__,__argcount__)\
	(strFuncName == __funcname__ && CheckFunctinArgument(__funcname__, args, __argcount__, pNode))


Variant::Variant()
{

}

Variant::~Variant()
{
	DELETE_PTR(m_pMapVariant);
	DELETE_PTR(m_pDrawData);
}

void Variant::Clear()
{
	m_nType = NULL_TYPE;
	m_aryValue.clear();
	m_strValue.clear();
	if (m_pMapVariant) m_pMapVariant->clear();
	if (m_pDrawData) m_pDrawData->_mapVaraint.clear();
}

void Variant::SetDoubleValue(double dValue)
{
	m_nType = DOUBLE_TYPE;

	if (m_aryValue.size() == 1)
	{
		m_aryValue[0]._dValue = dValue;
		m_aryValue[0]._sType = VARIANT_ITEM::VAILD_ID;
	}
	else
	{
		m_aryValue.clear();
		VARIANT_ITEM item;
		item.SetValue(dValue);
		m_aryValue.push_back(item);
	}
}

bool Variant::IsVaildDoulbe() const
{
	if (m_nType != DOUBLE_TYPE) return false;
	if (m_aryValue.size() != 1) return false;
	if (!m_aryValue[0].IsVaild()) return false;

	return true;
}

double Variant::GetDoubleValue() const
{
	return m_aryValue[0]._dValue;
}

bool Variant::TryGetLongValue(long& lValue, long lDefault) const
{
	lValue = lDefault;

	if (!IsVaildDoulbe()) return false;

	lValue = (long)GetDoubleValue();
	return true;
}

bool Variant::TryGetDobuleValue(double& dValue, double dDefault) const
{
	dValue = dDefault;

	if (!IsVaildDoulbe()) return false;

	dValue = GetDoubleValue();
	return true;
}

bool Variant::TryGetStringValue(std::wstring& strValue, const std::wstring& strDefault) const
{
	strValue = strDefault;
	if (!IsVaildString()) return false;

	strValue = GetStringValue();
	return true;
}


void Variant::SetStringValue(const std::wstring& strValue)
{
	m_nType = STRING_TYPE;
	m_strValue = strValue;
}

const std::wstring& Variant::GetStringValue() const
{
	return m_strValue;
}

bool Variant::IsVaildString() const
{
	return m_nType == STRING_TYPE;
}

void Variant::SetMemberValue(const std::wstring& strName, Variant* pVariant)
{
	if (m_pMapVariant == NULL) m_pMapVariant = new MAP_VARIANT;

	(*m_pMapVariant)[strName] = pVariant;

	m_nType = MEMBER_TYPE;
}

Variant* Variant::GetMemberValue(const std::wstring& strName) const
{
	if (!m_pMapVariant) return NULL;
	auto find = m_pMapVariant->find(strName);
	if (find == m_pMapVariant->end()) return NULL;

	return find->second;
}

void Variant::ResetDrawData()
{
	if (m_pDrawData == NULL)
	{
		m_pDrawData = new DrawData;
	}
	else
	{
		m_pDrawData->_mapVaraint.clear();
	}
}

void Variant::SetDrawKLineData(Variant* pVarHigh, Variant* pVarOpen, Variant* pVarLow, Variant* pVarClose)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWKLINE";
	m_nType = DRAWKLINE_TYPE;
	m_pDrawData->_mapVaraint[L"High"]=pVarHigh;
	m_pDrawData->_mapVaraint[L"Open"]=pVarOpen;
	m_pDrawData->_mapVaraint[L"Low"]=pVarLow;
	m_pDrawData->_mapVaraint[L"Close"]=pVarClose;
}

void Variant::SetDrawColorKLineData(Variant* pVarHigh, Variant* pVarOpen, Variant* pVarLow, Variant* pVarClose, Variant* pColor, Variant* pEmpty)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWCOLORKLINE";
	m_nType = DRAWCOLORKLINE_TYPE;
	m_pDrawData->_mapVaraint[L"High"] = pVarHigh;
	m_pDrawData->_mapVaraint[L"Open"] = pVarOpen;
	m_pDrawData->_mapVaraint[L"Low"] = pVarLow;
	m_pDrawData->_mapVaraint[L"Close"] = pVarClose;
	m_pDrawData->_mapVaraint[L"Color"] = pColor;
	m_pDrawData->_mapVaraint[L"Empty"] = pEmpty;
}

/*
绘制柱线.
用法: STICKLINE(COND,PRICE1,PRICE2,WIDTH,EMPTY),当COND条件满足时,在PRICE1和PRICE2位置之间画柱状线,宽度为WIDTH(10为标准间距),EMPTY为0画实心柱,-1画虚线空心柱,否则画实线空心柱.
例如: STICKLINE(CLOSE>OPEN,CLOSE,OPEN,0.8,1)表示画K线中阳线的空心柱体部分.
*/
void Variant::SetDrawStickLineData(Variant* pVarPrice, Variant* pVarPrice2, Variant* pVarWidth, Variant* pVarDrawType)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"STICKLINE";
	m_nType = STICKLINE_TYPE;
	m_pDrawData->_mapVaraint[L"Price"]=pVarPrice;
	m_pDrawData->_mapVaraint[L"Price2"]=pVarPrice2;
	m_pDrawData->_mapVaraint[L"Width"]=pVarWidth;
	m_pDrawData->_mapVaraint[L"Empty"]=pVarDrawType;
}

void Variant::SetDrawIconData(Variant* pVarPrice, Variant* pVarIcon, long lType)
{
	ResetDrawData();

	if (lType == DRAWICON_TYPE)
	{
		m_pDrawData->_strDrawName = L"DRAWICON";
	}
	else if (lType == DRAWICON2_TYPE)
	{
		m_pDrawData->_strDrawName = L"ICON";
	}
	
	m_nType = DRAWICON_TYPE;
	m_pDrawData->_mapVaraint[L"Price"]=pVarPrice;
	m_pDrawData->_mapVaraint[L"Type"]=pVarIcon;
}

void Variant::SetDrawTextData(Variant* pVarPrice, Variant* pVarText)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWTEXT";
	m_nType = DRAWTEXT_TYPE;
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Text"]=pVarText;
}

void Variant::SetDrawTextFixData(Variant* pCond,Variant* pX, Variant* pY, Variant* pType, Variant* pVarText)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWTEXT_FIX";
	m_nType = DRAWTEXT_FIX_TYPE;
	m_pDrawData->_mapVaraint[L"Cond"] = pCond;
	m_pDrawData->_mapVaraint[L"X"] = pX;
	m_pDrawData->_mapVaraint[L"Y"] = pY;
	m_pDrawData->_mapVaraint[L"Type"] = pType;
	m_pDrawData->_mapVaraint[L"Text"] = pVarText;
}

void Variant::SetDrawNumberFixData(Variant* pX, Variant* pY, Variant* pType, Variant* pNumber)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWNUMBER_FIX_TYPE";
	m_nType = DRAWNUMBER_FIX_TYPE;
	m_pDrawData->_mapVaraint[L"X"] = pX;
	m_pDrawData->_mapVaraint[L"Y"] = pY;
	m_pDrawData->_mapVaraint[L"Type"] = pType;
	m_pDrawData->_mapVaraint[L"Number"] = pNumber;
}

void Variant::SetDrawNumberData(Variant* pVarPrice, Variant* pVarNumber)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWNUMBER";
	m_nType = DRAWNUMBER_TYPE;
	m_pDrawData->_mapVaraint[L"Price"]=pVarPrice;
	m_pDrawData->_mapVaraint[L"Number"]=pVarNumber;
}

void Variant::SetDrawBandData(Variant* pVarPrice, Variant* pVarColor, Variant* pVarPrice2, Variant* pVarColor2)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWBAND";
	m_nType = DRAWBAND_TYPE;
	m_pDrawData->_mapVaraint[L"Val1"]=pVarPrice;
	m_pDrawData->_mapVaraint[L"Color1"]=pVarColor;
	m_pDrawData->_mapVaraint[L"Val2"]=pVarPrice2;
	m_pDrawData->_mapVaraint[L"Color2"]=pVarColor2;
}

void Variant::SetFillRNGData(Variant* pVarPrice, Variant* pVarPrice2, Variant* pColor)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"FILLRGN";
	m_nType = FILLRGN_TYPE;
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Price2"] = pVarPrice2;
	m_pDrawData->_mapVaraint[L"Color"] = pColor;
}

void Variant::SetPartLineData(Variant* pVarPrice, Variant* pColor)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"PARTLINE";
	m_nType = PARTLINE_TYPE;
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Color"] = pColor;
}

void Variant::SetExplain(Variant* pVarCond, Variant* pText)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"EXPLAIN";
	m_nType = EXPLAIN_TYPE;
	m_pDrawData->_mapVaraint[L"Cond"] = pVarCond;
	m_pDrawData->_mapVaraint[L"Text"] = pText;
	
}

void Variant::CopyTo(Variant& dest, VariantOperator* pVariantOperator) const
{
	dest.m_nType = m_nType;

	if (m_nType == ARRAY_DOUBLE_TYPE || m_nType == DOUBLE_TYPE)
	{
		dest.m_aryValue = m_aryValue;
	}
	else if (m_nType == STRING_TYPE)
	{
		dest.m_strValue = m_strValue;
	}
	else if (m_nType == ARRAY_STRING_TYPE)
	{
		for (long i = 0; i < (long)m_aryValue.size(); ++i)
		{
			const auto& item = m_aryValue[i];
			VARIANT_ITEM copyItem = VARIANT_ITEM();
			if (item.IsVaild())
			{
				copyItem.SetValue(pVariantOperator->CreateString(*item._pString));
			}

			dest.m_aryValue.push_back(item);
		}
	}
	else
	{	//TODO: 暂时就支持这几个类型复制，其他先不支持吧
		dest.m_nType = NULL_TYPE;
	}
}

void Variant::CopyTo(Variant& dest, const ARRAY_FIT_DATETIME_DATA& aryDateTime, VariantOperator* pVariantOperator) const
{
	dest.m_nType = m_nType;
	if (m_nType == DOUBLE_TYPE)
	{
		dest.m_aryValue = m_aryValue;
	}
	else if (m_nType == STRING_TYPE)
	{
		dest.m_strValue = m_strValue;
	}
	else if (m_nType == ARRAY_DOUBLE_TYPE)
	{
		long lCount = (long)aryDateTime.size();
		long lDataCount = (long)m_aryValue.size();
		dest.m_aryValue.resize(lCount, VARIANT_ITEM());
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = aryDateTime[i];
			if (item._lIndex < 0 || item._lIndex >= lDataCount) continue;
			const auto& dataItem = m_aryValue[item._lIndex];
			if (!dataItem.IsVaild()) continue;
			dest.m_aryValue[i].SetValue(dataItem._dValue);
		}
	}
	else if (m_nType == ARRAY_STRING_TYPE)
	{
		long lCount = (long)aryDateTime.size();
		long lDataCount = (long)m_aryValue.size();
		dest.m_aryValue.resize(lCount, VARIANT_ITEM());
		for (long i = 0; i < lCount; ++i)
		{
			const auto& item = aryDateTime[i];
			if (item._lIndex < 0 || item._lIndex >= lDataCount) continue;
			const auto& dataItem = m_aryValue[item._lIndex];
			if (!dataItem.IsVaild()) continue;
			dest.m_aryValue[i].SetValue(pVariantOperator->CreateString(*dataItem._pString));
		}
	}

	
}


///////////////////////////////////////////////////////////////////////////////////////////////
VariantOperator::VariantOperator()
{

}


VariantOperator::~VariantOperator()
{
	for (auto item : m_VariantCache)
	{
		DELETE_PTR(item);
	}

	m_VariantCache.clear();
	m_VariantUsedCache.clear();
	m_VariantFreeCache.clear();

	for (auto item : m_StringCache)
	{
		DELETE_PTR(item);
	}

	m_StringCache.clear();
}

Variant* VariantOperator::Create()
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

Variant* VariantOperator::Create(double dValue)
{
	Variant* pResult = Create();
	pResult->SetDoubleValue(dValue);
	return pResult;
}

Variant* VariantOperator::Clone(const Variant* pVar)
{
	Variant* pResult = Create();
	if (pVar)
	{
		pVar->CopyTo(*pResult, this);	
	}

	return pResult;
}

Variant* VariantOperator::Clone(const Variant* pVar, const ARRAY_FIT_DATETIME_DATA& aryDateTime)
{
	Variant* pResult = Create();
	if (pVar)
	{
		pVar->CopyTo(*pResult, aryDateTime, this);
	}

	return pResult;
}

std::wstring* VariantOperator::CreateString(const std::wstring& strValue)
{
	std::wstring* pStr = new std::wstring(strValue);
	m_StringCache.push_back(pStr);
	return pStr;
}

std::wstring* VariantOperator::CreateString()
{
	std::wstring* pStr = new std::wstring();
	m_StringCache.push_back(pStr);
	return pStr;
}


void VariantOperator::ClearCache()
{
	for (auto item : m_VariantUsedCache)
	{
		m_VariantFreeCache.push_back(item);
	}

	m_VariantUsedCache.clear();

	for (auto item : m_StringCache)
	{
		DELETE_PTR(item);
	}

	m_mapIndex.clear();
	m_mapCALCSTOCKINDEX.clear();
	m_mapSTKINDI.clear();
	m_StringCache.clear();
}


Variant* VariantOperator::Subtract(const Variant& left, const Variant& right)
{
	Variant* pResult = Create();
	if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		Subtract(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())
			Subtract(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
			Subtract(pResult->m_aryValue, left.m_aryValue, right.m_aryValue[0]._dValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild() &&
			right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
		{
			double dValue = left.m_aryValue[0]._dValue - right.m_aryValue[0]._dValue;
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::Subtract(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)
{
	size_t nLeftCount = left.size(), nRightCount = right.size();
	size_t nCount = std::max(nLeftCount, nRightCount);
	for (size_t i = 0; i < nCount; ++i)
	{
		VARIANT_ITEM item;
		if (i<nLeftCount || i<nRightCount)
		{
			const auto& leftItem = left[i];
			const auto& rightItem = right[i];
			if (leftItem._sType == VARIANT_ITEM::VAILD_ID && rightItem._sType == VARIANT_ITEM::VAILD_ID)
			{
				item.SetValue(leftItem._dValue - rightItem._dValue);
			}
		}

		dest.push_back(item);
	}
}

void VariantOperator::Subtract(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)
{
	for (auto rightItem : right)
	{
		VARIANT_ITEM item;
		if (rightItem._sType == VARIANT_ITEM::VAILD_ID)
		{
			item.SetValue(dLeft - rightItem._dValue);
		}

		dest.push_back(item);
	}
}

void VariantOperator::Subtract(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight)
{
	for (const auto& leftItem : left)
	{
		VARIANT_ITEM item;
		if (leftItem._sType == VARIANT_ITEM::VAILD_ID)
		{
			item.SetValue(leftItem._dValue - dRight);
		}

		dest.push_back(item);
	}
}

Variant* VariantOperator::Multiply(const Variant& left, const Variant& right)
{
	Variant* pResult = Create();
	if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		Multiply(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())
			Multiply(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
			Multiply(pResult->m_aryValue, right.m_aryValue[0]._dValue, left.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild() &&
			right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
		{
			double dValue = left.m_aryValue[0]._dValue * right.m_aryValue[0]._dValue;
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::Multiply(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)
{
	size_t nLeftCount = left.size(), nRightCount = right.size();
	size_t nCount = std::max(nLeftCount, nRightCount);
	for (size_t i = 0; i < nCount; ++i)
	{
		VARIANT_ITEM item;
		if (i<nLeftCount || i<nRightCount)
		{
			const auto& leftItem = left[i];
			const auto& rightItem = right[i];
			if (leftItem.IsVaild() && rightItem.IsVaild())
			{
				item.SetValue(leftItem._dValue * rightItem._dValue);
			}
		}

		dest.push_back(item);
	}
}

void VariantOperator::Multiply(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)
{
	for (auto rightItem : right)
	{
		VARIANT_ITEM item;
		if (rightItem._sType == VARIANT_ITEM::VAILD_ID)
		{
			item.SetValue(dLeft * rightItem._dValue);
		}

		dest.push_back(item);
	}
}

Variant* VariantOperator::Divide(const Variant& left, const Variant& right)
{
	Variant* pResult = Create();
	if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		if (!left.m_aryValue.empty() && !right.m_aryValue.empty())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			Divide(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);
		}
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		
		if (left.IsVaildDoulbe() && !right.m_aryValue.empty())
		{
			pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
			Divide(pResult->m_aryValue, left.GetDoubleValue(), right.m_aryValue);
		}
	}
	else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (right.IsVaildDoulbe() && !left.m_aryValue.empty())
			Divide(pResult->m_aryValue, left.m_aryValue, right.GetDoubleValue());
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (left.IsVaildDoulbe() && right.IsVaildDoulbe() && right.GetDoubleValue()!=0)
		{
			double dValue = left.GetDoubleValue() / right.GetDoubleValue();
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::Divide(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)
{
	size_t nLeftCount = left.size(), nRightCount = right.size();
	size_t nCount = std::max(nLeftCount, nRightCount);
	for (size_t i = 0; i < nCount; ++i)
	{
		VARIANT_ITEM item;
		if (i<nLeftCount || i<nRightCount)
		{
			const auto& leftItem = left[i];
			const auto& rightItem = right[i];
			if (leftItem.IsVaild() && rightItem.IsVaild() && rightItem._dValue!=0)
			{
				item.SetValue(leftItem._dValue / rightItem._dValue);
			}
		}

		dest.push_back(item);
	}
}

void VariantOperator::Divide(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)
{
	for (auto rightItem : right)
	{
		VARIANT_ITEM item;
		if (rightItem.IsVaild() && rightItem._dValue!=0)
		{
			item.SetValue(dLeft / rightItem._dValue);
		}

		dest.push_back(item);
	}
}

void VariantOperator::Divide(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight)
{
	for (auto leftItem : left)
	{
		VARIANT_ITEM item;
		if (leftItem.IsVaild() && dRight!=0)
		{
			item.SetValue(leftItem._dValue / dRight);
		}

		dest.push_back(item);
	}
}

Variant* VariantOperator::Add(const Variant& left, const Variant& right)
{
	Variant* pResult = Create();
	if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		Add(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())
			Add(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
			Add(pResult->m_aryValue, right.m_aryValue[0]._dValue, left.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild() &&
			right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
		{
			double dValue = left.m_aryValue[0]._dValue + right.m_aryValue[0]._dValue;
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::Add(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)
{
	size_t nLeftCount = left.size(), nRightCount = right.size();
	size_t nCount = std::max(nLeftCount, nRightCount);
	for (size_t i = 0; i < nCount; ++i)
	{
		VARIANT_ITEM item;
		if (i<nLeftCount || i<nRightCount)
		{
			const auto& leftItem = left[i];
			const auto& rightItem = right[i];
			if (leftItem.IsVaild() && rightItem.IsVaild())
			{
				item.SetValue(leftItem._dValue + rightItem._dValue);
			}
		}

		dest.push_back(item);
	}
}

void VariantOperator::Add(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)
{
	for (auto rightItem : right)
	{
		VARIANT_ITEM item;
		if (rightItem.IsVaild())
		{
			item.SetValue(dLeft + rightItem._dValue);
		}

		dest.push_back(item);
	}
}

/*  已使用宏VARIANT_COMPARE_FUNCTIONS替代 调试才用
Variant* VariantOperator::GTE(const Variant& left, const Variant& right)
{
	Variant* pResult = Create();
	if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		GTE(pResult->m_aryValue, left.m_aryValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::ARRAY_DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild())
			GTE(pResult->m_aryValue, left.m_aryValue[0]._dValue, right.m_aryValue);
	}
	else if (left.m_nType == Variant::ARRAY_DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		pResult->m_nType = Variant::ARRAY_DOUBLE_TYPE;
		if (right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
			GTE(pResult->m_aryValue, left.m_aryValue,right.m_aryValue[0]._dValue);
	}
	else if (left.m_nType == Variant::DOUBLE_TYPE && right.m_nType == Variant::DOUBLE_TYPE)
	{
		if (left.m_aryValue.size() == 1 && left.m_aryValue[0].IsVaild() &&
			right.m_aryValue.size() == 1 && right.m_aryValue[0].IsVaild())
		{
			double dValue = left.m_aryValue[0]._dValue >= right.m_aryValue[0]._dValue ? 1 : 0;
			pResult->SetDoubleValue(dValue);
		}
	}

	return pResult;
}

void VariantOperator::GTE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right)
{
	size_t nLeftCount = left.size(), nRightCount = right.size();
	size_t nCount = max(nLeftCount, nRightCount);
	for (size_t i = 0; i < nCount; ++i)
	{
		VARIANT_ITEM item;
		if (i<nLeftCount || i<nRightCount)
		{
			const auto& leftItem = left[i];
			const auto& rightItem = right[i];
			if (leftItem.IsVaild() && rightItem.IsVaild())
			{
				item.SetValue(leftItem._dValue >= rightItem._dValue ? 1:0);
			}
		}

		dest.push_back(item);
	}
}

void VariantOperator::GTE(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right)
{
	for (auto rightItem : right)
	{
		VARIANT_ITEM item;
		if (rightItem.IsVaild())
		{
			item.SetValue(dLeft >= rightItem._dValue ? 1 : 0);
		}

		dest.push_back(item);
	}
}

void VariantOperator::GTE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight)
{
	for (auto leftItem : left)
	{
		VARIANT_ITEM item;
		if (leftItem.IsVaild())
		{
			item.SetValue(leftItem._dValue >= dRight ? 1 : 0);
		}

		dest.push_back(item);
	}
}
*/

VARIANT_COMPARE_FUNCTIONS(GTE, >=);
VARIANT_COMPARE_FUNCTIONS(LT, <);
VARIANT_COMPARE_FUNCTIONS(LTE, <=);
VARIANT_COMPARE_FUNCTIONS(EQ, ==);
VARIANT_COMPARE_FUNCTIONS(NEQ, !=);
VARIANT_COMPARE_FUNCTIONS(And, && );
VARIANT_COMPARE_FUNCTIONS(Or, ||);


const std::set<std::wstring> DRAW_FUNCTION_NAME(
	{ 
		L"DRAWKLINE",L"STICKLINE",L"DRAWICON" ,L"DRAWTEXT",
		L"DRAWNUMBER", L"DRAWBAND",L"PLOYLINE", L"DRAWLINE",
		L"DRAWTEXT_FIX",L"DRAWNUMBER_FIX",L"FILLRGN",L"PARTLINE",
		L"RGB",L"ARGB",L"RGBA",L"EXPLAIN",L"DRAWCOLORKLINE", L"ICON",L"SOUND"
	});

Variant* VariantOperator::CallDrawFunction(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	//绘图
	if (SWITCH_CALL_FUNCTIONS(L"DRAWKLINE", 4)) return DRAWKLINE(*args[0], *args[1], *args[2], *args[3]);
	else if (SWITCH_CALL_FUNCTIONS(L"STICKLINE", 5)) return STICKLINE(*args[0], *args[1], *args[2], *args[3], *args[4]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWICON", 3)) return DRAWICON(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWTEXT", 3)) return DRAWTEXT(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWNUMBER", 3)) return DRAWNUMBER(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWBAND", 4)) return DRAWBAND(*args[0], *args[1], *args[2], *args[3]);
	else if (SWITCH_CALL_FUNCTIONS(L"PLOYLINE", 2)) return PLOYLINE(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWLINE", 5)) return DRAWLINE(*args[0], *args[1], *args[2], *args[3], *args[4]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWTEXT_FIX", 5)) return DRAWTEXT_FIX(*args[0], *args[1], *args[2], *args[3], *args[4]);
	else if (SWITCH_CALL_FUNCTIONS(L"DRAWNUMBER_FIX", 5)) return DRAWNUMBER_FIX(*args[0], *args[1], *args[2], *args[3], *args[4]);


	else if (SWITCH_CALL_FUNCTIONS(L"RGB", 3)) return COLOR_RGB(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"ARGB", 4)) return COLOR_ARGB(*args[0], *args[1], *args[2], *args[3]);
	else if (SWITCH_CALL_FUNCTIONS(L"RGBA", 4)) return COLOR_RGBA(*args[0], *args[1], *args[2], *args[3]);

	else if (strFuncName == L"FILLRGN" && args.size() >= 4) return FILLRGN(args, pHistoryData, pNode);
	else if (strFuncName == L"PARTLINE" && args.size() >= 3) return PARTLINE(args, pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"EXPLAIN", 2)) return EXPLAIN(*args[0], *args[1]);

	else if (SWITCH_CALL_FUNCTIONS(L"DRAWCOLORKLINE", 3)) return DRAWCOLORKLINE(*args[0], *args[1], *args[2], pHistoryData);
	else if (SWITCH_CALL_FUNCTIONS(L"ICON", 2)) return ICON(*args[0], *args[1], pHistoryData);

	return NULL;
}

const std::set<std::wstring> CALL_FUNCTION2_NAME(
	{
		L"TOPRANGE",L"LOWRANGE",
		L"BARSNEXT" ,
		L"LOD",L"HOD", 
		L"FINDLOW",L"FINDHIGH", 
		L"FINDHIGHBARS",L"FINDLOWBARS",
		L"AMA", L"TMA",L"RAND",L"ROUND", L"ROUND2",L"TRMA",L"HARMEAN",L"VALUEWHEN",
		L"DATETODAY",L"DAYTODATE",L"BARSSINCEN",
		L"LV",L"HV",
		L"CROSSDOWN",L"CROSSUP",
	});

Variant* VariantOperator::CallFunction2(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	if (SWITCH_CALL_FUNCTIONS(L"TOPRANGE", 1)) return TOPRANGE(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"LOWRANGE", 1)) return LOWRANGE(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"BARSNEXT", 1)) return BARSNEXT(*args[0]);

	else if (SWITCH_CALL_FUNCTIONS(L"LOD", 2)) return LOD(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"HOD", 2)) return HOD(*args[0], *args[1]);

	else if (SWITCH_CALL_FUNCTIONS(L"FINDLOW", 4)) return FINDLOW(*args[0], *args[1], *args[2], *args[3]);
	else if (SWITCH_CALL_FUNCTIONS(L"FINDHIGH", 4)) return FINDHIGH(*args[0], *args[1], *args[2], *args[3]);
	else if (SWITCH_CALL_FUNCTIONS(L"FINDHIGHBARS", 4)) return FINDHIGHBARS(*args[0], *args[1], *args[2], *args[3]);
	else if (SWITCH_CALL_FUNCTIONS(L"FINDLOWBARS", 4)) return FINDLOWBARS(*args[0], *args[1], *args[2], *args[3]);

	else if (SWITCH_CALL_FUNCTIONS(L"AMA", 2)) return AMA(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"TMA", 3)) return TMA(*args[0], *args[1], *args[2], pHistoryData);
	else if (SWITCH_CALL_FUNCTIONS(L"RAND", 1)) return RAND(*args[0], pHistoryData);
	else if (SWITCH_CALL_FUNCTIONS(L"ROUND", 1)) return ROUND(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"ROUND2", 2)) return ROUND2(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"TRMA", 2)) return TRMA(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"HARMEAN", 2)) return HARMEAN(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"VALUEWHEN", 2)) return VALUEWHEN(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"DATETODAY", 1)) return DATETODAY(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"DAYTODATE", 1)) return DAYTODATE(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"BARSSINCEN",2)) return BARSSINCEN(*args[0], *args[1]);

	else if (SWITCH_CALL_FUNCTIONS(L"LV", 2)) return LV(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"HV",2)) return HV(*args[0], *args[1]);

	/*
	   CROSSDOWN(A,B)：表示当A从上方向下穿B，成立返回1(Yes)，否则返回0(No)

	   注：1、CROSSDOWN(A,B)等同于CROSS(B,A)，CROSSDOWN(A,B)编写更利于理解

	   例1：
	   MA5:=MA(C,5);
	   MA10:=MA(C,10);
	   CROSSDOWN(MA5,MA10)//MA5下穿MA10
   */
	else if (SWITCH_CALL_FUNCTIONS(L"CROSSDOWN",2)) return CROSS(*args[1], *args[0] );

	/*
		CROSSUP(A,B) 表当A从下方向上穿过B，成立返回1(Yes)，否则返回0(No)

		注：
		1、CROSSUP(A,B)等同于CROSS(A,B)，CROSSUP(A,B)编写更利于理解。

		例1：
		MA5:=MA(C,5);
		MA10:=MA(C,10);
		CROSSUP(MA5,MA10),ICON(1,'ICO4');//MA5上穿MA10，标注红箭头。
	*/
	else if (SWITCH_CALL_FUNCTIONS(L"CROSSUP",2)) return CROSS(*args[0], *args[1]);
	
	
	return NULL;
}

Variant* VariantOperator::CallFunction(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	if (SWITCH_CALL_FUNCTIONS(L"MA", 2)) return MA(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"EMA", 2)) return EMA(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"SMA", 3)) return SMA(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"DMA", 2)) return DMA(*args[0], *args[1]);
	else if (strFuncName == L"WMA") return WMA(*args[0], *args[1]);
	else if (strFuncName == L"EXPMA") return EXPMA(*args[0], *args[1]);
	else if (strFuncName == L"EXPMEMA") return EXPMEMA(*args[0], *args[1]);
	else if (strFuncName == L"SUMBARS") return SUMBARS(*args[0], *args[1]);
	else if (strFuncName == L"MEMA") return MEMA(*args[0], *args[1]);
	else if (strFuncName == L"LLV") return LLV(*args[0], *args[1]);
	else if (strFuncName == L"HHV") return HHV(*args[0], *args[1]);
	else if (strFuncName == L"LLVBARS") return LLVBARS(*args[0], *args[1]);
	else if (strFuncName == L"HHVBARS") return HHVBARS(*args[0], *args[1]);
	else if (strFuncName == L"LONGCROSS") return LONGCROSS(*args[0], *args[1], *args[2]);
	else if (strFuncName == L"REVERSE") return REVERSE(*args[0]);
	else if (strFuncName == L"COUNT") return COUNT(*args[0], *args[1]);
	else if (strFuncName == L"CROSS") return CROSS(*args[0], *args[1]);
	else if (strFuncName == L"SUM") return SUM(*args[0], *args[1]);
	else if (strFuncName == L"MULAR") return MULAR(*args[0], *args[1]);
	else if (strFuncName == L"BARSCOUNT") return BARSCOUNT(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"BARSLASTCOUNT", 1)) return BARSLASTCOUNT(*args[0]);

	else if (SWITCH_CALL_FUNCTIONS(L"AVEDEV",2)) return AVEDEV(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STD",2)) return STD(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"DEVSQ", 2)) return DEVSQ(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"FORCAST", 2)) return FORCAST(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"SLOPE", 2)) return SLOPE(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STDP", 2)) return STDP(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"VAR", 2)) return VAR(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"VARP", 2)) return VARP(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"COVAR", 3)) return COVAR(*args[0], *args[1], *args[2]);

	
	else if (SWITCH_CALL_FUNCTIONS(L"NOT",1)) return NOT(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"IFF", 3) || SWITCH_CALL_FUNCTIONS(L"IF",3) || SWITCH_CALL_FUNCTIONS(L"IFELSE", 3)) return IFF(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"IFN",3)) return IFN(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"IFS", 3)) return IFS(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"BACKSET",2)) return BACKSET(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"BARSLAST",1)) return BARSLAST(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"BARSSINCE",1)) return BARSSINCE(*args[0]);

	else if (SWITCH_CALL_FUNCTIONS(L"REF", 2)) return REF(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"REFV", 2)) return REFV(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"REFX", 2) || SWITCH_CALL_FUNCTIONS(L"REFX1", 2)) return REFX(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"REFXV", 2)) return REFXV(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"REFDATE", 2)) return REFDATE(*args[0], *args[1], pHistoryData, pNode);

	else if (SWITCH_CALL_FUNCTIONS(L"MAX",2)) return MAX(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"MIN",2)) return MIN(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"RANGE", 3)) return RANGE(*args[0], *args[1], *args[2]);
	

	else if (SWITCH_CALL_FUNCTIONS(L"ACOS",1)) return ACOS(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"ASIN",1)) return ASIN(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"ATAN",1)) return ATAN(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"COS",1)) return COS(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"SIN",1)) return SIN(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"TAN",1)) return TAN(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"EXP",1)) return EXP(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"LN",1)) return LN(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"LOG",1)) return LOG(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"ABS",1)) return ABS(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"SQRT",1)) return SQRT(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"POW", 2)) return POW(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"CEILING", 1)) return CEILING(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"FLOOR", 1)) return FLOOR(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"INTPART", 1)) return INTPART(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"SGN", 1)) return SGN(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"FRACPART", 1)) return FRACPART(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"MOD", 2)) return MOD(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"ALL", 2)) return ALL(*args[0], *args[1], pHistoryData);
	else if (SWITCH_CALL_FUNCTIONS(L"ANY", 2)) return ANY(*args[0], *args[1], pHistoryData);
	else if (SWITCH_CALL_FUNCTIONS(L"DTPRICE", 2)) return DTPRICE(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"ZTPRICE", 2)) return ZTPRICE(*args[0], *args[1]);
	
	else if (SWITCH_CALL_FUNCTIONS(L"WINNER", 1)) return WINNER(*args[0], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"WINNERB", 1)) return WINNERB(*args[0], pHistoryData, pNode);

	else if (SWITCH_CALL_FUNCTIONS(L"LWINNER", 2)) return LWINNER(*args[0], *args[1], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"LWINNERB", 2)) return LWINNERB(*args[0], *args[1], pHistoryData, pNode);

	else if (SWITCH_CALL_FUNCTIONS(L"PWINNER", 2)) return PWINNER(*args[0], *args[1], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"PWINNERB", 2)) return PWINNERB(*args[0], *args[1], pHistoryData, pNode);

	else if (SWITCH_CALL_FUNCTIONS(L"COST", 1)) return COST(*args[0], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"COSTB", 1)) return COST(*args[0], pHistoryData, pNode);				//没有实现
	else if (SWITCH_CALL_FUNCTIONS(L"PPART", 1)) return PPART(*args[0], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"COSTEX", 2)) return COSTEX(*args[0], *args[1], pHistoryData, pNode);

	else if (SWITCH_CALL_FUNCTIONS(L"ZIG",2)) return ZIG(*args[0], *args[1], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"PEAK",3)) return PEAK(*args[0], *args[1], *args[2], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"PEAKBARS", 3)) return PEAKBARS(*args[0], *args[1], *args[2], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"SAR", 3)) return SAR(*args[0], *args[1], *args[2], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"SARTURN", 3)) return SARTURN(*args[0], *args[1], *args[2], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"TROUGHBARS", 3)) return TROUGHBARS(*args[0], *args[1], *args[2], pHistoryData, pNode);
	else if (SWITCH_CALL_FUNCTIONS(L"TROUGH", 3)) return TROUGH(*args[0], *args[1], *args[2], pHistoryData, pNode);
	

	else if (SWITCH_CALL_FUNCTIONS(L"EVERY", 2)) return EVERY(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"BETWEEN", 3)) return BETWEEN(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"NDAY", 3)) return NDAY(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"UPNDAY", 2)) return UPNDAY(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"DOWNNDAY", 2)) return DOWNNDAY(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"EXIST", 2)) return EXIST(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"LAST", 3)) return LAST(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"EXISTR", 3)) return EXISTR(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"FILTER", 2)) return FILTER(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"TFILTER", 3)) return TFILTER(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"CONST", 1)) return CONST_(*args[0]);

	else if (SWITCH_CALL_FUNCTIONS(L"NUMTOSTR", 1)) return NUMTOSTR(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"STR", 1)) return NUMTOSTR(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"NUMTOSTRN", 2)) return NUMTOSTRN(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRCMP", 2)) return STRCMP(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRTONUM", 1)) return STRTONUM(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"INBLOCK", 1)) return INBLOCK(*args[0], pHistoryData, pNode);

	else if (SWITCH_CALL_FUNCTIONS(L"STRFIND", 3)) return STRFIND(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRLEFT", 2)) return STRLEFT(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRRIGHT", 2)) return STRRIGHT(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRMID", 3)) return STRMID(*args[0], *args[1], *args[2]);
	else if (SWITCH_CALL_FUNCTIONS(L"DATESTR", 1)) return DATESTR(*args[0]);
	else if (SWITCH_CALL_FUNCTIONS(L"CON2STR", 2)) return CON2STR(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"VAR2STR", 2)) return VAR2STR(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRCAT", 2)) return STRCAT(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"VARCAT", 2)) return VARCAT(*args[0], *args[1]);
	else if (SWITCH_CALL_FUNCTIONS(L"STRSPACE", 1)) return STRSPACE(*args[0]);

	//指标调用
	else if (SWITCH_CALL_FUNCTIONS(L"CALCSTOCKINDEX", 3)) return CALCSTOCKINDEX(*args[0], *args[1], *args[2], pHistoryData, pNode);
	else if (strFuncName == L"STKINDI") return STKINDI(args, pHistoryData, pNode);

	else if (strFuncName == L"CALL") return CallScript(args, pHistoryData, pNode);	//自定义脚本调用
	else if (strFuncName == L"CURRENTTIME") return GetCurrentTime();
	else if (strFuncName == L"CURRENTDATE") return GetCurrentDate();

	//内置函数2
	else if (CALL_FUNCTION2_NAME.find(strFuncName)!= CALL_FUNCTION2_NAME.end())
		return CallFunction2(strFuncName, args, pHistoryData, pNode);
	
	//绘图函数
	else if (DRAW_FUNCTION_NAME.find(strFuncName) != DRAW_FUNCTION_NAME.end())
		return CallDrawFunction(strFuncName, args, pHistoryData, pNode);

	std::wstringstream strDescription;
	strDescription << L"函数'" << strFuncName << L"'不存在.";
	ExecuteExcept error(strDescription.str(), L"VariantOperator::CallFunction", pNode);
	throw error;

	return NULL;
}


bool VariantOperator::CheckFunctinArgument(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, long lArgsCount, Node* pNode)
{
	if (args.size() != lArgsCount)
	{
		std::wstringstream strDescription;
		strDescription << L"函数'" << strFuncName << L"'参数个数错误.";
		ExecuteExcept error(strDescription.str(), L"VariantOperator::CheckFunctinArgument", pNode);
		throw error;
	}

	for (int i = 0; i < lArgsCount;++i)
	{
		if (!args[i])
		{
			std::wstringstream strDescription;
			strDescription << L"函数'" << strFuncName << L"'参数" << i + 1 << L"错误";
			ExecuteExcept error(strDescription.str(), L"VariantOperator::CheckFunctinArgument", pNode);
			throw error;
		}
	}

	return true;
}

}
}