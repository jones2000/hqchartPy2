#pragma once

#ifdef _WIN32
#ifdef HQCHARTCSHARP_EXPORTS
#define HQCHART_DLL_API extern "C" __declspec(dllexport)
#else
#define HQCHART_DLL_API extern "C" __declspec(dllimport)
#endif
#else
#define HQCHART_DLL_API extern "C"
#endif

#ifdef _WIN32
#define HQ_CALL _stdcall
#else
#define HQ_CALL
#endif


#include "HQChart.Interface.Data.h"

#pragma pack(push,1)

//K线数据
typedef HQChart::Complier::HISTORY_ITEM HQCHART_KDATA;
typedef HQChart::Complier::KDATA_INFO HQCHART_KDATA_INFO;

typedef struct tagHQChartKDataResult
{
	HQCHART_KDATA* _pData;
	int				_lCount;
	wchar_t* _pszName;		//股票名称

	bool			_bResult;
	wchar_t* _pszError;

	int				_lPeriod;		//周期
	int				_lRight;		//复权

	int				_exData[4];
}HQCHART_KDATA_RESULT, * PHQCHART_KDATA_RESULT;

typedef struct tagValueItem
{
	double  _dValue;
	int		_lDate;
	int		_lTime;
	char	_exData[6];
}VALUE_ITEM, * PVALUE_ITEM;

typedef struct tagHQChartValueResult
{
	int _lType;	//0=单值 1=数组 2=带日期的需要自己拟合数据

	//单值
	double _dValue;

	//数组数据
	VALUE_ITEM* _pData;
	int			_lCount;

	bool		_bResult;
	wchar_t* _pszError;
}HQCHART_VALUE_RESULT, * PHQCHART_VALUE_RESULT;

typedef struct tagCustomFunctionArgment
{
	double	_dValue[20];
	int		_lCount;
}CUSTOM_FUNCTION_ARGUMENT, * PCUSTOM_FUNCTION_ARGUMENT;

typedef bool(HQ_CALL* pHQChart_LoadKData)(const wchar_t* lpSymbol, long lPeriod, long lRight, HQCHART_KDATA_RESULT* pResult, const wchar_t* pstrGuid);
typedef bool(HQ_CALL* pHQChart_LoadKData2)(const wchar_t* lpSymbol, long lPeriod, long lRight, const wchar_t* lpCallInfo, const HQCHART_KDATA_INFO* kDataInfo, HQCHART_KDATA_RESULT* pResult, const wchar_t* pstrGuid);

typedef bool(HQ_CALL* pHQChart_GetDataByNumber)(const wchar_t* lpSymbol, const wchar_t* pFunctionName, long lParam, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid);
typedef bool(HQ_CALL* pHQChart_GetDataByNumbers)(const wchar_t* lpSymbol, const wchar_t* pFunctionName, const CUSTOM_FUNCTION_ARGUMENT* pArgument, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid);
typedef bool(HQ_CALL* pHQChart_GetDataByName)(const wchar_t* lpSymbol, const wchar_t* pFunctionName, long lPeriod, long lRight, const HQCHART_KDATA_INFO* pKDataInfo, HQCHART_VALUE_RESULT* pResult, const wchar_t* pstrGuid);

typedef const wchar_t* (HQ_CALL* pHQChart_GetIndexScript)(const wchar_t* lpName, const wchar_t* lpCallInfo, const wchar_t* pstrGuid);

typedef void(HQ_CALL* pHQChart_Success)(const wchar_t* lpSymbol, const wchar_t* lpJSResul, const wchar_t* pstrGuidt);
typedef void(HQ_CALL* pHQChart_Failed)(const wchar_t* lpCode, const wchar_t* lpSymbol, const wchar_t* lpError, const wchar_t* pstrGuid);


typedef bool(HQ_CALL* pHQChart_Test)(const wchar_t* lpSymbol);


typedef struct HQCHART_CALLBACK_PTR
{
	pHQChart_LoadKData			_pLoadKData;
	pHQChart_LoadKData2			_pLoadKData2;

	pHQChart_GetDataByNumber	_pGetDataByNumber;
	pHQChart_GetDataByNumbers	_pGetDataByNumbers;
	pHQChart_GetDataByName		_pGetDataByName;
	pHQChart_GetIndexScript		_pGetIndexScript;
	pHQChart_Success			_pSuccess;
	pHQChart_Failed				_pFailed;

	pHQChart_Test				_pTest;

}HQCHART_CALLBACK_PTR, * PHQCHART_CALLBACK_PTR;

#pragma pack(pop)

HQCHART_DLL_API bool HQ_CALL Run(const wchar_t* pJonsConfig, HQCHART_CALLBACK_PTR callback);
HQCHART_DLL_API const wchar_t* HQ_CALL GetAuthorizeInfo();
HQCHART_DLL_API int HQ_CALL MainVersion();
HQCHART_DLL_API int HQ_CALL MinVersion();
HQCHART_DLL_API bool HQ_CALL AddFunction(const wchar_t* pStrName, int nArgCount);	// 添加自定义函数
HQCHART_DLL_API bool HQ_CALL AddVariant(const wchar_t* pStrName);	//添加自定义变量

