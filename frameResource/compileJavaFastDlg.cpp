#pragma once

#include "stdafx.h"
#include "SplashWnd.h"
#include "compileJavaFastDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "pThread.h"
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>

#define ELPP_NO_DEFAULT_LOG_FILE
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP
TIMED_SCOPE(appTimer, "Java Fast Compiler");

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CcompileJavaFastDlg 对话框
CcompileJavaFastDlg::CcompileJavaFastDlg(CWnd* pParent)	: CDialogEx(IDD_COMPILEJAVAFAST_DIALOG, pParent), m_pThread(nullptr) {
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::reconfigureAllLoggers(el::ConfigurationType::MaxLogFileSize, "2097152");
	el::Configurations conf("config/logsetting.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CcompileJavaFastDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CcompileJavaFastDlg, CDialogEx)
	ON_WM_NCHITTEST()
	ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDOK, &CcompileJavaFastDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CcompileJavaFastDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CcompileJavaFastDlg::OnBnClickedEditSourcepath)
	ON_BN_CLICKED(IDC_BUTTON2, &CcompileJavaFastDlg::OnBnClickedEditClasspath)
	ON_BN_CLICKED(IDC_BUTTON3, &CcompileJavaFastDlg::OnBnClickedEditObjectpath)
	ON_BN_CLICKED(IDC_BUTTON4, &CcompileJavaFastDlg::OnBnClickedEditEncodingList)

	ON_COMMAND(ID_JFC_32772, &CcompileJavaFastDlg::OnClickedCompile)
	ON_COMMAND(ID_JFC_32773, &CcompileJavaFastDlg::OnClickedRun)
	ON_COMMAND(ID_JFC_32774, &CcompileJavaFastDlg::OnClickedSaveAll)
	ON_COMMAND(ID_JFC_32775, &CcompileJavaFastDlg::OnBnClickedCancel)
	ON_COMMAND(ID_32781, &CcompileJavaFastDlg::OnClickedPreference)
	ON_COMMAND(ID_32776, &CcompileJavaFastDlg::OnClickedJarPackaging)
	ON_COMMAND(ID_32777, &CcompileJavaFastDlg::OnClickedCallCmd)
	ON_COMMAND(ID_32772, &CcompileJavaFastDlg::OnClickedDownloadNew)
	ON_COMMAND(ID_32778, &CcompileJavaFastDlg::OnClickedHelp)
	ON_COMMAND(ID_32780, &CcompileJavaFastDlg::OnClickedAbout)

	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CcompileJavaFastDlg::OnNMClickList)
END_MESSAGE_MAP()

// 初始化功能选择列表
void CcompileJavaFastDlg::UpdateOutlookBar() {
	SetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE, GetWindowLong(m_listCtrl.m_hWnd, GWL_STYLE) & ~LVS_TYPEMASK | LVS_ICON);

	m_listCtrl.SetBkColor(RGB(255, 255, 255));

	m_imageList.Create(24, 24, ILC_COLOR16 | ILC_MASK, 1, 1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON4));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON6));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON7));

	m_listCtrl.SetImageList(&m_imageList, LVSIL_NORMAL);
	CRect rc;
	m_listCtrl.GetClientRect(rc);
	m_listCtrl.SetIconSpacing(rc.Width(), 24);
	m_listCtrl.InsertColumn(0, _T("OutlookBar"));
	m_listCtrl.InsertItem(0, _T("编译"), 0);
	m_listCtrl.InsertItem(1, _T("运行"), 1);
	m_listCtrl.InsertItem(2, _T("打包"), 2);
	m_listCtrl.InsertItem(3, _T("启动CMD"), 3);
	m_listCtrl.InsertItem(4, _T("帮助"), 4);
	m_listCtrl.InsertItem(5, _T("关于…"), 5);

	GetDlgItem(IDC_STATIC1)->SetWindowText(_T("源代码文件存储路径："));
	GetDlgItem(IDC_STATIC2)->SetWindowText(_T("类文件存储路径："));
	GetDlgItem(IDC_STATIC3)->SetWindowText(_T("字节码文件生成路径："));
	GetDlgItem(IDC_STATIC4)->SetWindowText(_T("源文件编码格式："));
	GetDlgItem(IDC_STATIC5)->SetWindowText(_T("包名、主类名及参数："));

	GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("编辑"));
	GetDlgItem(IDC_BUTTON2)->SetWindowText(_T("编辑"));
	GetDlgItem(IDC_BUTTON3)->SetWindowText(_T("浏览"));
	GetDlgItem(IDC_BUTTON4)->SetWindowText(_T("编辑"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetMinVisibleItems(10);

	CString str, current;
	std::string Str;
	std::ifstream inFile;
	inFile.open("config/encodeConf.ini", std::ios::in | std::ios::binary, _SH_DENYRW);
	if (inFile.is_open()) {
		std::getline(inFile, Str);
		str = Str.c_str();
		int index;
		while (str.GetLength() != 0) {
			// 取Path中的第一个path
			index = str.Find(_T(";"));
			if (index == -1) { index = str.GetLength(); }
			current = str.Left(index);
			str.Delete(0, index + 1);

			// 向ListControl提交
			if (current.GetLength() != 0) {
				((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(current);
			}
		}
	}
	else {
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("ASCII"));
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("ISO-8859-1"));
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("GB18030"));
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("GB2312"));
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("GBK"));
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("UTF-16"));
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("UTF-8"));
		saveConfigs("encodeConf", "ASCII;ISO-8859-1;GB18030;GB2312;GBK;UTF-16;UTF-8");
	}	// 读取失败时显示默认值
	inFile.close();

}

