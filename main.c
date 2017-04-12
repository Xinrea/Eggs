/* Project:		GSMPre
   File Name:	Main.c
   Author:		Xinrea
   Date:		2017-4-11
   Purpose:		主程序模块  */

#include "System.h"
#include "Input.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "stdio.h"

#define FILE_PATH "output.txt"
FILE *fp;
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------
int Counter;
int Lives=1;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

int main(){
    fp=fopen(FILE_PATH,"a+");
    System_Initialize();
    GSM_Initialize();
    while(GS!=GS_Quit){
        if(GS==GS_L1){
            State.Current=State_Level1;
            State.Next=State_Level2;
        }
        if(GS==GS_L2){
            State.Previous=State.Current;
            State.Current=State.Next;
            State.Next=State_Quit;
        }

        GSM_Update();
        Load();
        do {
            Init();
            while (Counter > 0) {
                Input_Handle();
                Update();
                Draw();
            }
            Lives--;
            if(Lives>0)GS=GS_Restart;
            else if(GS==GS_L1)GS=GS_L2;
            else GS=GS_Quit;
            Free();
        }while(GS==GS_Restart);
            Unload();
        }
    System_Exit();
    return 0;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
