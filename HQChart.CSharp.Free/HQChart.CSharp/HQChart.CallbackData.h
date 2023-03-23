/*
Copyright (c) 2018 jones

http://www.apache.org/licenses/LICENSE-2.0

开源项目 https://github.com/jones2000/HQChart

jones_2000@163.com

股票数据回调接口类 (c++)
数据通过回调函数获取

*/

#pragma once
#include <vector>
#include "HQChart.KLineData.h"
#include "HQChart.Execute.h"
#include "hqchart.csharp.h"



namespace HQChart { namespace Complier {

class HistoryDataCallback;
class RunConfig;

class HistoryDataCallback : public HistoryDataCache
{
public:
	typedef HistoryDataCache::ARRAY_KDATA ARRAY_KDATA;
	typedef HistoryDataCache::ARRAY_OVERLAY_DATA ARRAY_OVERLAY_DATA;
	typedef HistoryDataCache::ARRAY_FINANCE_DATA ARRAY_FINANCE_DATA;
	typedef HistoryDataCache::ARRAY_EXTEND_DATA ARRAY_EXTEND_DATA;

	HistoryDataCallback(const std::wstring& strSymbol, long lPeriod, long lRight);
	HistoryDataCallback();
	virtual ~HistoryDataCallback();

	void SetSymbol(const std::wstring& strSymbol, long lPeriod, long lRight);

	void SetRunConfig(RunConfig* pRunConfig) { m_pRunConfig = pRunConfig; }

	virtual void Initialization(void* pData);
	virtual bool LoadKData();				//加载K线
	virtual bool LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo);
	virtual Variant* GetFromOpen(Node* pNode) const;	//开盘时间

	virtual Variant* GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetExchange(Node* pNode) const;	//换手率
	virtual Variant* GetCapital(Node* pNode) const;		//获取当前的流通股本 单位手
	virtual Variant* GetTotalCaptialAll(Node* pNode) const;	//TOTALCAPITAL  当前总股本 手
	virtual Variant* GetHisCapital(Node* pNode) const;	//获取历史所有的流通股本 单位股
	virtual Variant* GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetCustomValue(const std::wstring& strName, Node* pNode) const;

	virtual Variant* GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;	//板块股票个数
	virtual Variant* GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;		//多股统计
	virtual Variant* CallCustomFunction(const std::wstring& strName, const std::vector<double>& args, Node* pNode) const;	//自定义函数

	virtual bool GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const;	//获取系统指标脚本

	virtual void Reset();

protected:

	virtual bool LoadIndexData(const std::wstring& strName, Node* pNode) const;		//加载大盘数据 (用到才加载)

	Variant* Invoke_GetDataByNumber(const std::wstring& strSymbol, const std::wstring& strFunctionName, long lParam, long lKCount, Node* pNode) const;
	Variant* Invoke_GetDataByName(const std::wstring& strSymbol, const std::wstring& strFunctionName, long lKCount, Node* pNode) const;
	Variant* Invoke_GetDataByNumbers(const std::wstring& strSymbol, const std::wstring& strFunctionName, const std::vector<double>& aryArgs, long lKCount, Node* pNode) const;
	Variant* ValueResultToVariant(const HQCHART_VALUE_RESULT& result, Node* pNode, const std::wstring& strCallInfo) const;

	bool GetSystemIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const;	//获取系统指标脚本

	bool Invoke_LoadKData2(const std::wstring& strSymbol, long lPeriod, long lRight, ARRAY_KDATA& aryData, Node* pNode, const std::wstring& strCallInfo) const;

	//缓存读取
	bool GetCacheKData(const std::wstring& strSymbol, long lPeriod, long lRight, std::wstring* pStrName);
	bool GetCacheKData(const std::wstring& strSymbol, long lPeriod, long lRight, ARRAY_KDATA& aryData, std::wstring* pStrName) const;
	bool GetCacheDynaInfo(const std::wstring& strSymbol, long lId, double& dValue) const;

	//股票数据拟合到K线上
	void FitStockData(Variant& out, const ARRAY_FINANCE_DATA& fData) const;	// 平滑处理
	void FitStockData(Variant& out, const ARRAY_FINANCE_DATA& fData, double dDefault, bool bExactMatch) const;	//不平滑处理

	void FitKData(Variant& out, const ARRAY_KDATA& aryData,long lPeriod) const;
	void FitOverlayKData(ARRAY_OVERLAY_DATA& out, const ARRAY_KDATA& aryData, long lPeriod) const;

	RunConfig* m_pRunConfig = NULL;

	mutable ARRAY_KDATA m_aryTempCacheData;				//临时缓存数据
};


}
}