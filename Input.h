/* Project:		GSMPre
   File Name:	Input.h
   Author:		刘芳
   Date:		2015-9-15
   Purpose:		输入模块  */

#ifndef _Input
#define _Input

//------------------------------------------------------------------------------
// Typedef:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
enum Key{
	KeyR,
	KeyESC,
	Key1,
	Key2,
	KeyUp,
	KeyDown,
	KeyLeft,
	KeyRight,
	KeyReturn,
	KeyNUM    // 总放在最后一个
};
boolean KeyPressed[KeyNUM];
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// 处理输入
LRESULT CALLBACK Input_Handle(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void Input_Initialize(void);
#endif