#include "stdafx.h"
#include "Pipe.h"


DWORD __stdcall ThrPipeThreadRead(void *www)
{
	Pipe * pipe = (Pipe *)www;
	pipe->loop();
	return 0;
	//创建内核对象使用完之后一定记得关闭，有可能会产生内存泄露
}


DWORD __stdcall WaitPipe(void *www)
{
	Pipe * pipe = (Pipe *)www;
	pipe->isloop();
	return 0;
}


Pipe::Pipe(char * com)
{
	ai.nLength = sizeof(SECURITY_ATTRIBUTES);
	ai.bInheritHandle = true;
	ai.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&hpiperead, &hpipewrite, &ai, 0))  //创建读入管道
	{

		this->setStatus(-1, "[0x01]Read 流创建失效");
		return;
	}

	if (!CreatePipe(&hpiperead2, &hpipewrite2, &ai, 0))  //创建读入管道
	{

		this->setStatus(-1, "[0x02]Write 流创建失效");
		return;
	}
	GetStartupInfoA(&si);    //获取当前进程的STARTUPINFO
	si.cb = sizeof(STARTUPINFO);
	si.hStdError = hpipewrite;
	si.hStdOutput = hpipewrite;
	si.hStdInput = hpiperead2;
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	if (!(CreateProcessA(NULL, com, NULL, NULL, true, NULL, NULL, NULL, &si, &pi)))      //创建隐藏的CMD进程
	{
		this->setStatus(-1, "[0x03] CreateProcess函数执行出错");
		return;
	}


	DWORD dwThread = FALSE;
	hThrLoop = CreateThread(NULL, 0, ThrPipeThreadRead, this, 0, &dwThread);//chuangjian
	if (hThrLoop == false){
		this->setStatus(-1, "[0x11] 线程创建失败 CreateThread LOOP 失败");
		return;
	}
	hThrLoop = CreateThread(NULL, 0, WaitPipe, this, 0, &dwThread);//chuangjian
	if (hThrLoop == false){
		this->setStatus(-1, "[0x12] 线程创建失败 CreateThread ISLOOP失败");
		return;
	}
}


Pipe::~Pipe()
{
	//创建内核对象使用完之后一定记得关闭，有可能会产生内存泄露
	this->setPipeOn(false);
	this->setStatus(-1);
	CloseHandle(hThrisLoop);
	CloseHandle(hThrLoop);
	CloseHandle(hpipewrite);
	CloseHandle(hpiperead);
	CloseHandle(hpiperead2);
	CloseHandle(hpipewrite2);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

}


const INT Pipe::getStatus() const
{
	return this->status;
}


const string & Pipe::getError() const
{
	return this->errorString;
}


const BOOL Pipe::isPipeOff() const
{
	return pipe;
}


void Pipe::setPipeOn(const BOOL bools)
{
	this->pipe = bools;
}


void Pipe::setStatus(const INT status, const char * info)
{
	this->errorString = info;    //你说会不会有更好的赋值方法?
	this->status = status;
}


void Pipe::setStatus(const INT status = 1)
{
	this->status = status;
}


const BOOL Pipe::sendCommand(const char * com)     //执行命令
{
	DWORD dwWrite = 0;
	char www[1024];
	strcpy_s(www, com);
	strcat_s(www, "\n");
	return WriteFile(hpipewrite2, www, strlen(www), &dwWrite, NULL);
	//0x001C7796 处有未经处理的异常(在 ConsoleApplication2.exe 中):  0xC0000005:  读取位置 0x0000000C 时发生访问冲突。
}


void Pipe::loop(){
	char outbuff[4096];        //输出缓冲
	DWORD byteread;
	this->setPipeOn(true);
	while (true)
	{
		memset(outbuff, '\0', 4096);
		if (ReadFile(this->hpiperead, outbuff, 4095, &byteread, NULL) == NULL)
		{
			this->setPipeOn(false);
			break;
		}
		//Sleep(10);
		SendMessage(hWndUI, WM_READ_PIPE, (WPARAM)outbuff, NULL);
		memset(outbuff, '\0', 4096);
	}
	this->setPipeOn(false);
	SendMessage(hWndUI, WM_READ_PIPE, (WPARAM)"pipe Stoped", NULL);
}


void Pipe::isloop()
{
	DWORD dwRet = WaitForSingleObject(pi.hProcess, INFINITE);
	while (dwRet == WAIT_TIMEOUT)
	{
		dwRet = WaitForSingleObject(pi.hProcess, INFINITE);
	}

	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED)
	{
		this->setPipeOn(false);
		SendMessage(hWndUI, WM_READ_PIPE, (WPARAM)"已退出弈心引擎！", NULL);
	}
}
