#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "SplashWnd.h"

// CSplashWnd
CSplashWnd* CSplashWnd::m_pSplashWnd;

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd() {

}

CSplashWnd::~CSplashWnd() {
	ASSERT(m_pSplashWnd == this);
	m_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSplashWnd 消息处理程序
void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd) {
	if (m_pSplashWnd != NULL) return;
	m_pSplashWnd = new CSplashWnd;
	if (!m_pSplashWnd->Create(pParentWnd)) {
		delete m_pSplashWnd;	// 创建初始画面窗口
	}
	else {
		m_pSplashWnd->UpdateWindow();	// 显示初始画面窗口
	}
}

BOOL CSplashWnd::Create(CWnd* pParentWnd) {
	if (!m_bitmap.LoadBitmap(IDB_SPLASH)) { return FALSE; }
	BITMAP bm;
	m_bitmap.GetBitmap(&bm);
	return CreateEx(0, AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)), NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);//创建主框架窗口的子窗口
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1) { return -1; }
	CenterWindow();
	SetTimer(1, 1000, NULL);	// 时间控制
	return 0;
}

void CSplashWnd::OnPaint() {
	CPaintDC dc(this);
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc)) { return; }
	BITMAP bm;
	m_bitmap.GetBitmap(&bm);
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}


void CSplashWnd::OnTimer(UINT nIDEvent) {
	DestroyWindow(); //销毁初始画面窗口   
	AfxGetMainWnd()->UpdateWindow();
	CWnd::OnTimer(nIDEvent);
}

void CSplashWnd::PostNcDestroy(void) {
	DestroyWindow();
	CWnd::PostNcDestroy();
}
