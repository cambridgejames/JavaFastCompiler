#pragma once

// CEditFilePath �Ի���
class CEditFilePath : public CDialogEx
{
	DECLARE_DYNAMIC(CEditFilePath)

public:
	CEditFilePath(CString Path, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditFilePath();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITFILEPATH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
	BOOL isEditing;
	int m_Row;
	CString Path;
	CEdit m_edit;
	CListCtrl m_List;

public:
	CString getPath();
	void SetEditingMode(BOOL IsEditing);
	void NewOrEditItem();
	void DeleteItem();
	BOOL SubmitEdit();
	afx_msg void OnNewItem();
	afx_msg void OnEditItem();
	afx_msg void OnViewPath();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditInsert(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
};
