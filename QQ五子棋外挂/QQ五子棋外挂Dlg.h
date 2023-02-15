
// QQ五子棋外挂Dlg.h : 头文件
//

#pragma once
#include "GameController.h"
#include "atltypes.h"
#include "afxcmn.h"


// CQQ五子棋外挂Dlg 对话框
class CQQ五子棋外挂Dlg : public CDialogEx
{
// 构造
public:
	CQQ五子棋外挂Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QQ_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
