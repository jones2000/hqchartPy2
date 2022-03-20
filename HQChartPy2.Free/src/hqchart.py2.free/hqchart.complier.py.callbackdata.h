/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

股票数据回调接口类 (c++)
数据通过回调函数获取
*/

#pragma once
#include <vector>
#include "HQChart.KLineData.h"
#include "HQChart.Execute.h"
#include <Python.h>


namespace HQChart { namespace Complier { namespace Py {

class RunConfig;

class HistoryDataCallback : public HistoryDataCache
{
public:
	typedef HistoryDataCache::ARRAY_KDATA ARRAY_KDATA;
	typedef HistoryDataCache::ARRAY_FINANCE_DATA ARRAY_FINANCE_DATA;
	typedef HistoryDataCache::ARRAY_EXTEND_DATA ARRAY_EXTEND_DATA;

	HistoryDataCallback(const std::wstring& strSymbol, long lPeriod, long lRight);
	virtual ~HistoryDataCallback();

	void SetRunConfig(RunConfig* pRunConfig) { m_pRunConfig = pRunConfig;}
	void SetSymbol(const std::wstring& strSymbol, long lPeriod, long lRight);
	virtual void Reset();

	virtual void Initialization(void* pData);
	virtual bool LoadKData();				//加载K线
	virtual bool LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo);

	virtual Variant* GetFromOpen() const;	//开盘时间

	virtual Variant* GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetExchange(Node* pNode) const;	//换手率
	virtual Variant* GetCapital(Node* pNode) const;		//获取当前的流通股本 单位手
	virtual Variant* GetHisCapital(Node* pNode) const;	//获取历史所有的流通股本 单位股
	virtual Variant* GetTotalCaptialAll(Node* pNode) const;	//TOTALCAPITAL  当前总股本 手
	virtual Variant* GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetCustomValue(const std::wstring& strName, Node* pNode) const;

	virtual Variant* GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;	//板块股票个数
	virtual Variant* GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;		//多股统计
	virtual Variant* CallCustomFunction(const std::wstring& strName, const std::vector<double>& args, Node* pNode) const;	//自定义函数
	virtual bool GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const;	//获取系统指标脚本

protected:
	virtual bool LoadIndexData(const std::wstring& strCallInfo, Node* pNode) const;

	bool PyDataToKData(const std::wstring& strSymbol, PyObject* pResult, ARRAY_KDATA& aryData, std::wstring& strName, long lPeriod, long lRight) const;
	bool PyDataToVariant(const std::wstring& strInfo, PyObject* pResult, Variant& out) const;
	void UpdateKDataInfo();
	Variant* Invoke_GetDataByNumber(const std::wstring& strFunctionName) const;
	Variant* Invoke_GetDataByNumber(const std::wstring& strFunctionName, long lValue) const;
	Variant* Invoke_GetDataByNumbers(const std::wstring& strFunctionName, const std::vector<double>& aryArgs) const;
	Variant* Invoke_GetDataByName(const std::wstring& strFunctionName) const;
	bool ArgumentToDoubleArray(const ARRAY_CALL_ARGUMENT& args, std::vector<double>& aryArgs, long lCount) const;
	bool GetSystemIndex(const std::wstring& strName, ScriptIndex& script) const;	//获取系统指标

	void FitFinanceData(Variant& out, const ARRAY_FINANCE_DATA& fData) const;
	void FitFinanceData(Variant& out, const ARRAY_FINANCE_DATA& fData, double dDefault, bool bExactMatch) const;
	void FitKData(Variant& out, const ARRAY_KDATA& aryData, long lPeriod) const;

	mutable std::wstring m_strError;

	RunConfig*			m_pRunConfig = NULL;
};	

}
}
}