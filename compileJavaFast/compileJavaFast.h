
// compileJavaFast.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcompileJavaFastApp: 
// �йش����ʵ�֣������ compileJavaFast.cpp
//

class CcompileJavaFastApp : public CWinApp
{
public:
	CcompileJavaFastApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcompileJavaFastApp theApp;