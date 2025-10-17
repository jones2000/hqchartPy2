#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "HQChart.Interface.Data.h"
#include "..\HQChart.CSharp\hqchart.csharp.h"


using namespace HQChart::Complier;

typedef int (*PMainVersion)();
typedef int (*PMinVersion)();
typedef bool (*PRun)(const wchar_t* pJonsConfig, HQCHART_CALLBACK_PTR callback);
typedef bool (*PAddFunction)(const wchar_t* pStrName, int nArgCount);
typedef bool (*PAddVariant)(const wchar_t* pStrName);
typedef const wchar_t* (*PGetAuthorizeInfo)();

class HQChartControl
{
public:
	HQChartControl();
	~HQChartControl();

	//初始化
	bool Inital();

	void Release();

	int GetMainVersion() const { if (m_pMainVersion) return m_pMainVersion(); return 0; }
	int GetMinVersion() const { if (m_pMinVersion) return m_pMinVersion(); return 0; }
	std::wstring GetAuthorizeInfo() const;

	bool Run(const std::wstring& strJson);

private:

	HMODULE m_hDll = nullptr;
	std::wstring m_strDllPath=L"hqchart.csharp.dll";


	PMainVersion m_pMainVersion = nullptr;
	PMinVersion m_pMinVersion = nullptr;
	PRun m_pRun = nullptr;
	PAddFunction m_pAddFunction = nullptr;
	PAddVariant m_pAddVariant = nullptr;
	PGetAuthorizeInfo m_pGetAuthorizeInfo = nullptr;
};

class HQChartCache
{
public:
	typedef std::shared_ptr<HQChartCache> REF_PTR;

	std::vector<HQCHART_KDATA> m_aryKLineData;	//K线数据缓存
	std::vector<VALUE_ITEM> m_aryData;		//缓存数据
	std::wstring m_strSymbol;
	std::wstring m_strError;
	std::wstring m_strName;

	std::wstring m_strResult;	//计算结果
};

class HQChartCacheManage
{
public:
	static HQChartCacheManage& GetInstance();

	HQChartCache::REF_PTR Get(const std::wstring& strKey) const;
	void Create(const std::wstring& strKey);

private:
	HQChartCacheManage();
	~HQChartCacheManage();

	std::map<std::wstring, HQChartCache::REF_PTR> m_MapCache;
};


//数据对接函数
bool HQChart_LoadKData(const wchar_t* lpSymbol, long lPeriod, long lRight, HQCHART_KDATA_RESULT* pResult, const wchar_t* pstrGuid);
bool HQChart_LoadKData2(const wchar_t* lpSymbol, long lPeriod, long lRight, const wchar_t* lpCallInfo, const HQCHART_KDATA_INFO* kDataInfo, HQCHART_KDATA_RESULT* pResult, const wchar_t* pstrGuid);

bool HQChart_GetDataByNumber(const wchar_t* lpSymbol, const wchar_t* pFunctionName, long lParam, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid);
bool HQChart_GetDataByNumbers(const wchar_t* lpSymbol, const wchar_t* pFunctionName, const CUSTOM_FUNCTION_ARGUMENT* pArgument, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid);
bool HQChart_GetDataByName(const wchar_t* lpSymbol, const wchar_t* pFunctionName, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid);

const wchar_t* HQChart_GetIndexScript(const wchar_t* lpName, const wchar_t* lpCallInfo, const wchar_t* pstrGuid);

void HQChart_Success(const wchar_t* lpSymbol, const wchar_t* lpJSResul, const wchar_t* pstrGuid);
void HQChart_Failed(const wchar_t* lpCode, const wchar_t* lpSymbol, const wchar_t* lpError, const wchar_t* pstrGuid);


bool HQChart_Test(const wchar_t* lpSymbol);


class JsonFile
{
public:
	JsonFile();
	~JsonFile();

	//FILE* GetFile() const { return m_pFile; }

	bool Open(const wchar_t* lpFile);
	const rapidjson::Value& GetNode(const char* lpName);

private:
	FILE* m_pFile = NULL;
	rapidjson::Document m_JsonRoot;
};

