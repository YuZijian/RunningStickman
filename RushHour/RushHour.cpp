#include "RushHour.h"

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	WNDCLASSEX wcex;

	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON)); // 使用预定义图标
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);                      // 使用预定义光标
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;                                        // 不指定菜单
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));// 小图标

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// WNDWIDTH, WNDHEIGHT: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //设置窗口样式，不可改变大小，不可最大化
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd); //显示窗口刷新窗口客户区

	// Main message loop:
	//从消息队列中提取消息
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//转化键盘消息
		TranslateMessage(&msg);
		//将消息发送到相应的窗口函数
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//  窗口函数原型
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//初始化
		Init(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		//绘制（刷新窗口）
		Render(hWnd);
		break;
	case WM_KEYDOWN:
		//键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		//键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		//左鼠标事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		//定时器事件
		TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		Mouse_x = LOWORD(lParam);
		Mouse_y = HIWORD(lParam);
		//鼠标是否在开始按钮上
		if(Page == 0 && (Mouse_x >= 285 && Mouse_x <= 395 && Mouse_y >= 130 && Mouse_y <= 200) || 
			(Mouse_x - 285)*(Mouse_x - 285)+(Mouse_y - 165)*(Mouse_y - 165) <= 35*35 ||
			(Mouse_x - 395)*(Mouse_x - 395)+(Mouse_y - 165)*(Mouse_y - 165) <= 35*35)
			Is_Move = 1;
		else
			Is_Move = 0;
		//是否选关
		if(Page == 1 && Mouse_x >= 250 && Mouse_x <= 450 && Mouse_y >= 110 && Mouse_y <= 160)
			easy = 1;
		else if(Page == 1)
			easy = 0;
		if(Page == 1 && Mouse_x >= 250 && Mouse_x <= 450 && Mouse_y >= 170 && Mouse_y <= 220) 	
			hard = 1;
		else if(Page == 1)
			hard = 0;
		if(Page == 1 && Mouse_x >= 250 && Mouse_x <= 450 && Mouse_y >= 230 && Mouse_y <= 280)
			kendie = 1;
		else if(Page == 1)
		    kendie = 0;
		//鼠标是否在帮助按钮上
		if(Page == 0 && Mouse_x >= 350 && Mouse_x <= 410 &&  Mouse_y >= 240 && Mouse_y <= 300)
			Is_Help = 1;
		else
			Is_Help = 0;
		
		//是否返回按钮2
		if(Page == 3 && Mouse_x <= 353 && Mouse_x >= 287 && Mouse_y >= 208 && Mouse_y <= 269)
		{
			Is_Return2 = 1;
		}
		else
		{
			Is_Return2 = 0;
		}
		//是否重新开始
		if(Page == 3 && Mouse_x >= 367 && Mouse_x <= 437 && Mouse_y >= 207 && Mouse_y <= 273)
		{
			Is_ReStart = 1;
		}
		else
		{
			Is_ReStart = 0;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		// 将不处理的消息给电脑自动处理
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam)
{ 
	//加载开始界面
	m_hBeginPicture = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BEGINPICTURE));
	//加载开始按钮
	m_hStartButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_STARTBUTTON));
	//加载三个选关按钮
	m_heasy = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_EASY));
	m_hhard = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HARD));
	m_hkendie = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_KENDIE));
	//加载声音按钮
	m_hSoundButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_SOUNDBUTTON));
	//加载帮助按钮
	m_hHelpButton = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HELPBUTTON));
	//加载帮助界面
	m_hHelpPicture = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HELPPICTURE));
	//加载背景位图
	m_hBackgroundBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BACKGROUND));
	//加载Building位图
	m_hBuildingBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BUILDING));
	//加载Building位图
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO));
	//加载游戏状态位图
	m_hGameStatusBmp =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GAME_STATUS));
	//加载路灯
	m_hLight =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_LIGHT));
	//加载白云
	m_hCloud = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_CLOUD));
	//加载星星
	for(k = 0; k < STAR_NUM; k++)
	m_hStar[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_STAR));
	//加载弓箭
	m_hArrow = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_ARROW));
	m_hFire = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_FIREBALL));
	m_hMyfire = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_MYFIRE));
	//加载血瓶
	m_hBlood = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HP));
	//加载返回按钮1
	m_hReturnButton1 =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_RETURNBUTTON1));
	//加载结束界面
	m_hLosePicture =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
	MAKEINTRESOURCE(IDB_LOSEPICTURE));
	//加载返回按钮2
	m_hReturnButton2 =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
	MAKEINTRESOURCE(IDB_RETURNBUTTON2));
	//重新开始按钮
	m_hReStart =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
	MAKEINTRESOURCE(IDB_RESTART));
	//加载Block位图
	for (k = 0; k < BLOCK_COLOR_NUM; ++k)
	{
		m_hBlockBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_blockBmpNames[k]));
	}
	//加载屋顶位图
	for (k = 0; k < ROOF_COLOR_NUM; ++k)
	{
		m_hRoofkBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_roofBmpNames[k]));
	}

	
	m_building = CreateBuilding(0, 70, 500, 250, m_hBuildingBmp);
	m_light = CreateLight(400, 200, 110, 146, m_hLight);
	m_cloud = CreateCloud(150, 50, 76, 42, m_hCloud);
	m_arrow = CreateArrow(1500, 90, 50, 11, m_hArrow);
	m_blood = CreateBlood(100, 80, 25, 25, m_hBlood);
	m_fire  = CreateArrow(2500, 120, 50, 25, m_hFire);

	//创建英雄、建筑     x    y    w   l    
	m_hero = CreateHero(200, 200, 50, 50, m_hHeroBmp, 0, 10);
	//创建子弹
	m_bullet = CreateBullet(232, 224, 25, 13, m_hMyfire);

	//创建游戏状态
	m_gameStatus = CreateGameStatus(35, 0, 40, 30, m_hGameStatusBmp);
	
	//创建地形
	srand((unsigned)time(NULL));
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if(k >= 0 && k <= 5)
		{
			m_terrian[k] = CreateTerrian(k * 65, 250, 65, 300,
			m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], i%5 == 4?0:33, i%5==4?0:65);
			house[k] = 250; //记录y坐标
			continue;
		}
		else
		{
			i = rand();
			m_terrian[k] = CreateTerrian(k * 65, 100 + 50*(i%5), 65, 300,
			m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], i%5 == 4?0:33, i%5==4?0:65);
			if(i%5 == 4)
				house[k] = 500;
			else
				house[k] = 100 + 50*(i%5);
		}
	}
	//创建星星
	for (k = 0; k < STAR_NUM; ++k)
	{
		m_star[k] = CreateStar(5*k*65+10, house[k]-175, 25, 25, m_hStar[k]) ;
	}

	//播放音乐
	PlaySound((LPCWSTR)IDR_MUSIC, NULL, SND_RESOURCE|SND_LOOP|SND_ASYNC);

}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//开始绘制
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);    //创建适配大小的位图
	hdcBmp = CreateCompatibleDC(hdc);                             
	hdcBuffer = CreateCompatibleDC(hdc);              //创建缓冲区
	SelectObject(hdcBuffer, cptBmp);
	
	if(Page == 0)
	{
		//绘制开始界面
		SelectObject(hdcBmp, m_hBeginPicture);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);

		//绘制开始按钮
		SelectObject(hdcBmp, m_hStartButton);
		if(!Is_Move) //鼠标不在在开始按钮上
		{
			TransparentBlt(
					hdcBuffer, 240, 130,
					200, 75,
					hdcBmp, 0, 0, 200, 75,
					RGB(255, 255, 255)
					);
		}
		else //鼠标在开始按钮上
		{
			TransparentBlt(
					hdcBuffer, 240, 130,
					200, 75,
					hdcBmp, 0, 75, 200, 75,
					RGB(255, 255, 255)
					);
		}

		//绘制声音按钮
		SelectObject(hdcBmp, m_hSoundButton);
		if(Is_Sound)
		{
			TransparentBlt(
					hdcBuffer, 400, 20,
					60, 60,
					hdcBmp, 0, 0, 60, 60,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer, 400, 20,
					60, 60,
					hdcBmp, 0, 60, 60, 60,
					RGB(255, 255, 255)
					);
		}

		//绘制帮助按钮 及 帮助界面
		SelectObject(hdcBmp, m_hHelpButton);
		TransparentBlt(
			hdcBuffer, 350, 240,
			60, 60,
			hdcBmp, 0, 60, 60, 60,
			RGB(255, 255, 255)
			);
		if(Is_Help)
		{
			SelectObject(hdcBmp, m_hHelpPicture);
			TransparentBlt(
					hdcBuffer, 63, 30,
					375, 270,
					hdcBmp, 0, 0, 375, 270,
					RGB(255, 255, 255)
					);
		}
	}

	if(Page == 1)//点击开始按钮
	{
		//绘制开始界面
		SelectObject(hdcBmp, m_hBeginPicture);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);

		//绘制声音按钮
		SelectObject(hdcBmp, m_hSoundButton);
		if(Is_Sound)
		{
			TransparentBlt(
					hdcBuffer, 400, 20,
					60, 60,
					hdcBmp, 0, 0, 60, 60,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer, 400, 20,
					60, 60,
					hdcBmp, 0, 60, 60, 60,
					RGB(255, 255, 255)
					);
		}

		SelectObject(hdcBmp, m_hReturnButton1);
					TransparentBlt(hdcBuffer, 0, 3, 
					40, 30,
					hdcBmp, 0, 0,40, 30, 
					RGB(255, 255, 255)
					);

		SelectObject(hdcBmp, m_heasy);
		if(easy)
		{
			TransparentBlt(
					hdcBuffer, 250, 110,
					200, 50,
					hdcBmp, 0, 50, 200, 50,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer,250, 110,
					200, 50,
					hdcBmp, 0, 0, 200, 50,
					RGB(255, 255, 255)
					);
		}

		SelectObject(hdcBmp, m_hhard);
		if(hard)
		{
			TransparentBlt(
					hdcBuffer, 250, 170,
					200,50,
					hdcBmp, 0, 50, 200, 50,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer, 250, 170,
					200, 50,
					hdcBmp, 0, 0, 200, 50,
					RGB(255, 255, 255)
					);
		}

		SelectObject(hdcBmp, m_hkendie);
		if(kendie)
		{
			TransparentBlt(
					hdcBuffer, 250, 230,
					200, 50,
					hdcBmp, 0, 50, 200, 50,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer, 250, 230,
					200, 50,
					hdcBmp, 0, 0, 200, 50,
					RGB(255, 255, 255)
					);
		}
	}

	if(Page == 2 && Is_Dead == 0) //点击了选关按钮
	{
		//绘制背景到缓存
		SelectObject(hdcBmp, m_hBackgroundBmp);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
			hdcBmp, 0, 0, SRCCOPY);

		//绘制建筑到缓存
		SelectObject(hdcBmp, m_building.hBmp);
		TransparentBlt(
			hdcBuffer, m_building.pos.x, m_building.pos.y,
			m_building.size.cx, m_building.size.cy,
			hdcBmp, 0, 0, m_building.size.cx, m_building.size.cy,
			RGB(255, 255, 255)
			);

		//绘制白云
		SelectObject(hdcBmp, m_cloud.hBmp);
		TransparentBlt(
			hdcBuffer, m_cloud.pos.x, m_cloud.pos.y,
			m_cloud.size.cx, m_cloud.size.cy,
			hdcBmp, 0, 0, m_cloud.size.cx, m_cloud.size.cy,
			RGB(255, 255, 255)
			);

		//绘制Hero到缓存
		SelectObject(hdcBmp, m_hero.hBmp);
		if(Num_Jump == 2) //绘制不同的帧体现不同动作
		TransparentBlt(
			hdcBuffer, m_hero.pos.x, m_hero.pos.y,
			m_hero.size.cx, m_hero.size.cy,
			hdcBmp, 0, m_hero.size.cy * m_hero.curFrameIndex, m_hero.size.cx, m_hero.size.cy,
			RGB(255, 255, 255)
			);
		else if (Num_Jump == 1)
			TransparentBlt(
			hdcBuffer, m_hero.pos.x, m_hero.pos.y,
			m_hero.size.cx, m_hero.size.cy,
			hdcBmp, 0, m_hero.size.cy * 4, m_hero.size.cx, m_hero.size.cy,
			RGB(255, 255, 255)
			);
		else
			TransparentBlt(
			hdcBuffer, m_hero.pos.x, m_hero.pos.y,
			m_hero.size.cx, m_hero.size.cy,
			hdcBmp, 0, m_hero.size.cy * 7, m_hero.size.cx, m_hero.size.cy,
			RGB(255, 255, 255)
			);

		//绘制血瓶
		SelectObject(hdcBmp, m_hBlood);
		if(m_blood.iseaten < 1)
		{
			TransparentBlt(
				hdcBuffer, m_blood.pos.x, m_blood.pos.y,
				25, 25,
				hdcBmp, 0, 0, 35, 35,
				RGB(255, 255, 255)
				);
		}

			
		//绘制地形
		for (k = 0; k < MAX_TERRIAN_NUM; ++k)
		{
			Terrian terrian = m_terrian[k];
			SelectObject(hdcBmp, terrian.hRoofBmp);
			TransparentBlt(
				hdcBuffer, terrian.pos.x, terrian.pos.y,
				terrian.roofWidth, terrian.roofHeight,
				hdcBmp, 0, 0, terrian.roofWidth, terrian.roofHeight,
				RGB(255, 255, 255)
				);
			SelectObject(hdcBmp, terrian.hBlockBmp);
			int t;
			for (t = 0; t < terrian.blockNum; ++t)
			{
				TransparentBlt(
					hdcBuffer, terrian.pos.x, terrian.pos.y + terrian.roofHeight + terrian.blockHeight * t,
					terrian.blockWidth, terrian.blockHeight,
					hdcBmp, 0, 0, terrian.blockWidth, terrian.blockHeight,
					RGB(255, 255, 255)
					);
			}
	
		}

		//绘制路灯
		SelectObject(hdcBmp, m_hLight);
		TransparentBlt(
			hdcBuffer, m_light.pos.x, m_light.pos.y,
			m_light.size.cx, m_light.size.cy,
			hdcBmp, 0, 0, m_light.size.cx, m_light.size.cy,
			RGB(255, 255, 255)
		);

		//绘制星星
		SelectObject(hdcBmp, m_hStar[0]);
		for (k = 0; k < STAR_NUM; k++)
		{
			Star star = m_star[k];
			if(!star.iseaten)
			TransparentBlt(
				hdcBuffer, star.pos.x, star.pos.y,
				star.size.cx, star.size.cy,
				hdcBmp, 0, 0, star.size.cx, star.size.cy,
				RGB(255, 255, 255)
				);
		}

		//绘制弓箭
		SelectObject(hdcBmp, m_hArrow);
		if(m_arrow.touch < 1 && !m_arrow.crash)
		{
			TransparentBlt(
				hdcBuffer, m_arrow.pos.x, m_arrow.pos.y,
				50, 11,
				hdcBmp, 0, 0, 70, 15,
				RGB(255, 255, 255)
				);
		}

		//绘制火球
		SelectObject(hdcBmp, m_hFire);
		if(m_fire.touch < 1 && !m_fire.crash)
		{
			TransparentBlt(
				hdcBuffer, m_fire.pos.x, m_fire.pos.y,
				50, 25,
				hdcBmp, 0, 0, 50, 25,
				RGB(255, 255, 255)
				);
		}

		//绘制子弹
		SelectObject(hdcBmp, m_hMyfire);
		if(m_bullet.touch < 1 && m_bullet.shoot != 0)
		{
			TransparentBlt(
				hdcBuffer, m_bullet.pos.x, m_bullet.pos.y,
				25, 13,
				hdcBmp, 0, 0, 25, 13,
				RGB(255, 255, 255)
				);
		}

		//绘制游戏中声音按钮
		SelectObject(hdcBmp, m_hSoundButton);
		if(Is_Sound)
		{
			TransparentBlt(
					hdcBuffer, 400, 20,
					60, 60,
					hdcBmp, 0, 0, 60, 60,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer, 400, 20,
					60, 60,
					hdcBmp, 0, 60, 60, 60,
					RGB(255, 255, 255)
					);
		}
		//绘制返回按钮1
		SelectObject(hdcBmp, m_hReturnButton1);
					TransparentBlt(hdcBuffer, 0, 3, 
					40, 30,
					hdcBmp, 0, 0,40, 30, 
					RGB(255, 255, 255));

		//绘制游戏状态
		//暂停或继续位图
		SelectObject(hdcBmp, m_gameStatus.hBmp);
		TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
			hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
			m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));

		//绘制分数
		//改变字体大小
		LOGFONT logfont;
		ZeroMemory(&logfont, sizeof(LOGFONT));
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfHeight = 20;
		HFONT hFont = CreateFontIndirect(&logfont);
		SelectObject(hdcBuffer, hFont);
		TCHAR szDist[13];
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 250, 15, szDist, wsprintf(szDist, _T("你的得分 %d"), m_gameStatus.totalDist/2));
		DeleteObject(hFont);

		LOGFONT logfont1;
		ZeroMemory(&logfont1, sizeof(LOGFONT));
		logfont1.lfCharSet = GB2312_CHARSET;
		logfont1.lfHeight = 20;
		HFONT hFont1 = CreateFontIndirect(&logfont1);
		SelectObject(hdcBuffer, hFont1);
		TCHAR szDist1[13];
		SetTextColor(hdcBuffer, RGB(255, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 130, 10, szDist1, wsprintf(szDist1, _T("HP: %d/10"), hp));
		DeleteObject(hFont);

		LOGFONT logfont2;
		ZeroMemory(&logfont2, sizeof(LOGFONT));
		logfont2.lfCharSet = GB2312_CHARSET;
		logfont2.lfHeight = 20;
		HFONT hFont2 = CreateFontIndirect(&logfont2);
		SelectObject(hdcBuffer, hFont2);
		TCHAR szDist2[13];
		SetTextColor(hdcBuffer, RGB(0, 0, 255));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 130, 30, szDist2, wsprintf(szDist2, _T("MP: %d/10"), mp));
		DeleteObject(hFont);
	}

	//如果死亡
	if(Page == 3)
	{
		//结束界面
		SelectObject(hdcBmp, m_hLosePicture);
		BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	    //重新开始按钮
		SelectObject(hdcBmp, m_hReStart);
		if(Is_ReStart)
		{
			TransparentBlt(
					hdcBuffer, 360, 200,
					80, 80,
					hdcBmp, 0, 80, 80, 80,
					RGB(255, 255, 255)
					);
		}
		else
		TransparentBlt(
					hdcBuffer, 360, 200,
					80, 80,
					hdcBmp, 0, 0, 80, 80,
					RGB(255, 255, 255)
					);
		//返回按钮2
		SelectObject(hdcBmp, m_hReturnButton2);
		if(Is_Return2)
		{
			TransparentBlt(
					hdcBuffer, 280, 200,
					80, 80,
					hdcBmp, 0, 80, 80, 80,
					RGB(255, 255, 255)
					);
		}
		else
		{
			TransparentBlt(
					hdcBuffer, 280, 200,
					80, 80,
					hdcBmp, 0, 0, 80, 80,
					RGB(255, 255, 255)
					);
		}

		//显示得分
		LOGFONT logfont;
		ZeroMemory(&logfont, sizeof(LOGFONT));
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfHeight = 25;
		HFONT hFont = CreateFontIndirect(&logfont);
		SelectObject(hdcBuffer, hFont);
		TCHAR	szDist[13];
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 218, 120, szDist, wsprintf(szDist, _T("你的最终得分是%d"), m_gameStatus.totalDist/2));
		DeleteObject(hFont);
	}
	//最后将所有的信息绘制到屏幕上
		
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//回收资源所占的内存  Delete与Create成对出现
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);
	//结束绘制
	EndPaint(hWnd, &ps);
}

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	return hero;
}

Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Building building;
	building.hBmp = hBmp;
	building.pos.x = posX;
	building.pos.y = posY;
	building.size.cx = sizeX;
	building.size.cy = sizeY;
	return building;
}

