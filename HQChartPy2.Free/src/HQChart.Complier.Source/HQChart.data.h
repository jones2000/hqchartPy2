/*
	Copyright (c) 2018 jones

	https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

	开源项目 https://github.com/jones2000/hqchartPy2

	jones_2000@163.com

	内置函数类, 类型定义 (c++)
*/

#pragma once

#include "zwldef.h"
#include "HQChart.KLineData.h"

#undef MAX
#undef MIN

#undef max
#undef min

#include <vector>
#include <map>
#include <algorithm>

/* 使用std::max / std::min
#ifndef MAX_LOCAL
#define MAX_LOCAL(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef MIN_LOCAL
	#define MIN_LOCAL(a,b)	(((a)<(b))?(a):(b))
#endif
*/

namespace HQChart { namespace Complier {

struct ARGUMENT_ITEM
{
	ARGUMENT_ITEM():_dValue(0) 
	{ 
	}

	ARGUMENT_ITEM(const std::wstring& strName, double dValue)
		:_strName(strName), _dValue(dValue)
	{

	}

	std::wstring	_strName;
	double			_dValue;
};

struct VARIANT_ITEM
{
	enum 
	{
		NULL_ID = 0,
		VAILD_ID = 1,
	};

	double _dValue=0;						//数值
	short  _sType = NULL_ID;				//0=NULL, 1=有效数  
	std::wstring*	_pString = NULL;		//字符串 (由外部new)

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

struct  CHIP_ITEM	//每天筹码数据
{
	int		_nDate;
	int		_nTime;
	double	_dHigh;
	double	_dLow;
	double	_dVol;
};

class ChipData
{
public:
	ChipData();

	void Clear();
	void Add(const CHIP_ITEM& item);

	void SetMax(double dValue) { m_dMax = dValue; }
	void SetMin(double dValue) { m_dMin = dValue; }

	double GetMax() const { return m_dMax; }
	double GetMin() const { return m_dMin; }

	double GetProfit(double dPrice, long lCalculateType) const;		//计算dPrice价格位上的获利 lCalculateType=0 平均分布 1=三角分布
	double GetProfitPrice(double dValue, long lCalculateType) const; //计算获利dValue的价格 
	double GetProfitPrice(double dStart, double dEnd, long lCalculateType) const; //计算获利[dStart, dEnd] dValue的价格
	double GetPPartRate(long ln);				//n前的成本占总成本的比例

public:
	typedef std::vector<CHIP_ITEM> ARRAY_CHIP;

	double m_dMax=0;
	double m_dMin=0;
	ARRAY_CHIP m_aryData;
};

struct ZIG_ITEM
{
	int _nID;
	bool _bUp;
	VARIANT_ITEM _Value;
};

class Node;
class IHistoryData;
class VariantOperator;

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
		NULL_TYPE=0,
		ARRAY_DOUBLE_TYPE=1,
		DOUBLE_TYPE=2,

		STRING_TYPE=3,			//字符串类型
		ARRAY_STRING_TYPE=4,	//字符串数组

		MEMBER_TYPE=5,			//含有成员变量

		//绘图函数
		DRAWKLINE_TYPE=6,
		STICKLINE_TYPE=7,
		DRAWICON_TYPE=8,
		DRAWTEXT_TYPE=9,
		DRAWNUMBER_TYPE=10,
		DRAWBAND_TYPE=11,

		PLOYLINE_TYPE=12,
		DRAWTEXT_FIX_TYPE=13,
		DRAWNUMBER_FIX_TYPE=14,

		FILLRGN_TYPE=15,
		PARTLINE_TYPE=16,
		EXPLAIN_TYPE=17,

		DRAWCOLORKLINE_TYPE=18,
		DRAWICON2_TYPE =19,
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
	void SetDrawTextFixData(Variant* pCond,Variant* pX, Variant* pY, Variant* pType, Variant* pText);
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
	MAP_VARIANT*	m_pMapVariant = NULL;	//用来保存成员变量

