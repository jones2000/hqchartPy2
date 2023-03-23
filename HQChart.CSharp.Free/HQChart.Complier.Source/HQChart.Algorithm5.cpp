/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

内置函数类 (c++)
*/

#include "HQChart.data.h"
#include "HQChart.Execute.h"
#include <sstream>


///////////////////////////////////////////////////////////////////////////////
//
// 绘图函数
//
//////////////////////////////////////////////////////////////////////////////

namespace HQChart { namespace Complier {

//DRAWKLINE(HIGH,OPEN,LOW,CLOSE).
//用法: 以HIGH为最高价, OPEN为开盘价, LOW为最低, CLOSE收盘画K线
Variant* VariantOperator::DRAWKLINE(const Variant& varHigh, const Variant& varOpen, const Variant& varLow, const Variant& varClose)
{
	Variant* pVarHigh = Clone(&varHigh);
	Variant* pVarOpen = Clone(&varOpen);
	Variant* pVarLow = Clone(&varLow);
	Variant* pVarClose = Clone(&varClose);

	Variant* pResult = Create();
	pResult->SetDrawKLineData(pVarHigh, pVarOpen, pVarLow, pVarClose);

	return pResult;
}
/*
	DRAWCOLORKLINE 绘制K线
	用法：
	DRAWCOLORKLINE(Cond,Color,Empty);
	满足Cond条件时，按照Color颜色绘制K线，根据Empty标志判断是空心还是实心。COLOR代表颜色，Empty非0为空心。

	注：
	不支持将该函数定义为变量，即不支持下面的写法：
	A:DRAWCOLORKLINE(Cond,Color,Empty);

	例：
	DRAWCOLORKLINE(C>O,COLORBLUE,0);//收盘价大于开盘价，用蓝色绘制实心K线
*/
Variant* VariantOperator::DRAWCOLORKLINE(const Variant& cond, const Variant& varColor, const Variant& varEmpty, const IHistoryData* pHistoryData)
{
	Variant* pResult = Create();

	Variant* pVarHigh= Create();
	Variant* pVarOpen = Create();
	Variant* pVarLow = Create();
	Variant* pVarClose = Create();
	Variant* pColor = Clone(&varColor);
	Variant* pEmpty = Clone(&varEmpty);

	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && pHistoryData)
	{
		VariantOperator::DRAWCOLORKLINE(pVarOpen->m_aryValue, pVarHigh->m_aryValue, pVarLow->m_aryValue, pVarClose->m_aryValue, cond.m_aryValue, pHistoryData);
		pVarHigh->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pVarOpen->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pVarLow->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pVarClose->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE && pHistoryData)
	{
		if (cond.IsVaildDoulbe())
		{
			double dValue = cond.GetDoubleValue();
			VariantOperator::DRAWCOLORKLINE(pVarOpen->m_aryValue, pVarHigh->m_aryValue, pVarLow->m_aryValue, pVarClose->m_aryValue, dValue, pHistoryData);
			pVarHigh->SetType(Variant::ARRAY_DOUBLE_TYPE);
			pVarOpen->SetType(Variant::ARRAY_DOUBLE_TYPE);
			pVarLow->SetType(Variant::ARRAY_DOUBLE_TYPE);
			pVarClose->SetType(Variant::ARRAY_DOUBLE_TYPE);
		}
	}

