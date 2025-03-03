#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <time.h>
#include <stdlib.h>

//全局函数的声明
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				MyPaint(HDC hdc);
//全局变量声明
HINSTANCE hInst;
HBITMAP bj[3],man[2],fullmap,Hbm_Monsters[5],h_bg[12];
BITMAP	bitmap,bitmap2,bitmap3,Monsters[5],bg[12];
HDC		hdc,mdc,bufdc,mapdc,bgdc[12],monstersdc,monstersdc2,monstersdc3,monstersdc4,monstersdc5;
HWND	hWnd;
DWORD	tPre,tNow,tCheck;                 //声明三个函数来记录时间,tPre记录上一次绘图的时间，tNow记录此次准备绘图的时间，tCheck记录每秒开始的时间
int		num,frame,fps,x = 0,y = 0,j,game_time = 0,game_time1[10] = {0},x1,y1;                    
//num用来记录图号，frame用来累加每次画面更新的次数,fps（frame per second）用来记录每秒画面更新的次数
char    flag = 's';
int     blood = 3;//初始血量为三，血量为0时任务失败 
int     mouse_down = 0;//鼠标按下的flag，1按下
int     is_win = 0;//如果1，胜利，如果2，失败
int     docw=0,kerxw=0,jcw=0,bossw=0;
int 	bi=0;//boss_index 
int 	kerxan=0,jcan=0; //是否按住干员头像 
int 	kerxbs=0,jcbs=0;//干员是否部署 
int 	kerxbsd=5,jcbsd=16;//干员目前部署位置 
int 	am=0;//进蓝门怪物数量 
int 	dm=0,dmm=0;//已死亡怪物数量 
int 	bosszd=0;//boss是否被阻挡，同时开始攻击动作 
int 	rejcbsd=16,rekerxbsd=5;


int ms_blood[8] = {9,9,9,9,9,9,9,9};
int boss_blood  = 9;

//声明一个数组，记录怪物下一步行走的路线，1向下走，2向右走，-1向上，-2向左走
int fangxiang[30] = {1,1,1,1,1,1,1,1,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
int bossfx[20] = {2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2};
//记录怪物行走的坐标
POINT point[31];
POINT bossp[20]; 
//记录地图的数组，用于判定部署干员 
int kerxbsw[5][4]={
			248,345,180,247,
			345,442,180,247,
			518,630,247,320,
			518,630,320,401,
			732,840,180,247 };
int jcbsw[16][4]={
			248,345,120,180,
			345,442,120,180,
			442,518,120,180,
			518,630,120,180,
			630,732,120,180,
			630,732,180,247,
			630,732,247,320,
			630,732,320,401,
			732,810,320,401,
			810,908,120,180,
			810,908,180,247,
			810,908,247,320,
			810,908,320,401,
			442,518,180,247,
			442,518,247,320,
			442,518,320,401,};




//第三方函数1
void MyTransparentBlt( HDC hdcDest,      // 目标DC
					 int nXOriginDest,   // 目标X偏移
					 int nYOriginDest,   // 目标Y偏移
					 int nWidthDest,     // 目标宽度
					 int nHeightDest,    // 目标高度
					 HDC hdcSrc,         // 源DC
					 int nXOriginSrc,    // 源X起点
					 int nYOriginSrc,    // 源Y起点
					 int nWidthSrc,      // 源宽度
					 int nHeightSrc,     // 源高度
					 UINT crTransparent  // 透明色,COLORREF类型
					 )
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	// 将源DC中的位图拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	// 设置透明色
	SetBkColor(hImageDC, crTransparent);
	
	// 生成透明区域为白色，其它区域为黑色的掩码位图
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	
	// 生成透明区域为黑色，其它区域保持不变的位图
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// 透明部分保持屏幕不变，其它部分变成黑色
	SetBkColor(hdcDest,RGB(0xff,0xff,0xff));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// "或"运算,生成最终效果
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}


//***WinMain函数，程序入口点函数**************************************
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	//运行初始化函数
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	//游戏循环
	GetMessage(&msg,NULL,NULL,NULL);
    while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0,0 ,PM_REMOVE) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else
		{
			tNow = GetTickCount();
			if(tNow-tPre >= 100)        //当此次循环运行与上次绘图时间相差0.1秒时再进行重绘操作
				MyPaint(hdc);
		}
    }

	return msg.wParam;
}

//****设计一个窗口类，类似填空题，使用窗口结构体*************************
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
//	wcex.hCursor		= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "canvas";
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