Light CreateLight(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Light light;
	light.hBmp = hBmp;
	light.pos.x = posX;
	light.pos.y = posY;
	light.size.cx = sizeX;
	light.size.cy = sizeY;
	return light;
}

Cloud CreateCloud(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Cloud cloud;
	cloud.hBmp  = hBmp;
	cloud.pos.x = posX;
	cloud.pos.y = posY;
	cloud.size.cx = sizeX;
	cloud.size.cy = sizeY;
	return cloud;
}

Star CreateStar(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Star star;
	star.hBmp = hBmp;
	star.pos.x = posX;
	star.pos.y = posY;
	star.size.cx = sizeX;
	star.size.cy = sizeY;
	star.iseaten = 0;
	return star;
}

Arrow CreateArrow(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Arrow arrow;
	arrow.hBmp = hBmp;
	arrow.pos.x = posX;
	arrow.pos.y = posY;
	arrow.size.cx = sizeX;
	arrow.size.cy = sizeY;
	arrow.touch = 0;
	arrow.crash = 0;
	return arrow;
}

Bullet CreateBullet(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Bullet a;
	a.hBmp = hBmp;
	a.pos.x = posX;
	a.pos.y = posY;
	a.size.cx = sizeX;
	a.size.cy = sizeY;
	a.shoot = 0;
	a.touch = 0;
	return a;
}

