#pragma once
#include <string>
#include <vector>

#include "hqchart.csharp.h"
#include "HQChart.KLineData.h"
#include "HQChart.Execute.h"
using namespace HQChart::Complier;


namespace HQChart { namespace Complier {

class HistoryDataCallback;

class RunConfig
{
public:
	friend class HistoryDataCallback;

	RunConfig();
	~RunConfig();


	void SetCallbackProc(HQCHART_CALLBACK_PTR callback);
	bool LoadJsonConfig(const wchar_t* pJson);

	static bool RunScript(RunConfig& runConfig);

private:
	static bool VerifyAuthorizeInfo(RunConfig& runConfig);

	std::wstring m_strCode;
	std::wstring m_strGuid;

	Execute::ARRAY_ARGUMENT m_aryArgs;
	std::vector<std::wstring> m_arySymbol;

	long m_lPeriod = 0;
	long m_lRight = 0;
	long m_lOutCount = -1;	//-1返回全部
	long m_lKLineCount = 0;	//计算的K线个数 0=全部K线, 从最新的K线往前取.

	long m_lOutStartIndex = -1;	//起始时间
	long m_lOutEndIndex = -1;	//结束时间

	pHQChart_LoadKData m_pLoadKData = NULL;
	pHQChart_LoadKData2 m_pLoadKData2 = NULL;
	pHQChart_GetDataByNumber m_pGetDataByNumber = NULL;
	pHQChart_GetDataByNumbers m_pGetDataByNumbers = NULL;
	pHQChart_GetDataByName m_pGetDataByName = NULL;
	pHQChart_GetIndexScript m_pGetIndexScript = NULL;
	pHQChart_Success m_pSuccess = NULL;
	pHQChart_Failed m_pFailed = NULL;
	

	pHQChart_Test m_pTest = NULL;
};


}
}

