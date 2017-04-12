/* Project:		GSMPre
   File Name:	Level1.c
   Author:		Kobe
   Date:		2017-4-9
   Purpose:		LEVEL */

#include "Level1.h"
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
int Level1_Counter;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
int i=0;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void Load1(void){
	FILE *fpRead = fopen("Level1_Counter.txt", "r");
	if (fpRead == NULL)
	{
		return 0;
	}
	for (i = 0; i<10; i++)
	{
		fscanf(fpRead, "%d ", &Level1_Counter);
	}
    fprintf(fp, "Level1:Load\n");
}
void Ini1(void){
    fprintf(fp, "Level1:Ini\n");
}
void Update1(void){
	Level1_Counter--;
    fprintf(fp, "Level1:Update\n");
}
void Draw1(void){
    fprintf(fp, "Level1:Draw\n");
}
void Free1(void){
    fprintf(fp, "Level1:Free\n");
}
void Unload1(void){
    fprintf(fp, "Level1:Unload\n");
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
