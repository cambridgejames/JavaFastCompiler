// frameResource.cpp : ���� DLL �ĳ�ʼ�����̡�
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

// CframeResourceApp ����
CframeResourceApp::CframeResourceApp() {
}

CframeResourceApp theApp;

// CframeResourceApp ��ʼ��
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
		MessageBox(NULL, _T("����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n"), _T(""), MB_OK);
	}
}