	pResult->SetDrawColorKLineData(pVarOpen, pVarHigh, pVarLow, pVarClose, pColor, pEmpty);
	return pResult;
}

void VariantOperator::DRAWCOLORKLINE(ARRAY_DOUBLE& destOpen, ARRAY_DOUBLE& destHigh, ARRAY_DOUBLE& destLow, ARRAY_DOUBLE& destClose, const ARRAY_DOUBLE& cond, const IHistoryData* pHistoryData)
{
	long lKCount = pHistoryData->GetKCount();
	long lCondCount = (long)cond.size();
	destOpen.resize(lKCount, VARIANT_ITEM());
	destHigh.resize(lKCount, VARIANT_ITEM());
	destLow.resize(lKCount, VARIANT_ITEM());
	destClose.resize(lKCount, VARIANT_ITEM());

	for (long i = 0; i < lKCount && i< lCondCount; ++i)
	{
		const auto& item = cond[i];
		if (!item.IsVaild() || !item._dValue) continue;
		auto* pKItem=pHistoryData->GetKItem(i);
		if (!pKItem) continue;

		destOpen[i].SetValue(pKItem->_dOpen);
		destHigh[i].SetValue(pKItem->_dHigh);
		destLow[i].SetValue(pKItem->_dLow);
		destClose[i].SetValue(pKItem->_dClose);
	}
}

void VariantOperator::DRAWCOLORKLINE(ARRAY_DOUBLE& destOpen, ARRAY_DOUBLE& destHigh, ARRAY_DOUBLE& destLow, ARRAY_DOUBLE& destClose, double cond, const IHistoryData* pHistoryData)
{
	long lKCount = pHistoryData->GetKCount();
	destOpen.resize(lKCount, VARIANT_ITEM());
	destHigh.resize(lKCount, VARIANT_ITEM());
	destLow.resize(lKCount, VARIANT_ITEM());
	destClose.resize(lKCount, VARIANT_ITEM());

	for (long i = 0; i < lKCount; ++i)
	{
		if (!cond) continue;
		auto* pKItem = pHistoryData->GetKItem(i);
		if (!pKItem) continue;

		destOpen[i].SetValue(pKItem->_dOpen);
		destHigh[i].SetValue(pKItem->_dHigh);
		destLow[i].SetValue(pKItem->_dLow);
		destClose[i].SetValue(pKItem->_dClose);
	}
}

Variant* VariantOperator::STICKLINE(const Variant& cond, const Variant& data, const Variant& data2, const Variant& width, const Variant& drawType)
{
	Variant* pResult = Create();
	Variant* pWidth = Clone(&width);
	Variant* pDrawType = Clone(&drawType);
	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		Variant* pVarPrice = Create();
		Variant* pVarPrice2 = Create();

		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, cond.m_aryValue, data.m_aryValue, data2.m_aryValue);
		}
		else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			if (!data.IsVaildDoulbe()) return pResult;
			double dValue = data.GetDoubleValue();

			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, cond.m_aryValue, dValue, data2.m_aryValue);
		}
		else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
		{
			if (!data2.IsVaildDoulbe()) return pResult;
			double dValue = data2.GetDoubleValue();

			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, cond.m_aryValue, dValue, data.m_aryValue);
		}
		else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
		{
			if (!data2.IsVaildDoulbe() || !data.IsVaildDoulbe()) return pResult;
			double dValue = data.GetDoubleValue();
			double dValue2 = data2.GetDoubleValue();
			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, cond.m_aryValue, dValue, dValue2);
		}
		else
		{
			return pResult;
		}
		
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pVarPrice2->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pResult->SetDrawStickLineData(pVarPrice, pVarPrice2, pWidth, pDrawType);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!cond.IsVaildDoulbe()) return pResult;

		double dValue = cond.GetDoubleValue();
		if (!dValue) return pResult;

		Variant* pVarPrice = Create();
		Variant* pVarPrice2 = Create();
		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, data.m_aryValue, data2.m_aryValue);
		}
		else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE)
		{
			if (!data.IsVaildDoulbe()) return pResult;
			double dValue = data.GetDoubleValue();

			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, dValue, data2.m_aryValue);
		}
		else if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
		{
			if (!data2.IsVaildDoulbe()) return pResult;
			double dValue = data2.GetDoubleValue();

			STICKLINE(pVarPrice->m_aryValue, pVarPrice2->m_aryValue, dValue, data.m_aryValue);
		}
		else if (data.GetType() == Variant::DOUBLE_TYPE && data2.GetType() == Variant::DOUBLE_TYPE)
		{
			if (!data2.IsVaildDoulbe() || !data.IsVaildDoulbe()) return pResult;
			pVarPrice->SetDoubleValue(data.GetDoubleValue());
			pVarPrice2->SetDoubleValue(data2.GetDoubleValue());
			pResult->SetDrawStickLineData(pVarPrice, pVarPrice2, pWidth, pDrawType);
			return pResult;
		}
		else
		{
			return pResult;
		}

		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pVarPrice2->SetType(Variant::ARRAY_DOUBLE_TYPE);
		pResult->SetDrawStickLineData(pVarPrice, pVarPrice2, pWidth, pDrawType);
	}

	
	return pResult;
}

void VariantOperator::STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nCount = (int)cond.size();
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();

	dest.resize(nCount, VARIANT_ITEM());
	dest2.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		const auto& condItem = cond[i];
		if (!condItem.IsVaild()) continue;
		if (condItem._dValue <= 0) continue;

		if (i >= nData2Count || i >= nDataCount) continue;

		const auto& dataItem = data[i];
		const auto& data2Item = data2[i];
		if (!dataItem.IsVaild() || !data2Item.IsVaild()) continue;

		dest[i].SetValue(dataItem._dValue);
		dest2[i].SetValue(data2Item._dValue);
	}
}

void VariantOperator::STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& cond, double dData, const ARRAY_DOUBLE& data2)
{
	int nCount = (int)cond.size();
	int nData2Count = (int)data2.size();

	dest.resize(nCount, VARIANT_ITEM());
	dest2.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		const auto& condItem = cond[i];
		if (!condItem.IsVaild()) continue;
		if (condItem._dValue <= 0) continue;

		if (i >= nData2Count) continue;

		const auto& data2Item = data2[i];
		if (!data2Item.IsVaild()) continue;

		dest[i].SetValue(dData);
		dest2[i].SetValue(data2Item._dValue);
	}
}

void VariantOperator::STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& cond, double dData, double dData2)
{
	int nCount = (int)cond.size();

	dest.resize(nCount, VARIANT_ITEM());
	dest2.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		const auto& condItem = cond[i];
		if (!condItem.IsVaild()) continue;
		if (condItem._dValue <= 0) continue;

		dest[i].SetValue(dData);
		dest2[i].SetValue(dData2);
	}
}

