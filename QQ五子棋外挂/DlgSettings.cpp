// DlgSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "QQ五子棋外挂.h"
#include "DlgSettings.h"
#include "afxdialogex.h"


// CDlgSettings 对话框

IMPLEMENT_DYNAMIC(CDlgSettings, CDialogEx)

CDlgSettings::CDlgSettings(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgSettings::IDD, pParent)
{

}

CDlgSettings::~CDlgSettings()
{
}

void CDlgSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_isAutoMode);
}


BEGIN_MESSAGE_MAP(CDlgSettings, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TIME_L, &CDlgSettings::OnDeltaposSpinTimeL)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TIME_U, &CDlgSettings::OnDeltaposSpinTimeU)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CDlgSettings::OnBnClickedCheckAuto)
END_MESSAGE_MAP()


// CDlgSettings 消息处理程序



void CDlgSettings::OnDeltaposSpinTimeL(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (pNMUpDown->iDelta > 0)
	{
		m_nTimeL = m_nTimeL - 1 < 0 ? 0 : m_nTimeL - 1;
	}
	else
	{
		m_nTimeL = m_nTimeL + 1 > 10 ? 10 : m_nTimeL + 1;
		if (m_nTimeL > m_nTimeU)
		{
			m_nTimeU = m_nTimeL;
		}
	}
	UpdateTimeLimit();
	*pResult = 0;
}


void CDlgSettings::OnDeltaposSpinTimeU(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (pNMUpDown->iDelta > 0)
	{
		m_nTimeU = m_nTimeU - 1 < 0 ? 0 : m_nTimeU - 1;
		if (m_nTimeL > m_nTimeU)
		{
			m_nTimeL = m_nTimeU;
		}
	}
	else
	{
		m_nTimeU = m_nTimeU + 1 > 10 ? 10 : m_nTimeU + 1;
	}
	UpdateTimeLimit();
	*pResult = 0;
}


void CDlgSettings::OnBnClickedCheckAuto()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
}


void CDlgSettings::UpdateTimeLimit()
{
	CString str;
	str.Format(_T("佯装思考：%d秒"), m_nTimeL); SetDlgItemText(IDC_STATIC_TIMEL, str);
	str.Format(_T("思考上限：%d秒"), m_nTimeU); SetDlgItemText(IDC_STATIC_TIMEU, str);
}


BOOL CDlgSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE); 
	UpdateTimeLimit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
