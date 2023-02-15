#pragma once


// CDlgSettings �Ի���

class CDlgSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSettings)

public:
	CDlgSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSettings();

// �Ի�������
	enum { IDD = IDD_DLG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDeltaposSpinTimeL(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinTimeU(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckAuto();
	void UpdateTimeLimit();
public:
	int m_nTimeL;
	int m_nTimeU;
	BOOL m_isAutoMode;
	virtual BOOL OnInitDialog();
};
