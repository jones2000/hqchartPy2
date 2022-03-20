/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

py指标计算接口 (c++)
*/

#pragma once
#include <vector>
#include <sstream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <memory>
#include "HQChart.KLineData.h"
#include "HQChart.Execute.h"
#include "HQChart.Log.h"
#include <Python.h>

#define JSON_CHECK_OBJECT(node,key) (node.HasMember(key) && node[key].IsObject())
#define JSON_CHECK_INT(node,key) (node.HasMember(key) && node[key].IsInt())
#define JSON_CHECK_ARRAY(node,key) (node.HasMember(key) && node[key].IsArray())
#define JSON_CHECK_DOUBLE(node,key) (node.HasMember(key) && node[key].IsNumber())
#define JSON_CHECK_STRING(node,key) (node.HasMember(key) && node[key].IsString())
#define JSON_CHECK_INT64(node,key) (node.HasMember(key) && node[key].IsInt64())


namespace HQChart { namespace Complier { namespace Py {

class HistoryDataCallback;

class RunConfig
{
public:
	friend class HistoryDataCallback;

	RunConfig();
	~RunConfig();

	bool LoadConfig(PyObject* pConfig, PyObject* pCallback, std::wstring& strError);
	bool RunScript();

private:
	bool LoadJsonConfig(PyObject* pConfig, std::wstring& strError);
	bool LoadCallbackConfg(PyObject* pCallback, std::wstring& strError);

	bool VerifyAuthorizeInfo();

	std::wstring m_strCode;
	std::vector<std::wstring> m_arySymbol;
	std::string m_strGuid = "None";	//utf8
	Execute::ARRAY_ARGUMENT m_aryArgs;
	long m_lPeriod = 0;
	long m_lRight = 0;

	long m_lOutCount = -1;
	long m_lOutStartIndex = -1;
	long m_lOutEndIndex = -1;
	bool m_bClearCache = false;

	PyObject* m_pSuccess = NULL;
	PyObject* m_pFailed = NULL;

	PyObject* m_pGetKData = NULL;
	PyObject* m_pGetKData2 = NULL;
	PyObject* m_pGetDataByName = NULL;
	PyObject* m_pGetDataByNumber = NULL;
	PyObject* m_pGetDataByNumbers = NULL;
	PyObject* m_pGetDataByString = NULL;
	PyObject* m_pGetIndexScript = NULL;
};



}
}
}
