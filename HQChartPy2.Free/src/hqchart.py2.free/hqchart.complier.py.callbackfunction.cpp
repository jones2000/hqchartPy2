/*
Copyright (c) 2018 jones

https://github.com/jones2000/hqchartPy2/blob/master/LICENSE

开源项目 https://github.com/jones2000/hqchartPy2

jones_2000@163.com

*/

#include "hqchart.complier.py.callbackfunction.h"

namespace HQChart { namespace Complier { namespace Py {

PyCallbackFunction::PyCallbackFunction(PyObject* p)
	:m_pCallbackFunction(p)
{
	if (PyCallable_Check(m_pCallbackFunction)) Py_XINCREF(m_pCallbackFunction);
	else m_pCallbackFunction = NULL;
}

PyCallbackFunction::~PyCallbackFunction()
{
	if (m_pCallbackFunction) Py_XDECREF(m_pCallbackFunction);
	if (m_pResult) Py_XDECREF(m_pResult);
}

PyObject* PyCallbackFunction::Call(PyObject* pArgs)
{
	if (m_pResult) Py_XDECREF(m_pResult);
	if (!m_pCallbackFunction) return NULL;

	m_pResult = PyObject_CallObject(m_pCallbackFunction, pArgs);
	if (pArgs) Py_XDECREF(pArgs);

	return m_pResult;
}

}
}
}