Blood CreateBlood(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Blood blood;
	blood.hBmp = hBmp;
	blood.pos.x = posX;
	blood.pos.y = posY;
	blood.size.cx = sizeX;
	blood.size.cy = sizeY;
	blood.iseaten = 0;
	return blood;
}


GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0;
	gameStatus.isPaused = false;
	return gameStatus;
}

Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hBlockBmp = hBlockBmp;
	terrian.hRoofBmp = hRoofBmp;
	terrian.roofWidth = sizeX;
	terrian.roofHeight = roofHeight;
	terrian.blockWidth = sizeX;
	terrian.blockHeight = blockHeight;
	terrian.blockNum = (int)ceil((sizeY - roofHeight) * 1.0 / blockHeight); 
	return terrian;
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(velocity != 0)
	{
		m_hero.pos.y -= velocity;
		velocity -= 2;
	}
	
	if(m_hero.pos.y >= height && Is_Dead == 0)
	{
		velocity = -1;
		Num_Jump = 2;
		m_hero.pos.y = height;
	}

	HeroUpdate();
	LightUpdate();
	StarUpdate();
	ArrowUpdate();
	FireballUpdate();
	BloodUpdate();
	CloudUpdate();
	TerrianUpdate();
	GameStatusUpdate();
	BulletUpdate();
	InvalidateRect(hWnd, NULL, FALSE);
}

