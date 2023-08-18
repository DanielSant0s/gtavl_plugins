// tga2fnt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "tga2fnt.h"
#include "tga2fntDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctga2fntApp

BEGIN_MESSAGE_MAP(Ctga2fntApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ctga2fntApp construction

Ctga2fntApp::Ctga2fntApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Ctga2fntApp object

Ctga2fntApp theApp;


// Ctga2fntApp initialization

BOOL Ctga2fntApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	Ctga2fntDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