	//画图函数数据
	DrawData* m_pDrawData = NULL;

};

struct INVOKE_INDEX_ITEM
{
	std::wstring _strName;		//函数名
	std::wstring _strReturn;	//返回变量
	std::wstring _strPeriod;	//周期,如果没有设置就空

	long _lPeriod=-1;			//周期
	long _lRight=-1;
};

//调用的指标信息
class InvokeIndex
{
public:
	typedef std::vector<ARGUMENT_ITEM> ARRAY_ARGUMENT;

	void SetSymbol(const std::wstring& strValue);
	void SetIndex(const std::wstring& strName, const std::wstring& strCode);
	void SetPeriod(long lValue);
	void SetRight(long lValue);
	void SetArguments(const ARRAY_ARGUMENT& args);
	void SetDataClassName(const std::wstring& strValue);

	const std::wstring& GetSymbol() const;
	const std::wstring& GetCode() const;
	const std::wstring& GetIndexName() const;
	const std::wstring& GetDataClassName() const;
	long GetPeriod() const;
	long GetRight() const;

	ARRAY_ARGUMENT& GetArguments();
	const ARRAY_ARGUMENT& GetArguments() const;

private:
	std::wstring m_strCode;
	std::wstring m_strIndexName;
	std::wstring m_strSymbol;
	std::wstring m_strDataClassName;
	ARRAY_ARGUMENT m_args;

	long m_lPeriod = -1;
	long m_lRight = -1;
};

class Execute;
class VariantOperator
{
public:
	VariantOperator();
	~VariantOperator();

	typedef Variant::ARRAY_DOUBLE ARRAY_DOUBLE;
	typedef Variant::ARRAY_CALL_ARGUMENT ARRAY_CALL_ARGUMENT;
	typedef IHistoryData::ARRAY_FIT_DATETIME_DATA ARRAY_FIT_DATETIME_DATA;

	Variant* Create();
	Variant* Create(double dValue);
	Variant* Clone(const Variant* pVar);
	Variant* Clone(const Variant* pVar, const ARRAY_FIT_DATETIME_DATA& aryDateTime);
	std::wstring* CreateString(const std::wstring& strValue);
	std::wstring* CreateString();
	std::wstring* CreateDoubleString(bool bRemoveZero,const wchar_t* fmt, ...);
	void ClearCache();
	
	
	Variant* Subtract(const Variant& left, const Variant& right);	//-
	Variant* Multiply(const Variant& left, const Variant& right);	//*
	Variant* Divide(const Variant& left, const Variant& right);		///
	Variant* Add(const Variant& left, const Variant& right);		//+
	Variant* GTE(const Variant& left, const Variant& right);		//>=
	Variant* GT(const Variant& left, const Variant& right);			//>
	Variant* LT(const Variant& left, const Variant& right);			//<
	Variant* LTE(const Variant& left, const Variant& right);		//<=
	Variant* EQ(const Variant& left, const Variant& right);			//==
	Variant* NEQ(const Variant& left, const Variant& right);		//!=
	Variant* And(const Variant& left, const Variant& right);		//and, &&
	Variant* Or(const Variant& left, const Variant& right);			//Or, ||

	Variant* CallFunction(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);		//内置函数接口
	Variant* CallFunction2(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);	//内置函数接口2
	Variant* CallIndex(const std::wstring& strCallInfo, const IHistoryData* pHistoryData, Node* pNode);	//KDJ.K#WEEK;

	Variant* GetLunarCalendar(const IHistoryData* pHistoryData, long lType); //获取农历 lType 0=全部 1=年 2=月 3=日
	
	Variant* RPCCall(const Variant::ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);	//远程方法调用

	static bool CheckFunctinArgument(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, long lArgsCount, Node* pNode);

	static bool DoubleGT(const double& dValue, const double& dValue2);
	static bool DoubleLT(const double& dValue, const double& dValue2);
	static bool DoubleGTE(const double& dValue, const double& dValue2);
	static bool DoubleLTE(const double& dValue, const double& dValue2);
	static bool DobuleEQ(const double& dValue, const double& dValue2);
	
private:

