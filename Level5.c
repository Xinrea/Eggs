/* Project:		GSMPre
   File Name:	Level5.c
   Author:		Kobe
   Date:		2017-5-28
   Purpose:		LEVEL */

#include "Level5.h"
#include "stdio.h"
#include "System.h"
#include "Input.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "inc/AEEngine.h"
#include "inc/fmod.h"
#pragma comment (lib, "lib/fmod_vc.lib")
extern FILE *fp;
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
static AEGfxVertexList*	pMesh_obj;
static AEGfxVertexList*	pMesh_Back;				// 对象1的网格(mesh)模型
static AEGfxVertexList*	pMesh_hdoor;            // 上方障碍物网格模型 
static AEGfxVertexList*	pMesh_ldoor;            // 下方障碍物网格模型
static AEGfxVertexList*	pMesh_obj_n[8];	    
static AEGfxVertexList* pMesh_Question;
static AEGfxVertexList* pMesh_choice;
static AEGfxVertexList* pMesh_number;
static AEGfxVertexList* pMesh_reply;			//答题后选择界面
static AEGfxVertexList* pMesh_choice_s;	
static AEGfxVertexList* pMesh_ltime;	
static AEGfxVertexList* pMesh_mask;

static AEGfxTexture *obj_Texture;
static AEGfxTexture *Back_Texture;
static AEGfxTexture *hdoor_Texture;
static AEGfxTexture *ldoor_Texture;
static AEGfxTexture *obj_n_Texture[16];          // 对象一(red)对象二(green)对象三(blue) 对象四(yellow)  
static AEGfxTexture *Question_Texture;
static AEGfxTexture *choice_Texture;
static AEGfxTexture *number_Texture[10];
static AEGfxTexture *reply_Texture[2];
static AEGfxTexture *choice_s_Texture;
static AEGfxTexture *ltime_Texture;
static AEGfxTexture *help_Texture[3];
static AEGfxTexture *about_Texture[3];

static FMOD_SYSTEM* main_system;	//背景音乐
static FMOD_SOUND* main_bgm;
static FMOD_CREATESOUNDEXINFO* main_exinfo;
static FMOD_CHANNELGROUP* channelgroup;
static FMOD_CHANNEL* channel;
static FMOD_RESULT result;
static void* extradriverdata;

static int CurrentFrame;    					// 记录当前总帧数
static int game_time;       					// 记录当前游戏时间 
static int ObjFrame;       						// 记录物体运动帧数
static int v_obj[2];       						// 物体运动速度
static int nchoose;                        		// 记录当前选择框位置
static int sum[4];                              // 记录各种颜色的数目
static int turn[18];                            // 记录每一回合的颜色
static int answer;                              // 记录正确的选项 
static int reply;                               // 记录选择的正确或错误
static int nchoose_s ;                          // 记录当前小选择框位置
static int break_time[10];
static int wait_time;
static int out_time;							// 记录提问板出来的时间
static int help_choose;                         // 在帮助界面里的选择
static int help;								// 判断是否打开帮助页面
static int about;								// 判断是否打开关于页面
static int about_choose;						// 在关于界面里的选择
static int position[11];
static int decide[18],decision;                          // 决定是否出场

static float obj[18], objY[2];					// 对象1的位置
static float obj_X, obj_Y;						// 绘制时对象的位置
static float hdoor_X,hdoor_Y;					// 左侧障碍物位置	
static float ldoor_X,ldoor_Y;
static float scale,scale_obj[4];
static float time_Frame;

