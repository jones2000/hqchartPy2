/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

*/

#include "hqchart.complier.py.h"
#include "HQChart.Log.h"
#include <Python.h>
#include <string>
#include <thread>
#include "hqchart.complier.py.runconfig.h"

using namespace HQChart::Complier;

const int MAIN_VERSION = 1;
const int MIN_VERSION = 1101;	//后5位是小版本号


PyObject* GetVersion(PyObject* pSelf, PyObject* args)
{
	int nVersion = MAIN_VERSION * 100000 + MIN_VERSION;
	return PyLong_FromLong(nVersion);
}

PyObject* GetAuthorizeInfo(PyObject* pSelf, PyObject* args)
{
	std::wstring strValue;
	Py::GetAuthorizeInfo(strValue);

	return PyUnicode_FromWideChar(strValue.c_str(),-1);
}

PyObject* LoadAuthorizeInfo(PyObject* pSelf, PyObject* args)
{
	bool bResult = Py::LoadAuthorizeInfo("");

	return PyBool_FromLong(bResult ? 1 : 0);
}

PyObject* Run(PyObject* pSelf, PyObject* args) 
{
	PyObject* pConfig = NULL;
	PyObject* pCallback = NULL;

	if (!PyArg_ParseTuple(args, "OO", &pConfig, &pCallback))
	{
		return PyBool_FromLong(0);
	}

	Py::RunConfig config;
	std::wstring strError;
	if (!config.LoadConfig(pConfig, pCallback, strError)) return PyBool_FromLong(0);

	bool bResult = config.RunScript();

	return PyBool_FromLong(bResult ? 1 : 0);
}

PyObject* SetLog(PyObject* pSelf, PyObject* args)
{
	long lValue = 0;
	if (!PyArg_ParseTuple(args, "l", &lValue))
		return PyBool_FromLong(0);

	HQChart::Complier::Log::SetLogStatus(lValue);

	return PyBool_FromLong(1);
}

static PyMethodDef HQCHART_PY_METHODS[] =
{
	// The first property is the name exposed to Python, fast_tanh, the second is the C++
	// function name that contains the implementation.
	{ "Run", (PyCFunction)Run, METH_VARARGS, "run index by one symbol" },
	{ "GetVersion", (PyCFunction)GetVersion, METH_NOARGS, "get version" },
	{ "GetAuthorizeInfo", (PyCFunction)GetAuthorizeInfo, METH_NOARGS, "get Authorize information" },
	{ "LoadAuthorizeInfo",(PyCFunction)LoadAuthorizeInfo, METH_VARARGS, "load Authorize by key"},
	{ "SetLog",(PyCFunction)SetLog, METH_VARARGS, "enable/disenable log out"},

	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};


static PyModuleDef HQCHART_PY_MODULE =
{
	PyModuleDef_HEAD_INIT,
	"HQChartPy2",							// Module name to use with Python import statements
	"hqchart for index script",				// Module description
	0,
	HQCHART_PY_METHODS						// Structure that defines the methods of the module
};

void Test_Trace(LPCWSTR pszFormat, ...)
{
	va_list args;
	//va_list args2;
	va_start(args, pszFormat);
	//va_copy(args2, args);

	wchar_t szBuffer[1024 * 5] = { 0 };
	vswprintf(szBuffer, 1024, pszFormat, args);

	va_end(args);
}

PyMODINIT_FUNC PyInit_HQChartPy2()
{
	ScriptLibrary::GetInstance().Load();	//系统指标加载

	OutVariantAttribute::GetInstance();

	auto& customFunc = CustomFunction::GetInstance();
	customFunc.Add(L"HK2SHSZ", 0);
	customFunc.Add(L"MARGIN", 1);

	auto& customVar = CustomVariant::GetInstance();
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

	return PyModule_Create(&HQCHART_PY_MODULE);
}