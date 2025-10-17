#include "pch.h"
#include "HQChartControl.h"
#include <functional> 

HQChartControl::HQChartControl()
{

}


HQChartControl::~HQChartControl()
{
	Release();
}


bool HQChartControl::Inital()
{
	Release();
	m_hDll=::LoadLibraryW(m_strDllPath.c_str());
	if (!m_hDll) return false;

	//接口函数
	m_pMainVersion = (PMainVersion)::GetProcAddress(m_hDll, "MainVersion");
	m_pMinVersion= (PMinVersion)::GetProcAddress(m_hDll, "MinVersion");
	m_pRun= (PRun)::GetProcAddress(m_hDll, "Run");
	m_pAddFunction = (PAddFunction)::GetProcAddress(m_hDll, "AddFunction");
	m_pAddVariant = (PAddVariant)::GetProcAddress(m_hDll, "AddVariant");
	m_pGetAuthorizeInfo= (PGetAuthorizeInfo)::GetProcAddress(m_hDll, "GetAuthorizeInfo");

	//添加变量
	if (m_pAddVariant)
	{
		m_pAddVariant(L"USER_VAR1");	//添加变量
		m_pAddVariant(L"USER_VAR2");	//添加变量
	}

	//添加函数
	if (m_pAddFunction)
	{
		m_pAddFunction(L"USER_FUNC1", 1);	//1个参数
	}
	
	return true;
}


void HQChartControl::Release()
{
	if (!m_hDll) return;

	::FreeLibrary(m_hDll);
	m_hDll = nullptr;

	m_pMainVersion = nullptr;
	m_pMinVersion = nullptr;
	m_pRun = nullptr;
	m_pAddFunction = nullptr;
	m_pGetAuthorizeInfo = nullptr;
}

bool HQChartControl::Run(const std::wstring& strJson)
{
	if (!m_pRun) return false;

	HQCHART_CALLBACK_PTR callbackInfo = { 0 };
	callbackInfo._pLoadKData = HQChart_LoadKData;
	callbackInfo._pLoadKData2 = HQChart_LoadKData2;

	callbackInfo._pGetDataByNumber = HQChart_GetDataByNumber;
	callbackInfo._pGetDataByNumbers= HQChart_GetDataByNumbers;
	callbackInfo._pGetDataByName= HQChart_GetDataByName;
	callbackInfo._pGetIndexScript= HQChart_GetIndexScript;
	callbackInfo._pSuccess= HQChart_Success;
	callbackInfo._pFailed= HQChart_Failed;
	callbackInfo._pTest= HQChart_Test;

	bool bResult = m_pRun(strJson.c_str(), callbackInfo);

	return bResult;
}


std::wstring HQChartControl::GetAuthorizeInfo() const
{
	std::wstring strValue = L"";
	if (m_pGetAuthorizeInfo) strValue = m_pGetAuthorizeInfo();

	return strValue;
}


HQChartCacheManage::HQChartCacheManage()
{

}

HQChartCacheManage::~HQChartCacheManage()
{

}

HQChartCacheManage& HQChartCacheManage::GetInstance()
{
	static HQChartCacheManage instance;
	return instance;
}

HQChartCache::REF_PTR HQChartCacheManage::Get(const std::wstring& strKey) const
{
	//TODO:多线程 需要加锁 只读锁就可以
	auto finder = m_MapCache.find(strKey);
	if (finder == m_MapCache.end()) return NULL;

	return finder->second;
}

void HQChartCacheManage::Create(const std::wstring& strKey)
{
	if (strKey.empty()) return;
	if (m_MapCache.find(strKey) != m_MapCache.end()) return;

	HQChartCache::REF_PTR pRef = HQChartCache::REF_PTR(new HQChartCache);

	m_MapCache[strKey] = pRef;
}