VOID HeroUpdate()
{
	//TODO
	//更新位置
	if(Page == 2 && Is_Crash == 0)
		m_hero.pos.x -= 0;
	if(Page == 2 && Is_Crash == 1)
		m_hero.pos.x -= vbuilding;
	if(Page == 2 && (m_hero.pos.x <= -40 || m_hero.pos.y >= 380 || Is_Dead))
	{
		Is_Dead = 1;
		Page = 3;
		if(DeadMusic)
		{
		PlaySound((LPCWSTR)IDR_DEADMUSIC, NULL, SND_RESOURCE|SND_ASYNC);
		DeadMusic = 0;
		}
	}
	//更新动作
	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= m_hero.maxFrameSize ? 0 : m_hero.curFrameIndex;
}

VOID CloudUpdate()
{
	m_cloud.pos.x -= vbuilding;
	i++;
	if(i >= 49)
		i = -50;
	if(i>=0 && i<=49)
		m_cloud.pos.x -= 1;
	else
		m_cloud.pos.x += 1;
	if(m_cloud.pos.x + m_cloud.size.cx <= 0)
		m_cloud.pos.x += 600;
}

VOID LightUpdate()
{
	m_light.pos.x -= vbuilding;
	if(m_light.pos.x + m_light.size.cx <= 0)
		m_light.pos.x += 600;
}

