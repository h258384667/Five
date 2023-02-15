#pragma once


// CDlgSettings 对话框

class CDlgSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSettings)

public:
	CDlgSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSettings();

// 对话框数据
	enum { IDD = IDD_DLG_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