bool HQChart_LoadKData(const wchar_t* lpSymbol, long lPeriod, long lRight, HQCHART_KDATA_RESULT* pResult, const wchar_t* pstrGuid)
{
	wchar_t szPath[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	(wcsrchr(szPath, L'\\'))[1] = 0;		//删除文件名，只获得路径字串

	std::wstring strGuid = pstrGuid;
	HQChartCache::REF_PTR pRefCache = HQChartCacheManage::GetInstance().Get(strGuid);

	wchar_t szFile[MAX_PATH] = { 0 };
	wsprintfW(szFile, L"%s%s.json", szPath, lpSymbol);
	//TRACE_DEBUG(L"[HistoryData::LoadKData] Load original kline data. symbol=%s, m_lPeriod=%d lMainPeriod=%d file=%s", m_strSymbol.c_str(), m_lPeriod, lMainPeriod, szFile);

	JsonFile file;
	if (!file.Open(szFile))
	{
		//TRACE_WARNING(L"[HistoryData::LoadKData] Load original kline data failed. symbol=%s, m_lPeriod=%d lMainPeriod=%d file=%s", m_strSymbol.c_str(), m_lPeriod, lMainPeriod, szFile);
		return false;
	}

	const Value& aryData = file.GetNode("data");
	long lCount = (long)aryData.Size();

	const Value& jsName = file.GetNode("name");
	std::string strName(jsName.GetString());
	pRefCache->m_strName = UTF8ToWString(strName);

	pRefCache->m_aryKLineData.clear();
	pRefCache->m_aryKLineData.resize(lCount, HISTORY_ITEM());
	for (SizeType i = 0; i < aryData.Size(); ++i)
	{
		const Value& jsonItem = aryData[i];
		HISTORY_ITEM& item = pRefCache->m_aryKLineData[i];
		
		item._nDate = jsonItem[0].GetInt();
		item._dYClose = jsonItem[1].GetDouble();
		item._dOpen = jsonItem[2].GetDouble();
		item._dClose = jsonItem[5].GetDouble();
		item._dHigh = jsonItem[3].GetDouble();
		item._dLow = jsonItem[4].GetDouble();
		item._dAmount = jsonItem[7].GetDouble();
		item._dVol = jsonItem[6].GetDouble();
	}


	pResult->_pszName = (wchar_t* )pRefCache->m_strName.c_str();
	pResult->_lPeriod = lPeriod;
	pResult->_lRight = lRight;
	pResult->_lCount = (long)pRefCache->m_aryKLineData.size();
	if (pResult->_lCount>0) pResult->_pData = &pRefCache->m_aryKLineData[0];

	return true;
}

bool HQChart_LoadKData2(const wchar_t* lpSymbol, long lPeriod, long lRight, const wchar_t* lpCallInfo, const HQCHART_KDATA_INFO* kDataInfo, HQCHART_KDATA_RESULT* pResult, const wchar_t* pstrGuid)
{
	return false;
}

bool HQChart_GetDataByNumber(const wchar_t* lpSymbol, const wchar_t* pFunctionName, long lParam, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid)
{
	return false;
}

bool HQChart_GetDataByNumbers(const wchar_t* lpSymbol, const wchar_t* pFunctionName, const CUSTOM_FUNCTION_ARGUMENT* pArgument, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid)
{
	std::wstring strGuid = pstrGuid;
	HQChartCache::REF_PTR pRefCache = HQChartCacheManage::GetInstance().Get(strGuid);

	std::wstring strFuncName = pFunctionName;
	if (strFuncName == L"USER_FUNC1") //返回入参+100
	{
		const auto& aryItem = pArgument->_Args[0];
		if (aryItem._lType == 1)	//数组
		{
			long lCount = aryItem._lCount;
			pRefCache->m_aryData.clear();
			pRefCache->m_aryData.resize(lCount, VALUE_ITEM());
			for (int i = 0; i < lCount; ++i)
			{
				const auto& item = aryItem._pAryValue[i];
				auto& destItem = pRefCache->m_aryData[i];

				destItem._dValue = item._dValue + 100;
			}

			pResult->_lType = 1;	//数组
			pResult->_lCount = lCount;
			if (lCount > 0) pResult->_pData = &pRefCache->m_aryData[0];

			return true;
		}
		else if (aryItem._lType == 2)	//单值
		{
			pResult->_lType = 0;	//单值
			pResult->_dValue = aryItem._dValue + 100;
			return true;
		}
	}
	return false;
}

bool HQChart_GetDataByName(const wchar_t* lpSymbol, const wchar_t* pFunctionName, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid)
{
	std::wstring strGuid = pstrGuid;
	HQChartCache::REF_PTR pRefCache = HQChartCacheManage::GetInstance().Get(strGuid);

	std::wstring strVarName = pFunctionName;
	if (strVarName == L"CAPITAL") //当前流通股本
	{
		pResult->_lType = 0;	//单值
		pResult->_dValue = 100000000;
		return true;
	}
	else if (strVarName == L"TOTALCAPITAL")    //当前总股本
	{
		pResult->_lType = 0;	//单值
		pResult->_dValue = 100000000;
		return true;
	}
	else if (strVarName == L"USER_VAR1")
	{
		pResult->_lType = 0;	//单值
		pResult->_dValue = 200000;
		return true;
	}
	else if (strVarName == L"USER_VAR2")
	{
		long lCount = pKDataInfo->_lCount;	//数据长度跟K线长度一致
		pRefCache->m_aryData.clear();
		pRefCache->m_aryData.resize(lCount, VALUE_ITEM());
		for (long i = 0; i < lCount; ++i)
		{
			auto& item = pRefCache->m_aryData[i];
			item._dValue = i;
		}

		pResult->_lType = 1;	//数组
		pResult->_lCount = lCount;
		if (lCount > 0) pResult->_pData = &pRefCache->m_aryData[0];
		return true;
	}

	return false;
}

const wchar_t* HQChart_GetIndexScript(const wchar_t* lpName, const wchar_t* lpCallInfo, const wchar_t* pstrGuid)
{
	return NULL;
}

void HQChart_Success(const wchar_t* lpSymbol, const wchar_t* lpJSResul, const wchar_t* pstrGuid)
{
	std::wstring strGuid = pstrGuid;
	HQChartCache::REF_PTR pRefCache = HQChartCacheManage::GetInstance().Get(strGuid);

	pRefCache->m_strResult = lpJSResul;
}

void HQChart_Failed(const wchar_t* lpCode, const wchar_t* lpSymbol, const wchar_t* lpError, const wchar_t* pstrGuid)
{
	std::wstring strGuid = pstrGuid;
	HQChartCache::REF_PTR pRefCache = HQChartCacheManage::GetInstance().Get(strGuid);
	pRefCache->m_strResult = lpError;
}


bool HQChart_Test(const wchar_t* lpSymbol)
{
	return false;
}



//////////////////////////////////////////////////////////////////////////////////////////////
//
//

JsonFile::JsonFile()
{

}

JsonFile::~JsonFile()
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

bool JsonFile::Open(const wchar_t* lpFile)
{
	errno_t err = _wfopen_s(&m_pFile, lpFile, L"rb"); // non-Windows use "r"
	if (err != 0) return false;

	char readBuffer[1024 * 100] = { 0 };
	FileReadStream is(m_pFile, readBuffer, sizeof(readBuffer));
	m_JsonRoot.ParseStream(is);

	return true;
}

const Value& JsonFile::GetNode(const char* lpName)
{
	return m_JsonRoot[lpName];
}