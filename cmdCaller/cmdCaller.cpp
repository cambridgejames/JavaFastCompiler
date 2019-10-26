// cmdCaller.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "cmdCaller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CcmdCallerApp, CWinApp)
END_MESSAGE_MAP()

// CcmdCallerApp ����

CcmdCallerApp::CcmdCallerApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CcmdCallerApp ����

CcmdCallerApp theApp;


// CcmdCallerApp ��ʼ��

BOOL CcmdCallerApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

TCHAR* StringToChar(CString& str)
{
	int len = str.GetLength();
	TCHAR* tr = str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr;
}

extern "C" CString ExecuteCmd(CString str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		MessageBox(NULL, _T("Error on CreatePipe()!"), _T("Error"), MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
		return NULL;
	}
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline = StringToChar(str);
	if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
		MessageBox(NULL, _T("Error on CreateProcess()!"), _T("Error"), MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer, 0, 4096);
	CString output;
	DWORD byteRead;
	while (ReadFile(hRead, buffer, 4095, &byteRead, NULL) != NULL) {
		output += buffer;
	}
	output.Replace(_T("\t"), _T("  "));
	output = output == _T("") ? _T("����ɹ�") : output;
	return output;
}