void VariantOperator::STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2)
{
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();
	int nCount = std::max(nDataCount, nDataCount);

	dest.resize(nCount, VARIANT_ITEM());
	dest2.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		if (i >= nData2Count || i >= nDataCount) continue;

		const auto& dataItem = data[i];
		const auto& data2Item = data2[i];
		if (!dataItem.IsVaild() || !data2Item.IsVaild()) continue;

		dest[i].SetValue(dataItem._dValue);
		dest2[i].SetValue(data2Item._dValue);
	}
}

void VariantOperator::STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, double dData, const ARRAY_DOUBLE& data2)
{
	int nData2Count = (int)data2.size();
	int nCount = nData2Count;

	dest.resize(nCount, VARIANT_ITEM());
	dest2.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		const auto& data2Item = data2[i];
		if (!data2Item.IsVaild()) continue;

		dest[i].SetValue(dData);
		dest2[i].SetValue(data2Item._dValue);
	}
}

Variant* VariantOperator::DRAWICON(const Variant& cond, const Variant& data, const Variant& iconID)
{
	Variant* pResult = Create();
	if (iconID.GetType() != Variant::DOUBLE_TYPE && iconID.GetType()!= Variant::STRING_TYPE) return pResult;
	if (!iconID.IsVaildDoulbe() && !iconID.IsVaildString()) return pResult;

	Variant* pVarIcon = Clone(&iconID);
	Variant* pVarPrice = Create();
	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		MeetConditionData(pVarPrice->m_aryValue, cond.m_aryValue, data.m_aryValue);
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return pResult;

		double dValue = data.GetDoubleValue();
		MeetConditionData(pVarPrice->m_aryValue, cond.m_aryValue, dValue);
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!cond.IsVaildDoulbe()) return pResult;
		double dValue = cond.GetDoubleValue();
		if (dValue <= 0) return pResult;

		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE)
		{
			data.CopyTo(*pVarPrice, this);
		}
		else
		{
			return pResult;
		}
	}
	else
	{
		return pResult;
	}

	pResult->SetDrawIconData(pVarPrice, pVarIcon, Variant::DRAWICON_TYPE);
	return pResult;
}

/*
	ICON：在k线图上，显示小图标。

	用法：ICON(TYPE,ICON);
	当TYPE为1，则在K线最高价位置显示图标ICON，当TYPE为0，则在最低价位置显示
	图标ICON。

	注：
	1、该函数与判断条件连用，如：COND,ICON(TYPE,ICON);
	2、该函数支持在函数后设置垂直对齐方式：VALIGN0（上对齐）、VALIGN1（中对齐）、VALIGN2（下对齐）
	即可以写为如下格式：
	CLOSE<OPEN,ICON(1,'阴'),VALIGN0;

	例1：
	CLOSE>OPEN,ICON(1,'ICO1');//表示K线收盘大于开盘时，在最高价上显示图标1。
	写完“ICON(1,” 以后，点击插入图标按钮，再单击选中的图标插入到函数中，图标用
	'ICO1'~'ICO105'表示
*/
Variant* VariantOperator::ICON(const Variant& varPos, const Variant& varIconID, const IHistoryData* pHistoryData)
{
	Variant* pResult = Create();
	if (varIconID.GetType() != Variant::DOUBLE_TYPE && varIconID.GetType() != Variant::STRING_TYPE) return pResult;
	if (!varIconID.IsVaildDoulbe() && !varIconID.IsVaildString()) return pResult;
	if (!pHistoryData) return pResult;

	if (varPos.GetType() != Variant::DOUBLE_TYPE || !varPos.IsVaildDoulbe()) return pResult;

	Variant* pVarIcon = Clone(&varIconID);
	int nPos = (int)varPos.GetDoubleValue();

	Variant* pPrice = NULL;
	if (nPos == 1) pPrice=pHistoryData->GetHigh();
	else pPrice=pHistoryData->GetLow();

	pResult->SetDrawIconData(pPrice, pVarIcon, Variant::DRAWICON2_TYPE);
	return pResult;
}

void VariantOperator::MeetConditionData(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data)
{
	int nCount = (int)cond.size();
	int nDataCount = (int)data.size();

	dest.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		const auto& condItem = cond[i];
		if (!condItem.IsVaild()) continue;
		if (condItem._dValue <= 0) continue;

		if (i >= nDataCount) continue;

		const auto& dataItem = data[i];
		if (!dataItem.IsVaild()) continue;

		dest[i].SetValue(dataItem._dValue);
	}
}

void VariantOperator::MeetConditionData(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, double dData)
{
	int nCount = (int)cond.size();
	dest.resize(nCount, VARIANT_ITEM());

	for (int i = 0; i < nCount; ++i)
	{
		const auto& condItem = cond[i];
		if (!condItem.IsVaild()) continue;
		if (condItem._dValue <= 0) continue;

		dest[i].SetValue(dData);
	}
}

