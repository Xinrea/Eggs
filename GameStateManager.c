/* Project:		GSMPre
   File Name:	GameStateManager.c
   Author:		Xinrea
   Date:		2017-4-11
   Purpose:		GSM */

#include "GameStateManager.h"
#include "GameStateList.h"
#include "Level1.h"
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
void State_Level1();
void State_Level2();
void State_Quit();
//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void GSM_Initialize(){
    State.Previous=State_Level1;
    State.Current=State_Level1;
    State.Next=State_Level1;
    GS=GS_L1;
    fprintf(fp,"GSM:Initialize\n");
}

void GSM_Update(){
    State.Current();
    fprintf(fp,"GSM:Update\n");
}

void State_Level1(){//状态为第一关
    Load=Load1;
    Init=Ini1;
    Update=Update1;
    Draw=Draw1;
    Free=Free1;
    Unload=Unload1;
}

void State_Level2(){//状态为第二关
    Load=Load2;
    Init=Ini2;
    Update=Update2;
    Draw=Draw2;
    Free=Free2;
    Unload=Unload2;
}

void State_Quit(){//状态为退出
    Free();
    Unload();
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
