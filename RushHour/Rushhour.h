#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include <mmsystem.h>
#include <time.h>
#include "resource.h"
#pragma comment(lib, "WINMM.LIB")

int i = 0, j = 0, k, l = 0;
int Page = 0;
int Is_Click = 0;
int Is_Move = 0;
int Is_Sound = 1;
int Mouse_x, Mouse_y;
int Is_Help = 0;
int Is_Dead = 0;
int velocity = 0;
int Is_Press = 0;
int Num_Jump = 2;
int Is_ReStart = 0;
int Is_Return2 = 0;
int height = 200;
int vbuilding = 4;
int varrow = 8;
int Is_Crash = 0;
int DeadMusic = 1;
int hp = 10;
int mp = 10;
int easy = 0, hard = 0, kendie = 0;
int house[100];//用于判断碰撞

/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER                1
#define WNDWIDTH             500
#define WNDHEIGHT            360
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      100
#define STAR_NUM             20

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*地形结构体，地形方块位图、屋顶位图、位置、大小、屋顶以及方块大小、方块个数*/
typedef struct
{
	HBITMAP hBlockBmp;
	HBITMAP hRoofBmp;
	POINT	pos;
	SIZE    size;
	int     roofWidth;
	int     roofHeight;
	int     blockWidth;
	int     blockHeight;
	int     blockNum;
}Terrian;

/*背景建筑结构体，建筑位图、位置、大小、类型*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;

//白云结构体
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
}Cloud;

//路灯结构体
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
}Light;

//星星结构体
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int iseaten;
}Star;

//血瓶
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int iseaten;
}Blood;

//弓箭结构体
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int touch;
	int crash;
}Arrow;

//子弹结构体
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int touch;
	int shoot;
}Bullet;

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist; //当前主人物跑了多远
}GameStatus;

/*全局变量*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("奔跑吧火柴人");

/*声明位图句柄*/
HBITMAP m_hBackgroundBmp; //一般加m表示全局变量
HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP m_hStartButton;
HBITMAP m_hBeginPicture;
HBITMAP m_hSoundButton;
HBITMAP m_hHelpButton;
HBITMAP m_hHelpPicture;
HBITMAP m_hReturnButton1;
HBITMAP m_hLosePicture;
HBITMAP m_hReStart;
HBITMAP m_hReturnButton2;
HBITMAP m_hLight;
HBITMAP m_hCloud;
HBITMAP m_hArrow;
HBITMAP m_hBlood;
HBITMAP m_heasy;
HBITMAP m_hhard;
HBITMAP m_hkendie;
HBITMAP m_hFire;
HBITMAP m_hMyfire;
HBITMAP m_hStar[STAR_NUM];
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];

//音乐句柄
//HWAVE   m_hMusic;

/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM]; // 地形数组
GameStatus    m_gameStatus;
Light         m_light; 
Cloud         m_cloud;
Arrow         m_arrow;
Arrow         m_fire;
Bullet        m_bullet;
Blood         m_blood;
Star          m_star[STAR_NUM];
/*全局函数*/
//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : 创建英雄人物
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
	curFrameIndex当前帧
	maxFrameSize最大帧率
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : 创建背景建筑
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Light CreateLight(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Cloud CreateCloud(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Arrow CreateArrow(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Star CreateStar(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Blood CreateBlood(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Bullet CreateBullet(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
/*************************************************
Fucntion : 创建游戏状态
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建英雄飞机
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBlockBmp表示方块位图句柄
	hRoofBmp表示屋顶位图句柄
	roofWidth、roofHeight屋顶宽度和高度
	blockWidth、blockHeight方块宽度和高度
*************************************************/ 
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);

//双缓冲绘制
VOID Render(HWND hWnd);
//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero更新
VOID HeroUpdate();
//路灯更新
VOID CloudUpdate();
//白云更新
VOID LightUpdate();
//星星更新
VOID StarUpdate();
//弓箭更新
VOID ArrowUpdate();
//火球更新
VOID FireballUpdate();
//子弹更新
VOID BulletUpdate();
//血瓶更新
VOID BloodUpdate();
//地形更新
VOID TerrianUpdate();
//游戏状态更新
VOID GameStatusUpdate();
//重置数据
VOID Reset();
//判断是否点击暂停
BOOL Paused(POINT);
//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//键盘松开事件处理
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);