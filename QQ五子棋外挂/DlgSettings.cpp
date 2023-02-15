// DlgSettings.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQ���������.h"
#include "DlgSettings.h"
#include "afxdialogex.h"


// CDlgSettings �Ի���

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


// CDlgSettings ��Ϣ�������



void CDlgSettings::OnDeltaposSpinTimeL(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CDlgSettings::UpdateTimeLimit()
{
	CString str;
	str.Format(_T("��װ˼����%d��"), m_nTimeL); SetDlgItemText(IDC_STATIC_TIMEL, str);
	str.Format(_T("˼�����ޣ�%d��"), m_nTimeU); SetDlgItemText(IDC_STATIC_TIMEU, str);
}


BOOL CDlgSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE); 
	UpdateTimeLimit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
