
// QQ���������Dlg.h : ͷ�ļ�
//

#pragma once
#include "GameController.h"
#include "atltypes.h"
#include "afxcmn.h"


// CQQ���������Dlg �Ի���
class CQQ���������Dlg : public CDialogEx
{
// ����
public:
	CQQ���������Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnReadPipe(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRunCheat();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnNewTable();
	afx_msg void OnBnClickedBtnSettings();
private:
	GameController m_gameController;
	CPoint m_ptTimer;
	int m_nTimeL;
	int m_nTimeU;
	BOOL m_isAutoMode;
public:
	afx_msg void OnBnClickedBtnStop();
};