/*
显示文字;在指标排序中显示字符串栏目.
用法: DRAWTEXT(COND,PRICE,TEXT),当COND条件满足时,在PRICE位置书写文字TEXT.
例如: DRAWTEXT(CLOSE/OPEN>1.08,LOW,'大阳线')表示当日实体阳线大于8%时在最低价位置显示'大阳线'字样.
*/
Variant* VariantOperator::DRAWTEXT(const Variant& cond, const Variant& data, const Variant& text)
{
	Variant* pResult = Create();
	if (text.GetType() != Variant::STRING_TYPE && text.GetType()!=Variant::ARRAY_STRING_TYPE) return pResult;

	if (text.GetType() == Variant::STRING_TYPE && !text.IsVaildString()) return pResult;

	Variant* pVarText = Clone(&text);
	Variant* pVarPrice = Create();
	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		MeetConditionData(pVarPrice->m_aryValue, cond.m_aryValue, data.m_aryValue);
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return pResult;

		double dValue = data.GetDoubleValue();
		MeetConditionData(pVarPrice->m_aryValue, cond.m_aryValue, dValue);
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!cond.IsVaildDoulbe()) return pResult;
		double dValue = cond.GetDoubleValue();
		if (dValue <= 0) return pResult;

		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE)
		{
			data.CopyTo(*pVarPrice, this);
		}
		else
		{
			return pResult;
		}
	}
	else
	{
		return pResult;
	}

	pResult->SetDrawTextData(pVarPrice, pVarText);
	return pResult;
}

/*
固定位置显示文字;在指标排序中显示字符串栏目.
用法: DRAWTEXT_FIX(COND,X,Y,TYPE,TEXT),当COND条件满足时,在当前指标窗口内(X,Y)位置书写文字TEXT,X,Y为书写点在窗口中相对于左上角的百分比,TYPE:0为左对齐,1为右对齐.
例如: DRAWTEXT_FIX(CURRBARSCOUNT=1 AND CLOSE/OPEN>1.08,0.5,0.5,0,'大阳线')表示最后一个交易日实体阳线大于8%时在窗口中间位置显示'大阳线'字样.
*/
Variant* VariantOperator::DRAWTEXT_FIX(const Variant& cond, const Variant& x, const Variant& y, const Variant& type, const Variant& text)
{
	Variant* pResult = Create();
	if (text.GetType() != Variant::STRING_TYPE) return pResult;
	if (!text.IsVaildString()) return pResult;

	Variant* pVarText = Clone(&text);
	Variant* pOutCond = Create();
	Variant* pX = Clone(&x);
	Variant* pY = Clone(&y);
	Variant* pType = Clone(&type);
	
	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		const auto& condData = cond.m_aryValue;
		auto& outData = pOutCond->m_aryValue;
		int nCount= (int)condData.size();
		auto falseItem = VARIANT_ITEM();
		falseItem.SetValue(0);
		outData.resize(nCount, falseItem);
		for (int i = 0; i < nCount;  ++i)
		{
			const auto& condItem = condData[i];
			if (!condItem.IsVaild()) continue;
			if (condItem._dValue <= 0) continue;

			outData[i].SetValue(1);
		}
		pOutCond->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE)
	{
		if (cond.IsVaildDoulbe())
		{
			pOutCond->SetDoubleValue(cond.GetDoubleValue());
		}
		else
		{
			return pResult;
		}
	}
	else
	{
		return pResult;
	}

	pResult->SetDrawTextFixData(pOutCond, pX, pY, pType,pVarText);

	return pResult;
}

/*
固定位置显示数字.
用法: DRAWNUMBER_FIX(COND,X,Y,TYPE,NUMBER),当COND条件满足时,在当前指标窗口内(X,Y)位置书写数字NUMBER,X,Y为书写点在窗口中相对于左上角的百分比,TYPE:0为左对齐,1为右对齐.
例如: DRAWNUMBER_FIX(CURRBARSCOUNT=1 AND CLOSE/OPEN>1.08,0.5,0.5,0,C)表示最后一个交易日实体阳线大于8%时在窗口中间位置显示收盘价.
*/
Variant* VariantOperator::DRAWNUMBER_FIX(const Variant& cond, const Variant& x, const Variant& y, const Variant& type, const Variant& number)
{
	Variant* pResult = Create();
	if (number.GetType() != Variant::DOUBLE_TYPE && number.GetType() != Variant::ARRAY_DOUBLE_TYPE) return pResult;

	Variant* pNumber = Create();
	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && number.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		MeetConditionData(pNumber->m_aryValue, cond.m_aryValue, number.m_aryValue);
		pNumber->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && number.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!number.IsVaildDoulbe()) return pResult;

		double dValue = number.GetDoubleValue();
		MeetConditionData(pNumber->m_aryValue, cond.m_aryValue, dValue);
		pNumber->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!cond.IsVaildDoulbe()) return pResult;
		double dValue = cond.GetDoubleValue();
		if (dValue <= 0) return pResult;

		if (number.GetType() == Variant::ARRAY_DOUBLE_TYPE || number.GetType() == Variant::DOUBLE_TYPE)
		{
			number.CopyTo(*pNumber, this);
		}
		else
		{
			return pResult;
		}
	}
	else
	{
		return pResult;
	}

	Variant* pX = Clone(&x);
	Variant* pY = Clone(&y);
	Variant* pType = Clone(&type);

	pResult->SetDrawNumberFixData(pX, pY, pType, pNumber);
	return pResult;
}