VOID ArrowUpdate()
{
	m_arrow.pos.x -= varrow;
	if(kendie)
	{
		m_arrow.pos.y = m_hero.pos.y+20;
	}
	else if(hard)
	{
		j++;
		if(j >= 40)
			j = -40;
		if(j>=0 && j<=40)
			m_arrow.pos.y -= 2;
		else
			m_arrow.pos.y += 2;
	}
	if(m_arrow.pos.x + m_arrow.size.cx <= 0)
	{
		m_arrow.pos.x += 1000;
		m_arrow.touch = 0;
		m_arrow.crash = 0;
	}
	if(m_hero.pos.x+32 >= m_arrow.pos.x && m_hero.pos.x+18 <= m_arrow.pos.x+50
		&& m_hero.pos.y+40 >= m_arrow.pos.y && m_hero.pos.y+14 <= m_arrow.pos.y+11)
		m_arrow.touch++;
}

VOID FireballUpdate()
{
	m_fire.pos.x -= (varrow+2);
	if(kendie || hard)
	{
		l++;
		if(l >= 20)
			l = -20;
		if(l>=0 && l<=20)
			m_fire.pos.y+=2;
		else
			m_fire.pos.y-=2;
	}
	if(m_fire.pos.x + m_fire.size.cx <= 0)
	{
		m_fire.pos.x += 852;
		m_fire.touch = 0;
		m_fire.crash = 0;
	}
	if(m_hero.pos.x+32 >= m_fire.pos.x && m_hero.pos.x+18 <= m_fire.pos.x+50
		&& m_hero.pos.y+40 >= m_fire.pos.y && m_hero.pos.y+14 <= m_fire.pos.y+24)
		m_fire.touch++;
}

