#pragma once

class CSplashWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplashWnd)

public:
	CSplashWnd();
	virtual ~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	static CSplashWnd* m_pSplashWnd;//指向初始画面的指针
	BOOL Create(CWnd* pParentWnd);

public:
	CBitmap m_bitmap;//初始画面位图 
	static void ShowSplashScreen(CWnd* pParentWnd);
};
