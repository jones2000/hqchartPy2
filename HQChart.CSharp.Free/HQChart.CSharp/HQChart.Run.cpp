
#include "hqchart.csharp.h"
#include <thread>
#include <string>

#include "HQChart.CallbackData.h"
#include "HQChart.RunConfig.h"
#include "HQChart.Log.h"
#include "HQChart.Execute.h"


const int MAIN_VERSION = 1;
const int MIN_VERSION = 65;	//后5位是小版本号
std::wstring AUTHORIZE_INFO = L"HQChart for c# 社区版 by Jones";


HQCHART_DLL_API int MainVersion()
{
	return MAIN_VERSION;
}

HQCHART_DLL_API int MinVersion()
{
	return MIN_VERSION;
}


HQCHART_DLL_API bool Run(const wchar_t* pJonsConfig, HQCHART_CALLBACK_PTR callback)
{
#ifdef _DEBUG
	HQChart::Complier::Log::SetLogStatus(1);
#endif

	ScriptLibrary::GetInstance();

	auto& customFunc = HQChart::Complier::CustomFunction::GetInstance();
	customFunc.Add(L"HK2SHSZ", 0);
	customFunc.Add(L"MARGIN", 1);
	customFunc.Add(L"EXTDATA_USER", 2);

	auto& customVar = HQChart::Complier::CustomVariant::GetInstance();
	customVar.Add(L"GONORTH");
	customVar.Add(L"MARGIN");
	customVar.Add(L"GONORTHR");
	customVar.Add(L"MARGINR");

	customVar.Add(L"HYBLOCK");		//所属行业板块
	customVar.Add(L"DYBLOCK");		//所属地域板块
	customVar.Add(L"GNBLOCK");		//所属概念
	customVar.Add(L"FGBLOCK");		//所属风格板块
	customVar.Add(L"ZSBLOCK");		//所属指数板块
	customVar.Add(L"ZHBLOCK");		//所属组合板块
	customVar.Add(L"ZDBLOCK");		//所属自定义板块
	customVar.Add(L"HYZSCODE");
	customVar.Add(L"GNBLOCKNUM");	//所属概念板块的个数
	customVar.Add(L"FGBLOCKNUM");	//所属风格板块的个数
	customVar.Add(L"ZSBLOCKNUM");	//所属指数板块的个数
	customVar.Add(L"ZHBLOCKNUM");	//所属组合板块的个数
	customVar.Add(L"ZDBLOCKNUM");	//所属自定义板块的个数
	customVar.Add(L"HYSYL");		//指数市盈率或个股所属行业的市盈率
	customVar.Add(L"HYSJL");		//指数市净率或个股所属行业的市净率

	HQChart::Complier::RunConfig config;
	config.SetCallbackProc(callback);
	if (!config.LoadJsonConfig(pJonsConfig)) return false;

	bool bSuccess = config.RunScript(config);
	return bSuccess;
}

HQCHART_DLL_API bool HQ_CALL AddFunction(const wchar_t* pStrName, int nArgCount)
{
	auto& func = HQChart::Complier::CustomFunction::GetInstance();
	std::wstring strName(pStrName);
	if (strName.empty()) return false;
	if (nArgCount < 0) return false;

	func.Add(pStrName, nArgCount);
	return true;
}

HQCHART_DLL_API bool HQ_CALL AddVariant(const wchar_t* pStrName)
{
	auto& customVar = HQChart::Complier::CustomVariant::GetInstance();
	std::wstring strName(pStrName);
	if (strName.empty()) return false;

	customVar.Add(strName);

	return true;
}

HQCHART_DLL_API const wchar_t* HQ_CALL GetAuthorizeInfo()
{
	return AUTHORIZE_INFO.c_str();
}