VOID BulletUpdate()
{
	if(m_bullet.shoot == 0)
	{
		m_bullet.pos.x = m_hero.pos.x+32;
		m_bullet.pos.y = m_hero.pos.y+24;
	}
	else
	{
		m_bullet.pos.x += 6;
	}
	if(m_bullet.pos.x >= 500)
	{
		m_bullet.shoot = 0;
		m_bullet.touch = 0;
	}
	if(m_bullet.shoot && m_bullet.pos.x+24 >= m_arrow.pos.x && m_bullet.pos.x <= m_arrow.pos.x+50
		&& m_bullet.pos.y+12 >= m_arrow.pos.y && m_bullet.pos.y <= m_arrow.pos.y+10)
	{
		m_arrow.crash = 1;
		m_bullet.touch = 1;
		m_bullet.pos.x = 600;
	}
	if(m_bullet.shoot && m_bullet.pos.x+24 >= m_fire.pos.x && m_bullet.pos.x <= m_fire.pos.x+50
		&& m_bullet.pos.y+12 >= m_fire.pos.y && m_bullet.pos.y <= m_fire.pos.y+24)
	{
		m_fire.crash = 1;
		m_bullet.touch = 1;
		m_bullet.pos.x = 600;
	}
}

VOID BloodUpdate()
{
	m_blood.pos.x -= vbuilding;
	if(kendie)
	{
		if(m_blood.pos.x + m_blood.size.cx <= 0)
		{
			m_blood.pos.x += 600;
			m_blood.iseaten = 0;
		}
	}
	else
	{
		if(m_blood.pos.x + m_blood.size.cx <= 0)
		{
			m_blood.pos.x += 2329;
			m_blood.iseaten = 0;
		}
	}
	if(m_hero.pos.x+35 >= m_blood.pos.x+2 && m_hero.pos.x+15 <= m_blood.pos.x+33
			&& m_hero.pos.y + 45 >= m_blood.pos.y && m_hero.pos.y <= m_blood.pos.y+32)
			m_blood.iseaten++;
}
VOID StarUpdate()
{
	for(k = 0; k < STAR_NUM; k++)
	{
		m_star[k].pos.x -= vbuilding;
		if(m_hero.pos.x+35 >= m_star[k].pos.x+5 && m_hero.pos.x+15 <= m_star[k].pos.x+20
			&& m_hero.pos.y + 45 >= m_star[k].pos.y && m_hero.pos.y <= m_star[k].pos.y+20)
		{
			m_star[k].iseaten = 1;
		}
		if(m_star[k].pos.x + m_star[k].size.cx < 0)
		{
			m_star[k].iseaten = 0;
			m_star[k].pos.x += 5000;
		}
	}
}