	typedef std::vector<Variant*> ARRAY_VARIANT;
	typedef std::list<Variant*> LIST_VARIANT;
	mutable ARRAY_VARIANT m_VariantCache;			//所有的缓存
	mutable ARRAY_VARIANT m_VariantUsedCache;		//保存所有创建的变量数据
	mutable LIST_VARIANT m_VariantFreeCache;		//释放以后放入缓存.

	typedef std::vector<std::wstring*> ARRAY_PSTRING;
	mutable ARRAY_PSTRING m_StringCache;	//保存所有字符串的变量数据

	typedef std::map<std::wstring, Variant*> MAP_INDEX_VARIANT;
	std::map<std::wstring, MAP_INDEX_VARIANT> m_mapIndex;		//指标数据

	typedef std::vector<Variant*> ARRAY_INDEX_VARIANT;
	std::map<std::wstring, ARRAY_INDEX_VARIANT> m_mapCALCSTOCKINDEX;	//CALCSTOCKINDEX() 缓存

	std::map<std::wstring, MAP_INDEX_VARIANT> m_mapSTKINDI;				//STKINDI() 缓存

private:
	static void Subtract(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void Subtract(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void Subtract(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void Multiply(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void Multiply(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);

	static void Divide(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void Divide(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void Divide(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void Add(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void Add(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);

	static void GTE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void GTE(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void GTE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void GT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void GT(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void GT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void LT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void LT(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void LT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void LTE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void LTE(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void LTE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void EQ(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void EQ(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void EQ(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void NEQ(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void NEQ(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void NEQ(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void And(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void And(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void And(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	static void Or(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, const ARRAY_DOUBLE& right);
	static void Or(ARRAY_DOUBLE& dest, double dLeft, const ARRAY_DOUBLE& right);
	static void Or(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& left, double dRight);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//内置函数

	//逻辑函数
	Variant* LONGCROSS(const Variant& data, const Variant& data2, const Variant& n);
	Variant* CROSS(const Variant& data, const Variant& data2);
	Variant* EVERY(const Variant& data, const Variant& n);
	Variant* NDAY(const Variant& data, const Variant& data2, const Variant& n);
	Variant* UPNDAY(const Variant& data, const Variant& n);
	Variant* DOWNNDAY(const Variant& data, const Variant& n);
	Variant* EXIST(const Variant& data, const Variant& n);
	Variant* NOT(const Variant& data);
	Variant* LAST(const Variant& data, const Variant& n, const Variant& n2);
	Variant* EXISTR(const Variant& data, const Variant& n, const Variant& n2);

	//选择函数
	Variant* IFF(const Variant& data, const Variant& trueData, const Variant& falseData);	//IF, IFF
	Variant* IFN(const Variant& data, const Variant& trueData, const Variant& falseData);
	Variant* IFS(const Variant& data, const Variant& trueData, const Variant& falseData);	//字符串条件函数

	//数学函数
	Variant* MAX(const Variant& data, const Variant& data2);
	Variant* MIN(const Variant& data, const Variant& data2);
	Variant* ACOS(const Variant& data);
	Variant* ASIN(const Variant& data);
	Variant* ATAN(const Variant& data);
	Variant* COS(const Variant& data);
	Variant* SIN(const Variant& data);
	Variant* TAN(const Variant& data);
	Variant* EXP(const Variant& data);
	Variant* LN(const Variant& data);
	Variant* LOG(const Variant& data);
	Variant* ABS(const Variant& data);
	Variant* SQRT(const Variant& data);
	Variant* POW(const Variant& data, const Variant& n);
	Variant* CEILING(const Variant& data);
	Variant* FLOOR(const Variant& data);
	Variant* INTPART(const Variant& data);
	Variant* BETWEEN(const Variant& data, const Variant& startData, const Variant& endData);
	Variant* SGN(const Variant& data);
	Variant* FRACPART(const Variant& data);
	Variant* MOD(const Variant& data, const Variant& data2);
	Variant* RAND(const Variant& data, const IHistoryData* pHistoryData);
	Variant* ROUND(const Variant& data);
	Variant* ROUND2(const Variant& data, const Variant& n);
	

	//统计函数
	Variant* AVEDEV(const Variant& data, const Variant& data2);    //平均绝对方差
	Variant* STD(const Variant& data, const Variant& n);		   //估算标准差	
	Variant* DEVSQ(const Variant& data, const Variant& n);		   //偏差平方和
	Variant* FORCAST(const Variant& data, const Variant& n);	   //线性回归预测值
	Variant* SLOPE(const Variant& data, const Variant& n);		   //线性回归斜率
	Variant* STDP(const Variant& data, const Variant& n);		   //总体标准差
	Variant* VAR(const Variant& data, const Variant& n);		   //估算样本方差
	Variant* VARP(const Variant& data, const Variant& n);		   //总体样本方差
	Variant* COVAR(const Variant& data, const Variant& data2, const Variant& n);	//协方差

	//引用函数
	Variant* MA(const Variant& data, const Variant& dayCount);
	Variant* EMA(const Variant& data, const Variant& dayCount);
	Variant* SMA(const Variant& data, const Variant& n, const Variant& m);
	Variant* DMA(const Variant& data, const Variant& data2);
	Variant* WMA(const Variant& data, const Variant& dayCount);					//动态移动平均
	Variant* EXPMA(const Variant& data, const Variant& dayCount);				//加权移动平均
	Variant* EXPMEMA(const Variant& data, const Variant& dayCount);				//加权平滑平均
	Variant* SUMBARS(const Variant& data, const Variant& data2);				//累加到指定值的周期数
	Variant* MEMA(const Variant& data, const Variant& dayCount);
	Variant* ALL(const Variant& data, const Variant& dayCount, const IHistoryData* pHistoryData);					//一直满足条件
	Variant* ANY(const Variant& data, const Variant& dayCount, const IHistoryData* pHistoryData);					//至少一次满足条件、
	Variant* XMA(const Variant& data, const Variant& dayCount);
	Variant* AMA(const Variant& data, const Variant& n);
	Variant* TMA(const Variant& data, const Variant& a, const Variant& b, const IHistoryData* pHistoryData);
	Variant* TRMA(const Variant& data, const Variant& n);
	Variant* HARMEAN(const Variant& data, const Variant& n);
	Variant* VALUEWHEN(const Variant& data, const Variant& n);

	Variant* BACKSET(const Variant& data, const Variant& n);		//向前赋值
	Variant* BARSCOUNT(const Variant& data);						//第一个有效数据周期数
	Variant* BARSLASTCOUNT(const Variant& data);
	Variant* BARSLAST(const Variant& data);							//上一条件成立位置
	Variant* BARSSINCE(const Variant& data);						//第一个条件成立位置
	Variant* COUNT(const Variant& data, const Variant& n);			//统计总数
	Variant* REF(const Variant& data, const Variant& n);			//向前引用(平滑处理)
	Variant* REFV(const Variant& data, const Variant& n);			//向前引用(未作平滑处理)
	Variant* REFX(const Variant& data, const Variant& n);			//引用若干周期后的数据(平滑处理)
	Variant* REFXV(const Variant& data, const Variant& n);			//引用若干周期后的数据(未作平滑处理).
	Variant* REFDATE(const Variant& data, const Variant& date, const IHistoryData* pHistory, Node* pNode);
	Variant* SUM(const Variant& data, const Variant& data2);		//求和
	Variant* MULAR(const Variant& data, const Variant& data2);
	Variant* REVERSE(const Variant& data);
	Variant* BARSSINCEN(const Variant& data, const Variant& n);

	Variant* LLV(const Variant& data, const Variant& n);			//最低值
	Variant* HHV(const Variant& data, const Variant& n);			//上一低点位置
	Variant* LLVBARS(const Variant& data, const Variant& n);		//最高值
	Variant* HHVBARS(const Variant& data, const Variant& n);		//上一高点位置
	Variant* LV(const Variant& data, const Variant& n);
	Variant* HV(const Variant& data, const Variant& n);
	
	Variant* RANGE(const Variant& a, const Variant& b, const Variant& c);			//RANGE(A,B,C):A在B和C范围之间,B<A<C.
	Variant* FILTER(const Variant& data, const Variant& n);			//过滤连续出现的信号.
	Variant* TFILTER(const Variant& buy, const Variant& sell, const Variant& n);
	Variant* CONST_(const Variant& data);							//CONST(A):取A最后的值为常量.

	Variant* ZTPRICE(const Variant& data, const Variant& rate);
	Variant* DTPRICE(const Variant& data, const Variant& rate);

	Variant* TOPRANGE(const Variant& data);
	Variant* LOWRANGE(const Variant& data);
	Variant* BARSNEXT(const Variant& data);
	Variant* LOD(const Variant& data, const Variant& n);
	Variant* HOD(const Variant& data, const Variant& n);
	Variant* FINDLOW(const Variant& data, const Variant& n, const Variant& m, const Variant& t);
	Variant* FINDHIGH(const Variant& data, const Variant& n, const Variant& m, const Variant& t);
	Variant* FINDHIGHBARS(const Variant& data, const Variant& n, const Variant& m, const Variant& t);
	Variant* FINDLOWBARS(const Variant& data, const Variant& n, const Variant& m, const Variant& t);

	//形态函数
	Variant* GetHisCapital(const IHistoryData* pHistoryData, Node* pNode, std::wstring& strError);	//获取历史流通股本
	Variant* WINNER(const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* WINNERB(const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* COST(const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* COSTEX(const Variant& data, const Variant& data2, const IHistoryData* pHistoryData, Node* pNode);
	Variant* LWINNER(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* LWINNERB(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* PWINNER(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* PWINNERB(const Variant& n, const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* PPART(const Variant& data, const IHistoryData* pHistoryData, Node* pNode);
	Variant* PEAKBARS(const Variant& data, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode);
	Variant* PEAK(const Variant& data, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode);
	Variant* ZIG(const Variant& k, const Variant& n, const IHistoryData* pHistory, Node* pNode);
	Variant* SAR(const Variant& n, const Variant& s, const Variant& m, const IHistoryData* pHistory, Node* pNode);
	Variant* SARTURN(const Variant& n, const Variant& s, const Variant& m, const IHistoryData* pHistory, Node* pNode);
	Variant* TROUGHBARS(const Variant& n, const Variant& s, const Variant& m, const IHistoryData* pHistory, Node* pNode);
	Variant* TROUGH(const Variant& k, const Variant& n, const Variant& m, const IHistoryData* pHistory, Node* pNode);

	//字符串函数
	Variant* NUMTOSTR(const Variant& data);
	Variant* NUMTOSTRN(const Variant& data, const Variant& m);
	Variant* STRCMP(const Variant& data, const Variant& data2);
	Variant* STRTONUM(const Variant& data);
	Variant* INBLOCK(const Variant& data, const IHistoryData* pHistory, Node* pNode);
	Variant* LOWERSTR(const Variant& data);
	Variant* UPPERSTR(const Variant& data);
	Variant* STRFIND(const Variant& data, const Variant& findData, const Variant& n);
	Variant* STRLEFT(const Variant& data, const Variant& n);
	Variant* STRMID(const Variant& data, const Variant& first, const Variant& n);
	Variant* STRRIGHT(const Variant& data, const Variant& n);
	Variant* DATESTR(const Variant& data);
	Variant* CON2STR(const Variant& data, const Variant& n);
	Variant* VAR2STR(const Variant& data, const Variant& n);
	Variant* STRCAT(const Variant& data, const Variant& data2);
	Variant* VARCAT(const Variant& data, const Variant& data2);
	Variant* STRSPACE(const Variant& data);

	//时间序列
	Variant* DATETODAY(const Variant& data);
	Variant* DAYTODATE(const Variant& data);

	//自定义函数调用
	Variant* CallScript(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);
	
	//通达信 指标引用 CALCSTOCKINDEX(品种代码,指标名称,指标线) 
	Variant* CALCSTOCKINDEX(const Variant& varSymbol, const Variant& varIndexName, const Variant& varOutID, const IHistoryData* pHistoryData, Node* pNode);
	//东方财富 指标引用 用法:STKINDI('STKNAME','INDINAME',PARAMs)，STKNAME是股票代码，INDINAME是指标及其指标线名称,PARAM是可选参数，其数量应当等于被应用指标的参数数量，否则将使用缺省参数数值。
	Variant* STKINDI(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);

	//执行指标
	bool ExecuteIndex(const InvokeIndex& index, Execute* pExec, HQChart::Complier::IHistoryData* pHistoryData, Node* pNode);
	INVOKE_INDEX_ITEM GetFunctionInfo(const std::wstring& str, const IHistoryData* pHistoryData);
	bool IsVaildPeriod(long lPeriod, long lIndexPeriod) const;

	//绘图函数
	Variant* CallDrawFunction(const std::wstring& strFuncName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);

	Variant* DRAWKLINE(const Variant& varHigh, const Variant& varOpen, const Variant& varLow, const Variant& varClose);
	Variant* STICKLINE(const Variant& cond, const Variant& data, const Variant& data2, const Variant& width, const Variant& drawType);
	Variant* DRAWICON(const Variant& cond, const Variant& data, const Variant& iconID);
	Variant* DRAWTEXT(const Variant& cond, const Variant& data, const Variant& text);
	Variant* DRAWNUMBER(const Variant& cond, const Variant& data, const Variant& number);
	Variant* DRAWBAND(const Variant& data, const Variant& color, const Variant& data2, const Variant& color2);
	Variant* PLOYLINE(const Variant& cond, const Variant& data);
	Variant* DRAWLINE(const Variant& cond, const Variant& data, const Variant& cond2, const Variant& data2, const Variant& expend);
	Variant* DRAWTEXT_FIX(const Variant& cond, const Variant& x, const Variant& y, const Variant& type, const Variant& text);
	Variant* DRAWNUMBER_FIX(const Variant& cond, const Variant& x, const Variant& y, const Variant& type, const Variant& number);
	Variant* COLOR_RGB(const Variant& r, const Variant& g, const Variant& b);
	Variant* COLOR_ARGB(const Variant& a,const Variant& r, const Variant& g, const Variant& b);
	Variant* COLOR_RGBA(const Variant& r, const Variant& g, const Variant& b, const Variant& a);
	Variant* FILLRGN(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData,Node* pNode);
	Variant* PARTLINE(const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode);
	Variant* EXPLAIN(const Variant& cond, const Variant& text);
	Variant* DRAWCOLORKLINE(const Variant& cond, const Variant& varColor, const Variant& varEmpty, const IHistoryData* pHistoryData);
	Variant* ICON(const Variant& varPos, const Variant& varIconID, const IHistoryData* pHistoryData);

	static void STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);
	static void STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& cond, double dData, const ARRAY_DOUBLE& data2);
	static void STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& cond, double dData, double dData2);
	static void STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);
	static void STICKLINE(ARRAY_DOUBLE& dest, ARRAY_DOUBLE& dest2, double dData, const ARRAY_DOUBLE& data2);

	static void DRAWCOLORKLINE(ARRAY_DOUBLE& destOpen, ARRAY_DOUBLE& destHigh, ARRAY_DOUBLE& destLow, ARRAY_DOUBLE& destClose, const ARRAY_DOUBLE& cond, const IHistoryData* pHistoryData);
	static void DRAWCOLORKLINE(ARRAY_DOUBLE& destOpen, ARRAY_DOUBLE& destHigh, ARRAY_DOUBLE& destLow, ARRAY_DOUBLE& destClose, double cond,const IHistoryData* pHistoryData);

	static void DRAWLINE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& condition, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& condition2, const ARRAY_DOUBLE& data2, long lExpand);
	static void CalculateDrawDataExtendLine(ARRAY_DOUBLE& dest, int nCount);
	

	static void MeetConditionData(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data);
	static void MeetConditionData(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, double dData);

	static void PLOYLINE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data);


	//变量方法
	Variant* GetCurrentTime();
	Variant* GetCurrentDate();
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static long GetFirstVaildIndex(const ARRAY_DOUBLE& data);

	static void MAX(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);
	static void MAX(ARRAY_DOUBLE& dest, const double dData, const ARRAY_DOUBLE& data2);

	static void MIN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);
	static void MIN(ARRAY_DOUBLE& dest, const double dData, const ARRAY_DOUBLE& data2);

	static void RAND(ARRAY_DOUBLE& dest, double dData, int nCount);
	static void RAND(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data);

	static void ROUND(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data);
	static void ROUND2(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, int nDecimal);

	static void MA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount);
	static void MA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void COUNT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);
	static void SUM(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);

	static void EMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount);
	static void EMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void SMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, double m);

	static void DMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void WMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount);
	static void WMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void XMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static void EXPMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount);
	static void EXPMEMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lDayCount);

	static void AMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dA);
	static void TMA(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dA, double dB);
	static void TMA(ARRAY_DOUBLE& dest, double dData, double dA, double dB, int nCount);
	
	static void VALUEWHEN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, double dValue);
	static void VALUEWHEN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& cond, const ARRAY_DOUBLE& data);

	static void HARMEAN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, int n);
	static void HARMEAN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);

	static void LLV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lPeriod);
	static void LLV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void HHV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lPeriod);
	static void HHV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void LOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);
	static void LOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static void HOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);
	static void HOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static void BACKSET(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void FINDLOW(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t);
	static void FINDHIGH(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t);

	static void FINDHIGHBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t);
	static void FINDLOWBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n, long m, long t);
	

	static void LLVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void LLVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);
	static void HHVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void HHVBARS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void AVEDEV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void STD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void DEVSQ(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void FORCAST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void SLOPE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void STDP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void VAR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void VARP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void COVAR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, long n);

	static void REF(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void REF(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);

	static void REFV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void REFV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);

	static void REFX(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void REFX(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);

	static void REFXV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void REFXV(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& n);

	static bool CalculateChip(ChipData& chipData, int nIndex, const ARRAY_DOUBLE& exchangeData, const IHistoryData* pHistory, double dRate = 1);	//计算筹码
	static bool CalculateLChip(ChipData& chipData, int nIndex, const ARRAY_DOUBLE& exchangeData, const IHistoryData* pHistory, double dRate,long ln);	//计算近期筹码
	static bool CalculatePChip(ChipData& chipData, int nIndex, const ARRAY_DOUBLE& exchangeData, const IHistoryData* pHistory, double dRate, long ln);	//计算远期筹码


	static void ZIG(ARRAY_DOUBLE& dest, const long k, double n, const IHistoryData* pHistory);
	static void ZIG(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double n);

	static void EVERY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static void NDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, long n);
	static void NDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dData2, long n);
	static void NDAY(ARRAY_DOUBLE& dest, double dData, const ARRAY_DOUBLE& data2, long n);

	static void UPNDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void DOWNNDAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void EXIST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static void LAST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lEnd, long lStart);

	static void EXISTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lEnd, long lStart);
	static void EXIST(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);

