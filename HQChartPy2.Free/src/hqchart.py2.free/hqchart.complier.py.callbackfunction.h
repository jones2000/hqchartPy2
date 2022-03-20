/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

*/

#pragma once

#include <Python.h>

namespace HQChart { namespace Complier { namespace Py {

class PyCallbackFunction
{
public:
	PyCallbackFunction(PyObject* p);
	~PyCallbackFunction();


	PyObject* Call(PyObject* pArgs);
private:

	PyObject* m_pCallbackFunction = NULL;	//回调函数
	PyObject* m_pResult = NULL;				//返回数据
};


}
}
}