Variant* VariantOperator::DRAWNUMBER(const Variant& cond, const Variant& data, const Variant& number)
{
	Variant* pResult = Create();
	if (number.GetType() != Variant::DOUBLE_TYPE && number.GetType()!=Variant::ARRAY_DOUBLE_TYPE) return pResult;

	Variant* pVarNumber = Clone(&number);
	Variant* pVarPrice = Create();
	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		MeetConditionData(pVarPrice->m_aryValue, cond.m_aryValue, data.m_aryValue);
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!data.IsVaildDoulbe()) return pResult;

		double dValue = data.GetDoubleValue();
		MeetConditionData(pVarPrice->m_aryValue, cond.m_aryValue, dValue);
		pVarPrice->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}
	else if (cond.GetType() == Variant::DOUBLE_TYPE)
	{
		if (!cond.IsVaildDoulbe()) return pResult;
		double dValue = cond.GetDoubleValue();
		if (dValue <= 0) return pResult;

		if (data.GetType() == Variant::ARRAY_DOUBLE_TYPE || data.GetType() == Variant::DOUBLE_TYPE)
		{
			data.CopyTo(*pVarPrice, this);
		}
		else
		{
			return pResult;
		}
	}
	else
	{
		return pResult;
	}

	pResult->SetDrawNumberData(pVarPrice, pVarNumber);
	return pResult;
}

Variant* VariantOperator::COLOR_RGB(const Variant& r, const Variant& g, const Variant& b)
{
	Variant* pResult = Create();
	if (r.GetType() != Variant::DOUBLE_TYPE || g.GetType() != Variant::DOUBLE_TYPE || b.GetType() != Variant::DOUBLE_TYPE) return pResult;
	if (!r.IsVaildDoulbe() || !g.IsVaildDoulbe() || !b.IsVaildDoulbe()) return pResult;

	int nR =(int) r.GetDoubleValue();
	int nG = (int)g.GetDoubleValue();
	int nB = (int)b.GetDoubleValue();

	std::wstringstream strStream;
	strStream<< L"RGB(" << nR << L"," << nG << L"," << nB << L")";
	pResult->SetStringValue(strStream.str());

	return pResult;
}

Variant* VariantOperator::COLOR_ARGB(const Variant& a, const Variant& r, const Variant& g, const Variant& b)
{
	Variant* pResult = Create();
	if (r.GetType() != Variant::DOUBLE_TYPE || g.GetType() != Variant::DOUBLE_TYPE || b.GetType() != Variant::DOUBLE_TYPE) return pResult;
	if (!r.IsVaildDoulbe() || !g.IsVaildDoulbe() || !b.IsVaildDoulbe()) return pResult;

	int nR = (int)r.GetDoubleValue();
	int nG = (int)g.GetDoubleValue();
	int nB = (int)b.GetDoubleValue();
	double dA = a.GetDoubleValue();

	std::wstringstream strStream;
	strStream << L"ARGB(" << dA<<L"," << nR << L"," << nG << L"," << nB << L")";

	pResult->SetStringValue(strStream.str());

	return pResult;
}

Variant* VariantOperator::COLOR_RGBA(const Variant& r, const Variant& g, const Variant& b, const Variant& a)
{
	return COLOR_ARGB(a, r, g, b);
}


//画出带状线.
//用法:DRAWBAND(VAL1, COLOR1, VAL2, COLOR2), 当VAL1 > VAL2时, 在VAL1和VAL2之间填充COLOR1; 当VAL1 < VAL2时, 填充COLOR2, 这里的颜色均使用RGB函数计算得到.
//例如:DRAWBAND(OPEN, RGB(0, 224, 224), CLOSE, RGB(255, 96, 96));
Variant* VariantOperator::DRAWBAND(const Variant& data, const Variant& color, const Variant& data2, const Variant& color2)
{
	Variant* pResult = Create();

	Variant* pVarPrice = Clone(&data);
	Variant* pVarPrice2 = Clone(&data2);
	Variant* pVarColor = Clone(&color);
	Variant* pVarColor2 = Clone(&color2);

	pResult->SetDrawBandData(pVarPrice, pVarColor,pVarPrice2, pVarColor2);

	return pResult;
}