	static void SAR(ARRAY_DOUBLE& dest, long n, double dStep, double dExValue, const IHistoryData* pHistory);

	static void RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, const ARRAY_DOUBLE& b, const ARRAY_DOUBLE& c);
	static void RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, double dB, double dC);
	static void RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, const ARRAY_DOUBLE& b, double dC);
	static void RANGE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, double dB, const ARRAY_DOUBLE& c);

	static void MOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, const ARRAY_DOUBLE& b);
	static void MOD(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& a, double dB);
	static double MOD(double dA, double dB);

	static void FILTER(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void TFILTER(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& buy, const ARRAY_DOUBLE& sell, long n);

	static void ALL(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);
	static void ALL(ARRAY_DOUBLE& dest, double dValue, long n, const IHistoryData* pHistory);
	static void ANY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static void NUMTOSTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long ldecimal, bool bRemoveZero, VariantOperator* pVariantOperator);
	static void RemoveStringZero(wchar_t* lpStr);

	static void STRCMP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& leftData, const ARRAY_DOUBLE& rightData);
	static void STRCMP(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& leftData, const std::wstring& strRight);
	static void STRCMP(ARRAY_DOUBLE& dest, const std::wstring& strLeft, const ARRAY_DOUBLE& rightData);

	static void STRTONUM(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data);

	static void LOWERSTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, VariantOperator* pVariantOperator);
	static void UPPERSTR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, VariantOperator* pVariantOperator);

	static void STRFIND(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const std::wstring strFind, long n);
	static void STRLEFT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lCount, VariantOperator* pVariantOperator);
	static void STRRIGHT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lCount, VariantOperator* pVariantOperator);
	static void STRMID(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long lFirst,long lCount, VariantOperator* pVariantOperator);
	static void VAR2STR(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long ln, VariantOperator* pVariantOperator);
	static void VARCAT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const std::wstring& str2, VariantOperator* pVariantOperator);
	static void VARCAT(ARRAY_DOUBLE& dest, const std::wstring& str, const ARRAY_DOUBLE& data2, VariantOperator* pVariantOperator);
	static void VARCAT(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2, VariantOperator* pVariantOperator);

	static void StringRight(std::wstring& str, long lCount);
	static void StringLeft(std::wstring& str, long lCount);
	static void StringMid(std::wstring& str, long lFirst, long lCount);

	static void ZTPRICE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dRate);
	static void DTPRICE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dRate);

	static void CROSS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, const ARRAY_DOUBLE& data2);
	static void CROSS(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, double dData2);
	static void CROSS(ARRAY_DOUBLE& dest, double dData, const ARRAY_DOUBLE& data2);

	static void DATETODAY(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data);
	static void DAYTODATE(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data);

	static void BARSSINCEN(ARRAY_DOUBLE& dest, const ARRAY_DOUBLE& data, long n);

	static std::vector<std::wstring>  StringSplit(const std::wstring& str, const std::wstring& strPattern);
	
};