//****初始化函数*************************************
// 从文件加载位图
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	char filename[20] = "";
	hInst = hInstance;
	hWnd = CreateWindow(
		"canvas",
		"Arknights",
		WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		return FALSE;
	}

	MoveWindow(hWnd,100,100,1165,580,true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

//////////////////////////////////////////////////////////////控制出怪速度 
	for(j=1;j<10;j++)
		game_time1[j] = game_time1[j-1] - 20;
		
		
	
//////////////////////////////////////////////////////////////位图绑定 
	hdc = GetDC(hWnd);
	mdc = CreateCompatibleDC(hdc);
	bufdc = CreateCompatibleDC(hdc);
	mapdc = CreateCompatibleDC(hdc);
	for(j=0;j<12;j++)
	bgdc[j] = CreateCompatibleDC(hdc);
	monstersdc = CreateCompatibleDC(hdc);
	monstersdc2 = CreateCompatibleDC(hdc);
	monstersdc3 = CreateCompatibleDC(hdc);
	monstersdc4 = CreateCompatibleDC(hdc);
	monstersdc5 = CreateCompatibleDC(hdc);
	fullmap = CreateCompatibleBitmap(hdc,1150,540);//虚拟位图
	SelectObject(mdc,fullmap);//绑定虚拟位图
	//防止屏幕闪烁 
	
	bj[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));//战斗界面 
	man[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));//棘刺 
	man[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));//凯尔希 
	Hbm_Monsters[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//源石虫行动 
	Hbm_Monsters[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP16));//boss行动r 
	Hbm_Monsters[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP17));//boss行动l
//	Hbm_Monsters[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP17));//源石虫攻击 
	Hbm_Monsters[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP18));//boss攻击 
	h_bg[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));//博士 
	h_bg[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));//剩余血量 
	h_bg[8] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP12));
	h_bg[9] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP13));
	h_bg[10] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP14));
	h_bg[11] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP15));
	SelectObject(bgdc[0],bj[0]);
	GetObject(bj[0],sizeof(BITMAP),&bitmap3);
	SelectObject(mapdc,man[0]);
	GetObject(man[0],sizeof(BITMAP),&bitmap);
	SelectObject(bufdc,man[1]);
	GetObject(man[1],sizeof(BITMAP),&bitmap2);
	for(j = 1;j<= 11;j++)
	{
		SelectObject(bgdc[j],h_bg[j]);
		GetObject(h_bg[j],sizeof(BITMAP),&bg[j]);
	}
	SelectObject(monstersdc,Hbm_Monsters[0]);
	GetObject(Hbm_Monsters[0],sizeof(BITMAP),&Monsters[0]);
	SelectObject(monstersdc2,Hbm_Monsters[1]);
	GetObject(Hbm_Monsters[1],sizeof(BITMAP),&Monsters[1]);
	SelectObject(monstersdc3,Hbm_Monsters[2]);
	GetObject(Hbm_Monsters[2],sizeof(BITMAP),&Monsters[2]);
	SelectObject(monstersdc4,Hbm_Monsters[3]);
	GetObject(Hbm_Monsters[3],sizeof(BITMAP),&Monsters[3]);
	
	num = 0;
	frame = 0;

	MyPaint(hdc);

	return TRUE;
}



