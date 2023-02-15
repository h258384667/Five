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
	//��֤���ھ��
	RefreshHandles();
	if (m_gameHandles.hWndRoom == NULL)
	{
		m_strError = _T("Ѱ����λʧ�ܣ�δ��⵽��Ϸ���䣡");
		return false;
	}
	//������ڱ���С������ָ�
	if (IsIconic(m_gameHandles.hWndRoom))
	{
		SendMessage(m_gameHandles.hWndRoom, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	//ѭ��ֱ���ҵ��������Ҫ�����λ
	while (m_gameHandles.hWndBtnBegin == NULL)
	{
		//������ٿ�ʼ��Ϸ��ť
		ClickFindTable();
		Sleep(500);
		//�ж���ʾ��Ϣ�Ի���
		HWND hWndWarning = FindWindow(_T("GRootViewClass"), _T("��ʾ��Ϣ"));
		if (IsWindowEnabled(hWndWarning))
		{
			PostMessage(hWndWarning, WM_CLOSE, NULL, NULL);
			EnableWindow(hWndWarning, FALSE);
			continue;
		}
		//ˢ����Ϸ���
		RefreshHandles();
	}
	//�ɹ��ҵ���λ�������ʼ��Ϸ��ť
	MoveWindow(m_gameHandles.hWndTable, 0, 0, 500, 500, TRUE);
	SendMessage(m_gameHandles.hWndQQGame, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	ClickButton(m_gameHandles.hWndBtnBegin);
	EnableWindow(GetDlgItem(m_hWndUI, IDC_BTN_NEW_TABLE), TRUE);
	return true;
}


bool GameController::StartCheat()
{
	RefreshHandles();
	//�����û��ѡ����λ����ѡ����λ
	if (!m_gameHandles.hWndChessboard)
	{
		if (!FindTable())
		{
			m_strError = _T("��ҿ���ʧ�ܣ�") + m_strError;
			return false;
		}
	}
	//����ɹ�ѡ����λ
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
		m_strError = _T("��ҿ���ʧ�ܣ�δ�ܳɹ�ѡ����λ��");
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
	//����Ѿ�����λ�У�����δ��ʼ��Ϸ�����˳���ǰ��λ
	if (!m_gameHandles.hWndBtnLeave)
	{
		m_strError = _T("��ǰ��������λ�ϣ��޷�������");
		return false;
	}
	HWND hBtnGiveUp = FindWindowEx(m_gameHandles.hWndChessboard, m_gameHandles.hWndBtnBegin, NULL, NULL);
	if (IsWindowEnabled(hBtnGiveUp))
	{
		m_strError = _T("��ǰ������Ϸ�У��޷�������");
		return false;
	}
	//ѭ���ȴ��˳��ɹ�
	while (m_gameHandles.hWndBtnBegin)
	{
		ClickButton(m_gameHandles.hWndBtnLeave);
		RefreshHandles();
	}
	//Ѱ���µ���λ
	if (!FindTable())
	{
		m_strError = _T("������λʧ�ܣ�") + m_strError;
		return false;
	}
	return true;
}


bool GameController::LoadChessboardMat(Mat& screen)
{
	//�жϾ���Ƿ���Ч
	if (m_gameHandles.hWndTable == NULL)
	{
		m_strError = _T("��������ʧ��,δѡ����λ��");
		return false;
	}
	//������ڱ���С������ָ�
	if (IsIconic(m_gameHandles.hWndTable))
	{
		SendMessage(m_gameHandles.hWndTable, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
	//������Ϸͼ��
	CRect chessboardClient;
	GetClientRect(m_gameHandles.hWndChessboard, chessboardClient);
	HDC hDCGame = GetDC(m_gameHandles.hWndChessboard);
	HDC hDCMem = CreateCompatibleDC(hDCGame);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDCGame, chessboardClient.Width(), chessboardClient.Height()); SelectObject(hDCMem, hBitmap); 
	if (!PrintWindow(m_gameHandles.hWndChessboard, hDCMem, PW_CLIENTONLY))
	{
		m_strError = _T("��ȡ���̻���ʧ��");
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
	//�ؽ�mat  
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}
	//��������  
	uchar* pucRow;                                  //ָ������������ָ��  
	uchar* pucImage = (uchar*)cimage.GetBits();     //ָ����������ָ��  
	int nStep = cimage.GetPitch();                  //ÿ�е��ֽ���,ע���������ֵ�����и�  
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
	//�������Ͻ�����
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
	//��ȡ��ǰ����ͼ��
	Mat curScreen;
	if (!LoadChessboardMat(curScreen))
	{
		m_strError = _T("��ȡ������������ʧ�ܣ�") + m_strError;
		return false;
	}
	//�����Է�����������
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
	//���û�㿪ʼ��ť��������ʼ��ť
	if (IsWindowEnabled(m_gameHandles.hWndBtnBegin))
	{
		ClickButton(m_gameHandles.hWndBtnBegin);
	}
	//������Ϸ
	ResetGame();
	//ѭ���ȴ��Է������ʼ��ť����ʱ����
	int chessType = CHESS_EMPTY;
	double start = getTickCount();
	while (chessType == CHESS_EMPTY && m_isRunning)
	{
		if (!GetOppoChessType(chessType))
		{
			m_strError = _T("�ȴ���Ϸ��ʼʱ��������") + m_strError;
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
		//����ȴ���ʱ�����ߵ�����Զ�������ť������
		double spend = (getTickCount() - start) / getTickFrequency();
		if (spend > timeLimit || m_isNewTableBtnClicked)
		{
			m_isNewTableBtnClicked = false;
			if (!NewTable())
			{
				m_strError = _T("�ȴ���Ϸ��ʼʱ��������") + m_strError;
				return false;
			}
			start = getTickCount();
		}
	}
	return true;
}


bool GameController::DetectExcactPoint(int row, int col, int& type)
{
	//���ص�ǰ����ͼ�񲢴���
	Mat gameMat;
	if (!LoadChessboardMat(gameMat))
	{
		m_strError = _T("�����������ʧ�ܣ�") + m_strError;
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
		m_strError = _T("����������ͷ����쳣��") + str;
		return false;
	}
	//�������Ͻ�����
	CPoint tl, org(219, 125);
	tl.x = org.x + (2 * col - 1) * 123 / 7;
	tl.y = org.y + (2 * row - 1) * 123 / 7;
	//ͳ�ư׵�����
	int counter = GetWhiteCount(diff(Rect(tl.x, tl.y, 35, 35)));
	//���ݰ׵������б���������
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
	//���ص�ǰ����ͼ��
	Mat gameMat;
	if (!LoadChessboardMat(gameMat))
	{
		m_strError = _T("������̱䶯ʧ�ܣ�") + m_strError;
		return false;
	}
	//Ԥ����ǰ����ͼ��
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
		m_strError = _T("������̱䶯�����쳣��") + str;
		return false;
	}
	//��������ʶ������
	res.isValid = false;
	int detectedType, blackCount = 0, whiteCount = 0, diffCount = 0;
	for (int row = 0; row < 15; row++)
	{
		for (int col = 0; col < 15; col++)
		{
			//�������Ͻ�����
			CPoint tl, org(219, 125);
			tl.x = org.x + (2 * col - 1) * 123 / 7;
			tl.y = org.y + (2 * row - 1) * 123 / 7;
			//ͳ�ư׵�����
			int counter = GetWhiteCount(diff(Rect(tl.x, tl.y, 35, 35)));
			//���ݰ׵������б���������
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
			//��������������ڴ��б��������
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
	//�����ʼ��ť��������ʾ��Ϸ����
	while (!IsWindowEnabled(m_gameHandles.hWndBtnBegin) && m_isRunning)
	{
		RES_MOVE_POINT res;
		//����������
		if (!DetectOppoMove(res))
		{
			m_strError = _T("��Ϸ�г��ִ���") + m_strError;
			return false;
		}
		//�����⵽�������壬����YiXin����turn����
		if (res.isValid)
		{
			if (!Turn(res.move_row, res.move_col))
			{
				m_strError = _T("��Ϸ�г��ִ���") + m_strError;
				return false;
			}
			CString strState; strState.Format(_T("�Է���%d,%d\r\n�ҷ�����˼��..."), res.move_row, res.move_col);
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
		m_strError.Format(_T("����Turn����ʧ�ܣ���ǰλ��(%d,%d)�Ѿ�����:%d��"), row, col, m_mapCur[row][col]);
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
	//������Ϣ
	CString strState; strState.Format(_T("�ҷ���%d,%d\r\n���ڵȴ��Է�����"), row, col);
	SetDlgItemText(m_hWndUI, IDC_EDIT_LOG, strState);
	m_mapCur[row][col] = m_nSide;
	//ȷ������ɹ�,���˳�������
	int type = CHESS_EMPTY;
	while (type == CHESS_EMPTY)
	{
		ClickChessboard(Module2Screen(row, col));
		Sleep(200);
		if (!DetectExcactPoint(row, col, type))
		{
			m_strError = _T("����ʧ�ܣ�") + m_strError;
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