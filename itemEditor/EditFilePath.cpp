#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "EditFilePath.h"
#include "afxdialogex.h"

// CEditFilePath 对话框
IMPLEMENT_DYNAMIC(CEditFilePath, CDialogEx)

CEditFilePath::CEditFilePath(CString Path, CWnd* pParent) : CDialogEx(IDD_EDITFILEPATH, pParent), isEditing(FALSE), m_Row(-1) {
	this->Path = Path;
}

CEditFilePath::~CEditFilePath() {}

void CEditFilePath::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CEditFilePath, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditFilePath::OnNewItem)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditFilePath::OnEditItem)
	ON_BN_CLICKED(IDC_BUTTON3, &CEditFilePath::OnViewPath)
	ON_BN_CLICKED(IDC_BUTTON4, &CEditFilePath::OnDeleteItem)
	ON_BN_CLICKED(IDC_BUTTON5, &CEditFilePath::OnMoveUp)
	ON_BN_CLICKED(IDC_BUTTON6, &CEditFilePath::OnMoveDown)

	ON_BN_CLICKED(IDOK, &CEditFilePath::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditFilePath::OnBnClickedCancel)

	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CEditFilePath::OnEnKillfocusEditInsert)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CEditFilePath::OnNMDblclkList)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CEditFilePath 消息处理程序
BOOL CEditFilePath::OnInitDialog() {
	CDialogEx::OnInitDialog();

	((CListCtrl*)GetDlgItem(IDC_LIST1))->SetBkColor(RGB(255, 255, 255));
	((CListCtrl*)GetDlgItem(IDC_LIST1))->SetOutlineColor(RGB(100, 0, 0));
	((CListCtrl*)GetDlgItem(IDC_LIST1))->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_COLUMNSNAPPOINTS);

	CRect rect;
	((CListCtrl*)GetDlgItem(IDC_LIST1))->GetClientRect(&rect);
	((CListCtrl*)GetDlgItem(IDC_LIST1))->InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	((CListCtrl*)GetDlgItem(IDC_LIST1))->InsertColumn(1, _T(""), LVCFMT_LEFT, rect.Width());

	GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("新建"));
	GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("编辑"));
	GetDlgItem(IDC_BUTTON3)->SetWindowText(_T("浏览"));
	GetDlgItem(IDC_BUTTON4)->SetWindowText(_T("删除"));
	GetDlgItem(IDC_BUTTON5)->SetWindowText(_T("上移"));
	GetDlgItem(IDC_BUTTON6)->SetWindowText(_T("下移"));

	GetDlgItem(IDOK)->SetWindowText(_T("确定"));
	GetDlgItem(IDCANCEL)->SetWindowText(_T("取消"));

	int index = 0;
	CString currentString;
	while (Path.GetLength() != 0) {
		// 取Path中的第一个path
		index = Path.Find(_T(";"));
		if (index == -1) { index = Path.GetLength(); }
		currentString = Path.Left(index);
		Path.Delete(0, index + 1);

		// 向ListControl提交
		if (currentString.GetLength() != 0) {
			int NRow = m_List.GetItemCount();
			m_List.InsertItem(NRow, _T(""));
			m_List.SetItemText(NRow, 1, currentString);
		}
	}

	return TRUE;
}

void CEditFilePath::OnPaint() {
	CPaintDC dc(this);

	if (m_List.GetItemCount() != 0) {
		m_Row = 0;
		m_List.SetFocus();	//设置焦点
		m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	//设置状态
		m_List.EnsureVisible(m_Row, FALSE);	//设置当前视图可见
	}
}

CString CEditFilePath::getPath() {
	return this->Path;
}

void CEditFilePath::SetEditingMode(BOOL IsEditing) {
	isEditing = IsEditing;
	if (isEditing) {
		GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("保存"));
	}
	else {
		GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("编辑"));
	}
}

void CEditFilePath::NewOrEditItem() {
	SetEditingMode(TRUE);

	CRect rc;
	CString strTemp;

	if (m_Row == -1) {
		m_Row = m_List.GetItemCount();
		strTemp.Format(_T("%d"), m_Row + 1);
		m_List.InsertItem(m_Row, strTemp);
		m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_List.EnsureVisible(m_Row, FALSE);

		((CListCtrl*)GetDlgItem(IDC_LIST1))->GetClientRect(&rc);
		((CListCtrl*)GetDlgItem(IDC_LIST1))->SetColumnWidth(1, rc.Width());
	}

	m_List.GetSubItemRect(m_Row, 1, LVIR_LABEL, rc);
	m_edit.SetParent(&m_List);
	m_edit.MoveWindow(rc);
	m_edit.SetWindowTextW(m_List.GetItemText(m_Row, 1));
	m_edit.ShowWindow(SW_SHOW);
	m_edit.SetFocus();	//设置Edit焦点
	m_edit.ShowCaret();	//显示光标
	m_edit.SetSel(0, -1);	//全选
}

