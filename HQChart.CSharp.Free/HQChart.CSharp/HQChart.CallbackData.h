/*
Copyright (c) 2018 jones

http://www.apache.org/licenses/LICENSE-2.0

��Դ��Ŀ https://github.com/jones2000/HQChart

jones_2000@163.com

��Ʊ���ݻص��ӿ��� (c++)
����ͨ���ص�������ȡ

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

	//�������
	typedef std::vector<ARG_ARRAY_ITEM> ARRAY_ARG_VALUE;
	typedef std::shared_ptr<ARRAY_ARG_VALUE> REF_ARRAY_ARG_VALUE;

	HistoryDataCallback(const std::wstring& strSymbol, long lPeriod, long lRight);
	HistoryDataCallback();
	virtual ~HistoryDataCallback();

	void SetSymbol(const std::wstring& strSymbol, long lPeriod, long lRight);

	void SetRunConfig(RunConfig* pRunConfig) { m_pRunConfig = pRunConfig; }

	virtual void Initialization(void* pData);
	virtual bool LoadKData();				//����K��
	virtual bool LoadKData(const std::wstring& strSymbol, long lPeriod, long lRight, Node* pNode, const std::wstring& strCallInfo);
	virtual Variant* GetFromOpen(Node* pNode) const;	//����ʱ��

	virtual Variant* GetFinance(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetExchange(Node* pNode) const;	//������
	virtual Variant* GetCapital(Node* pNode) const;		//��ȡ��ǰ����ͨ�ɱ� ��λ��
	virtual Variant* GetTotalCaptialAll(Node* pNode) const;	//TOTALCAPITAL  ��ǰ�ܹɱ� ��
	virtual Variant* GetHisCapital(Node* pNode) const;	//��ȡ��ʷ���е���ͨ�ɱ� ��λ��
	virtual Variant* GetDynaInfo(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;
	virtual Variant* GetCustomValue(const std::wstring& strName, Node* pNode) const;

	virtual Variant* GetBlockMemberCount(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;	//����Ʊ����
	virtual Variant* GetBlockCalculate(const ARRAY_CALL_ARGUMENT& args, Node* pNode) const;		//���ͳ��
	virtual Variant* CallCustomFunction(const std::wstring& strName, const ARRAY_CALL_ARGUMENT& args, const IHistoryData* pHistoryData, Node* pNode) const;	//�Զ��庯��

	virtual bool GetBlockMember(const std::wstring& strBlockID, long lDate, std::vector<std::wstring>& arySymbol, Node* pNode) const;			//��ȡ���ɷ�
	virtual bool GetIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const;	//��ȡϵͳָ��ű�

	virtual void Reset();

protected:

	virtual bool LoadIndexData(const std::wstring& strName, Node* pNode) const;		//���ش������� (�õ��ż���)

	Variant* Invoke_GetDataByNumber(const std::wstring& strSymbol, const std::wstring& strFunctionName, long lParam, long lKCount, Node* pNode) const;
	Variant* Invoke_GetDataByName(const std::wstring& strSymbol, const std::wstring& strFunctionName, long lKCount, Node* pNode) const;
	Variant* Invoke_GetDataByNumbers(const std::wstring& strSymbol, const std::wstring& strFunctionName, const ARRAY_CALL_ARGUMENT& args, long lKCount, const IHistoryData* pHistoryData, Node* pNode) const;
	Variant* ValueResultToVariant(const HQCHART_VALUE_RESULT& result, Node* pNode, const std::wstring& strCallInfo) const;

	bool GetSystemIndexScript(const std::wstring& strName, const std::wstring& strCallInfo, ScriptIndex& script, Node* pNode) const;	//��ȡϵͳָ��ű�

	bool Invoke_LoadKData2(const std::wstring& strSymbol, long lPeriod, long lRight, ARRAY_KDATA& aryData, Node* pNode, const std::wstring& strCallInfo) const;

	//�����ȡ
	bool GetCacheKData(const std::wstring& strSymbol, long lPeriod, long lRight, std::wstring* pStrName);
	bool GetCacheKData(const std::wstring& strSymbol, long lPeriod, long lRight, ARRAY_KDATA& aryData, std::wstring* pStrName) const;
	bool GetCacheDynaInfo(const std::wstring& strSymbol, long lId, double& dValue) const;

	//��Ʊ������ϵ�K����
	void FitStockData(Variant& out, const ARRAY_FINANCE_DATA& fData) const;	// ƽ������
	void FitStockData(Variant& out, const ARRAY_FINANCE_DATA& fData, double dDefault, bool bExactMatch) const;	//��ƽ������

	void FitKData(Variant& out, const ARRAY_KDATA& aryData,long lPeriod) const;
	void FitOverlayKData(ARRAY_OVERLAY_DATA& out, const ARRAY_KDATA& aryData, long lPeriod) const;

	RunConfig* m_pRunConfig = NULL;

	mutable ARRAY_KDATA m_aryTempCacheData;				//��ʱ��������
};


}
}