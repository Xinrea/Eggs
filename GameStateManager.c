/* Project:		GSMPre
   File Name:	GameStateManager.c
   Author:		Xinrea
   Date:		2017-4-8
   Purpose:		GSM */

#include "GameStateManager.h"
#include "Level1.h"
#include "Level2.h"

//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef Sruct State{
    int (* Previous)(void);
    int (* Current)(void);
    int (* Next)(void);
};
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
int (*Load )(void);
int (*Ini )(void);
int (*Update )(void);
int (*Draw )(void);
int (*Free )(void);
int (*Unload )(void);
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void GSM_Initialize(){
    State.Current=State_Level1;
    State.Next=State_Level2;
    //More...
}

void GSM_Update(){

}

void State_Level1(){
    Load=Load1;
    Init=Init1;
    Update=Update1;
    Draw=Draw1;
    Free=Free1;
    Unload=Unload1;
}

void State_Level2(){
    Load=Load2;
    Init=Init2;
    Update=Update2;
    Draw=Draw2;
    Free=Free2;
    Unload=Unload2;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