//****自定义绘图函数*********************************
// 1.计算与显示每秒画面更新次数
// 2.按照图号顺序进行窗口贴图
void MyPaint(HDC hdc)
{
	int i;
	RECT rect;
	GetClientRect(hWnd,&rect);
	//贴背景图片
	BitBlt(mdc,0, 0,bitmap3.bmWidth,bitmap3.bmHeight,bgdc[0],0,0,SRCCOPY);

	if(kerxan==1){//凯尔希 
	MyTransparentBlt(mdc,1043,448,bg[10].bmWidth,bg[10].bmHeight,bgdc[10],0,0,bg[10].bmWidth,bg[10].bmHeight,RGB(0,0,0));//棘刺头像 
	MyTransparentBlt(mdc,x-50,y-50,bg[11].bmWidth,bg[11].bmHeight,bgdc[11],0,0,bg[11].bmWidth,bg[11].bmHeight,RGB(0,0,0));}//凯尔希头像 
	else if(jcan==1){//棘刺 
	MyTransparentBlt(mdc,x-50,y-50,bg[10].bmWidth,bg[10].bmHeight,bgdc[10],0,0,bg[10].bmWidth,bg[10].bmHeight,RGB(0,0,0));//棘刺头像 
	MyTransparentBlt(mdc,960,448,bg[11].bmWidth,bg[11].bmHeight,bgdc[11],0,0,bg[11].bmWidth,bg[11].bmHeight,RGB(0,0,0));}//凯尔希头像 
	else{
	MyTransparentBlt(mdc,1043,448,bg[10].bmWidth,bg[10].bmHeight,bgdc[10],0,0,bg[10].bmWidth,bg[10].bmHeight,RGB(0,0,0));//棘刺头像 
	MyTransparentBlt(mdc,960,448,bg[11].bmWidth,bg[11].bmHeight,bgdc[11],0,0,bg[11].bmWidth,bg[11].bmHeight,RGB(0,0,0));//凯尔希头像 
	}	
		 



for(int i=0;i<7;i++){
	if(ms_blood[i]<=0)//初始为9 
		dm++;
} 
dmm=dm>dmm?dm:dmm;
dm=0; 
	if(am<4) 
		MyTransparentBlt(mdc,655,5,18,bg[3].bmHeight,bgdc[3],54-18*am,0,18,bg[3].bmHeight,RGB(255,0,0));//进蓝门怪量 
	else
		MyTransparentBlt(mdc,655,5,18,bg[3].bmHeight,bgdc[3],0,0,18,bg[3].bmHeight,RGB(255,0,0));//进蓝门怪量 

	
	MyTransparentBlt(mdc,531,3,18,bg[3].bmHeight,bgdc[3],144,0,18,bg[3].bmHeight,RGB(255,0,0));//总怪量 

	MyTransparentBlt(mdc,505,3,18,bg[3].bmHeight,bgdc[3],18*(dmm+am),0,18,bg[3].bmHeight,RGB(255,0,0));//已死亡+进蓝门怪量 



////////////////////////////////////////////////////////////画网格
		HPEN hpen = (HPEN)GetStockObject(BLACK_PEN);
		SelectObject(mdc,hpen);
		MoveToEx(mdc,0,60,NULL);
		LineTo(mdc,1600,60);
		MoveToEx(mdc,0,120,NULL);
		LineTo(mdc,1600,60*2);
		MoveToEx(mdc,0,60*3,NULL);
		LineTo(mdc,1600,60*3);
		MoveToEx(mdc,0,60*4+7,NULL);
		LineTo(mdc,1600,60*4+7);
		MoveToEx(mdc,0,60*5+20,NULL);
		LineTo(mdc,1600,60*5+20);
		MoveToEx(mdc,0,60*6+41,NULL);
		LineTo(mdc,1600,60*6+41);
	
	//横 
		MoveToEx(mdc,343,0,NULL);
		LineTo(mdc,218,604);
		MoveToEx(mdc,413,0,NULL);
		LineTo(mdc,315,604);
		MoveToEx(mdc,483,0,NULL);
		LineTo(mdc,412,604);
		MoveToEx(mdc,543,0,NULL);
		LineTo(mdc,518,604);
		MoveToEx(mdc,610,0,NULL);
		LineTo(mdc,630,604);
		MoveToEx(mdc,675,0,NULL);
		LineTo(mdc,732,604);
		MoveToEx(mdc,750,0,NULL);
		LineTo(mdc,840,604);
		MoveToEx(mdc,810,0,NULL);
		LineTo(mdc,938,604);
	//竖 
///////////////////////////////////////////////////////////////源石虫行走路线
for(j = 0; j < 30;j++)
{
	
	point[0].x = 775;
	point[0].y = 0;
	if(fangxiang[j] == 1)
	{	
		point[j+1].x = point[j].x;
		point[j+1].y = point[j].y+ 36;
	}
	if(fangxiang[j] == -1)
	{	
		point[j+1].x = point[j].x;
		point[j+1].y = point[j].y - 36;
	}
	if(fangxiang[j] == 2)
	{	
		point[j+1].x = point[j].x + 40;
		point[j+1].y = point[j].y;
	}
	if(fangxiang[j] == -2)
	{	
		point[j+1].x = point[j].x - 40;
		point[j+1].y = point[j].y;
	}
}

/////////////////////////////////////////////////////////////boss行走路线
for(j = 0; j < 20;j++)
{
	
	bossp[0].x = 70;
	bossp[0].y = 230;
	if(bossfx[j] == 1)
	{	
		bossp[j+1].x = bossp[j].x;
		bossp[j+1].y = bossp[j].y+ 36;
	}
	if(bossfx[j] == -1)
	{	
		bossp[j+1].x = bossp[j].x;
		bossp[j+1].y = bossp[j].y - 36;
	}
	if(bossfx[j] == 2)
	{	
		bossp[j+1].x = bossp[j].x + 40;
		bossp[j+1].y = bossp[j].y;
	}
	if(bossfx[j] == -2)
	{	
		bossp[j+1].x = bossp[j].x - 40;
		bossp[j+1].y = bossp[j].y;
	}
}

	if(num == 7)
		num = 0;
	frame++;			       //画面更新次数加1
	if(tNow - tCheck >= 1000)               //判断此次绘图时间由前一秒算起是否已经达到1秒钟的时间间隔。若是，则将目前的'frame'值赋给"fps"，表示这一秒内所更新的画面次数，然后将“frame”值回0，并重设下次计算每秒画面数的起始时间"iCheck"。
	{
		fps = frame;
		frame = 0;
		tCheck = tNow;
	}	
	
	
/////////////////////////////////////////////////////////////////////源石虫行动
	for(i = 0; i<7;i++)//一共7只源石虫 
	{
		
		if(game_time1[i]> 0&&game_time1[i]<29&&ms_blood[i]>0) //怪物活着
		{		 
				MyTransparentBlt(mdc,point[game_time1[i]].x+20,point[game_time1[i]].y+100,80,16,bgdc[8],0,16*(9-ms_blood[i]),80,16,RGB(0,0,0));//血条

			
				MyTransparentBlt(mdc,point[game_time1[i]].x,point[game_time1[i]].y,Monsters[0].bmWidth,Monsters[0].bmHeight,monstersdc,
										0,0,Monsters[0].bmWidth,Monsters[0].bmHeight,RGB(0,0,0));//源石虫 

		}

		if(game_time1[i]>0&&game_time1[i]<29&&ms_blood[i]<=0&&game_time1[i]-game_time1[i+1]>12) //怪物死亡动画 
		{
			if(game_time%2==1)
				MyTransparentBlt(mdc,point[game_time1[i]].x+3,point[game_time1[i]].y+3,75,67,bgdc[9],15,102,75,67,RGB(0,0,0));
			if(game_time%2==0)
				MyTransparentBlt(mdc,point[game_time1[i]].x-10,point[game_time1[i]].y-15,100,102,bgdc[9],0,0,100,102,RGB(0,0,0));

		}

	}



///////////////////////////////////////////////////////////////////////干员 
	switch(jcbsd){
		case 0:
			MyTransparentBlt(mdc,200,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 1:
			MyTransparentBlt(mdc,270,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 2:
			MyTransparentBlt(mdc,350,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		
		case 3:
			MyTransparentBlt(mdc,430,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 4:
			MyTransparentBlt(mdc,500,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 5:
			MyTransparentBlt(mdc,510,50,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 6:
			MyTransparentBlt(mdc,510,120,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 7:
			MyTransparentBlt(mdc,510,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 8:
			MyTransparentBlt(mdc,600,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 9:
			MyTransparentBlt(mdc,670,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 10:
			MyTransparentBlt(mdc,675,50,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 11:
			MyTransparentBlt(mdc,685,120,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 12:
			MyTransparentBlt(mdc,700,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 13:
			MyTransparentBlt(mdc,350,50,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 14:
			MyTransparentBlt(mdc,340,120,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 15:
			MyTransparentBlt(mdc,330,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
		case 16:
			MyTransparentBlt(mdc,1500,1500,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//棘刺 
			break;
	}
	switch(kerxbsd){
		case 0:
			MyTransparentBlt(mdc,300,80,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//凯尔希 
			break;
		case 1:
			MyTransparentBlt(mdc,375,80,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//凯尔希 
			break;
		case 2:
			MyTransparentBlt(mdc,530,160,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//凯尔希 
			break;
		case 3:
			MyTransparentBlt(mdc,530,230,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//凯尔希 
			break;
		case 4:
			MyTransparentBlt(mdc,700,70,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//凯尔希 
			break;
		case 5:
			MyTransparentBlt(mdc,1500,1500,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//凯尔希 
			break;
	}
	if(blood>0)
		MyTransparentBlt(mdc,200,50,bg[2].bmWidth/4,bg[2].bmHeight,bgdc[2],bg[2].bmWidth*docw/4,0,bg[2].bmWidth/4,bg[2].bmHeight,RGB(0,0,0));//博士 
	
	kerxw=(kerxw+1)%8;//凯尔希战斗 
//	if()
	jcw=(jcw+1)%11;//棘刺战斗 
	docw=(docw+1)%4;//Q版博士运动 



////////////////////////////////////////////////////////////////////////////boss
	if(bi>=40&&bi<72&&bosszd==0&&boss_blood!=0){ //boss行动 
	MyTransparentBlt(mdc,bossp[(bi-40)/4].x,bossp[(bi-40)/4].y,Monsters[1].bmWidth/6,Monsters[1].bmHeight,monstersdc2,
		Monsters[1].bmWidth*bossw/6,0,Monsters[1].bmWidth/6,Monsters[1].bmHeight,RGB(0,0,0));//bossr 
		bossw=(bossw+1)%6;
		}
	else if(bi>=72&&bi<120&&bosszd==0&&boss_blood!=0){
	MyTransparentBlt(mdc,bossp[(bi-40)/4].x,bossp[(bi-40)/4].y,Monsters[2].bmWidth/6,Monsters[2].bmHeight,monstersdc3,
		Monsters[2].bmWidth*bossw/6,0,Monsters[2].bmWidth/6,Monsters[2].bmHeight,RGB(0,0,0));//bossl 
	bossw=(bossw+1)%6;
		}
	else if(bosszd>0&&boss_blood!=0){
	MyTransparentBlt(mdc,bossp[(bi-40)/4].x,bossp[(bi-40)/4].y-80,Monsters[3].bmWidth/6,Monsters[3].bmHeight,monstersdc4,
		Monsters[3].bmWidth*bossw/6,0,Monsters[3].bmWidth/6,Monsters[3].bmHeight,RGB(0,0,0));//bossa 
	bossw=(bossw+1)%6;
	}
		
//		if(bi<120)
			bi+=1;

////////////////////////////////////////////////////////////////////////////整体绘图+循环 
	BitBlt(hdc,0,0,rect.right,rect.bottom,mdc,0,0,SRCCOPY);//；mdc在hdc中显示，fullmap 
	tPre = GetTickCount();     //记录此次绘图时间，供下次游戏循环中判断是否已经达到画面更新操作设定的时间间隔。
	num++;

	game_time++;

 	
///////////////////////////////////////////////////////////////////////////干员阻挡+进入蓝门 
	for(i=0;i<8;i++)//源石虫 
	{
		if(game_time%2==1&&ms_blood[i]>0)
		{	
			if(jcbsd>=13||jcbsd<13&&25-jcbsd*2!=game_time1[i]) 
				game_time1[i]++;
			if(game_time1[i]-game_time1[i+1]<15)
				ms_blood[i]=0;
			if(game_time1[i] == 30){
					blood--;//博士受到攻击
					am++; 
				}
		}
	}
	if(bosszd>18&&kerxbsd==5) jcbsd=16;
	else if(bosszd>18&&kerxbsd!=5&&boss_blood>0) boss_blood=0,am++; 
	
	if(boss_blood>0){//boss 
			switch(jcbsd){
				case 15:
					if(bi>60)
					bi=60,bosszd++; 
					break;
				case 14:
					if(bi>70)
					bi=70,bosszd++;
					break;
				case 13:
					if(bi>80)
					bi=80,bosszd++;
					break;
				case 2:
					if(bi>90)
					bi=90,bosszd++;
					break;
				case 1:
					if(bi>100)
					bi=100,bosszd++;
					break;
				case 0:
					if(bi>110)
					bi=110,bosszd++;
					break;	
			}
			if(jcbsd==16) bosszd=0;
			if(bi>=120){
					blood--;//博士受到攻击
					bi=-10000;
					am++; 
				}
	}
/////////////////////////////////////////////////////////////////////////////胜利判断
	if(am+dmm==8&&blood>0&&is_win!=3)
		is_win = 1;
	if(blood<=0&&is_win!=3)
		is_win = 2;
		
	

//////////////////////////////////////////////////////////////////////////游戏结束 
	srand(time(NULL));
	int yy;
	yy=rand()%2; 
	if(is_win == 2&&game_time%5 == 0&&yy==0)
	{
		int result = MessageBox( NULL , TEXT("\t\t任务失败\n\n棘刺：考虑到小队的损耗，除了立即撤退，没有其他更好的选择。") ,
		 TEXT("") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:exit(0);break;
			case IDNO:is_win=3;break;
		}
	}
	if(is_win == 2&&game_time%5 == 0&&yy==1)
	{
		int result = MessageBox( NULL , TEXT("\t任务失败\n\n凯尔希：保全撤退路线，准备手术！") ,
		 TEXT("") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:exit(0);break;
			case IDNO:is_win=3;break;
		}
	}
	if(is_win == 1&&game_time%3 == 1&&yy==0)
	{
		int result = MessageBox( NULL , TEXT("\t任务完成\n\n棘刺：任务完成。非常精准有效率的指挥，博士。") , 
		TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:{
						int result2 = MessageBox( NULL , TEXT("是否进行代理作战？") , 
						TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
						switch(result2)
						{
							case IDYES:{
//								清零
									x=0;y=0;game_time=0;
									game_time1[10] = {0};
									for(j=1;j<10;j++)
										game_time1[j] = game_time1[j-1] - 20;
									blood=3;is_win=0;
									docw=0;kerxw=0;jcw=0;bossw=0;
									bi=0;
									am=0;dm=0;dmm=0;
									bosszd=0;
									for(int i=0;i<8;i++)
									ms_blood[i] = 9;
									boss_blood  = 9;
								break;
							} 
							case IDNO:exit(0);break;
						}
				break;}
			case IDNO:is_win=3;break;
		}
	}
	if(is_win == 1&&game_time%3 == 1&&yy==1)
	{
		int result = MessageBox( NULL , TEXT("\t任务完成\n\n凯尔希：Mon3tr，采集好我需要的组织样本。") , 
		TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:{
				int result3 = MessageBox( NULL , TEXT("是否进行代理作战？") , 
							TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
				switch(result3)
						{
							case IDYES:{
//								清零
									x=0;y=0;game_time=0;
									game_time1[10] = {0};
									for(j=1;j<10;j++)
										game_time1[j] = game_time1[j-1] - 20;
									blood=3;is_win=0;
									docw=0;kerxw=0;jcw=0;bossw=0;
									bi=0;
									am=0;dm=0;dmm=0;
									bosszd=0;
									for(int i=0;i<8;i++)
									ms_blood[i] = 9;
									boss_blood  = 9;
								break;
							} 
							case IDNO:exit(0);break;
						}
			break;}
			case IDNO:is_win=3;break;
		}
	}
	
}

//******消息处理函数*********************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int i;
	RECT rect;
	GetClientRect(hWnd,&rect);
	switch (message)
	{
		case WM_LBUTTONDOWN://部署  
			if(LOWORD(lParam)>=960&&LOWORD(lParam)<1043&&HIWORD(lParam)>448){
		  	 	mouse_down=1;
		  	 	kerxan=1; 
			 	}
			if(LOWORD(lParam)>1043&&HIWORD(lParam)>448){
		  	 	mouse_down=1;
		  	 	jcan=1; 
			 	}
			 break;
		case WM_RBUTTONDOWN://撤退  
				x = LOWORD(lParam);
			 	y = HIWORD(lParam);
			 	if(kerxbsd!=5&&x>kerxbsw[kerxbsd][0]&&x<kerxbsw[kerxbsd][1]&&y>kerxbsw[kerxbsd][2]&&y<kerxbsw[kerxbsd][3])
			 		kerxbsd=5;
			 	if(jcbsd!=16&&x>jcbsw[jcbsd][0]&&x<jcbsw[jcbsd][1]&&y>jcbsw[jcbsd][2]&&y<jcbsw[jcbsd][3])
			 		jcbsd=16;
			 break;
		case WM_MOUSEMOVE: 
			 if(kerxan==1||jcan==1){
			 	x = LOWORD(lParam);
			 	y = HIWORD(lParam);
			 }
			 break;
		case WM_LBUTTONUP:
			 if(kerxan==1){
			 	for(int i=0;i<5;i++){
			 		if(x>kerxbsw[i][0]&&x<kerxbsw[i][1]&&y>kerxbsw[i][2]&&y<kerxbsw[i][3])
			 		kerxbsd=i;
				 }
			 }
			 if(jcan==1){
			 	for(int i=0;i<16;i++){
			 		if(x>jcbsw[i][0]&&x<jcbsw[i][1]&&y>jcbsw[i][2]&&y<jcbsw[i][3])
			 		jcbsd=i;
				 }
			 }
			 kerxan=0;
			 jcan = 0;
			 break;
		case WM_CHAR:
			 break;
		case WM_DESTROY:			//窗口结束消息
			DeleteDC(mdc);
			DeleteDC(mapdc);
			DeleteDC(bufdc);
			for(i=0;i<2;i++)
				DeleteObject(man[i]);
			ReleaseDC(hWnd,hdc);
			PostQuitMessage(0);
			break;
		default:					//其他消息
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


