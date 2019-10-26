// frameResource.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "frameResource.h"
#include "Resource.h"
#include "compileJavaFastDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CframeResourceApp
BEGIN_MESSAGE_MAP(CframeResourceApp, CWinApp)
END_MESSAGE_MAP()

// CframeResourceApp 构造
CframeResourceApp::CframeResourceApp() {
}

CframeResourceApp theApp;

// CframeResourceApp 初始化
BOOL CframeResourceApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

extern "C" void mainFrame() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CcompileJavaFastDlg dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == -1) {
		MessageBox(NULL, _T("警告: 对话框创建失败，应用程序将意外终止。\n"), _T(""), MB_OK);
	}
}