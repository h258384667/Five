#pragma once
#include <Windows.h>

struct GAME_HANDLES
{
	HWND hWndQQGame;
	HWND hWndRoom;
	HWND hWndTable;
	HWND hWndChessboard;
	HWND hWndBtnBegin;
	HWND hWndBtnLeave;
};

UINT WaitForBeginLoop(LPVOID param);
UINT YiXinLoop(LPVOID param);
BOOL CALLBACK FindGameWnds(HWND hwnd, LPARAM lParam);