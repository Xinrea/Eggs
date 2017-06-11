/* Project:		GSMPre
   File Name:	Level1.c
   Author:		Kobe
   Date:		2017-5-28
   Purpose:		LEVEL */

#include "Level1.h"
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
static AEGfxVertexList*	pMesh_ldoor;            // 左侧障碍物网格模型 
static AEGfxVertexList*	pMesh_rdoor;            // 右侧障碍物网格模型
static AEGfxVertexList*	pMesh_obj_n[2];		    
static AEGfxVertexList* pMesh_Question;
static AEGfxVertexList* pMesh_choice;
static AEGfxVertexList* pMesh_number;
static AEGfxVertexList* pMesh_reply;			//答题后选择界面
static AEGfxVertexList* pMesh_choice_s;	
static AEGfxVertexList* pMesh_ltime;
static AEGfxVertexList* pMesh_mask;

static AEGfxTexture *obj_Texture;
static AEGfxTexture *Back_Texture;
static AEGfxTexture *ldoor_Texture;
static AEGfxTexture *rdoor_Texture;
static AEGfxTexture *obj_n_Texture[4];          // 对象一(red)对象二(green)对象三(blue) 对象四(yellow)  
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
static int v_obj1;       						// 物体运动速度
static int nchoose;                        	 	// 记录当前选择框位置
static int sum[4];                      	  	// 记录各种颜色的数目
static int turn[6];                      		// 记录每一回合的颜色
static int answer;                         		// 记录正确的选项 
static int reply;                           	// 记录选择的正确或错误
static int nchoose_s;                       	// 记录当前小选择框位置
static int wait_time;
static int out_time;
static int time_Frame; 
static int help_choose;                         // 在帮助界面里的选择
static int help;								// 判断是否打开帮助页面
static int about;								// 判断是否打开关于页面
static int about_choose;						// 在关于界面里的选择

static float objX, objY;						// 对象1的位置
static float ldoor_X,ldoor_Y;					// 左侧障碍物位置	
static float rdoor_X,rdoor_Y;

static float Question_X,Question_Y;             // 提问见面位置
static float choice_X[4],choice_Y[4];           // 记录答案选择的位置
static float choice_s_X[4],choice_s_Y[4];       // 记录答题后操作选择的位置
static float choice_a_X[4],choice_a_Y[4];       // 关于界面内操作选择的位置

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------
void Load1(void){
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
		64.0f, -128.0f, 0xFFFF0000, 0.0714285f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.0714285f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.0714285f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_obj_n[0] = AEGfxMeshEnd();

	//添加对象（右到左）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-64.0f, -128.0f, 0xFFFF0000, 0.0714285f, 1.0f,
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0714285f, 0.0f);
	AEGfxTriAdd(
		64.0f, -128.0f, 0xFFFF0000, 0.0f, 1.0f,
		64.0f, 128.0f, 0xFFFF0000, 0.0f, 0.0f,
		-64.0f, 128.0f, 0xFFFF0000, 0.0714285f, 0.0f);
	pMesh_obj_n[1] = AEGfxMeshEnd();

	// 添加障碍物（左）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-200.0f, -450.0f, 0xFFFF0000, 0.0f, 1.0f,
		200.0f, -450.0f, 0xFFFF0000, 0.25f, 1.0f,
		-200.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	AEGfxTriAdd(
		200.0f, -450.0f, 0xFFFF0000, 0.25f, 1.0f,
		200.0f, 450.0f, 0xFFFF0000, 0.25f, 0.0f,
		-200.0f, 450.0f, 0xFFFF0000, 0.0f, 0.0f);
	pMesh_ldoor = AEGfxMeshEnd();

	// 添加障碍物（右）
	AEGfxMeshStart();
	AEGfxTriAdd(
		-200.0f, -450.0f, 0xFFFF0000, 0.75f, 1.0f,
		200.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		-200.0f, 450.0f, 0xFFFF0000, 0.75f, 0.0f);
	AEGfxTriAdd(
		200.0f, -450.0f, 0xFFFF0000, 1.0f, 1.0f,
		200.0f, 450.0f, 0xFFFF0000, 1.0f, 0.0f,
		-200.0f, 450.0f, 0xFFFF0000, 0.75f, 0.0f);
	pMesh_rdoor = AEGfxMeshEnd();

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

	Back_Texture = AEGfxTextureLoad("res\\back1.png");
	obj_n_Texture[0] = AEGfxTextureLoad("res\\RED.png");
	obj_n_Texture[1] = AEGfxTextureLoad("res\\YELLOW.png");
	obj_n_Texture[2] = AEGfxTextureLoad("res\\BULE.png");
	obj_n_Texture[3] = AEGfxTextureLoad("res\\GREEN.png");
	ldoor_Texture = AEGfxTextureLoad("res\\door.png");
	rdoor_Texture = AEGfxTextureLoad("res\\door.png");
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


