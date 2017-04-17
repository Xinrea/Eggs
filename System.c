/* Project:		GSMPre
   File Name:	System.c
   Author:		Xinrea
   Date:		2017-4-9
   Purpose:		SYSTEM */

#include "System.h"
#include "lib/AEEngine.h"
#include "Input.h"
#pragma comment (lib, "lib/Alpha_Engine.lib")

extern FILE* fp;
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
int System_Initialize(HINSTANCE hInstance, int nCmdShow)
{
    // 利用Alpha创建窗口
    AESysInitInfo sysInitInfo;
    sysInitInfo.mAppInstance		= hInstance;	// WinMain的第1个参数
    sysInitInfo.mShow				= nCmdShow;		// WinMain的第4个参数
    sysInitInfo.mWinWidth			= 800;
    sysInitInfo.mWinHeight			= 600;
    sysInitInfo.mCreateConsole		= 1;			// 是否需要打开控制台
    sysInitInfo.mCreateWindow		= 1;			// 是否需要创建窗口
    sysInitInfo.mMaxFrameRate		= 60;			// 设置帧率（如果使用Alpha的帧率控制功能的话）
    sysInitInfo.mpWinCallBack		= Input_Handle;
    sysInitInfo.mClassStyle			= CS_HREDRAW | CS_VREDRAW;		// 窗口类定义的重绘方式
    sysInitInfo.mWindowStyle		= WS_OVERLAPPEDWINDOW;			// 窗口风格，取值：WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    // Alpha系统初始化 及 模块重置
    if(0 == AESysInit (&sysInitInfo))
        return -1;
    AESysReset();

    // 打开签到表
    if ((fp = fopen("Output.txt","w")) == NULL)
    {
        printf("Cannot find file Output.txt\n");
        return -1;
    }
    // 签到
    fprintf(fp,"System:Initialize\n");

    return 0;
}

void System_Exit(void)
{
    // Alpha系统退出
    AESysExit();

    // 签到
    fprintf(fp,"System:Exit\n");
    fclose(fp);
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