//绘制折线段.
//用法:PLOYLINE(COND, PRICE), 当COND条件满足时, 以PRICE位置为顶点画折线连接.
//例如:PLOYLINE(HIGH >= HHV(HIGH, 20), HIGH)表示在创20天新高点之间画折线
Variant* VariantOperator::PLOYLINE(const Variant& cond, const Variant& data)
{
	Variant* pResult = Create();

	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::ARRAY_DOUBLE_TYPE)
	{
		PLOYLINE(pResult->m_aryValue, cond.m_aryValue, data.m_aryValue);
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

void VariantOperator::PLOYLINE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data)
{
	int nCount = (int)cond.size();
	dest.resize(nCount, VARIANT_ITEM());

	int i = 0;
	for (i = 0; i < nCount; ++i)
	{
		const auto& condItem = cond[i];
		if (!condItem.IsVaild()) continue;
		if (fabs(condItem._dValue - 1) < 0.0001)
		{
			const auto& dataItem = data[i];
			if (!dataItem.IsVaild()) continue;

			dest[i].SetValue(dataItem._dValue);
		}
	}

	double dTopGap = 0, dGap = 0;
	for (i = 0; i < nCount; ++i)
	{
		if (dest[i].IsVaild()) break;
	}
	int nLFirst = i++;

	for (; i < nCount; ++i)
	{
		if (dest[i].IsVaild()) break;
	}
	int nLSecond = i;

	dTopGap = dest[nLSecond]._dValue - dest[nLFirst]._dValue;
	for (i = nLFirst; i < nLSecond; ++i)
	{
		dGap = dTopGap * (i - nLFirst) / (nLSecond - nLFirst);
		dest[i].SetValue(dest[nLFirst]._dValue + dGap);
	}

	for (i = nCount - 1; i > nLSecond; --i)
	{
		if (dest[i].IsVaild()) break;
	}
	int nRFirst = i--;

	for (; i > nLSecond; --i)
	{
		if (dest[i].IsVaild()) break;
	}
	int nRSecond = i;

	dTopGap = dest[nRFirst]._dValue - dest[nRSecond]._dValue;
	for (i = nCount - 1; i > nRSecond; --i)
	{
		dGap = dTopGap * (i - nRSecond) / (nRFirst - nRSecond);
		dest[i].SetValue(dest[nRSecond]._dValue + dGap);
	}

	i = nLSecond;
	while (i <= nRSecond)
	{
		for (; i < nRSecond; ++i)
		{
			if (dest[i].IsVaild()) break;
		}
		int nLeft = i++;

		for (; i < nRSecond; ++i)
		{
			if (dest[i].IsVaild()) break;
		}
		int nRight = i;

		dTopGap = dest[nRight]._dValue - dest[nLeft]._dValue;
		for (i = nLeft; i < nRight; ++i)
		{
			dGap = dTopGap * (i - nLeft) / (nRight - nLeft);
			dest[i].SetValue(dest[nLeft]._dValue + dGap);
		}
	}
	
}

//属于未来函数, 绘制直线段.
//用法:DRAWLINE(COND1, PRICE1, COND2, PRICE2, EXPAND), 当COND1条件满足时, 在PRICE1位置画直线起点, 当COND2条件满足时, 在PRICE2位置画直线终点, EXPAND为延长类型.
//例如 :DRAWLINE(HIGH >= HHV(HIGH, 20), HIGH, LOW <= LLV(LOW, 20), LOW, 1)表示在创20天新高与创20天新低之间画直线并且向右延长.
Variant* VariantOperator::DRAWLINE(const Variant& cond, const Variant& data, const Variant& cond2, const Variant& data2, const Variant& expend)
{
	Variant* pResult = Create();

	if (cond.GetType() == Variant::ARRAY_DOUBLE_TYPE && data.GetType() == Variant::ARRAY_DOUBLE_TYPE &&
		cond2.GetType() == Variant::ARRAY_DOUBLE_TYPE && data2.GetType() == Variant::ARRAY_DOUBLE_TYPE && expend.IsVaildDoulbe())
	{
		DRAWLINE(pResult->m_aryValue, cond.m_aryValue, data.m_aryValue, cond2.m_aryValue, data2.m_aryValue, (long)expend.GetDoubleValue());
		pResult->SetType(Variant::ARRAY_DOUBLE_TYPE);
	}

	return pResult;
}

struct DRAWLINE_POINT_ITEM
{
	int		_nID;
	double _dValue;
};

struct DRAWLINE_DATA
{
	DRAWLINE_POINT_ITEM _Start;
	DRAWLINE_POINT_ITEM	_End;
};

//计算2点间的连线
void CalculateDrawLine(const DRAWLINE_DATA& lineCache, std::vector< DRAWLINE_POINT_ITEM>& out)
{
	if (lineCache._Start._nID < 0 || lineCache._End._nID < 0) return;

	std::vector<int> aryID;
	for (int i = lineCache._Start._nID; i <= lineCache._End._nID; ++i)
	{
		aryID.push_back(i);
	}

	if (aryID.empty()) return;

	double dHeight = std::abs(lineCache._Start._dValue - lineCache._End._dValue);
	int nWidth = ((int)aryID.size()) - 1;

	out.clear();
	out.push_back(lineCache._Start);	//第1个点

	double dValue = 0;
	int nCount = (int)aryID.size();
	if (lineCache._Start._dValue > lineCache._End._dValue)
	{
		for (int i = 1; i < nCount -1; ++i)
		{
			dValue = dHeight * (nCount - 1 - i) / nWidth + lineCache._End._dValue;
			out.push_back({ aryID[i], dValue });
		}
	}
	else
	{
		for (int i = 1; i < (int)aryID.size() - 1; ++i)
		{
			dValue = dHeight * i / nWidth + lineCache._Start._dValue;
			out.push_back({ aryID[i], dValue });
		}
	}

	out.push_back(lineCache._End);	//最后一个点
}