void CEditFilePath::DeleteItem() {
	m_List.DeleteItem(m_Row);
	if (m_Row == m_List.GetItemCount()) { m_Row--; }
}

BOOL CEditFilePath::SubmitEdit() {
	BOOL isDeleted = TRUE;
	if (isEditing) {
		CString str;
		m_edit.GetWindowTextW(str);
		if (str == _T("")) { DeleteItem(); isDeleted = FALSE; }
		else { m_List.SetItemText(m_Row, 1, str); }
		m_edit.ShowWindow(SW_HIDE);

		m_List.SetFocus();	//设置焦点
		m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	//设置状态
		m_List.EnsureVisible(m_Row, FALSE);	//设置当前视图可见
	}
	SetEditingMode(FALSE);
	return isDeleted;
}

void CEditFilePath::OnNewItem() {
	SubmitEdit();
	m_Row = -1;
	NewOrEditItem();
}

void CEditFilePath::OnEditItem() {
	if (!isEditing) {
		if (m_Row == -1 && m_List.GetItemCount() != 0) { m_Row = 0; }
		NewOrEditItem();
	}
	else{
		SubmitEdit();
	}
}

void CEditFilePath::OnViewPath() {
	if (isEditing) {
		BROWSEINFO bInfo;
		ZeroMemory(&bInfo, sizeof(bInfo));
		bInfo.hwndOwner = m_hWnd;
		TCHAR tchPath[255];
		bInfo.lpszTitle = NULL;
		bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST lpDlist = SHBrowseForFolder(&bInfo); //显示选择对话框
		if (lpDlist != NULL) {
			SHGetPathFromIDList(lpDlist, tchPath); //把项目标识列表转化成目录
			CString filePath;
			filePath.Format(_T("%s"), tchPath);
			m_edit.SetWindowText(filePath);
		}
	}
	else {
		m_List.SetFocus();	//设置焦点
		m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	//设置状态
		m_List.EnsureVisible(m_Row, FALSE);	//设置当前视图可见
	}
}

void CEditFilePath::OnDeleteItem() {
	if (SubmitEdit()) { DeleteItem(); }
	m_List.SetFocus();	//设置焦点
	m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	//设置状态
	m_List.EnsureVisible(m_Row, FALSE);	//设置当前视图可见
}

void CEditFilePath::OnMoveUp() {
	SubmitEdit();

	if (m_Row != 0 && m_Row != -1) {
		CString str = m_List.GetItemText(m_Row - 1, 1);
		m_List.SetItemText(m_Row - 1, 1, m_List.GetItemText(m_Row, 1));
		m_List.SetItemText(m_Row, 1, str);
		m_Row--;
	}

	m_List.SetFocus();	//设置焦点
	m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	//设置状态
	m_List.EnsureVisible(m_Row, FALSE);	//设置当前视图可见
}

void CEditFilePath::OnMoveDown() {
	SubmitEdit();

	if (m_Row != m_List.GetItemCount() - 1 && m_Row != -1) {
		CString str = m_List.GetItemText(m_Row + 1, 1);
		m_List.SetItemText(m_Row + 1, 1, m_List.GetItemText(m_Row, 1));
		m_List.SetItemText(m_Row, 1, str);
		m_Row++;
	}

	m_List.SetFocus();	//设置焦点
	m_List.SetItemState(m_Row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);	//设置状态
	m_List.EnsureVisible(m_Row, FALSE);	//设置当前视图可见
}

void CEditFilePath::OnBnClickedOk() {
	SubmitEdit();
	Path = _T("");
	for (int i = 0; i < m_List.GetItemCount(); i++) {
		Path += m_List.GetItemText(i, 1) + _T(";");
	}
	Path.Delete(Path.GetLength() - 1);
	CDialogEx::OnOK();
}

void CEditFilePath::OnBnClickedCancel() {
	CDialogEx::OnCancel();
}

void CEditFilePath::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	m_Row = pNMListView->iItem;
	NewOrEditItem();

	*pResult = 0;
}

void CEditFilePath::OnEnKillfocusEditInsert(NMHDR *pNMHDR, LRESULT *pResult) {
	SubmitEdit();
	POSITION pos = ((CListCtrl*)GetDlgItem(IDC_LIST1))->GetFirstSelectedItemPosition();
	m_Row = ((CListCtrl*)GetDlgItem(IDC_LIST1))->GetNextSelectedItem(pos);
}
