#pragma once
#include "atltypes.h"
#include "pipe.h"
#include "common.h"
#include <opencv.hpp>
using namespace cv;

#define	CHESS_EMPTY	0
#define	CHESS_BLACK	1
#define	CHESS_WHITE	2

struct RES_MOVE_POINT
{
	int move_row;
	int move_col;
	bool isValid = false;
};

class GameController
{
public:
	void Test();
	GameController();
	~GameController();

	bool StartCheat();												//����������
	void Init(HWND hWnd);											//��ʼ����Ϸ������,��ʼ��YiXin����
	void SetTimesLimit(int time_sec);								//����YiXin˼��ʱ������ֵ
	bool RunStartGame(int timeLimit = 60);							//�ȴ���Ϸֱ����Ϸ��ʼ
	bool RunGame();													//�Զ�����ֱ����Ϸ����
	CString GetError();												//��ȡ������Ϣ
	bool LoadChessboardMat(Mat& screen);							//������Ϸ����
	double GetSpend();												//��ȡʱ����
	bool PutChess(int row, int col);								//ִ��YiXin���������λ��
	HWND GetUIWnd();												//��ȡUI����ľ��
	void SetAutoMode(bool bAuto);									//�����Զ�ģʽ
	bool IsAutoMode();												//�жϵ�ǰ�Ƿ�Ϊ�Զ�ģʽ
	void NewTableRightNow();										//�򿪻�������
	bool IsCheatRunning();											//����Ƿ�������
private:
	bool NewTable();												//����
	void ResetGame();												//�������̣�����YiXin����
	bool GetOppoChessType(int& type);								//��ȡ���ֵ���������
	void RefreshHandles();											//ˢ����Ϸ���
	void ClickButton(HWND hWnd);									//�����Ϸ�еİ�ť
	void ClickChessboard(CPoint point);								//ģ������������
	void ClickFindTable();											//ģ�������ٿ�ʼ��ť
	CPoint Module2Screen(int row, int col);							//���������ת��Ϊ���̴���λ��
	int GetWhiteCount(Mat mat);										//��ȡͼ���а׵�����
	void CImageToMat(CImage& cimage, Mat& mat);						//CImageת��ΪMat
	bool Turn(int row, int col);									//�������������Ӧ�Ե�
	bool DetectExcactPoint(int row, int col, int& type);			//���ָ��λ�õ���������
	bool DetectOppoMove(RES_MOVE_POINT& res);						//��ȡ���̱䶯���
	bool FindTable();												//Ѱ��һ����Ϸ��
private:
	GAME_HANDLES m_gameHandles;										//��Ϸ�е�һЩ���
	CString m_strError;												//������Ϣ
	int m_mapCur[15][15];											//��ǰ�����̾���
	HWND m_hWndUI;													//���ƽ��洰�ھ��
	int m_nSide;													//��ǰ�ҷ���������
	Pipe* m_pPipe;													//�����������ӹܵ�
	int64 m_nTime;													//ʱ���¼��
	Mat m_matBlackSide;												//��ɫ���ı�־ͼƬ
	Mat m_matEmptyTable;											//�հ׵�����ͼƬ
	bool m_isAutoMode;												//�Ƿ��Զ���ʼ��һ��
	bool m_isNewTableBtnClicked;									//�Ƿ����˻�����ť
	bool m_isRunning;												//��ǰ�Ƿ������������
public:
	void StopCheat();
};