//计算延长线
void VariantOperator::CalculateDrawDataExtendLine(ARRAY_DOUBLE& dest, int nCount)
{
	if (nCount < 0) return;
	int x2 = -1;
	for (int i = nCount - 1; i >= 0; --i)
	{
		const auto& item = dest[i];
		if (item.IsVaild() && i-1>=0)
		{
			const auto& item2 = dest[i - 1];
			if (item2.IsVaild())	//2个点同时有效
			{
				x2 = i;
				break;
			}
		}
	}

	if (x2 >= 0 && x2 - 1 >= 0)
	{
		int x1 = x2 - 1;
		double y1 = 0, y2 = 0, y3;
		for (int i = x2 + 1; i < nCount; ++i)
		{
			y1 = dest[x1]._dValue;
			y2 = dest[x2]._dValue;
			y3 = (y1 - y2) * (i - x1) / (x2 - x1);
			if (y1 - y3 < 0) break;
			dest[i].SetValue(y1 - y3);
		}
	}
}



void VariantOperator::DRAWLINE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& condition, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& condition2, const ARRAY_DOUBLE& data2, long lExpand)
{
	int nCondCount = (int)condition.size();
	int nCond2Count = (int)condition2.size();
	int nDataCount = (int)data.size();
	int nData2Count = (int)data2.size();
	int nCount = std::max(nCond2Count, nCondCount);
	dest.resize(nCount, VARIANT_ITEM());

	bool bFirstPoint = false, bSecondPoint = false;
	DRAWLINE_DATA lineCache = { 0 };
	for (int i = 0, j=0; i < nCount; ++i)
	{
		if (i < nCondCount && i < nCond2Count && i<nData2Count && i< nDataCount)
		{
			if (bFirstPoint == false && bSecondPoint == false)
			{
				const auto& item = condition[i];
				const auto& dataItem = data[i];
				if (!item.IsVaild() || !item._dValue || !dataItem.IsVaild()) continue;
				
				bFirstPoint = true;
				lineCache._Start = { i, dataItem._dValue };	//第1个点
			}
			else if (bFirstPoint == true && bSecondPoint == false)
			{
				const auto& item2 = condition2[i];
				const auto& dataItem2 = data2[i];
				if (!item2.IsVaild() || !item2._dValue)
				{
					const auto& item = condition[i];
					const auto& dataItem = data[i];
					if (item.IsVaild() && item._dValue && dataItem.IsVaild())	//条件1满足
					{
						lineCache._Start = { i, dataItem._dValue };	//移动第1个点
					}
					continue;
				}
				else if (dataItem2.IsVaild())
				{
					bSecondPoint = true;
					lineCache._End = { i, dataItem2._dValue };	//第2个点
				}
			}
			else if (bFirstPoint == true && bSecondPoint == true)
			{
				const auto& item2 = condition2[i];
				const auto& dataItem2 = data2[i];

				const auto& item = condition[i];
				const auto& dataItem = data[i];
				if (item2.IsVaild() && item2._dValue && dataItem2.IsVaild())
				{
					lineCache._End = { i, dataItem2._dValue };   //移动第2个点
				}
				else if (item.IsVaild() && item._dValue)
				{
					std::vector<DRAWLINE_POINT_ITEM> aryLine;
					CalculateDrawLine(lineCache, aryLine);
					for (j = 0; j < (int)aryLine.size(); ++j)
					{
						const auto& lineItem = aryLine[j];
						dest[lineItem._nID].SetValue(lineItem._dValue);
					}

					if (lExpand == 1) CalculateDrawDataExtendLine(dest, lineCache._Start._nID - 2);	//上一个连线的延长线

					bFirstPoint = bSecondPoint = false;
					lineCache._End = { -1, -1 };
					lineCache._Start = { -1,-1 };

					if (dataItem.IsVaild())
					{
						bFirstPoint = true;
						lineCache._Start = { i, dataItem._dValue };  //第1个点
						lineCache._End = { -1, -1 };
					}
				}
			}
		}
	}

	if (bFirstPoint == true && bSecondPoint == true)
	{
		std::vector<DRAWLINE_POINT_ITEM> aryLine;
		CalculateDrawLine(lineCache, aryLine);
		for (int j = 0; j < (int)aryLine.size(); ++j)
		{
			const auto& lineItem = aryLine[j];
			dest[lineItem._nID].SetValue(lineItem._dValue);
		}
	}

	if (lExpand == 1) CalculateDrawDataExtendLine(dest, nCount);//右延长线
}

