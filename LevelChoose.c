/* Project:		GSMPre
   File Name:	LevelChoose.c
   Author:		Xinrea
   Date:		2017-6-4
   Purpose:		LEVELCHOOSE */

#include "LevelChoose.h"
#include "stdio.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "System.h"
#include "inc/AEEngine.h"
#include "Input.h"
#include "inc/fmod.h"
#pragma comment (lib, "lib/fmod_vc.lib")
#pragma comment (lib, "lib/Alpha_Engine.lib")
extern FILE* fp;
//------------------------------------------------------------------------------
// Private Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------
typedef struct postion {
	float pos_x;
	float pos_y;
};
//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static AEGfxVertexList*	pBackground;
static AEGfxTexture *MainBackground;
static AEGfxVertexList* pMenue1;
static AEGfxVertexList* pMenue2;
static AEGfxTexture* MenueTexture;
static int currentframe;
static FMOD_SYSTEM* mbgm;
static FMOD_SOUND* main;
static FMOD_CREATESOUNDEXINFO* smain;
static FMOD_CHANNELGROUP* channelgroup;
static FMOD_CHANNEL* channel;
static FMOD_RESULT result;
static void* extradriverdata;
static float volume;
static struct postion pos[5]; //开始，选关，帮助，退出，关于
static int tab_record;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------ ------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void LoadC(void){
	AEGfxMeshStart();//添加背景图片
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pBackground = AEGfxMeshEnd();

	AEGfxMeshStart();//添加菜单效果1
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMenue1 = AEGfxMeshEnd();

	AEGfxMeshStart();//添加菜单效果2
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMenue2 = AEGfxMeshEnd();
	MainBackground = AEGfxTextureLoad("res\\main.png");
	MenueTexture = AEGfxTextureLoad("res\\highlight.png");
	FMOD_System_Create(&mbgm);
	FMOD_System_Init(mbgm, 32, FMOD_INIT_NORMAL, extradriverdata);
	FMOD_System_CreateSound(mbgm, "res\\mp3\\main.mp3", FMOD_LOOP_NORMAL, smain, &main);
    fprintf(fp, "Choose:Load\n");
}
void IniC(void){
	currentframe = 0;
	channel = 0;
	volume = 0.5f;
	tab_record = 0;
	pos[0].pos_x = 136.0f;
	pos[0].pos_y = -136.0f;
	pos[1].pos_x = 0.0f;
	pos[1].pos_y = 0.0f;
	pos[2].pos_x = 272.0f;
	pos[2].pos_y = 0.0f;
	pos[3].pos_x = 272.0f;
	pos[3].pos_y = -272.0f;
	pos[4].pos_x = 0.0f;
	pos[4].pos_y = -272.0f;

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	FMOD_System_PlaySound(mbgm, main, channelgroup, FALSE, &channel);
	FMOD_Channel_SetVolume(channel, volume);

    fprintf(fp, "Choose:Ini\n");
}
void UpdateC(void)
{
	
	// 状态切换
	if (KeyPressed[KeyR]) {
		State.Next = State_Restart;
		GS = GS_Restart;
	}
		
	if (KeyPressed[KeyESC]) {
		State.Next = State_Quit;
		GS = GS_Quit;
	}

	if(KeyPressed[KeyReturn]){
		switch (tab_record)
		{
		case 0: State.Next = State_Level1; GS_L1; break;
		case 1: break;
		case 2: break;
		case 3: State.Next = State_Quit; GS = GS_Quit; break;
		case 4: break;
		default:break;
		}
	}

	if (KeyPressed[KeyTab]) {
		tab_record = (tab_record + 1) % 5;
	}
	// 输入重置
	Input_Initialize();

	// 签到
	fprintf(fp, "Choose:Update\n");
}
void DrawC(void){
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0.0f, 0.0f);
	AEGfxTextureSet(MainBackground, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pBackground, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(pos[tab_record].pos_x, pos[tab_record].pos_y);
	AEGfxTextureSet(MenueTexture, 0.0f, 0.0f);
	AEGfxSetTransparency(1.0f);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMenue1, AE_GFX_MDM_TRIANGLES);

	currentframe++;
	currentframe = currentframe % 60;

    fprintf(fp, "Choose:Draw\n");
}
void FreeC(void){
	FMOD_Sound_Release(main);
	FMOD_System_Release(mbgm);
    fprintf(fp, "Choose:Free\n");
}
void UnloadC(void){
    AEGfxMeshFree(pBackground);
	AEGfxMeshFree(pMenue1);
	AEGfxMeshFree(pMenue2);
	AEGfxTextureUnload(MainBackground);
	AEGfxTextureUnload(MenueTexture);
	FMOD_System_Close(mbgm);
    fprintf(fp, "Choose:Unload\n");
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
