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
int house[100];//�����ж���ײ

/*����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�*/
#define TIMER                1
#define WNDWIDTH             500
#define WNDHEIGHT            360
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      100
#define STAR_NUM             20

/*Ӣ�۽ṹ�壬Ӣ��λͼ��λ�á���С*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

/*���νṹ�壬���η���λͼ���ݶ�λͼ��λ�á���С���ݶ��Լ������С���������*/
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

/*���������ṹ�壬����λͼ��λ�á���С������*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;

//���ƽṹ��
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
}Cloud;

//·�ƽṹ��
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
}Light;

//���ǽṹ��
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int iseaten;
}Star;

//Ѫƿ
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int iseaten;
}Blood;

//�����ṹ��
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int touch;
	int crash;
}Arrow;

//�ӵ��ṹ��
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	int touch;
	int shoot;
}Bullet;

/*��Ϸ״̬�ṹ��*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist; //��ǰ���������˶�Զ
}GameStatus;

/*ȫ�ֱ���*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("���ܰɻ����");

/*����λͼ���*/
HBITMAP m_hBackgroundBmp; //һ���m��ʾȫ�ֱ���
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

//���־��
//HWAVE   m_hMusic;

/*���巽����ɫ���飬��m_hBlockBmp[BLOCK_COLOR_NUM]������Ӧ��0��ʾ��ɫ���飬1��ʾ��ɫ���飬2��ʾ��ɫ���飬3��ʾ��ɫ����*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*�����ݶ���ɫ���飬��m_hRoofkBmp[ROOF_COLOR_NUM]������Ӧ��0��ʾ��ɫ�ݶ���1��ʾ��ɫ�ݶ�*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM]; // ��������
GameStatus    m_gameStatus;
Light         m_light; 
Cloud         m_cloud;
Arrow         m_arrow;
Arrow         m_fire;
Bullet        m_bullet;
Blood         m_blood;
Star          m_star[STAR_NUM];
/*ȫ�ֺ���*/
//������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//��ʼ��
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : ����Ӣ������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
	curFrameIndex��ǰ֡
	maxFrameSize���֡��
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : ������������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Light CreateLight(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Cloud CreateCloud(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Arrow CreateArrow(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Star CreateStar(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Blood CreateBlood(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Bullet CreateBullet(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
/*************************************************
Fucntion : ������Ϸ״̬
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : ����Ӣ�۷ɻ�
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBlockBmp��ʾ����λͼ���
	hRoofBmp��ʾ�ݶ�λͼ���
	roofWidth��roofHeight�ݶ���Ⱥ͸߶�
	blockWidth��blockHeight�����Ⱥ͸߶�
*************************************************/ 
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);

//˫�������
VOID Render(HWND hWnd);
//��ʱ���¼�
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero����
VOID HeroUpdate();
//·�Ƹ���
VOID CloudUpdate();
//���Ƹ���
VOID LightUpdate();
//���Ǹ���
VOID StarUpdate();
//��������
VOID ArrowUpdate();
//�������
VOID FireballUpdate();
//�ӵ�����
VOID BulletUpdate();
//Ѫƿ����
VOID BloodUpdate();
//���θ���
VOID TerrianUpdate();
//��Ϸ״̬����
VOID GameStatusUpdate();
//��������
VOID Reset();
//�ж��Ƿ�����ͣ
BOOL Paused(POINT);
//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//�����ɿ��¼�����
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);