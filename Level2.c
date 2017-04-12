/* Project:		GSMPre
   File Name:	Level2.c
   Author:		Kobe
   Date:		2017-4-9
   Purpose:		LEVEL */

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
extern int Counter;
extern int Lives;
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
void Load2(void){
	FILE *fpRead1 = fopen("Level2_Lives.txt", "r");
	if (fpRead1 == NULL)
	{
		return 0;
	}
	fscanf(fpRead1, "%d ", &Lives);
    fprintf(fp, "Level2:Load\n");
}
void Ini2(void){
	FILE *fpRead2 = fopen("Level2_Counter.txt", "r");
	if (fpRead2 == NULL)
	{
		return 0;
	}
	fscanf(fpRead2, "%d ", &Level2_Counter);
    fprintf(fp, "Level2:Ini\n");
}
void Update2(void){
	Counter--;
    fprintf(fp, "Level2:Update\n");
}
void Draw2(void){
    fprintf(fp, "Level2:Draw\n");
}
void Free2(void){
    fprintf(fp, "Level2:Free\n");
}
void Unload2(void){
    fprintf(fp, "Level2:Unload\n");
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
