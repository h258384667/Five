
// QQ五子棋外挂Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQ五子棋外挂.h"
#include "QQ五子棋外挂Dlg.h"
#include "afxdialogex.h"
#include "DataParser.h"
#include "DlgSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CQQ五子棋外挂Dlg 对话框



CQQ五子棋外挂Dlg::CQQ五子棋外挂Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CQQ五子棋外挂Dlg::IDD, pParent)
, m_ptTimer(0)
, m_nTimeL(1)
, m_nTimeU(3)
, m_isAutoMode(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQ五子棋外挂Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQ五子棋外挂Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_READ_PIPE, &CQQ五子棋外挂Dlg::OnReadPipe)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_RUN_CHEAT, &CQQ五子棋外挂Dlg::OnBnClickedRunCheat)
	ON_BN_CLICKED(IDC_BTN_NEW_TABLE, &CQQ五子棋外挂Dlg::OnBnClickedBtnNewTable)
	ON_BN_CLICKED(IDC_BTN_SETTINGS, &CQQ五子棋外挂Dlg::OnBnClickedBtnSettings)
	ON_BN_CLICKED(IDC_BTN_STOP, &CQQ五子棋外挂Dlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CQQ五子棋外挂Dlg 消息处理程序

BOOL CQQ五子棋外挂Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	m_gameController.Init(m_hWnd);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQQ五子棋外挂Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQQ五子棋外挂Dlg::OnPaint()
{
	if (IsIconic())
	{
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
	else
	{
		CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQQ五子棋外挂Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQQ五子棋外挂Dlg::OnBnClickedRunCheat()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_gameController.StartCheat())
	{
		MessageBox(m_gameController.GetError());
	}
	else
	{
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		GetDlgItem(ID_RUN_CHEAT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SETTINGS)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BTN_NEW_TABLE)->EnableWindow(TRUE);
	}
}


void CQQ五子棋外挂Dlg::OnBnClickedBtnStop()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(ID_RUN_CHEAT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_NEW_TABLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SETTINGS)->EnableWindow(TRUE);
	m_gameController.StopCheat();
}



void CQQ五子棋外挂Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//m_gameController.Test();
	CDialogEx::OnLButtonUp(nFlags, point);
}


afx_msg LRESULT CQQ五子棋外挂Dlg::OnReadPipe(WPARAM wParam, LPARAM lParam)
{
	char* buffer = (char*)wParam;
	CString str(buffer);
	if (buffer[0] >= '0' && buffer[0] <= '9')
	{
		CDataParser parser;
		CArray<int> arr;
		parser.ParseIntArray(arr, str);
		int timeL = m_nTimeL * 1000;
		double spend = m_gameController.GetSpend();
		if (spend < timeL)
		{
			m_ptTimer.SetPoint(arr[0], arr[1]);
			SetTimer(1, timeL-spend, NULL);
		}
		else
		{
			if (!m_gameController.PutChess(arr[0], arr[1]))
			{
				m_gameController.GetError();
			}
		}
	}
	return 0;
}


void CQQ五子棋外挂Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);
	if (!m_gameController.PutChess(m_ptTimer.x, m_ptTimer.y))
	{
		MessageBox(m_gameController.GetError());
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CQQ五子棋外挂Dlg::OnBnClickedBtnNewTable()
{
	// TODO:  在此添加控件通知处理程序代码
	m_gameController.NewTableRightNow();
}


void CQQ五子棋外挂Dlg::OnBnClickedBtnSettings()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgSettings dlg;
	dlg.m_nTimeL = m_nTimeL;
	dlg.m_nTimeU = m_nTimeU;
	dlg.m_isAutoMode = m_isAutoMode;
	if (IDOK == dlg.DoModal())
	{
		m_nTimeL = dlg.m_nTimeL;
		m_nTimeU = dlg.m_nTimeU;
		m_isAutoMode = dlg.m_isAutoMode;
		m_gameController.SetTimesLimit(m_nTimeU);
		m_gameController.SetAutoMode(m_isAutoMode);
	}
}