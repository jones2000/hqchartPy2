/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

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
#include "hqchart.complier.py.callbackfunction.h"
#include <Python.h>


namespace HQChart { namespace Complier { namespace Py {


bool LoadAuthorizeInfo(const std::string& strEncrypted);
void GetAuthorizeInfo(std::wstring& strValue);
bool AddSystemIndex(PyObject* pConfig);

}
}
}