void Ini1(void){

	CurrentFrame = 0;    				// 记录当前总帧数
	game_time = 0;       				// 记录当前游戏时间 
	ObjFrame = 0;       				// 记录物体运动帧数
	v_obj1 = 19;       					// 物体运动速度
	nchoose = 0;                        // 记录当前选择框位置
	sum[0] = 0;                         // 记录各种颜色的数目
	sum[1] = 0; 
	sum[2] = 0;
	sum[3] = 0;
	turn[0] = 0;                        // 记录每一回合的颜色
	turn[1] = 0;
	turn[2] = 0;
	turn[3] = 0;
	turn[4] = 0;
	turn[5] = 0;
	answer = 0;                         // 记录正确的选项 
	reply = 0;                          // 记录选择的正确或错误
	nchoose_s = 0;                      // 记录当前小选择框位置
	channel = 0;						// 声道初始化

	help_choose = 0;                    // 在帮助界面里的选择
	help = 0;

	wait_time = 7;						// 倒计时持续时间
	time_Frame = 0;						
	out_time = 36;						// 答案界面显示时间

	about = 0;							// 关于界面				

	// 对象1的初始位置
	objX = -680.0f;
	objY = 0.0f;

	// 对象左侧障碍物的初始位置
	ldoor_X = -600.0f;
	ldoor_Y = 0.0f;

	// 对象右侧障碍物的初始位置
	rdoor_X = 600.0f;
	rdoor_Y = 0.0f;

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
	srand((unsigned)time(NULL));
	turn[0] = rand()%4;  sum[turn[0]]++;
	turn[1] = rand()%4;  sum[turn[1]]++;
	turn[2] = rand()%4;  sum[turn[2]]++;
	turn[3] = rand()%4;  sum[turn[3]]++;
	turn[4] = rand()%4;  sum[turn[4]]++;
	turn[5] = rand()%4;  sum[turn[5]]++;

	// 为提问选项做准备
	answer = rand()%4;
	sum[answer]--;

	// 为开始画对象做准备
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);   // 背景色RGB
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	FMOD_System_CreateSound(main_system, "res\\mp3\\level1.mp3", FMOD_LOOP_NORMAL, main_exinfo, &main_bgm);
	FMOD_System_PlaySound(main_system, main_bgm, channelgroup, FALSE, &channel);	// 播放音乐
	FMOD_Channel_SetVolume(channel, 0.5f);
    fprintf(fp, "Level1:Ini\n");
}
void Update1(void)
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

	//小选择框选择切换
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
			State.Next = State_Level2;
			GS = GS_L2;
			break;
		}		//下一关
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
void Draw1(void){

	if (CurrentFrame >= 20)
	{
		game_time++;
		CurrentFrame = 0;
		CurrentFrame++;
	}
	CurrentFrame++;

	if (CurrentFrame % 3 == 0) {
			ObjFrame++;
			ObjFrame = ObjFrame % 14;	
		}
	if (CurrentFrame % 10 == 0) {
			time_Frame++;
		}
    // 画背景
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(0, 0);
	AEGfxTextureSet(Back_Texture, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh_Back, AE_GFX_MDM_TRIANGLES);

	if ((game_time >= wait_time) && (game_time <= 3 + wait_time))
	{
		obj_Texture = obj_n_Texture[turn[0]];
		pMesh_obj = pMesh_obj_n[0];
		objX = objX + v_obj1;
	}
	else if ((game_time >= 5 + wait_time)&& (game_time <= 8 + wait_time))
	{
		obj_Texture = obj_n_Texture[turn[1]];
		pMesh_obj = pMesh_obj_n[1];
		objX = objX - v_obj1;
  	}
  	else if ((game_time >= 10 + wait_time)&& (game_time <= 13 + wait_time))
	{
		obj_Texture = obj_n_Texture[turn[2]];
		pMesh_obj = pMesh_obj_n[0];
		objX = objX + v_obj1;
  	}
  	else if ((game_time >= 15 + wait_time)&& (game_time <= 18 + wait_time))
	{
		obj_Texture = obj_n_Texture[turn[3]];
		pMesh_obj = pMesh_obj_n[1];
		objX = objX - v_obj1;
  	}
  	else if ((game_time >= 20 + wait_time)&& (game_time <= 23 + wait_time))
	{
		obj_Texture = obj_n_Texture[turn[4]];
		pMesh_obj = pMesh_obj_n[0];
		objX = objX + v_obj1;
  	}
  	else if ((game_time >= 25 + wait_time)&& (game_time <= 28 + wait_time))
  	{
  		obj_Texture = obj_n_Texture[turn[5]];
		pMesh_obj = pMesh_obj_n[1];
		objX = objX - v_obj1;
  	}

	// 画对象1-5
	if (game_time >= wait_time)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetPosition(objX, objY);
		AEGfxTextureSet(obj_Texture, 0.0714285f*ObjFrame, 0.0f);
		AEGfxSetTransparency(1);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
		AEGfxMeshDraw(pMesh_obj, AE_GFX_MDM_TRIANGLES);
	}
	
	
	// 画左侧障碍物
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(ldoor_X, ldoor_Y);
	AEGfxTextureSet(ldoor_Texture, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh_ldoor, AE_GFX_MDM_TRIANGLES);

    // 画右侧障碍物
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetPosition(rdoor_X, rdoor_Y);
	AEGfxTextureSet(rdoor_Texture, 0.0f, 0.0f);
	AEGfxSetTransparency(1);
	AEGfxSetBlendColor(0.0f, 0.0f, 0.0, 0.0f);
	AEGfxMeshDraw(pMesh_rdoor, AE_GFX_MDM_TRIANGLES);

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
void Free1(void){
    fprintf(fp, "Level1:Free\n");
}
void Unload1(void){
    AEGfxMeshFree(pMesh_Back);          // 注销创建的对象
    AEGfxMeshFree(pMesh_obj);
    AEGfxMeshFree(pMesh_ldoor);
    AEGfxMeshFree(pMesh_rdoor);
    AEGfxMeshFree(pMesh_choice);
    AEGfxMeshFree(pMesh_number);
    AEGfxMeshFree(pMesh_reply);
    AEGfxMeshFree(pMesh_Question);
	AEGfxMeshFree(pMesh_mask);
	FMOD_Sound_Release(main_bgm);
	FMOD_System_Release(main_system);
	FMOD_System_Close(main_system);
    fprintf(fp, "Level1:Unload\n");
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------
