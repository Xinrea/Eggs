/* Project:		GSMPre
   File Name:	GameStateManager.c
   Author:		Xinrea
   Date:		2017-4-11
   Purpose:		GSM */

#include "GameStateManager.h"
#include "GameStateList.h"
#include "LevelChoose.h"
#include "Level2.h"
#include "stdio.h"
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
void GSM_Initialize(){
	State.Previous = State_Level1;
	State.Current = State_Level1;
    State.Next=State_Level1;
	GS = GS_L1;
    fprintf(fp,"GSM:Initialize\n");
}

void GSM_Update(){
    State.Current();
    fprintf(fp,"GSM:Update\n");
}

void State_Level1(void){//状态为第一关
	GS = GS_L2;
    Load=Load1;
    Init=Ini1;
    Update=Update1;
    Draw=Draw1;
    Free=Free1;
    Unload=Unload1;
}

void State_Level2(void){//状态为第二关
	GS = GS_L1;
    Load=Load2;
    Init=Ini2;
    Update=Update2;
    Draw=Draw2;
    Free=Free2;
    Unload=Unload2;
}

void State_Restart(void) {//状态为重新开始
}

void State_Quit(void){//状态为退出
    Free();
    Unload();
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