VOID TerrianUpdate()
{
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		//建筑后移速度
		m_terrian[k].pos.x -= vbuilding;
		//判定处在哪一块建筑上
		if(m_terrian[k].pos.x <= m_hero.pos.x+25 && m_terrian[k+1].pos.x >= m_hero.pos.x+25)
		{	
			height = house[k] - 50;
			if(house[k+1] < house[k] && m_terrian[k+1].pos.x <= m_hero.pos.x + 47)
			{
				height = house[k+1] - 50;
				if(m_hero.pos.y+50 > m_terrian[k+1].pos.y)
				{
					Is_Crash = 1;
					height = house[k] - 50;
				}
				else
				{
					Is_Crash = 0;
				}
			}
			else if(house[k+1] > house[k] && m_terrian[k+1].pos.x <= m_hero.pos.x + 9)
				height = house[k+1] - 50;
		}
			
		//如果建筑跑出左侧界面
		if (m_terrian[k].pos.x + m_terrian[k].size.cx < 0)
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
		}
	}
}

VOID GameStatusUpdate()
{
	if(Page == 2 && !Is_Dead)
		++m_gameStatus.totalDist;

	for(k = 0; k < STAR_NUM; k++)
	{
		if(m_star[k].iseaten == 1)
		{
			m_gameStatus.totalDist += 50;
			m_star[k].iseaten = 2;
		}
	}

	if(m_arrow.touch==1 && m_arrow.crash == 0)
	{
		hp--;
		m_arrow.touch++;
	}

	if(m_fire.touch == 1 && m_fire.crash == 0)
	{
		hp-=2;
		m_fire.touch++;
	}

	if(m_blood.iseaten == 1)
	{
		if(hp < 10)
			hp++;
		if(mp < 10)
			mp++;
	}

	if(hp <= 0)
	{
		Is_Dead = 1;
	}

	if((m_gameStatus.totalDist+1) % 800 == 0)
	{
		if(easy && vbuilding <= 7)
			vbuilding++;
		else if(hard && vbuilding <= 9)
			vbuilding++;
		else if(kendie && vbuilding <= 8)
			vbuilding++;
	}
}

VOID Reset()
{
	m_hero = CreateHero(200, 200, 50, 50, m_hHeroBmp, 0, 10);
	m_arrow = CreateArrow(1500, 90, 50, 11, m_hArrow);
	m_fire  = CreateArrow(2500, 120, 50, 25, m_hFire);
	m_blood = CreateBlood(100, 80, 25, 25, m_hBlood);
	Is_Dead = 0;
	Is_Crash = 0;
	hp = 10;
	mp = 10;
	m_arrow.touch = 0;
	m_arrow.crash = 0;
	m_fire.touch = 0;
	m_fire.crash = 0;
	m_bullet.shoot = 0;
	m_bullet.touch = 0;
	m_gameStatus.totalDist = 0;
	DeadMusic = 1;
	m_gameStatus.isPaused = false;
}

