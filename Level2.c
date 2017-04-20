/* Project:		GSMPre
   File Name:	Level2.c
   Author:		Kobe
   Date:		2017-4-9
   Purpose:		LEVEL */

#include "Level2.h"
#include "stdio.h"
#include "System.h"
#include "GameStateList.h"
#include "lib/AEEngine.h"
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
extern int Lives;
//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList*	pMesh1;				// 对象1的网格(mesh)模型
static float obj1X, obj1Y;		// 对象1的位置
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
		return;
	}
	fscanf(fpRead1, "%d ", &Lives);
	// 开始添加对象1
	AEGfxMeshStart();
	AEGfxTriAdd(
		-25.5f, -25.5f, 0xFFFF0000, 0.0f, 0.0f,
		25.5f,  0.0f, 0x00FF0000, 0.0f, 0.0f,
		-25.5f,  25.5f, 0xFFFFFF00, 0.0f, 0.0f);
	pMesh1 = AEGfxMeshEnd();
	AE_ASSERT_MESG(pMesh1, "Failed to create mesh 1!!");

    fprintf(fp, "Level2:Load\n");
}
void Ini2(void){
	// 对象1的初始位置
	obj1X = 0.0f;
	obj1Y = 0.0f;

	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);   // 背景色RGB
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

    fprintf(fp, "Level2:Ini\n");
}
void Update2(void){
    fprintf(fp, "Level2:Update\n");
}
void Draw2(void){
    // 画对象1
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetPosition(obj1X, obj1Y);
	AEGfxTextureSet(NULL, 0, 0);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh1, AE_GFX_MDM_TRIANGLES);

    fprintf(fp, "Level2:Draw\n");
}
void Free2(void){
    fprintf(fp, "Level2:Free\n");
}
void Unload2(void){
    AEGfxMeshFree(pMesh1);          // 注销创建的对象
    fprintf(fp, "Level2:Unload\n");
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
