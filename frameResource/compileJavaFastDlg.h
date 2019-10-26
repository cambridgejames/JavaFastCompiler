#pragma once

class CcompileJavaFastDlg : public CDialogEx {
public:
	CcompileJavaFastDlg(CWnd* pParent = NULL);	// 标准构造函数

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPILEJAVAFAST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	HICON m_hIcon;
	CMenu m_Menu;
	CString m_edit3;
	CString m_TempFile;
	CListCtrl m_listCtrl;
	CImageList m_imageList;
	CWinThread* m_pThread;

protected:
	virtual BOOL OnInitDialog();
	void UpdateOutlookBar();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	BOOL QuestionBeforeRun(CString command, BOOL isCmd);
	void static saveConfigs();
	template<typename Type_1, typename Type_2, typename... Types>
	void static saveConfigs(const Type_1& config, const Type_2& arg, const Types&... args);
	void saveAllConfigs(BOOL isTip = TRUE);
	void runTools(INT toolNumber);

protected:
	afx_msg void OnClickedCompile();
	afx_msg void OnClickedRun();
	afx_msg void OnClickedSaveAll();
	afx_msg void OnClickedPreference();
	afx_msg void OnClickedJarPackaging();
	afx_msg void OnClickedCallCmd();
	afx_msg void OnClickedDownloadNew();
	afx_msg void OnClickedHelp();
	afx_msg void OnClickedAbout();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedEditSourcepath();
	afx_msg void OnBnClickedEditClasspath();
	afx_msg void OnBnClickedEditObjectpath();
	afx_msg void OnBnClickedEditEncodingList();
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
