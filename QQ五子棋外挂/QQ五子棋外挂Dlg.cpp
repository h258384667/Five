
// QQ���������Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQ���������.h"
#include "QQ���������Dlg.h"
#include "afxdialogex.h"
#include "DataParser.h"
#include "DlgSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CQQ���������Dlg �Ի���



CQQ���������Dlg::CQQ���������Dlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CQQ���������Dlg::IDD, pParent)
, m_ptTimer(0)
, m_nTimeL(1)
, m_nTimeU(3)
, m_isAutoMode(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQQ���������Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQ���������Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_READ_PIPE, &CQQ���������Dlg::OnReadPipe)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_RUN_CHEAT, &CQQ���������Dlg::OnBnClickedRunCheat)
	ON_BN_CLICKED(IDC_BTN_NEW_TABLE, &CQQ���������Dlg::OnBnClickedBtnNewTable)
	ON_BN_CLICKED(IDC_BTN_SETTINGS, &CQQ���������Dlg::OnBnClickedBtnSettings)
	ON_BN_CLICKED(IDC_BTN_STOP, &CQQ���������Dlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CQQ���������Dlg ��Ϣ�������

BOOL CQQ���������Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_gameController.Init(m_hWnd);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CQQ���������Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQQ���������Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CQQ���������Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQQ���������Dlg::OnBnClickedRunCheat()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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


void CQQ���������Dlg::OnBnClickedBtnStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(ID_RUN_CHEAT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_NEW_TABLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_SETTINGS)->EnableWindow(TRUE);
	m_gameController.StopCheat();
}



void CQQ���������Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//m_gameController.Test();
	CDialogEx::OnLButtonUp(nFlags, point);
}


afx_msg LRESULT CQQ���������Dlg::OnReadPipe(WPARAM wParam, LPARAM lParam)
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


void CQQ���������Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer(1);
	if (!m_gameController.PutChess(m_ptTimer.x, m_ptTimer.y))
	{
		MessageBox(m_gameController.GetError());
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CQQ���������Dlg::OnBnClickedBtnNewTable()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_gameController.NewTableRightNow();
}


void CQQ���������Dlg::OnBnClickedBtnSettings()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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