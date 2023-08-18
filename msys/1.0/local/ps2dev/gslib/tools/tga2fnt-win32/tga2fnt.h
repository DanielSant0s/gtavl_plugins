// tga2fnt.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// Ctga2fntApp:
// See tga2fnt.cpp for the implementation of this class
//

class Ctga2fntApp : public CWinApp
{
public:
	Ctga2fntApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ctga2fntApp theApp;