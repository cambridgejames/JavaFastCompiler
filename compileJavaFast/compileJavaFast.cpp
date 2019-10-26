#pragma once

#include "stdafx.h"
#include "compileJavaFast.h"
#include <windows.h>

#define srandom (rand() % 100 + 1)
#define probability 5
#define logNumber 5;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CcompileJavaFastApp
BEGIN_MESSAGE_MAP(CcompileJavaFastApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CcompileJavaFastApp 构造
CcompileJavaFastApp::CcompileJavaFastApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	srand((unsigned)time(0));
	if (srandom <= probability) {
		_wsystem(_T("logCleaner ,logNumber"));
	}
}

CcompileJavaFastApp theApp;

BOOL CcompileJavaFastApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("Java快速编译工具箱"));

	try {
		typedef void(*lpFun)(void);
		HINSTANCE hDll = LoadLibrary(L"frameResource.dll");
		if (hDll == NULL) { throw (CString)_T("DLL_NOT_FOUND"); }
		lpFun pMainFrame = (lpFun)GetProcAddress(hDll, "mainFrame");
		if (pMainFrame == NULL) { throw (CString)_T("FUNCTION_NOT_FOUND"); }
		pMainFrame();
		CloseHandle(hDll);
	}
	catch (CString msg) {
		MessageBox(NULL, _T("加载资源时发生未知错误\r\n请修复程序后重试"), msg, MB_ICONERROR | MB_OK);
	}

	if (pShellManager != NULL) {
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	return FALSE;
}

