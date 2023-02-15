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

	bool StartCheat();												//开启作弊器
	void Init(HWND hWnd);											//初始化游戏控制器,初始化YiXin引擎
	void SetTimesLimit(int time_sec);								//设置YiXin思考时间上限值
	bool RunStartGame(int timeLimit = 60);							//等待游戏直到游戏开始
	bool RunGame();													//自动对弈直到游戏结束
	CString GetError();												//获取错误信息
	bool LoadChessboardMat(Mat& screen);							//加载游戏画面
	double GetSpend();												//获取时间间隔
	bool PutChess(int row, int col);								//执行YiXin计算的走棋位置
	HWND GetUIWnd();												//获取UI界面的句柄
	void SetAutoMode(bool bAuto);									//设置自动模式
	bool IsAutoMode();												//判断当前是否为自动模式
	void NewTableRightNow();										//打开换桌开关
	bool IsCheatRunning();											//外挂是否运行中
private:
	bool NewTable();												//换桌
	void ResetGame();												//重置棋盘，重置YiXin引擎
	bool GetOppoChessType(int& type);								//获取对手的棋子类型
	void RefreshHandles();											//刷新游戏句柄
	void ClickButton(HWND hWnd);									//点击游戏中的按钮
	void ClickChessboard(CPoint point);								//模拟下棋点击操作
	void ClickFindTable();											//模拟点击快速开始按钮
	CPoint Module2Screen(int row, int col);							//棋盘走棋点转化为棋盘窗口位置
	int GetWhiteCount(Mat mat);										//获取图像中白点数量
	void CImageToMat(CImage& cimage, Mat& mat);						//CImage转化为Mat
	bool Turn(int row, int col);									//计算对手走棋后的应对点
	bool DetectExcactPoint(int row, int col, int& type);			//检测指定位置的棋子类型
	bool DetectOppoMove(RES_MOVE_POINT& res);						//获取棋盘变动情况
	bool FindTable();												//寻找一个游戏桌
private:
	GAME_HANDLES m_gameHandles;										//游戏中的一些句柄
	CString m_strError;												//错误信息
	int m_mapCur[15][15];											//当前的棋盘矩阵
	HWND m_hWndUI;													//控制界面窗口句柄
	int m_nSide;													//当前我方棋子类型
	Pipe* m_pPipe;													//弈心引擎连接管道
	int64 m_nTime;													//时间记录器
	Mat m_matBlackSide;												//黑色方的标志图片
	Mat m_matEmptyTable;											//空白的棋盘图片
	bool m_isAutoMode;												//是否自动开始下一局
	bool m_isNewTableBtnClicked;									//是否点击了换桌按钮
	bool m_isRunning;												//当前是否正在运行外挂
public:
	void StopCheat();
};