class ScriptIndex
{
public:
	typedef std::vector<ARGUMENT_ITEM>	ARRAY_ARGUMENT;

	ScriptIndex();
	ScriptIndex(const std::wstring& strName, const std::wstring& strCode, const std::wstring& strDescription);
	ScriptIndex(const std::wstring& strName, const std::wstring& strCode, const ARRAY_ARGUMENT& argArgs, const std::wstring& strDescription);

	void SetName(const std::wstring& strValue) { m_strName = strValue; }
	void SetCode(const std::wstring& strValue) { m_strCode = strValue; }
	void SetDescription(const std::wstring& strValue) { m_strDescription = strValue; }
	void SetArgument(const ARRAY_ARGUMENT& argArgs) { m_aryArgument = argArgs; }

	const std::wstring& GetName() const { return m_strName; }
	const std::wstring& GetCode() const { return m_strCode; }
	const ARRAY_ARGUMENT& GetArgument() const { return m_aryArgument; }
	const std::wstring& GetDescription() const { return m_strDescription; }

	void CopyTo(ScriptIndex& dest) const;

private:

	std::wstring m_strName;			//名称
	std::wstring m_strCode;			//代码
	std::wstring m_strDescription;	//描述信息
	ARRAY_ARGUMENT m_aryArgument;	//参数
};

class ScriptLibrary
{
public:
	typedef std::vector<ARGUMENT_ITEM>	ARRAY_ARGUMENT;

	static ScriptLibrary& GetInstance();
	
	const ScriptIndex* Get(const std::wstring& strName) const;
	bool Load();

	void Add(std::wstring& strName, std::wstring& strCode, std::wstring& strDescription);
	void Add(std::wstring& strName, std::wstring& strCode, const ARRAY_ARGUMENT& argArgs, std::wstring& strDescription);

private:
	ScriptLibrary();
	~ScriptLibrary();

	typedef std::map<std::wstring, ScriptIndex> MAP_FUNCTION;
	
	MAP_FUNCTION m_mapFunction;

	// 系统指标
	void MACD();
	void KDJ();
	void MA();
	void WR();
	void BOLL();
	void BBI();
	void DKX();
	void MIKE();

};

}
}
