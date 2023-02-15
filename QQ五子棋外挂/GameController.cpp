#include "stdafx.h"
#include "GameController.h"
#include "resource.h"


GameController::GameController()
: m_strError(_T(""))
, m_nSide(0)
, m_pPipe(NULL)
, m_isAutoMode(false)
, m_isNewTableBtnClicked(false)
, m_isRunning(false)
{
}


void GameController::SetTimesLimit(int time_sec)
{
	char timeInfo[25];
	sprintf_s(timeInfo, "infotimeout_turn%d", time_sec * 1000);
	m_pPipe->sendCommand(timeInfo);
}


void GameController::Init(HWND hWnd)
{
	m_hWndUI = hWnd;
	m_pPipe = new Pipe("pbrain-Yixin17_64.exe");
	m_pPipe->sendCommand("start15");
	m_pPipe->hWndUI = m_hWndUI;
	SetTimesLimit(3);
	m_matEmptyTable = imread("images\\EmptyTable.jpg");
	m_matBlackSide = imread("images\\blackSide.jpg", IMREAD_GRAYSCALE);
	m_isAutoMode = true;
}


GameController::~GameController()
{
	m_pPipe->sendCommand("end");
}


void GameController::RefreshHandles()
{
	m_gameHandles.hWndBtnBegin = NULL;
	m_gameHandles.hWndBtnLeave = NULL;
	m_gameHandles.hWndRoom = NULL;
	m_gameHandles.hWndTable = NULL;
	m_gameHandles.hWndChessboard = NULL;
	EnumWindows(FindGameWnds, (LPARAM)&m_gameHandles);
}


