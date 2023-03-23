/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	指标数据类 (c++)
*/

#include "HQChart.Variant.h"
#include "HQChart.data.h"


namespace HQChart { namespace Complier {


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
	m_pDrawData->_mapVaraint[L"High"] = pVarHigh;
	m_pDrawData->_mapVaraint[L"Open"] = pVarOpen;
	m_pDrawData->_mapVaraint[L"Low"] = pVarLow;
	m_pDrawData->_mapVaraint[L"Close"] = pVarClose;
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
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Price2"] = pVarPrice2;
	m_pDrawData->_mapVaraint[L"Width"] = pVarWidth;
	m_pDrawData->_mapVaraint[L"Empty"] = pVarDrawType;
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
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Type"] = pVarIcon;
}

void Variant::SetDrawTextData(Variant* pVarPrice, Variant* pVarText)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWTEXT";
	m_nType = DRAWTEXT_TYPE;
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Text"] = pVarText;
}

void Variant::SetDrawTextFixData(Variant* pCond, Variant* pX, Variant* pY, Variant* pType, Variant* pVarText)
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
	m_pDrawData->_mapVaraint[L"Price"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Number"] = pVarNumber;
}

void Variant::SetDrawBandData(Variant* pVarPrice, Variant* pVarColor, Variant* pVarPrice2, Variant* pVarColor2)
{
	ResetDrawData();

	m_pDrawData->_strDrawName = L"DRAWBAND";
	m_nType = DRAWBAND_TYPE;
	m_pDrawData->_mapVaraint[L"Val1"] = pVarPrice;
	m_pDrawData->_mapVaraint[L"Color1"] = pVarColor;
	m_pDrawData->_mapVaraint[L"Val2"] = pVarPrice2;
	m_pDrawData->_mapVaraint[L"Color2"] = pVarColor2;
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

}
}