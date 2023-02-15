#include "stdafx.h"
#include "common.h"
#include "GameController.h"
#include "resource.h"

BOOL CALLBACK FindGameWnds(HWND hwnd, LPARAM lParam)
{
	GAME_HANDLES* pHandles = (GAME_HANDLES*)lParam;
	CString windowName;
	GetWindowText(hwnd, windowName.GetBuffer(200), 200); windowName.ReleaseBuffer();
	if (windowName == _T("QQ游戏"))
	{
		pHandles->hWndQQGame = hwnd;
	}
	if (windowName == _T("五子棋"))
	{
		CString className;
		GetClassName(hwnd, className.GetBuffer(200), 200); className.ReleaseBuffer();
		if (className.Find(_T("Afx")) == 0)
		{
			//寻找游戏窗口句柄
			pHandles->hWndTable = hwnd;
			HWND hDlg = FindWindowEx(pHandles->hWndTable, NULL, NULL, NULL);
			pHandles->hWndChessboard = FindWindowEx(hDlg, NULL, NULL, NULL);
			//寻找开始按钮句柄
			for (int i = 0; i < 5; i++)
			{
				pHandles->hWndBtnBegin = FindWindowEx(pHandles->hWndChessboard, pHandles->hWndBtnBegin, NULL, NULL);
			}
			//寻找退出按钮句柄
			HWND rightPart = FindWindowEx(hDlg, pHandles->hWndChessboard, NULL, NULL);
			for (int i = 0; i < 4; i++)
			{
				pHandles->hWndBtnLeave = FindWindowEx(rightPart, pHandles->hWndBtnLeave, _T("Button"), NULL);
			}
		}
		if (className == _T("GRootViewClass") && FindWindowExW(hwnd, NULL, NULL, NULL))
		{
			pHandles->hWndRoom = hwnd;
			int count = 0;
			HWND child = NULL;
			while (true)
			{
				child = FindWindowEx(pHandles->hWndRoom, child, NULL, NULL);
				if (child)
					count++;
				else
					break;
			}
			if (count == 2)
			{
				pHandles->hWndRoom = NULL;
			}
		}
	}
	return TRUE;
}


UINT WaitForBeginLoop(LPVOID param)
{
	GameController *p = (GameController*)param;
	SetDlgItemText(p->GetUIWnd(), IDC_EDIT_LOG, _T("正在等待游戏开始...\r\n"));
	if (!p->RunStartGame())
	{
		AfxMessageBox(p->GetError());
	}
	else
	{
		AfxBeginThread(YiXinLoop, p);
		SetDlgItemText(p->GetUIWnd(), IDC_EDIT_LOG, _T("比赛开始...\r\n"));
	}
	return 0;
}


UINT YiXinLoop(LPVOID param)
{
	GameController *p = (GameController*)param;
	if (!p->RunGame())
	{
		AfxMessageBox(p->GetError());
	}
	SetDlgItemText(p->GetUIWnd(), IDC_EDIT_LOG, _T("游戏结束,等待下一局开始！\r\n"));
	if (p->IsAutoMode()&&p->IsCheatRunning())
	{
		Sleep(1000);
		AfxBeginThread(WaitForBeginLoop, p);
	}
	return 0;
}
