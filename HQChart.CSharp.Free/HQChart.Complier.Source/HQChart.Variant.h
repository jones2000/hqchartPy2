/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	指标数据类 (c++)
*/

#pragma once

#include "HQChart.KLineData.h"


namespace HQChart { namespace Complier {

class IHistoryData;
class VariantOperator;
class Variant;

struct VARIANT_ITEM
{
	enum
	{
		NULL_ID = 0,
		VAILD_ID = 1,
	};

	double _dValue = 0;						//数值
	short  _sType = NULL_ID;				//0=NULL, 1=有效数  
	std::wstring* _pString = NULL;		//字符串 (由外部new)

	void SetValue(double dValue) { _dValue = dValue; _sType = VAILD_ID; }
	void SetValue(long lValue) { _dValue = lValue; _sType = VAILD_ID; }
	void SetValue(int nValue) { _dValue = nValue; _sType = VAILD_ID; }

	void SetNull() { _dValue = 0; _sType = NULL_ID; }
	bool IsVaild() const { return _sType == VAILD_ID; }	//是否有效

	void SetValue(const std::wstring& strValue)
	{
		if (_pString)
		{
			(*_pString) = strValue;
			_sType = VAILD_ID;
		}
	}

	void SetValue(std::wstring* pString)
	{
		_pString = pString;
		_sType = VAILD_ID;
	}
};

//自定义绘图数据
class DrawData
{
public:
	typedef std::map<std::wstring, Variant*>	MAP_VARAINT;

	MAP_VARAINT _mapVaraint;
	std::wstring _strDrawName;
};



class Variant
{
public:
	friend VariantOperator;
	typedef IHistoryData::ARRAY_FIT_DATETIME_DATA ARRAY_FIT_DATETIME_DATA;

	Variant();
	~Variant();

	enum
	{
		NULL_TYPE = 0,
		ARRAY_DOUBLE_TYPE = 1,
		DOUBLE_TYPE = 2,

		STRING_TYPE = 3,			//字符串类型
		ARRAY_STRING_TYPE = 4,		//字符串数组

		MEMBER_TYPE = 5,			//含有成员变量

		//绘图函数
		DRAWKLINE_TYPE = 6,
		STICKLINE_TYPE = 7,
		DRAWICON_TYPE = 8,
		DRAWTEXT_TYPE = 9,
		DRAWNUMBER_TYPE = 10,
		DRAWBAND_TYPE = 11,

		PLOYLINE_TYPE = 12,
		DRAWTEXT_FIX_TYPE = 13,
		DRAWNUMBER_FIX_TYPE = 14,

		FILLRGN_TYPE = 15,
		PARTLINE_TYPE = 16,
		EXPLAIN_TYPE = 17,

		DRAWCOLORKLINE_TYPE = 18,
		DRAWICON2_TYPE = 19,

		TESTSKIP_TYPE=20,
	};

	typedef std::vector<VARIANT_ITEM> ARRAY_DOUBLE;
	typedef std::vector<Variant* > ARRAY_CALL_ARGUMENT;
	typedef std::map<std::wstring, Variant*> MAP_VARIANT;


	int GetType() const { return m_nType; }
	void SetType(int nValue) { m_nType = nValue; }

	void Clear();

	void SetDoubleValue(double dValue);
	double GetDoubleValue() const;
	bool IsVaildDoulbe() const;	//是都是有效的单数值

	void SetStringValue(const std::wstring& strValue);
	const std::wstring& GetStringValue() const;
	bool IsVaildString() const;	//是都是有效的单字符串

	ARRAY_DOUBLE& GetArrayValue() { return m_aryValue; }
	const ARRAY_DOUBLE& GetArrayValue() const { return m_aryValue; }

	void SetMemberValue(const std::wstring& strName, Variant* pVariant);
	Variant* GetMemberValue(const std::wstring& strName) const;

	void SetDrawKLineData(Variant* pVarHigh, Variant* pVarOpen, Variant* pVarLow, Variant* pVarClose);
	void SetDrawColorKLineData(Variant* pVarHigh, Variant* pVarOpen, Variant* pVarLow, Variant* pVarClose, Variant* pColor, Variant* pEmpty);
	void SetDrawStickLineData(Variant* pVarPrice, Variant* pVarPrice2, Variant* pVarWidth, Variant* pVarDrawType);
	void SetDrawIconData(Variant* pVarPrice, Variant* pVarIcon, long lType);
	void SetDrawTextData(Variant* pVarPrice, Variant* pVarText);
	void SetDrawNumberData(Variant* pVarPrice, Variant* pVarNumber);
	void SetDrawBandData(Variant* pVarPrice, Variant* pVarColor, Variant* pVarPrice2, Variant* pVarColor2);
	void SetDrawTextFixData(Variant* pCond, Variant* pX, Variant* pY, Variant* pType, Variant* pText);
	void SetDrawNumberFixData(Variant* pX, Variant* pY, Variant* pType, Variant* pNumber);
	void SetFillRNGData(Variant* pVarPrice, Variant* pVarPrice2, Variant* pColor);
	void SetPartLineData(Variant* pVarPrice, Variant* pColor);
	void SetExplain(Variant* pVarCond, Variant* pText);
	const DrawData* GetDrawData() const { return m_pDrawData; }

	void CopyTo(Variant& dest, VariantOperator* pVariantOperator) const;
	void CopyTo(Variant& dest, const ARRAY_FIT_DATETIME_DATA& aryDateTime, VariantOperator* pVariantOperator) const;

	bool TryGetLongValue(long& lValue, long lDefault) const;
	bool TryGetDobuleValue(double& dValue, double dDefault) const;
	bool TryGetStringValue(std::wstring& strValue, const std::wstring& strDefault) const;

protected:

	void ResetDrawData();

	int m_nType = NULL_TYPE;

	ARRAY_DOUBLE m_aryValue;
	std::wstring m_strValue;
	MAP_VARIANT* m_pMapVariant = NULL;	//用来保存成员变量

	//画图函数数据
	DrawData* m_pDrawData = NULL;

};

}
}