BOOL Paused(POINT ptMouse)
{
	RECT rPause;

	rPause.left = m_gameStatus.pos.x;
	rPause.top = m_gameStatus.pos.y;
	rPause.right = m_gameStatus.pos.x+m_gameStatus.size.cx;
	rPause.bottom = m_gameStatus.pos.y+m_gameStatus.size.cy;

	return PtInRect(&rPause, ptMouse);
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
	case 'W':
		if(!kendie && Num_Jump >= 1 && !Is_Press)
		{
		velocity = 21;
		Is_Press = 1;
		}
		break;
	case VK_DOWN:
	case 'S':
		if(kendie && Num_Jump >= 1 && !Is_Press)
		{
		velocity = 21;
		Is_Press = 1;
		}
		break;
	case VK_LEFT:
	case 'A': 
		if(!kendie && Is_Crash == 0)
			m_hero.pos.x -= 5;
		else if(!kendie && Is_Crash == 0)
			m_hero.pos.x += 5;
		break;
	case VK_RIGHT:
	case 'D':
		if(!kendie && Is_Crash == 0)
			m_hero.pos.x += 5;
		else if(kendie && Is_Crash == 0)
			m_hero.pos.x -= 5;
		break;
	case 'R':
		hp = 5;
		break;
	case ' ':
		m_bullet.shoot++;
		if(m_bullet.shoot == 1 && mp > 0)
		{
			mp--;
			m_bullet.shoot++;
		}
		break;
	default:
		break;
	}
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
	case 'W':
		if(!kendie)
		{
			Is_Press = 0;
			Num_Jump--;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case VK_DOWN:
	case 'S':
		if(kendie)
		{
			Is_Press = 0;
			Num_Jump--;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case ' ':
		break;
	default:
		break;
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
	//如果点击了开始图标
	if(	Page == 0 && Is_Move)
	{
		Page = 1;
		easy = 0;
		hard = 0;
		kendie = 0;
	}

	if(Page == 1 && (easy || hard || kendie))
	{
		//重设地形
		for (k = 0; k < MAX_TERRIAN_NUM; ++k)
		{
			if(k >= 0 && k <= 5)
			{
				m_terrian[k] = CreateTerrian(k * 65, 250, 65, 300,
				m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], i%5 == 4?0:33, i%5==4?0:65);
				house[k] = 250; 
				continue;
			}
			else
			{
				i = rand();
				m_terrian[k] = CreateTerrian(k * 65, 100 + 50*(i%5), 65, 300,
				m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], i%5 == 4?0:33, i%5==4?0:65);
				if(i%5 == 4)
					house[k] = 500;
				else
					house[k] = 100 + 50*(i%5);
			}
		}

		if(easy)
		{
			vbuilding = 4;
			varrow = 8;
			easy = 1;
			hard = 0;
			kendie = 0;
		}
		else if(hard)
		{
			vbuilding = 6;
			varrow = 12;
			hard = 1;
			easy = 0;
			kendie = 0;
		}
		else if(kendie)
		{
			vbuilding = 5;
			varrow = 10;
			kendie = 1;
			easy = 0;
			hard = 0;
		}
		Reset();
		Page = 2;
		SetTimer(hWnd, TIMER, 32, NULL);
	}
	//如果点击了声音图标
	if(Page < 3 && ptMouse.x >= 400 && ptMouse.x <= 480 && ptMouse.y <= 80 && ptMouse.y >= 20)
	{
		Is_Sound = Is_Sound ? 0 : 1;//来回切换图标
		if(Is_Sound)
			PlaySound((LPCWSTR)IDR_MUSIC, NULL, SND_RESOURCE|SND_LOOP|SND_ASYNC);
		else
			PlaySound(NULL, NULL, SND_PURGE);
	}
	//如果点击了继续或暂停图标
	if (Page == 2 && ptMouse.x >= 38 && ptMouse.x <= 75 && ptMouse.y <= 30 && ptMouse.y >= 0)
	{
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER, 32, NULL);
			m_gameStatus.isPaused = FALSE;
		}
	}
	//如果点击了返回图标1
	//重设数据和地形
	if(Page >= 1 && Page <= 2 && ptMouse.x >= 0 && ptMouse.x <= 37 && ptMouse.y <= 33 && ptMouse.y >= 2) 
	{
		Page = 0;
		Reset();
	}

	//如果点击了返回图标2
	//重设数据和地形
	if(Page == 3 && Is_Dead && Is_Return2)
	{
		Page = 0;
		Reset();
		PlaySound((LPCWSTR)IDR_MUSIC, NULL, SND_RESOURCE|SND_LOOP|SND_ASYNC);
	}

	//如果点击重新开始
	//重置所有数据，重设地形
	if(Page == 3 && Is_Dead && Is_ReStart)
	{
		for (k = 0; k < MAX_TERRIAN_NUM; ++k)
		{
			if(k >= 0 && k <= 5)
			{
				m_terrian[k] = CreateTerrian(k * 65, 250, 65, 300,
				m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], i%5 == 4?0:33, i%5==4?0:65);
				house[k] = 250; 
				continue;
			}
			else
			{
				i = rand();
				m_terrian[k] = CreateTerrian(k * 65, 100 + 50*(i%5), 65, 300,
				m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], i%5 == 4?0:33, i%5==4?0:65);
				if(i%5 == 4)
					house[k] = 500;
				else
					house[k] = 100 + 50*(i%5);
			}
		}
		Reset();
		if(easy)
			vbuilding = 4;
		else if(hard)
			vbuilding = 6;
		else
			vbuilding = 5;
		SetTimer(hWnd, TIMER, 32, NULL);
		Page = 2;
		PlaySound((LPCWSTR)IDR_MUSIC, NULL, SND_RESOURCE|SND_LOOP|SND_ASYNC);
	}
	InvalidateRect(hWnd, NULL, FALSE);	
}