bool GameController::FindTable()
{
	//验证窗口句柄
	RefreshHandles();
	if (m_gameHandles.hWndRoom == NULL)
	{
		m_strError = _T("寻找座位失败，未检测到游戏房间！");
		return false;
	}
	//如果窗口被最小化，则恢复
	if (IsIconic(m_gameHandles.hWndRoom))
	{
		SendMessage(m_gameHandles.hWndRoom, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	//循环直到找到满足分数要求的座位
	while (m_gameHandles.hWndBtnBegin == NULL)
	{
		//点击快速开始游戏按钮
		ClickFindTable();
		Sleep(500);
		//判断提示信息对话框
		HWND hWndWarning = FindWindow(_T("GRootViewClass"), _T("提示信息"));
		if (IsWindowEnabled(hWndWarning))
		{
			PostMessage(hWndWarning, WM_CLOSE, NULL, NULL);
			EnableWindow(hWndWarning, FALSE);
			continue;
		}
		//刷新游戏句柄
		RefreshHandles();
	}
	//成功找到座位，点击开始游戏按钮
	MoveWindow(m_gameHandles.hWndTable, 0, 0, 500, 500, TRUE);
	SendMessage(m_gameHandles.hWndQQGame, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	ClickButton(m_gameHandles.hWndBtnBegin);
	EnableWindow(GetDlgItem(m_hWndUI, IDC_BTN_NEW_TABLE), TRUE);
	return true;
}


bool GameController::StartCheat()
{
	RefreshHandles();
	//如果还没有选择座位，则选择座位
	if (!m_gameHandles.hWndChessboard)
	{
		if (!FindTable())
		{
			m_strError = _T("外挂开启失败，") + m_strError;
			return false;
		}
	}
	//如果成功选择座位
	if (m_gameHandles.hWndChessboard)
	{
		MoveWindow(m_gameHandles.hWndTable, 0, 0, 0, 0, TRUE);
		CRect rect; GetWindowRect(m_gameHandles.hWndTable, rect);
		CRect rectUI; GetWindowRect(m_hWndUI, rectUI);
		MoveWindow(m_hWndUI, rect.Width(), 0, rectUI.Width(), rectUI.Height(), TRUE);
		m_isRunning = true;
		EnableWindow(GetDlgItem(m_hWndUI, IDC_BTN_NEW_TABLE), TRUE);
		AfxBeginThread(WaitForBeginLoop, this);
		return true;
	}
	else
	{
		m_strError = _T("外挂开启失败，未能成功选择座位！");
		return false;
	}
}


void GameController::StopCheat()
{
	m_isRunning = false;
}


bool GameController::NewTable()
{
	RefreshHandles();
	//如果已经在座位中，并且未开始游戏，则退出当前座位
	if (!m_gameHandles.hWndBtnLeave)
	{
		m_strError = _T("当前并不在座位上，无法换座！");
		return false;
	}
	HWND hBtnGiveUp = FindWindowEx(m_gameHandles.hWndChessboard, m_gameHandles.hWndBtnBegin, NULL, NULL);
	if (IsWindowEnabled(hBtnGiveUp))
	{
		m_strError = _T("当前正在游戏中，无法换座！");
		return false;
	}
	//循环等待退出成功
	while (m_gameHandles.hWndBtnBegin)
	{
		ClickButton(m_gameHandles.hWndBtnLeave);
		RefreshHandles();
	}
	//寻找新的座位
	if (!FindTable())
	{
		m_strError = _T("更换座位失败，") + m_strError;
		return false;
	}
	return true;
}


bool GameController::LoadChessboardMat(Mat& screen)
{
	//判断句柄是否有效
	if (m_gameHandles.hWndTable == NULL)
	{
		m_strError = _T("加载棋盘失败,未选择座位！");
		return false;
	}
	//如果窗口被最小化，则恢复
	if (IsIconic(m_gameHandles.hWndTable))
	{
		SendMessage(m_gameHandles.hWndTable, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	//加载游戏图像
	CRect chessboardClient;
	GetClientRect(m_gameHandles.hWndChessboard, chessboardClient);
	HDC hDCGame = GetDC(m_gameHandles.hWndChessboard);
	HDC hDCMem = CreateCompatibleDC(hDCGame);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDCGame, chessboardClient.Width(), chessboardClient.Height()); SelectObject(hDCMem, hBitmap); 
	if (!PrintWindow(m_gameHandles.hWndChessboard, hDCMem, PW_CLIENTONLY))
	{
		m_strError = _T("读取棋盘画面失败");
		return false;
	}
	CImage img; img.Create(chessboardClient.Width(), chessboardClient.Height(), 24); BitBlt(img.GetDC(), 0, 0, chessboardClient.Width(), chessboardClient.Height(), hDCMem, 0, 0, SRCCOPY); img.ReleaseDC();
	CImageToMat(img, screen);
	return true;
}


void GameController::CImageToMat(CImage& cimage, Mat& mat)
{
	if (true == cimage.IsNull())
	{
		return;
	}
	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();
	//重建mat  
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}
	//拷贝数据  
	uchar* pucRow;                                  //指向数据区的行指针  
	uchar* pucImage = (uchar*)cimage.GetBits();     //指向数据区的指针  
	int nStep = cimage.GetPitch();                  //每行的字节数,注意这个返回值有正有负  
	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}


int GameController::GetWhiteCount(Mat mat)
{
	try
	{
		int count = 0;
		Mat_<uchar>::iterator it = mat.begin<uchar>();
		Mat_<uchar>::iterator itend = mat.end<uchar>();
		for (; it != itend; ++it)
		{
			if ((*it) > 0) count += 1;
		}
		return count;
	}
	catch (cv::Exception err)
	{
		AfxMessageBox(CString(err.what()));
		return 0;
	}
}


CPoint GameController::Module2Screen(int row, int col)
{
	//计算左上角坐标
	CPoint tl, org(227, 128);
	tl.x = org.x + col * 247 / 7;
	tl.y = org.y + row * 247 / 7;
	return tl;
}


void GameController::ClickChessboard(CPoint point)
{
	SetForegroundWindow(m_gameHandles.hWndRoom);
	CRect windowRect; ::GetWindowRect(m_gameHandles.hWndChessboard, windowRect);
	CPoint curPoint; GetCursorPos(&curPoint);
	CPoint clcPoint = point + windowRect.TopLeft();
	SetCursorPos(clcPoint.x, clcPoint.y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
	SetCursorPos(curPoint.x, curPoint.y);
}


void GameController::ClickFindTable()
{
	CPoint beginPoint(354, 156);
	SetForegroundWindow(m_gameHandles.hWndRoom);
	CRect windowRect; ::GetWindowRect(m_gameHandles.hWndRoom, windowRect);
	CPoint curPoint; GetCursorPos(&curPoint);
	CPoint clcPoint = beginPoint + windowRect.TopLeft();
	SetCursorPos(clcPoint.x, clcPoint.y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
	Sleep(100);
	SetCursorPos(curPoint.x, curPoint.y);
}


void GameController::ClickButton(HWND hWnd)
{
	CPoint point(0, 0);
	SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(point.x, point.y));
	SendMessage(hWnd, WM_LBUTTONUP, NULL, MAKELPARAM(point.x, point.y));
	//SendMessage(hWnd, WM_MOUSEMOVE, NULL, MAKELPARAM(point.x, point.y));
	//PostMessage(hWnd, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(point.x, point.y));
	//Sleep(100);
}


CString GameController::GetError()
{
	return m_strError;
}


bool GameController::GetOppoChessType(int& type)
{
	//获取当前棋盘图像
	Mat curScreen;
	if (!LoadChessboardMat(curScreen))
	{
		m_strError = _T("获取对手棋子类型失败，") + m_strError;
		return false;
	}
	//解析对方的棋子类型
	cvtColor(curScreen, curScreen, COLOR_RGB2GRAY);
	Mat oppoType = curScreen(Rect(11, 403, 46, 42));
	Mat diff; absdiff(oppoType, m_matBlackSide, diff); threshold(diff, diff, 50, 255, THRESH_BINARY);
	//imshow("oppoType", oppoType); 
	//imshow("diff", diff);waitKey(1);
	int count = GetWhiteCount(diff);
	if (count == 0)
	{
		type = CHESS_BLACK;
	}
	else if (count == 371)
	{
		type = CHESS_WHITE;
	}
	else
	{
		type = CHESS_EMPTY;
	}
	return true;
}


bool GameController::RunStartGame(int timeLimit)
{
	//如果没点开始按钮，则点击开始按钮
	if (IsWindowEnabled(m_gameHandles.hWndBtnBegin))
	{
		ClickButton(m_gameHandles.hWndBtnBegin);
	}
	//重置游戏
	ResetGame();
	//循环等待对方点击开始按钮，超时换桌
	int chessType = CHESS_EMPTY;
	double start = getTickCount();
	while (chessType == CHESS_EMPTY && m_isRunning)
	{
		if (!GetOppoChessType(chessType))
		{
			m_strError = _T("等待游戏开始时发生错误，") + m_strError;
			return false;
		}
		if (chessType == CHESS_BLACK)
		{
			m_nSide = CHESS_WHITE; break;
		}
		if (chessType == CHESS_WHITE)
		{
			m_nSide = CHESS_BLACK; m_pPipe->sendCommand("begin"); break;
		}
		//如果等待超时，或者点击了自动换桌按钮，则换桌
		double spend = (getTickCount() - start) / getTickFrequency();
		if (spend > timeLimit || m_isNewTableBtnClicked)
		{
			m_isNewTableBtnClicked = false;
			if (!NewTable())
			{
				m_strError = _T("等待游戏开始时发生错误，") + m_strError;
				return false;
			}
			start = getTickCount();
		}
	}
	return true;
}


bool GameController::DetectExcactPoint(int row, int col, int& type)
{
	//加载当前棋盘图像并处理
	Mat gameMat;
	if (!LoadChessboardMat(gameMat))
	{
		m_strError = _T("检测棋子类型失败，") + m_strError;
		return false;
	}
	Mat diff;
	try
	{
		absdiff(gameMat, m_matEmptyTable, diff);
		cvtColor(diff, diff, COLOR_RGB2GRAY);
		threshold(diff, diff, 60, 255, THRESH_BINARY);
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		erode(diff, diff, element);
		dilate(diff, diff, element);
	}
	catch (cv::Exception err)
	{
		CString str(err.what());
		AfxMessageBox(str);
		m_strError = _T("检测棋子类型发生异常，") + str;
		return false;
	}
	//计算左上角坐标
	CPoint tl, org(219, 125);
	tl.x = org.x + (2 * col - 1) * 123 / 7;
	tl.y = org.y + (2 * row - 1) * 123 / 7;
	//统计白点数量
	int counter = GetWhiteCount(diff(Rect(tl.x, tl.y, 35, 35)));
	//根据白点数量判别棋子类型
	if (counter < 60)
	{
		type = CHESS_EMPTY;
	}
	else if (counter < 600)
	{
		type = CHESS_WHITE; 
	}
	else
	{
		type = CHESS_BLACK; 
	}
	return true;
}


bool GameController::DetectOppoMove(RES_MOVE_POINT& res)
{
	//加载当前棋盘图像
	Mat gameMat;
	if (!LoadChessboardMat(gameMat))
	{
		m_strError = _T("检测棋盘变动失败，") + m_strError;
		return false;
	}
	//预处理当前棋盘图像
	Mat diff;
	try
	{
		absdiff(gameMat, m_matEmptyTable, diff);
		cvtColor(diff, diff, COLOR_RGB2GRAY);
		threshold(diff, diff, 60, 255, THRESH_BINARY);
		Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		erode(diff, diff, element);
		dilate(diff, diff, element);
	}
	catch (cv::Exception err)
	{
		CString str(err.what());
		AfxMessageBox(str);
		m_strError = _T("检测棋盘变动发生异常，") + str;
		return false;
	}
	//逐行逐列识别棋子
	res.isValid = false;
	int detectedType, blackCount = 0, whiteCount = 0, diffCount = 0;
	for (int row = 0; row < 15; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			//计算左上角坐标
			CPoint tl, org(219, 125);
			tl.x = org.x + (2 * col - 1) * 123 / 7;
			tl.y = org.y + (2 * row - 1) * 123 / 7;
			//统计白点数量
			int counter = GetWhiteCount(diff(Rect(tl.x, tl.y, 35, 35)));
			//根据白点数量判别棋子类型
			if (counter < 10)
			{
				detectedType = CHESS_EMPTY;
			}
			else if (counter < 60)
			{
				res.isValid = false;
				return true;
			}
			else if (counter < 600)
			{
				detectedType = CHESS_WHITE; whiteCount++;
			}
			else
			{
				detectedType = CHESS_BLACK; blackCount++;
			}
			//如果棋子类型与内存中保存的有异
			if (detectedType != m_mapCur[row][col])
			{
				if (detectedType != 3 - m_nSide)
				{
					res.isValid = false;
					return true;
				}
				diffCount++;
				res.move_row = row; res.move_col = col;
			}
		}
	}
	if (diffCount != 1 || blackCount < whiteCount || blackCount - whiteCount > 1)
	{
		res.isValid = false;
	}
	else
	{
		res.isValid = true;
	}
	return true;
}


bool GameController::RunGame()
{
	//如果开始按钮被激活，则表示游戏结束
	while (!IsWindowEnabled(m_gameHandles.hWndBtnBegin) && m_isRunning)
	{
		RES_MOVE_POINT res;
		//检测对手走棋
		if (!DetectOppoMove(res))
		{
			m_strError = _T("游戏中出现错误，") + m_strError;
			return false;
		}
		//如果检测到对面走棋，则向YiXin发送turn命令
		if (res.isValid)
		{
			if (!Turn(res.move_row, res.move_col))
			{
				m_strError = _T("游戏中出现错误，") + m_strError;
				return false;
			}
			CString strState; strState.Format(_T("对方：%d,%d\r\n我方正在思考..."), res.move_row, res.move_col);
			SetDlgItemText(m_hWndUI, IDC_EDIT_LOG, strState);
		}
		Sleep(200);
	}
	//AfxMessageBox(_T(""));
	return true;
}


void GameController::ResetGame()
{
	for (int row = 0; row < 15; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			m_mapCur[row][col] = 0;
		}
	}
	m_pPipe->sendCommand("restart");
}


bool GameController::Turn(int row, int col)
{
	if (m_mapCur[row][col] != CHESS_EMPTY)
	{
		m_strError.Format(_T("发送Turn命令失败，当前位置(%d,%d)已经有子:%d！"), row, col, m_mapCur[row][col]);
		return false;
	}
	m_mapCur[row][col] = 3 - m_nSide;
	char Command[50];
	sprintf_s(Command, "turn%d,%d", row, col);
	m_pPipe->sendCommand(Command);
	m_nTime = getTickCount();
	return true;
}


double GameController::GetSpend()
{
	int64 nTime = getTickCount();
	double spend = (nTime - m_nTime) / getTickFrequency() * 1000;
	m_nTime = nTime;
	return spend;
}


bool GameController::PutChess(int row, int col)
{
	//更新信息
	CString strState; strState.Format(_T("我方：%d,%d\r\n正在等待对方落子"), row, col);
	SetDlgItemText(m_hWndUI, IDC_EDIT_LOG, strState);
	m_mapCur[row][col] = m_nSide;
	//确保走棋成功,才退出本函数
	int type = CHESS_EMPTY;
	while (type == CHESS_EMPTY)
	{
		ClickChessboard(Module2Screen(row, col));
		Sleep(200);
		if (!DetectExcactPoint(row, col, type))
		{
			m_strError = _T("落子失败，") + m_strError;
			return false;
		}
	}
	return true;
}


HWND GameController::GetUIWnd()
{
	return m_hWndUI;
}


void GameController::SetAutoMode(bool bAuto)
{
	m_isAutoMode = bAuto;
}


bool GameController::IsAutoMode()
{
	return m_isAutoMode;
}


void GameController::Test()
{
	RefreshHandles();
	HDC hDC = GetDC(m_gameHandles.hWndTable);
	Rectangle(hDC, 10, 10, 100, 100);
	ReleaseDC(m_gameHandles.hWndTable, hDC);
	//if (!FindTable())
	//{
	//	AfxMessageBox(m_strError);
	//}
	//RefreshHandles();
	//int type;
	//if (!GetOppoChessType(type))
	//{
	//	AfxMessageBox(m_strError);
	//}
}


void GameController::NewTableRightNow()
{
	m_isNewTableBtnClicked = true;
}


bool GameController::IsCheatRunning()
{
	return m_isRunning;
}