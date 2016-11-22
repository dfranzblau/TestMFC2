
// TestMFC2.h : main header file for the TestMFC2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTestMFC2App:
// See TestMFC2.cpp for the implementation of this class
//

class CTestMFC2App : public CWinApp
{
public:
	CTestMFC2App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTestMFC2App theApp;