// CcompileJavaFastDlg 消息处理程序
BOOL CcompileJavaFastDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_Menu.LoadMenu(IDR_MENU1);
	SetMenu(&m_Menu);

	UpdateOutlookBar();

	// 获取应用程序的存储路径
	char exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, (LPWCH)exeFullPath, MAX_PATH); //将其格式化为字符串
	m_TempFile.Format(L"%s", exeFullPath);
	int begin = m_TempFile.ReverseFind('\\'), end = m_TempFile.GetLength();
	m_TempFile.Delete(begin, end - begin);

	int Encoding;
	std::string SourcePath, ClassPath, ObjectPath, MainName;
	CString sourcePath, objectPath, classPath, mainName, encoding;

	std::ifstream inFile;
	inFile.open("config/sourcePath.ini", std::ios::in | std::ios::binary, _SH_DENYRW);
	if (inFile.is_open()) { std::getline(inFile, SourcePath); sourcePath = SourcePath.c_str(); }
	else { sourcePath = m_TempFile; }	// 读取失败时显示默认值
	inFile.close();
	inFile.open("config/classPath.ini", std::ios::in | std::ios::binary, _SH_DENYRW);
	if (inFile.is_open()) { std::getline(inFile, ClassPath); classPath = ClassPath.c_str(); }
	else { classPath.Empty(); }	// 读取失败时显示默认值
	inFile.close();
	inFile.open("config/objectPath.ini", std::ios::in | std::ios::binary, _SH_DENYRW);
	if (inFile.is_open()) { std::getline(inFile, ObjectPath); objectPath = ObjectPath.c_str(); }
	else { objectPath = m_TempFile; }	// 读取失败时显示默认值
	inFile.close();
	inFile.open("config/encoding.ini", std::ios::in | std::ios::binary, _SH_DENYRW);
	if (inFile.is_open()) { inFile >> Encoding; ((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(Encoding); }
	else {
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SelectString(0, _T("UTF-8"));
		GetDlgItem(IDC_COMBO1)->GetWindowText(encoding);
		if (encoding != _T("UTF-8")) {
			((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(_T("UTF-8"));
			((CComboBox*)GetDlgItem(IDC_COMBO1))->SelectString(0, _T("UTF-8"));
		}
	}	// 读取失败时显示默认值
	inFile.close();
	inFile.open("config/mainName.ini", std::ios::in | std::ios::binary, _SH_DENYRW);
	if (inFile.is_open()) { std::getline(inFile, MainName); mainName = MainName.c_str(); }
	else { mainName.Empty(); }	// 读取失败时显示默认值
	inFile.close();

	GetDlgItem(IDC_EDIT1)->SetWindowText(sourcePath);
	GetDlgItem(IDC_EDIT5)->SetWindowText(classPath);
	GetDlgItem(IDC_EDIT2)->SetWindowText(objectPath);
	GetDlgItem(IDC_EDIT4)->SetWindowText(mainName);

	LOG(INFO) << "The window JavaFastCompiler has been started.";

	return TRUE;
}

void CcompileJavaFastDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

int CcompileJavaFastDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialogEx::OnCreate(lpCreateStruct) == -1) { return -1; }
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。
void CcompileJavaFastDlg::OnPaint() {
	if (IsIconic())	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else {
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CcompileJavaFastDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}

// 禁止用户改变窗口大小
LRESULT CcompileJavaFastDlg::OnNcHitTest(CPoint point) {
	int ret = CDialog::OnNcHitTest(point);
	if (HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret || HTBOTTOMLEFT == ret || HTBOTTOMRIGHT == ret || HTTOPLEFT == ret || HTTOPRIGHT == ret) {
		return HTCLIENT;
	}
	return ret;
}

BOOL CcompileJavaFastDlg::QuestionBeforeRun(CString command, BOOL isCmd) {
	if (MessageBox(_T("确定要执行以下语句吗？\r\n") + command, TEXT("提示"), MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL) == 6) {
		if (isCmd) {
			LOG(INFO) << "A command prompt window has been opened.";
			m_pThread = AfxBeginThread(ThreadProc, this);
			_wsystem(CT2A(command.GetBuffer()) + " && pause");
			return TRUE;
		}
		else {
			LOG(INFO) << "Compilation has started.";

			typedef CString(*cmdCaller)(CString);
			HINSTANCE hDLL;
			hDLL = LoadLibrary(L"cmdCaller.dll");
			cmdCaller ExecuteCmd = (cmdCaller)GetProcAddress(hDLL, "ExecuteCmd");
			CString solution = ExecuteCmd(command);
			FreeLibrary(hDLL);

			LOG(INFO) << "Compile completed successfully.";
			if (solution != _T("编译成功")) { LOG(WARNING) << "An error occurred at compile time."; }

			GetDlgItem(IDC_EDIT3)->SetWindowText(solution);
			return TRUE;
		}
	}
	else {
		return FALSE;
	}
}

void CcompileJavaFastDlg::saveConfigs() {} // 递归终止条件

template<typename Type_1, typename Type_2, typename... Types>
void CcompileJavaFastDlg::saveConfigs(const Type_1& config, const Type_2& arg, const Types&... args) {
	std::ofstream outFile;
	outFile.open("config/" + (std::string)config + ".ini", std::ios::trunc);
	outFile << arg;
	outFile.close();
	saveConfigs(args...);
}

void CcompileJavaFastDlg::saveAllConfigs(BOOL isTip) {
	CString sourcePath, classPath, objectPath, encoding, mainName;

	GetDlgItemText(IDC_EDIT1, sourcePath);
	GetDlgItemText(IDC_EDIT5, classPath);
	GetDlgItemText(IDC_EDIT2, objectPath);
	GetDlgItem(IDC_COMBO1)->GetWindowText(encoding);
	GetDlgItemText(IDC_EDIT4, mainName);

	std::string SourcePath, ClassPath, ObjectPath, MainName;
	int Encoding;

	SourcePath = CT2A(sourcePath.GetBuffer());
	ClassPath = CT2A(classPath.GetBuffer());
	ObjectPath = CT2A(objectPath.GetBuffer());
	Encoding = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	MainName = CT2A(mainName.GetBuffer());

	saveConfigs("sourcePath", SourcePath, "classPath", ClassPath, "objectPath", ObjectPath, "encoding", Encoding, "mainName", MainName);

	if (isTip) { MessageBox(_T("成功保存设置"), TEXT("提示"), MB_OK | MB_SYSTEMMODAL); }
}

void CcompileJavaFastDlg::OnClickedCompile() {
	m_edit3 = _T("");
	UpdateData(FALSE);

	saveAllConfigs(FALSE);

	CString mainPath, sourcePath, classPath, objectPath, encoding, mainName;

	GetDlgItemText(IDC_EDIT1, sourcePath);
	GetDlgItemText(IDC_EDIT5, classPath);
	GetDlgItemText(IDC_EDIT2, objectPath);
	GetDlgItem(IDC_COMBO1)->GetWindowText(encoding);
	GetDlgItemText(IDC_EDIT4, mainName);

	int NRow = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCount();
	CString str, current;
	for (int i = 0; i < NRow; i++) {
		((CComboBox*)GetDlgItem(IDC_COMBO1))->GetLBText(i, current);
		str += current + _T(";");
	}
	saveConfigs("encodeConf", CT2A(str.GetBuffer()));

	// 取Path中的第一个path
	int index = sourcePath.Find(_T(";"));
	if (index == -1) { index = sourcePath.GetLength(); }
	mainPath = sourcePath.Left(index);
	sourcePath.Delete(0, index + 1);

	if (mainPath.IsEmpty() || mainName.IsEmpty()) {
		MessageBox(_T("文件信息不能为空！"), TEXT("错误"), MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL);
		LOG(ERROR) << "Empty file information was set.";
	}
	else {
		CString command = _T("javac ");
		if (!encoding.IsEmpty()) { command += _T("-encoding \"") + encoding + _T("\" "); }
		if (!sourcePath.IsEmpty()) { command += _T("-sourcepath \"") + mainPath + _T(";") + sourcePath + _T("\" "); }
		if (!classPath.IsEmpty()) { command += _T("-cp \"") + classPath + _T("\" "); }
		if (!objectPath.IsEmpty()) { command += _T("-d \"") + objectPath + _T("\" "); }
		command += _T("\"") + mainPath + _T("\\") + mainName + _T(".java\"");
		QuestionBeforeRun(command, FALSE);
	}
}

void CcompileJavaFastDlg::OnClickedRun() {
	m_edit3 = _T("");
	UpdateData(FALSE);

	CString objectPath, classPath;

	GetDlgItemText(IDC_EDIT2, objectPath);
	GetDlgItemText(IDC_EDIT4, classPath);

	CString command = _T("java -cp \"") + objectPath + _T("\" ") + classPath;
	QuestionBeforeRun(command, TRUE);
}


void CcompileJavaFastDlg::OnClickedSaveAll() {
	saveAllConfigs();
}

void CcompileJavaFastDlg::OnClickedPreference() {
	MessageBox(_T("成功加载首选项"), TEXT("提示"), MB_OK | MB_SYSTEMMODAL);
}

void CcompileJavaFastDlg::OnClickedJarPackaging() {
	if (MessageBox(_T("是否要启动Java快速打包工具？"), TEXT("提示"), MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL) == 6) {
			WinExec("plugin/fastPackaging.exe", SW_SHOW);
	}
}

void CcompileJavaFastDlg::OnClickedCallCmd() {
	int index;
	CString command, sourcePath;
	GetDlgItemText(IDC_EDIT1, sourcePath);
	if (sourcePath == _T("")) { sourcePath = m_TempFile; }
	index = sourcePath.Find(_T(";"));
	if (index == -1) { index = sourcePath.GetLength(); }
	sourcePath = sourcePath.Left(index);
	command = _T("cmd /k cd/d \"") + sourcePath + _T("\"");
	QuestionBeforeRun(command, TRUE);
}

void CcompileJavaFastDlg::OnClickedDownloadNew() {
	ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("https://github.com/cambridgejames/Java_Fast_Compiler"), NULL, SW_SHOW);
}

void CcompileJavaFastDlg::OnClickedHelp() {
	ShellExecute(NULL, _T("open"), _T("explorer.exe"), _T("https://www.jianshu.com/p/adc367336a3a"), NULL, SW_SHOW);
}

void CcompileJavaFastDlg::OnClickedAbout() {
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CcompileJavaFastDlg::runTools(INT toolNumber) {
	int index;
	CString command, sourcePath;
	CAboutDlg dlgAbout;

	switch (toolNumber) {
		case 0: OnClickedCompile(); break;
		case 1: OnClickedRun(); break;
		case 2: OnClickedJarPackaging(); break;
		case 3: OnClickedCallCmd(); break;
		case 4: OnClickedHelp(); break;
		case 5: OnClickedAbout(); break;
		default: break;
	}
}

void CcompileJavaFastDlg::OnBnClickedOk() {}// 重写消息处理程序，禁止回车键关闭窗口

void CcompileJavaFastDlg::OnBnClickedCancel() {
	CDialogEx::OnCancel();
}

void CcompileJavaFastDlg::OnBnClickedEditSourcepath() {
	CString sourcePath;
	GetDlgItemText(IDC_EDIT1, sourcePath);

	try {
		CString errStr;
		typedef CString(*lpFun)(CString);
		HINSTANCE hDll = LoadLibrary(L"itemEditor.dll");
		if (hDll == NULL) { errStr = _T("DLL NOT FOUND");  throw errStr; }
		lpFun pEditItem = (lpFun)GetProcAddress(hDll, "editItem");
		if (pEditItem == NULL) { errStr = _T("FUNCTION NOT FOUND"); throw errStr; }
		sourcePath = pEditItem(sourcePath);
		CloseHandle(hDll);
	}
	catch (CString msg) {
		MessageBox(_T("加载资源时出错\r\n请修复程序后重试"), msg, MB_ICONERROR | MB_OK);
	}
	
	if (!sourcePath.IsEmpty()) {
		// 取Path中的第一个path
		int index = sourcePath.Find(_T(";"));
		if (index == -1) { index = sourcePath.GetLength(); }

		GetDlgItem(IDC_EDIT1)->SetWindowText(sourcePath);	// 获取编辑对话框返回的数据
		GetDlgItem(IDC_EDIT2)->SetWindowText(sourcePath.Left(index));

		saveConfigs("sourcePath", CT2A(sourcePath.GetBuffer()), "objectPath", CT2A(sourcePath.Left(index).GetBuffer()));
	}
}

void CcompileJavaFastDlg::OnBnClickedEditClasspath() {
	CString classPath;
	GetDlgItemText(IDC_EDIT5, classPath);

	try {
		CString errStr;
		typedef CString(*lpFun)(CString);
		HINSTANCE hDll = LoadLibrary(L"itemEditor.dll");
		if (hDll == NULL) { errStr = _T("DLL NOT FOUND");  throw errStr; }
		lpFun pEditItem = (lpFun)GetProcAddress(hDll, "editItem");
		if (pEditItem == NULL) { errStr = _T("FUNCTION NOT FOUND"); throw errStr; }
		classPath = pEditItem(classPath);
		CloseHandle(hDll);
	}
	catch (CString msg) {
		MessageBox(_T("加载资源时出错\r\n请修复程序后重试"), msg, MB_ICONERROR | MB_OK);
	}

	if (!classPath.IsEmpty()) {
		GetDlgItem(IDC_EDIT5)->SetWindowText(classPath);	// 获取编辑对话框返回的数据
		saveConfigs("classPath", CT2A(classPath.GetBuffer()));
	}
}

void CcompileJavaFastDlg::OnBnClickedEditObjectpath() {
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = m_hWnd;
	TCHAR tchPath[255];
	bInfo.lpszTitle = _T("请选择Java字节码文件存放的目录");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_EDITBOX | BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST lpDlist = SHBrowseForFolder(&bInfo); //显示选择对话框
	if (lpDlist != NULL) {
		SHGetPathFromIDList(lpDlist, tchPath); //把项目标识列表转化成目录
		CString filePath;
		filePath.Format(_T("%s"), tchPath);
		GetDlgItem(IDC_EDIT2)->SetWindowText(filePath);
		saveConfigs("objectPath", CT2A(filePath.GetBuffer()));
	}
}

void CcompileJavaFastDlg::OnBnClickedEditEncodingList() {
	int NRow = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCount();
	CString str, current;
	for (int i = 0; i < NRow; i++) {
		((CComboBox*)GetDlgItem(IDC_COMBO1))->GetLBText(i, current);
		str += current + _T(";");
	}

	try {
		CString errStr;
		typedef CString(*lpFun)(CString);
		HINSTANCE hDll = LoadLibrary(L"itemEditor.dll");
		if (hDll == NULL) { errStr = _T("DLL NOT FOUND");  throw errStr; }
		lpFun pEditItem = (lpFun)GetProcAddress(hDll, "editItem");
		if (pEditItem == NULL) { errStr = _T("FUNCTION NOT FOUND"); throw errStr; }
		str = pEditItem(str);
		CloseHandle(hDll);
	}
	catch (CString msg) {
		MessageBox(_T("加载资源时出错\r\n请修复程序后重试"), msg, MB_ICONERROR | MB_OK);
	}

	int index;
	if (!str.IsEmpty()) {
		((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();
		saveConfigs("encodeConf", CT2A(str.GetBuffer()));
		while (str.GetLength() != 0) {
			// 取Path中的第一个path
			index = str.Find(_T(";"));
			if (index == -1) { index = str.GetLength(); }
			current = str.Left(index);
			str.Delete(0, index + 1);
			
			// 向ListControl提交
			if (current.GetLength() != 0) {
				((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(current);
			}
		}
		((CComboBox*)GetDlgItem(IDC_COMBO1))->SelectString(0, _T("UTF-8"));
		GetDlgItem(IDC_COMBO1)->GetWindowText(str);
		if (str != _T("UTF-8")) {
			((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);
		}
		saveConfigs("encoding", ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel());
	}
}

void CcompileJavaFastDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult) {
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	runTools(pNMItemActivate->iItem);
	*pResult = 0;
}
