// cmdCaller.h : cmdCaller DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcmdCallerApp
// �йش���ʵ�ֵ���Ϣ������� cmdCaller.cpp
//

class CcmdCallerApp : public CWinApp
{
public:
	CcmdCallerApp();

// ��д
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

TCHAR* StringToChar(CString& str);
extern "C" __declspec(dllexport) CString ExecuteCmd(CString str);