#pragma once

#include "stdafx.h"
#include "itemEditor.h"
#include "EditFilePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CitemEditorApp
BEGIN_MESSAGE_MAP(CitemEditorApp, CWinApp)
END_MESSAGE_MAP()

// CitemEditorApp 构造
CitemEditorApp::CitemEditorApp() {
}

CitemEditorApp theApp;

// CitemEditorApp 初始化
BOOL CitemEditorApp::InitInstance() {
	CWinApp::InitInstance();

	return TRUE;
}

extern "C" CString editItem(CString str) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CEditFilePath dlgEdit(str);
	if (dlgEdit.DoModal() == IDOK) {
		return dlgEdit.getPath();
	}
	else {
		return _T("");
	}
}