static float Question_X,Question_Y;             // 提问见面位置
static float choice_X[4],choice_Y[4];           // 记录答案选择的位置
static float choice_s_X[4],choice_s_Y[4];       // 记录答题后操作选择的位置
static float choice_a_X[3],choice_a_Y[3];       // 关于界面内操作选择的位置

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void Load5(void){
	// 添加遮罩
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_mask = AEGfxMeshEnd();

	// 添加背景
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_Back = AEGfxMeshEnd();

	// 添加对象(左到右)
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.1f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.1f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.1f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_obj_n[0] = AEGfxMeshEnd();

	//添加对象（右到左）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.1f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.1f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.1f, 0.0f);
	pMesh_obj_n[1] = AEGfxMeshEnd();

	// 添加对象(左到右)
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.125f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.125f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.125f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_obj_n[2] = AEGfxMeshEnd();

	//添加对象（右到左）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.125f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.125f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.125f, 0.0f);
	pMesh_obj_n[3] = AEGfxMeshEnd();

	// 添加对象(左到右)
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.1f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.1f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.1f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_obj_n[4] = AEGfxMeshEnd();

	//添加对象（右到左）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.1f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.1f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.1f, 0.0f);
	pMesh_obj_n[5] = AEGfxMeshEnd();

	// 添加对象(左到右)
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.1111111f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.1111111f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.1111111f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_obj_n[6] = AEGfxMeshEnd();

	//添加对象（右到左）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.1111111f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.1111111f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.1111111f, 0.0f);
	pMesh_obj_n[7] = AEGfxMeshEnd();

	// 添加障碍物（上）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_hdoor = AEGfxMeshEnd();

	// 添加障碍物（下）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_ldoor = AEGfxMeshEnd();

	// 添加提问界面
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_Question = AEGfxMeshEnd();

	// 添加选择框
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_choice = AEGfxMeshEnd();

	//添加数字0-9
	AEGfxMeshStart();
	AEGfxTriAdd(
		-32.0f, -32.0f, 0xFFFF0000, 0.0f, 1.0f,
		32.0f, -32.0f, 0xFFFF0000, 1.0f, 1.0f,
		-32.0f, 32.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		32.0f, -32.0f, 0xFFFF0000, 1.0f, 1.0f,
		32.0f, 32.0f, 0xFFFF0000, 1.0f, 0.0f,
		-32.0f, 32.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_number = AEGfxMeshEnd();

	//添加答题后选择界面
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_reply = AEGfxMeshEnd();

	// 添加小提问框
	AEGfxMeshStart();
	AEGfxTriAdd(
		-800.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		800.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		800.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-800.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_choice_s = AEGfxMeshEnd();

	//添加倒计时
	AEGfxMeshStart();
	AEGfxTriAdd(
		-200.0f, -200.0f, 0xFFFF0000, 0.0f, 1.0f,
		200.0f, -200.0f, 0xFFFF0000, 0.06667f, 1.0f,
		-200.0f, 200.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		200.0f, -200.0f, 0xFFFF0000, 0.06667f, 1.0f,
		200.0f, 200.0f, 0xFFFF0000, 0.06667f, 0.0f,
		-200.0f, 200.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_ltime = AEGfxMeshEnd();

	AE_ASSERT_MESG(pMesh_Back, "Failed to create mesh 1!!");

	Back_Texture = AEGfxTextureLoad("res\\back2.png");
	obj_n_Texture[0] = AEGfxTextureLoad("res\\hamburgerRED.png");
	obj_n_Texture[1] = AEGfxTextureLoad("res\\hamburgerYELLOW.png");
	obj_n_Texture[2] = AEGfxTextureLoad("res\\hamburgerBULE.png");
	obj_n_Texture[3] = AEGfxTextureLoad("res\\hamburgerGREEN.png");
	obj_n_Texture[4] = AEGfxTextureLoad("res\\sadfrogRED.png");
	obj_n_Texture[5] = AEGfxTextureLoad("res\\sadfrogYELLOW.png");
	obj_n_Texture[6] = AEGfxTextureLoad("res\\sadfrogBULE.png");
	obj_n_Texture[7] = AEGfxTextureLoad("res\\sadfrogGREEN.png");
	obj_n_Texture[8] = AEGfxTextureLoad("res\\eggRED.png");
	obj_n_Texture[9] = AEGfxTextureLoad("res\\eggYELLOW.png");
	obj_n_Texture[10] = AEGfxTextureLoad("res\\eggBLUE.png");
	obj_n_Texture[11] = AEGfxTextureLoad("res\\eggGREEN.png");
	obj_n_Texture[12] = AEGfxTextureLoad("res\\PikachuNon.png");
	obj_n_Texture[13] = AEGfxTextureLoad("res\\PikachuNon.png");
	obj_n_Texture[14] = AEGfxTextureLoad("res\\PikachuNon.png");
	obj_n_Texture[15] = AEGfxTextureLoad("res\\PikachuNon.png");
	hdoor_Texture = AEGfxTextureLoad("res\\door2-h.png");
	ldoor_Texture = AEGfxTextureLoad("res\\door2.png");
	Question_Texture = AEGfxTextureLoad("res\\Question.png");
	choice_Texture = AEGfxTextureLoad("res\\choice.png");
	number_Texture[0] = AEGfxTextureLoad("res\\0.png");
	number_Texture[1] = AEGfxTextureLoad("res\\1.png");
	number_Texture[2] = AEGfxTextureLoad("res\\2.png");
	number_Texture[3] = AEGfxTextureLoad("res\\3.png");
	number_Texture[4] = AEGfxTextureLoad("res\\4.png");
	number_Texture[5] = AEGfxTextureLoad("res\\5.png");
	number_Texture[6] = AEGfxTextureLoad("res\\6.png");
	number_Texture[7] = AEGfxTextureLoad("res\\7.png");
	number_Texture[8] = AEGfxTextureLoad("res\\8.png");
	number_Texture[9] = AEGfxTextureLoad("res\\9.png");
	reply_Texture[0] = AEGfxTextureLoad("res\\right.png");
	reply_Texture[1] = AEGfxTextureLoad("res\\wrong.png");
	choice_s_Texture = AEGfxTextureLoad("res\\choice_s.png");
	ltime_Texture = AEGfxTextureLoad("res\\late_time.png");
	help_Texture[0] = AEGfxTextureLoad("res\\help1.png");
	help_Texture[1] = AEGfxTextureLoad("res\\help2.png");
	help_Texture[2] = AEGfxTextureLoad("res\\help3.png");
	about_Texture[0] = AEGfxTextureLoad("res\\Member1.png");
	about_Texture[1] = AEGfxTextureLoad("res\\Member2.png");
	about_Texture[2] = AEGfxTextureLoad("res\\Member3.png");

	FMOD_System_Create(&main_system);
	FMOD_System_Init(main_system, 32, FMOD_INIT_NORMAL, extradriverdata);

	fprintf(fp, "Choose:Load\n");
}


void Ini5(void){
	int num;

	CurrentFrame = 0;    				// 记录当前总帧数
	game_time = 0;       				// 记录当前游戏时间 
	ObjFrame = 0;       				// 记录物体运动帧数


	v_obj[0] = 17;
	v_obj[1] = 22;       				// 物体运动速度


	nchoose = 0;                       	// 记录当前选择框位置
	sum[0] = 0;                        	// 记录各种颜色的数目
	sum[1] = 0;
	sum[2] = 0;
	sum[3] = 0;

	answer = 0;                         // 记录正确的选项 
	reply = 0;                          // 记录选择的正确或错误
	nchoose_s = 0;                      // 记录当前小选择框位置
	channel = 0;						// 声道初始化

	help_choose = 0;                    // 在帮助界面里的选择
	help = 0;

	wait_time = 14;						// 倒计时持续时间
	time_Frame = 0;						
	out_time = 110;						// 答案界面显示时间

	about = 0;							// 关于界面

	//为出场的时间最准备
	srand((unsigned)time(NULL));
	break_time[0] = rand()%5 + wait_time + 1;
	break_time[1] = rand()%5 + break_time[0];
	break_time[2] = rand()%5 + break_time[1];
	break_time[3] = rand()%5 + break_time[2];
	break_time[4] = rand()%5 + break_time[3];
	break_time[5] = rand()%5 + break_time[4];
	break_time[6] = rand()%5 + break_time[5];
	break_time[7] = rand()%5 + break_time[6];
	break_time[8] = rand()%5 + break_time[7];
	break_time[9] = rand()%5 + break_time[8];

	//为是否出场作准备
	decide[0] = rand()%2; decide[1] = rand()%2; 
	decide[2] = rand()%2; decide[3] = rand()%2; 
	decide[4] = rand()%2; decide[5] = rand()%2;
	decide[6] = rand()%2; decide[7] = rand()%2; 
	decide[8] = rand()%2; 
	decide[9] = rand()%2;  	if (!(decide[0]||decide[9])) decide[0+(rand()%2)*9] = 1;
	decide[10] = rand()%2; 	if (!(decide[1]||decide[10])) decide[1+(rand()%2)*9] = 1;
	decide[11] = rand()%2; 	if (!(decide[2]||decide[11])) decide[2+(rand()%2)*9] = 1;
	decide[12] = rand()%2; 	if (!(decide[3]||decide[12])) decide[3+(rand()%2)*9] = 1;
	decide[13] = rand()%2; 	if (!(decide[4]||decide[13])) decide[4+(rand()%2)*9] = 1;
	decide[14] = rand()%2; 	if (!(decide[5]||decide[14])) decide[5+(rand()%2)*9] = 1;
	decide[15] = rand()%2; 	if (!(decide[6]||decide[15])) decide[6+(rand()%2)*9] = 1;
	decide[16] = rand()%2; 	if (!(decide[7]||decide[16])) decide[7+(rand()%2)*9] = 1;
	decide[17] = rand()%2;	if (!(decide[8]||decide[17])) decide[8+(rand()%2)*9] = 1;

	decision = 0;


	//初始化出场位置
	for (num=0;num<9;num=num+2)
	obj[num] = -500;
	for (num=1;num<9;num=num+2)
	obj[num] = 500;
	for (num=9;num<18;num=num+2)
	obj[num] = -700;
	for (num=10;num<18;num=num+2)
	obj[num] = 700;

	objY[1] = 0.0f;
	objY[0] = 280.0f;

	// 绘制时对象的位置
	obj_X = 0;
	obj_Y = 0;

	//比例
	scale_obj[0] = 0.1f;
	scale_obj[1] = 0.125f;
	scale_obj[2] = 0.1f;
	scale_obj[3] = 0.1111111f;

	// 对象左侧障碍物的初始位置
	hdoor_X = 0.0f;
	hdoor_Y = 0.0f;

	// 对象右侧障碍物的初始位置
	ldoor_X = 0.0f;
	ldoor_Y = 0.0f;

	// 提问界面的位置
	Question_X = 0.0f;
	Question_Y = 0.0f;

	// 选择框位置
	choice_X[0] = 0.0f;
	choice_Y[0] = 0.0f;
	choice_X[1] = 340.0f;
	choice_Y[1] = 0.0f;
	choice_X[2] = 0.0f;
	choice_Y[2] = -300.0f;
	choice_X[3] = 340.0f;
	choice_Y[3] = -300.0f;

	// 小选择框位置
	choice_s_X[0] = 0.0f;
	choice_s_Y[0] = 0.0f;
	choice_s_X[1] = 180.0f;
	choice_s_Y[1] = 0.0f;
	choice_s_X[2] = 0.0f;
	choice_s_Y[2] = -155.0f;
	choice_s_X[3] = 180.0f;
	choice_s_Y[3] = -155.0f;

	// 关于界面里的选择框位置
	choice_a_X[0] = -106.0f;
	choice_a_Y[0] = -206.0f;
	choice_a_X[1] = 90.0f;
	choice_a_Y[1] = -206.0f;
	choice_a_X[2] = 290.0f;
	choice_a_Y[2] = -206.0f;


	// 为出场的颜色作准备

	turn[0] = rand()%16;  if ((turn[0] <= 11)&&decide[0]) sum[(turn[0]%4)]++;
	turn[1] = rand()%16;  if ((turn[1] <= 11)&&decide[1])  sum[(turn[1]%4)]++;
	turn[2] = rand()%16;  if ((turn[2] <= 11)&&decide[2])  sum[(turn[2]%4)]++;
	turn[3] = rand()%16;  if ((turn[3] <= 11)&&decide[3])  sum[(turn[3]%4)]++;
	turn[4] = rand()%16;  if ((turn[4] <= 11)&&decide[4])  sum[(turn[4]%4)]++;
	turn[5] = rand()%16;  if ((turn[5] <= 11)&&decide[5])  sum[(turn[5]%4)]++;
 	turn[6] = rand()%16;  if ((turn[6] <= 11)&&decide[6])  sum[(turn[6]%4)]++;
 	turn[7] = rand()%16;  if ((turn[7] <= 11)&&decide[7])  sum[(turn[7]%4)]++;
	turn[8] = rand()%16;  if ((turn[8] <= 11)&&decide[8])  sum[(turn[8]%4)]++;
	turn[9] = rand()%16;  if ((turn[9] <= 11)&&decide[9])  sum[(turn[9]%4)]++;
	turn[10] = rand()%16;  if ((turn[10] <= 11)&&decide[10])  sum[(turn[10]%4)]++;
	turn[11] = rand()%16;  if ((turn[11] <= 11)&&decide[11]) sum[(turn[11]%4)]++;
	turn[12] = rand()%16;  if ((turn[12] <= 11)&&decide[12]) sum[(turn[12]%4)]++;
	turn[13] = rand()%16;  if ((turn[13] <= 11)&&decide[13]) sum[(turn[13]%4)]++;
	turn[14] = rand()%16;  if ((turn[14] <= 11)&&decide[14]) sum[(turn[14]%4)]++;
	turn[15] = rand()%16;  if ((turn[15] <= 11)&&decide[15]) sum[(turn[15]%4)]++;
	turn[16] = rand()%16;  if ((turn[16] <= 11)&&decide[16]) sum[(turn[16]%4)]++;
 	turn[17] = rand()%16;  if ((turn[17] <= 11)&&decide[17]) sum[(turn[17]%4)]++;

	// 为提问选项做准备
	answer = rand()%4;
	sum[answer]--;

	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);   // 背景色RGB
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	FMOD_System_CreateSound(main_system, "res\\mp3\\level2.mp3", FMOD_LOOP_NORMAL, main_exinfo, &main_bgm);
	FMOD_System_PlaySound(main_system, main_bgm, channelgroup, FALSE, &channel);	// 播放音乐
	FMOD_Channel_SetVolume(channel, 0.5f);

    fprintf(fp, "Level1:Ini\n");
}
void Update5(void)
{
	// 状态切换

	if (KeyPressed[KeyESC]) {
		State.Next = State_Quit;
		GS = GS_Quit;
	}

	//提问框选择切换
	if ((game_time > out_time)&&(!help)&&(!about)) {
		if ((KeyPressed[KeyUp])||(KeyPressed[KeyDown]))
		{
			nchoose = (nchoose+2)%4;
		}
		if ((KeyPressed[KeyLeft])||(KeyPressed[KeyRight]))
		{
			switch (nchoose){
				case 0: nchoose = 1; break;
				case 1: nchoose = 0; break;
				case 2: nchoose = 3; break;
				case 3: nchoose = 2; break;
			}
		}
	}
	//小选择框选择切换
	if (reply&&(!help)&&(!about)){
		if ((KeyPressed[KeyUp])||(KeyPressed[KeyDown]))
		{
			nchoose_s = (nchoose_s+2)%4;
		}
		if ((KeyPressed[KeyLeft])||(KeyPressed[KeyRight]))
		{
			switch (nchoose_s){
				case 0: nchoose_s = 1; break;
				case 1: nchoose_s = 0; break;
				case 2: nchoose_s = 3; break;
				case 3: nchoose_s = 2; break;
			}
		}
	}

	//帮助页面里小选择框选择切换
	if (help&&(!about)){
		if ((KeyPressed[KeyUp])||(KeyPressed[KeyDown]))
		{
			help_choose = (help_choose+2)%4;
		}
		if ((KeyPressed[KeyLeft])||(KeyPressed[KeyRight]))
		{
			switch (help_choose){
				case 0: help_choose = 1; break;
				case 1: help_choose = 0; break;
				case 2: help_choose = 3; break;
				case 3: help_choose = 2; break;
			}
		}
	}

		//答题正确后选择
	if ((reply == 1)&&KeyPressed[KeyReturn]&&(!help)&&(!about)){
		switch (nchoose_s){
		case 0: {
			State.Next = State_Restart;
			FMOD_Sound_Release(main_bgm);
			GS = GS_Restart;
			break;
		}		//返回
		case 1: {

			break;
		}		//下一关（无效）
		case 2: {  
			State.Next = State_LevelC;
			GS = GS_LC;
			break;
		}		//主界面
				case 3: {
			help = 1;
			KeyPressed[KeyReturn] = 0;
			break;
		} 		//帮助
		}
	}

	//答题错误后选择
	if ((reply == 2)&&KeyPressed[KeyReturn]&&(!help)&&(!about)){
		switch (nchoose_s) {
			case 0: {
				State.Next = State_Restart;
				FMOD_Sound_Release(main_bgm);
				GS = GS_Restart;
				break;
			}		//返回
			case 1: {
				break;
			}		//下一关
			case 2: {
				State.Next = State_LevelC;
				GS = GS_LC;
				break;
			}		//主界面
			case 3:  {
				help = 1;
				KeyPressed[KeyReturn] = 0;
				break;		//帮助
			}
		}
	}

	//处于帮助页面的选择
	if (help&&KeyPressed[KeyReturn]&&(!about)){
		if (help == 1){				//处于第一张帮助界面
			switch (help_choose){
				case 0: {
			
				break;
				}		//上一页（无效）
				case 1: {
					help = help + 1;
				break;
				}		//下一页
				case 2: {
					help = 0;
				break;
				}		//返回
				case 3:  {
					about = 1;
					KeyPressed[KeyReturn] = 0;
				break;	//成员
				}	
			}
		}
		else if (help == 2){
			switch (help_choose){
				case 0: {
					help  = help - 1;
				break;
				}		//上一页
				case 1: {
					help = help + 1;
				break;
				}		//下一页
				case 2: {
					help = 0;
				break;
				}		//返回
				case 3:  {
					about = 1;
					KeyPressed[KeyReturn] = 0;
				break;	//成员
				}	
			}
		}
		else {
			switch (help_choose){
				case 0: {
					help  = help - 1;
				break;
				}		//上一页
				case 1: {

				break;
				}		//下一页（无效）
				case 2: {
					help = 0;
				break;
				}		//返回
				case 3:  {
					about = 1;
					KeyPressed[KeyReturn] = 0;
				break;	//成员
				}	
			}
		}
	}

	//在关于页面里的选项切换
	if (about){
		if (KeyPressed[KeyLeft])
			about_choose = (about_choose + 2) % 3;
		if (KeyPressed[KeyRight])
			about_choose = (about_choose +1 ) % 3;
	}

	//在关于页面里的选择
	if (about && KeyPressed[KeyReturn])
	{
		if (about == 1){				//关于第一页
			switch (about_choose) {
				case 0:{
					break;
				}	//上一页（无效）
				case 1:{
					about = 0;
					break;
				}	//返回
				case 2:{
					about = about + 1;
					break;
				}	//下一页
			}
		}
		else if (about == 2){
			switch (about_choose) {
				case 0:{
					about = about - 1;
					break;
				}	//上一页
				case 1:{
					about = 0;
					break;
				}	//返回
				case 2:{
					about = about + 1;
					break;
				}	//下一页
			}
		}
		else {
			switch (about_choose) {
				case 0:{
					about = about - 1;
					break;
				}	//上一页
				case 1:{
					about = 0;
					break;
				}	//返回
				case 2:{
					break;
				}	//下一页(无效)
			}	
		}
	}

	//答题反馈
	if ((about == 0)&&(reply == 0) && (game_time > out_time) && KeyPressed[KeyReturn] && (nchoose == answer)&&(!help)){
		reply = 1;
	}
	else if ((about == 0)&&(reply == 0) && (game_time > out_time)&& KeyPressed[KeyReturn] &&(nchoose != answer)&&(!help)){
		reply = 2;
	}
	

	// 输入重置
	Input_Initialize();

	// 签到
	fprintf(fp, "Level1:Update\n");
}
void Draw5(void){

	if (CurrentFrame >= 10)
	{
		game_time++;
		CurrentFrame = 0;
		time_Frame++;
	}
	CurrentFrame++;

	if (CurrentFrame % 4 == 0) {
			ObjFrame++;
			ObjFrame = ObjFrame % 14;	
		}
    // 画背景
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0, 0);
	AEGfxTextureSet(Back_Texture, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh_Back, AE_GFX_MDM_TRIANGLES);

	if ((game_time >= wait_time)&&(game_time <= 6 + wait_time)) 
	{
		decision = decide[0];
		obj_Texture = obj_n_Texture[turn[0]];
		scale = scale_obj[turn[0]/4];
		pMesh_obj = pMesh_obj_n[(turn[0]/4)*2];
		obj[0] = obj[0] + v_obj[0];
		obj_X = obj[0];
		obj_Y = objY[0];
	}
	else if ((game_time >= 7 + break_time[0])&& (game_time <= 13 + break_time[0]))
	{
		decision = decide[1];
		obj_Texture = obj_n_Texture[turn[1]];
		scale = scale_obj[turn[1]/4];
		pMesh_obj = pMesh_obj_n[(turn[1]/4)*2+1];
		obj[1] = obj[1] - v_obj[0];
		obj_X = obj[1];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 14 + break_time[1])&& (game_time <= 20 + break_time[1]))
	{
		decision = decide[2];
		obj_Texture = obj_n_Texture[turn[2]];
		scale = scale_obj[turn[2]/4];
		pMesh_obj = pMesh_obj_n[(turn[2]/4)*2];
		obj[2] = obj[2] + v_obj[0];
		obj_X = obj[2];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 21 + break_time[2])&& (game_time <= 27 + break_time[2]))
	{
		decision = decide[3];
		obj_Texture = obj_n_Texture[turn[3]];
		scale = scale_obj[turn[3]/4];
		pMesh_obj = pMesh_obj_n[(turn[3]/4)*2+1];
		obj[3] = obj[3] - v_obj[0];
		obj_X = obj[3];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 28 + break_time[3])&& (game_time <= 34 + break_time[3]))
	{
		decision = decide[4];
		obj_Texture = obj_n_Texture[turn[4]];
		scale = scale_obj[turn[4]/4];
		pMesh_obj = pMesh_obj_n[(turn[4]/4)*2];
		obj[4] = obj[4] + v_obj[0];
		obj_X = obj[4];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 35 + break_time[4])&& (game_time <= 41 + break_time[4]))
  	{
  		decision = decide[5];
  		obj_Texture = obj_n_Texture[turn[5]];
  		scale = scale_obj[turn[5]/4];
		pMesh_obj = pMesh_obj_n[(turn[5]/4)*2+1];
		obj[5] = obj[5] - v_obj[0];
		obj_X = obj[5];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 42 + break_time[5])&& (game_time <= 48 + break_time[5]))
  	{
  		decision = decide[6];
		obj_Texture = obj_n_Texture[turn[6]];
		scale = scale_obj[turn[6]/4];
		pMesh_obj = pMesh_obj_n[(turn[6]/4)*2];
		obj[6] = obj[6] + v_obj[0];
		obj_X = obj[6];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 49 + break_time[6])&& (game_time <= 55 + break_time[6]))
  	{
  		decision = decide[7];
		obj_Texture = obj_n_Texture[turn[7]];
		scale = scale_obj[turn[7]/4];
		pMesh_obj = pMesh_obj_n[(turn[7]/4)*2+1];
		obj[7] = obj[7] - v_obj[0];
		obj_X = obj[7];
		obj_Y = objY[0];
  	}
  	else if ((game_time >= 56 + break_time[7])&& (game_time <= 62 + break_time[7]))
  	{
  		decision = decide[8];
		obj_Texture = obj_n_Texture[turn[8]];
		scale = scale_obj[turn[8]/4];
		pMesh_obj = pMesh_obj_n[(turn[8]/4)*2];
		obj[8] = obj[8] + v_obj[0];
		obj_X = obj[8];
		obj_Y = objY[0];
  	}

	// 画上方对象
  	if ((game_time >= wait_time)&&decision)
  	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(obj_X, obj_Y);
		AEGfxTextureSet(obj_Texture, scale*ObjFrame, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_obj, AE_GFX_MDM_TRIANGLES);
  	}

  	if ((game_time >= wait_time)&&(game_time <= 6 + wait_time)) 
	{
		decision = decide[9];
		obj_Texture = obj_n_Texture[turn[9]];
		scale = scale_obj[turn[9]/4];
		pMesh_obj = pMesh_obj_n[(turn[9]/4)*2];
		obj[9] = obj[9] + v_obj[1];
		obj_X = obj[9];
		obj_Y = objY[1];
	}
	else if ((game_time >= 7 + break_time[0])&& (game_time <= 13 + break_time[0]))
	{
		decision = decide[10];
		obj_Texture = obj_n_Texture[turn[10]];
		scale = scale_obj[turn[10]/4];
		pMesh_obj = pMesh_obj_n[(turn[10]/4)*2+1];
		obj[10] = obj[10] - v_obj[1];
		obj_X = obj[10];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 14 + break_time[1])&& (game_time <= 20 + break_time[1]))
	{
		decision = decide[11];
		obj_Texture = obj_n_Texture[turn[11]];
		scale = scale_obj[turn[11]/4];
		pMesh_obj = pMesh_obj_n[(turn[11]/4)*2];
		obj[11] = obj[11] + v_obj[1];
		obj_X = obj[11];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 21 + break_time[2])&& (game_time <= 27 + break_time[2]))
	{
		decision = decide[12];
		obj_Texture = obj_n_Texture[turn[12]];
		scale = scale_obj[turn[12]/4];
		pMesh_obj = pMesh_obj_n[(turn[12]/4)*2+1];
		obj[12] = obj[12] - v_obj[1];
		obj_X = obj[12];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 28 + break_time[3])&& (game_time <= 34 + break_time[3]))
	{
		decision = decide[13];
		obj_Texture = obj_n_Texture[turn[13]];
		scale = scale_obj[turn[13]/4];
		pMesh_obj = pMesh_obj_n[(turn[13]/4)*2];
		obj[13] = obj[13] + v_obj[1];
		obj_X = obj[13];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 35 + break_time[4])&& (game_time <= 41 + break_time[4]))
  	{
  		decision = decide[14];
  		obj_Texture = obj_n_Texture[turn[14]];
  		scale = scale_obj[turn[14]/4];
		pMesh_obj = pMesh_obj_n[(turn[14]/4)*2+1];
		obj[14] = obj[14] - v_obj[1];
		obj_X = obj[14];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 42 + break_time[5])&& (game_time <= 48 + break_time[5]))
  	{
  		decision = decide[15];
		obj_Texture = obj_n_Texture[turn[15]];
		scale = scale_obj[turn[15]/4];
		pMesh_obj = pMesh_obj_n[(turn[15]/4)*2];
		obj[15] = obj[15] + v_obj[1];
		obj_X = obj[15];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 49 + break_time[6])&& (game_time <= 55 + break_time[6]))
  	{
  		decision = decide[16];
		obj_Texture = obj_n_Texture[turn[16]];
		scale = scale_obj[turn[16]/4];
		pMesh_obj = pMesh_obj_n[(turn[16]/4)*2+1];
		obj[16] = obj[16] - v_obj[1];
		obj_X = obj[16];
		obj_Y = objY[1];
  	}
  	else if ((game_time >= 56 + break_time[7])&& (game_time <= 62 + break_time[7]))
  	{
  		decision = decide[17];
		obj_Texture = obj_n_Texture[turn[17]];
		scale = scale_obj[turn[17]/4];
		pMesh_obj = pMesh_obj_n[(turn[17]/4)*2];
		obj[17] = obj[17] + v_obj[1];
		obj_X = obj[17];
		obj_Y = objY[1];
  	}

	// 画下方对象
  	if ((game_time >= wait_time)&&decision)
  	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(obj_X, obj_Y);
		AEGfxTextureSet(obj_Texture, scale*ObjFrame, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_obj, AE_GFX_MDM_TRIANGLES);
  	}
  	
	
	
	// 画上方障碍物
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(hdoor_X, hdoor_Y);
	AEGfxTextureSet(hdoor_Texture, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh_hdoor, AE_GFX_MDM_TRIANGLES);

    // 画下方障碍物
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(ldoor_X, ldoor_Y);
	AEGfxTextureSet(ldoor_Texture, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh_ldoor, AE_GFX_MDM_TRIANGLES);

	if (game_time < wait_time)
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxSetTransparency(0.2f);
		AEGfxMeshDraw(pMesh_mask, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(ltime_Texture, 0.06667f*time_Frame, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_ltime, AE_GFX_MDM_TRIANGLES);
	}

	// 画提问界面和选择框
	if ((game_time >= out_time)&&(reply == 0)) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(Question_X, Question_Y);
		AEGfxTextureSet(Question_Texture, 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_Question, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(choice_X[nchoose], choice_Y[nchoose]);
		AEGfxTextureSet(choice_Texture, 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_choice, AE_GFX_MDM_TRIANGLES);
	}


	// 画选项数字
	if ((game_time >= out_time)&&(reply == 0)){
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-215.0f, 40.0f);
		AEGfxTextureSet(number_Texture[sum[0]+1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_number, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(140.0f, 40.0f);
		AEGfxTextureSet(number_Texture[sum[1]+1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_number, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(-215.0f, -260.0f);
		AEGfxTextureSet(number_Texture[sum[2]+1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_number, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(140.0f, -258.0f);
		AEGfxTextureSet(number_Texture[sum[3]+1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_number, AE_GFX_MDM_TRIANGLES);
	}

	//画答题后操作界面
	if (reply != 0){
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(reply_Texture[reply -1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_reply, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(choice_s_X[nchoose_s], choice_s_Y[nchoose_s]);
		AEGfxTextureSet(choice_s_Texture, 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_choice_s, AE_GFX_MDM_TRIANGLES);
	}

	if (help){
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(help_Texture[help-1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_reply, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(choice_s_X[help_choose], choice_s_Y[help_choose]);
		AEGfxTextureSet(choice_s_Texture, 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_choice_s, AE_GFX_MDM_TRIANGLES);	
	}

	if (about){
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(0.0f, 0.0f);
		AEGfxTextureSet(about_Texture[about-1], 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_reply, AE_GFX_MDM_TRIANGLES);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(choice_a_X[about_choose], choice_a_Y[about_choose]);
		AEGfxTextureSet(choice_s_Texture, 0.0f, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_choice_s, AE_GFX_MDM_TRIANGLES);	
	}


}
void Free5(void){
    fprintf(fp, "Level1:Free\n");
}
void Unload5(void){
    AEGfxMeshFree(pMesh_Back);          // 注销创建的对象
    AEGfxMeshFree(pMesh_obj);
    AEGfxMeshFree(pMesh_hdoor);
    AEGfxMeshFree(pMesh_ldoor);
    AEGfxMeshFree(pMesh_choice);
    AEGfxMeshFree(pMesh_number);
    AEGfxMeshFree(pMesh_reply);
    AEGfxMeshFree(pMesh_Question);
    AEGfxMeshFree(pMesh_ltime);
	AEGfxMeshFree(pMesh_mask);
//  AEGfxMeshFree(pMesh_obj_n[0]);
//	AEGfxMeshFree(pMesh_obj_n[1]);
//	AEGfxMeshFree(pMesh_obj_n[2]);
//	AEGfxMeshFree(pMesh_obj_n[3]);
//	AEGfxMeshFree(pMesh_obj_n[4]);
//	AEGfxMeshFree(pMesh_obj_n[5]);
	FMOD_Sound_Release(main_bgm);
	FMOD_System_Release(main_system);
	FMOD_System_Close(main_system);
    fprintf(fp, "Level1:Unload\n");
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