/*
函数:FILLRGN,根据条件用RGN颜色填充区域
用法：FILLRGN(PRICE1,PRICE2,COND1,COLOR1,COND2,COLOR2...),填充PRICE1到PRICE2之间的区域，当COND1条件满足时,用COLOR1颜色，当COND2条件满足时,用COLOR2颜色，否则不填充，从COLOR1之后的参数均可以省略，最多可以有15组条件。
例如：FILLRGN(CLOSE,OPEN,CLOSE>OPEN,RGB(255,0,0),CLOSE<OPEN,RGB(0,255,0))表示填充开盘价和收盘价之间的区域，阳线时用红色，阴线时用绿色，平盘不绘制。
*/

Variant* VariantOperator::FILLRGN(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	Variant* pResult = Create();

	const Variant& price1 = *args[0];
	const Variant& price2 = *args[1];

	std::vector<std::pair<const Variant*, std::wstring>> aryCond;	//first=条件,second=颜色

	int nCount = (int)args.size();
	std::wstring strColor;
	for (int i = 2; i < nCount; ++i)
	{
		const Variant* pCond = args[i];
		++i;
		if (i >= nCount) break;

		if (!pCond) continue;

		const Variant* pRGB = args[i];
		if (!pRGB || !pRGB->IsVaildString()) continue;
		strColor = pRGB->GetStringValue();

		aryCond.push_back(std::pair< const Variant*, std::wstring>(pCond, strColor));
	}

	long lKCount = pHistoryData->GetKCount();
	long lCondType = 0;
	bool bMatched = false;
	Variant* pColor = Create();
	auto& dest = pColor->m_aryValue;
	dest.resize(lKCount, VARIANT_ITEM());
	
	for (int i = 0,j=0; i < lKCount; ++i)
	{
		bMatched = false;
		for (const auto& item : aryCond)
		{
			lCondType = item.first->GetType();
			if (lCondType == Variant::DOUBLE_TYPE)
			{
				if (item.first->IsVaildDoulbe() && item.first->GetDoubleValue())
				{
					bMatched = true;
					strColor = item.second;
					break;
				}
			}
			else if (lCondType == Variant::ARRAY_DOUBLE_TYPE)
			{
				const auto& aryData = item.first->m_aryValue;
				if (i >= (int)aryData.size()) continue;

				const auto& itemCond = aryData[i];
				if (itemCond.IsVaild() && itemCond._dValue)
				{
					bMatched = true;
					strColor = item.second;
					break;
				}
			}
		}

		if (bMatched)
		{
			dest[i].SetValue(CreateString(strColor));
		}
	}

	pColor->SetType(Variant::ARRAY_STRING_TYPE);
	pResult->SetFillRNGData(args[0], args[1], pColor);

	return pResult;
}

/*
函数:PARTLINE,画线
用法：PARTLINE(PRICE,COND1,COLOR1,COND2,COLOR2...),绘制PRICE线，当COND1条件满足时,用COLOR1颜色，当COND2条件满足时,用COLOR2颜色，否则不绘制，从COLOR1之后的参数均可以省略，最多可以有15组条件。
*/

Variant* VariantOperator::PARTLINE(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode)
{
	Variant* pResult = Create();

	const Variant& price1 = *args[0];
	std::vector<std::pair<const Variant*, std::wstring>> aryCond;	//first=条件,second=颜色

	int nCount = (int)args.size();
	std::wstring strColor;
	for (int i = 1; i < nCount; ++i)
	{
		const Variant* pCond = args[i];
		++i;
		if (i >= nCount) break;

		if (!pCond) continue;

		const Variant* pRGB = args[i];
		if (!pRGB || !pRGB->IsVaildString()) continue;
		strColor = pRGB->GetStringValue();

		aryCond.push_back(std::pair< const Variant*, std::wstring>(pCond, strColor));
	}

	long lKCount = pHistoryData->GetKCount();
	long lCondType = 0;
	bool bMatched = false;
	Variant* pColor = Create();
	auto& dest = pColor->m_aryValue;
	dest.resize(lKCount, VARIANT_ITEM());

	for (int i = 0, j = 0; i < lKCount; ++i)
	{
		bMatched = false;
		for (const auto& item : aryCond)
		{
			lCondType = item.first->GetType();
			if (lCondType == Variant::DOUBLE_TYPE)
			{
				if (item.first->IsVaildDoulbe() && item.first->GetDoubleValue())
				{
					bMatched = true;
					strColor = item.second;
					break;
				}
			}
			else if (lCondType == Variant::ARRAY_DOUBLE_TYPE)
			{
				const auto& aryData = item.first->m_aryValue;
				if (i >= (int)aryData.size()) continue;

				const auto& itemCond = aryData[i];
				if (itemCond.IsVaild() && itemCond._dValue)
				{
					bMatched = true;
					strColor = item.second;
					break;
				}
			}
		}

		if (bMatched)
		{
			dest[i].SetValue(CreateString(strColor));
		}
	}

	pColor->SetType(Variant::ARRAY_STRING_TYPE);
	pResult->SetPartLineData(args[0], pColor);

	return pResult;
}


Variant* VariantOperator::EXPLAIN(const Variant& cond, const Variant& text)
{
	Variant* pResult = Create();

	Variant* pCond = Clone(&cond);
	Variant* pText = Clone(&text);

	pResult->SetExplain(pCond, pText);
	return pResult;
}

}
}


