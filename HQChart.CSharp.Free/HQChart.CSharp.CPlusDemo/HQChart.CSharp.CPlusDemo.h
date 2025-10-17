
// HQChart.CSharp.CPlusDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHQChartCSharpCPlusDemoApp:
// See HQChart.CSharp.CPlusDemo.cpp for the implementation of this class
//

class CHQChartCSharpCPlusDemoApp : public CWinApp
{
public:
	CHQChartCSharpCPlusDemoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHQChartCSharpCPlusDemoApp theApp;
