
// fastPackaging.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfastPackagingApp: 
// �йش����ʵ�֣������ fastPackaging.cpp
//

class CfastPackagingApp : public CWinApp
{
public:
	CfastPackagingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfastPackagingApp theApp;