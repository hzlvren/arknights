#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include <time.h>
#include <stdlib.h>

//ȫ�ֺ���������
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				MyPaint(HDC hdc);
//ȫ�ֱ�������
HINSTANCE hInst;
HBITMAP bj[3],man[2],fullmap,Hbm_Monsters[5],h_bg[12];
BITMAP	bitmap,bitmap2,bitmap3,Monsters[5],bg[12];
HDC		hdc,mdc,bufdc,mapdc,bgdc[12],monstersdc,monstersdc2,monstersdc3,monstersdc4,monstersdc5;
HWND	hWnd;
DWORD	tPre,tNow,tCheck;                 //����������������¼ʱ��,tPre��¼��һ�λ�ͼ��ʱ�䣬tNow��¼�˴�׼����ͼ��ʱ�䣬tCheck��¼ÿ�뿪ʼ��ʱ��
int		num,frame,fps,x = 0,y = 0,j,game_time = 0,game_time1[10] = {0},x1,y1;                    
//num������¼ͼ�ţ�frame�����ۼ�ÿ�λ�����µĴ���,fps��frame per second��������¼ÿ�뻭����µĴ���
char    flag = 's';
int     blood = 3;//��ʼѪ��Ϊ����Ѫ��Ϊ0ʱ����ʧ�� 
int     mouse_down = 0;//��갴�µ�flag��1����
int     is_win = 0;//���1��ʤ�������2��ʧ��
int     docw=0,kerxw=0,jcw=0,bossw=0;
int 	bi=0;//boss_index 
int 	kerxan=0,jcan=0; //�Ƿ�ס��Աͷ�� 
int 	kerxbs=0,jcbs=0;//��Ա�Ƿ��� 
int 	kerxbsd=5,jcbsd=16;//��ԱĿǰ����λ�� 
int 	am=0;//�����Ź������� 
int 	dm=0,dmm=0;//�������������� 
int 	bosszd=0;//boss�Ƿ��赲��ͬʱ��ʼ�������� 
int 	rejcbsd=16,rekerxbsd=5;


int ms_blood[8] = {9,9,9,9,9,9,9,9};
int boss_blood  = 9;

//����һ�����飬��¼������һ�����ߵ�·�ߣ�1�����ߣ�2�����ߣ�-1���ϣ�-2������
int fangxiang[30] = {1,1,1,1,1,1,1,1,-2,-2,-2,-2,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2};
int bossfx[20] = {2,2,2,2,2,2,2,2,-1,-1,-1,-1,-1,-1,-2,-2,-2,-2,-2,-2};
//��¼�������ߵ�����
POINT point[31];
POINT bossp[20]; 
//��¼��ͼ�����飬�����ж������Ա 
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




//����������1
void MyTransparentBlt( HDC hdcDest,      // Ŀ��DC
					 int nXOriginDest,   // Ŀ��Xƫ��
					 int nYOriginDest,   // Ŀ��Yƫ��
					 int nWidthDest,     // Ŀ����
					 int nHeightDest,    // Ŀ��߶�
					 HDC hdcSrc,         // ԴDC
					 int nXOriginSrc,    // ԴX���
					 int nYOriginSrc,    // ԴY���
					 int nWidthSrc,      // Դ���
					 int nHeightSrc,     // Դ�߶�
					 UINT crTransparent  // ͸��ɫ,COLORREF����
					 )
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// ��������λͼ
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// ������ɫ����λͼ
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	// ��ԴDC�е�λͼ��������ʱDC��
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	// ����͸��ɫ
	SetBkColor(hImageDC, crTransparent);
	
	// ����͸������Ϊ��ɫ����������Ϊ��ɫ������λͼ
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	
	// ����͸������Ϊ��ɫ���������򱣳ֲ����λͼ
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// ͸�����ֱ�����Ļ���䣬�������ֱ�ɺ�ɫ
	SetBkColor(hdcDest,RGB(0xff,0xff,0xff));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// "��"����,��������Ч��
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}


//***WinMain������������ڵ㺯��**************************************
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	//���г�ʼ������
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	//��Ϸѭ��
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
			if(tNow-tPre >= 100)        //���˴�ѭ���������ϴλ�ͼʱ�����0.1��ʱ�ٽ����ػ����
				MyPaint(hdc);
		}
    }

	return msg.wParam;
}

//****���һ�������࣬��������⣬ʹ�ô��ڽṹ��*************************
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

//****��ʼ������*************************************
// ���ļ�����λͼ
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

//////////////////////////////////////////////////////////////���Ƴ����ٶ� 
	for(j=1;j<10;j++)
		game_time1[j] = game_time1[j-1] - 20;
		
		
	
//////////////////////////////////////////////////////////////λͼ�� 
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
	fullmap = CreateCompatibleBitmap(hdc,1150,540);//����λͼ
	SelectObject(mdc,fullmap);//������λͼ
	//��ֹ��Ļ��˸ 
	
	bj[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));//ս������ 
	man[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));//���� 
	man[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));//����ϣ 
	Hbm_Monsters[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));//Դʯ���ж� 
	Hbm_Monsters[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP16));//boss�ж�r 
	Hbm_Monsters[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP17));//boss�ж�l
//	Hbm_Monsters[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP17));//Դʯ�湥�� 
	Hbm_Monsters[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP18));//boss���� 
	h_bg[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));//��ʿ 
	h_bg[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));//ʣ��Ѫ�� 
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



//****�Զ����ͼ����*********************************
// 1.��������ʾÿ�뻭����´���
// 2.����ͼ��˳����д�����ͼ
void MyPaint(HDC hdc)
{
	int i;
	RECT rect;
	GetClientRect(hWnd,&rect);
	//������ͼƬ
	BitBlt(mdc,0, 0,bitmap3.bmWidth,bitmap3.bmHeight,bgdc[0],0,0,SRCCOPY);

	if(kerxan==1){//����ϣ 
	MyTransparentBlt(mdc,1043,448,bg[10].bmWidth,bg[10].bmHeight,bgdc[10],0,0,bg[10].bmWidth,bg[10].bmHeight,RGB(0,0,0));//����ͷ�� 
	MyTransparentBlt(mdc,x-50,y-50,bg[11].bmWidth,bg[11].bmHeight,bgdc[11],0,0,bg[11].bmWidth,bg[11].bmHeight,RGB(0,0,0));}//����ϣͷ�� 
	else if(jcan==1){//���� 
	MyTransparentBlt(mdc,x-50,y-50,bg[10].bmWidth,bg[10].bmHeight,bgdc[10],0,0,bg[10].bmWidth,bg[10].bmHeight,RGB(0,0,0));//����ͷ�� 
	MyTransparentBlt(mdc,960,448,bg[11].bmWidth,bg[11].bmHeight,bgdc[11],0,0,bg[11].bmWidth,bg[11].bmHeight,RGB(0,0,0));}//����ϣͷ�� 
	else{
	MyTransparentBlt(mdc,1043,448,bg[10].bmWidth,bg[10].bmHeight,bgdc[10],0,0,bg[10].bmWidth,bg[10].bmHeight,RGB(0,0,0));//����ͷ�� 
	MyTransparentBlt(mdc,960,448,bg[11].bmWidth,bg[11].bmHeight,bgdc[11],0,0,bg[11].bmWidth,bg[11].bmHeight,RGB(0,0,0));//����ϣͷ�� 
	}	
		 



for(int i=0;i<7;i++){
	if(ms_blood[i]<=0)//��ʼΪ9 
		dm++;
} 
dmm=dm>dmm?dm:dmm;
dm=0; 
	if(am<4) 
		MyTransparentBlt(mdc,655,5,18,bg[3].bmHeight,bgdc[3],54-18*am,0,18,bg[3].bmHeight,RGB(255,0,0));//�����Ź��� 
	else
		MyTransparentBlt(mdc,655,5,18,bg[3].bmHeight,bgdc[3],0,0,18,bg[3].bmHeight,RGB(255,0,0));//�����Ź��� 

	
	MyTransparentBlt(mdc,531,3,18,bg[3].bmHeight,bgdc[3],144,0,18,bg[3].bmHeight,RGB(255,0,0));//�ܹ��� 

	MyTransparentBlt(mdc,505,3,18,bg[3].bmHeight,bgdc[3],18*(dmm+am),0,18,bg[3].bmHeight,RGB(255,0,0));//������+�����Ź��� 



////////////////////////////////////////////////////////////������
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
	
	//�� 
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
	//�� 
///////////////////////////////////////////////////////////////Դʯ������·��
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

/////////////////////////////////////////////////////////////boss����·��
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
	frame++;			       //������´�����1
	if(tNow - tCheck >= 1000)               //�жϴ˴λ�ͼʱ����ǰһ�������Ƿ��Ѿ��ﵽ1���ӵ�ʱ���������ǣ���Ŀǰ��'frame'ֵ����"fps"����ʾ��һ���������µĻ��������Ȼ�󽫡�frame��ֵ��0���������´μ���ÿ�뻭��������ʼʱ��"iCheck"��
	{
		fps = frame;
		frame = 0;
		tCheck = tNow;
	}	
	
	
/////////////////////////////////////////////////////////////////////Դʯ���ж�
	for(i = 0; i<7;i++)//һ��7ֻԴʯ�� 
	{
		
		if(game_time1[i]> 0&&game_time1[i]<29&&ms_blood[i]>0) //�������
		{		 
				MyTransparentBlt(mdc,point[game_time1[i]].x+20,point[game_time1[i]].y+100,80,16,bgdc[8],0,16*(9-ms_blood[i]),80,16,RGB(0,0,0));//Ѫ��

			
				MyTransparentBlt(mdc,point[game_time1[i]].x,point[game_time1[i]].y,Monsters[0].bmWidth,Monsters[0].bmHeight,monstersdc,
										0,0,Monsters[0].bmWidth,Monsters[0].bmHeight,RGB(0,0,0));//Դʯ�� 

		}

		if(game_time1[i]>0&&game_time1[i]<29&&ms_blood[i]<=0&&game_time1[i]-game_time1[i+1]>12) //������������ 
		{
			if(game_time%2==1)
				MyTransparentBlt(mdc,point[game_time1[i]].x+3,point[game_time1[i]].y+3,75,67,bgdc[9],15,102,75,67,RGB(0,0,0));
			if(game_time%2==0)
				MyTransparentBlt(mdc,point[game_time1[i]].x-10,point[game_time1[i]].y-15,100,102,bgdc[9],0,0,100,102,RGB(0,0,0));

		}

	}



///////////////////////////////////////////////////////////////////////��Ա 
	switch(jcbsd){
		case 0:
			MyTransparentBlt(mdc,200,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 1:
			MyTransparentBlt(mdc,270,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 2:
			MyTransparentBlt(mdc,350,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		
		case 3:
			MyTransparentBlt(mdc,430,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 4:
			MyTransparentBlt(mdc,500,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 5:
			MyTransparentBlt(mdc,510,50,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 6:
			MyTransparentBlt(mdc,510,120,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 7:
			MyTransparentBlt(mdc,510,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 8:
			MyTransparentBlt(mdc,600,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 9:
			MyTransparentBlt(mdc,670,0,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 10:
			MyTransparentBlt(mdc,675,50,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 11:
			MyTransparentBlt(mdc,685,120,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 12:
			MyTransparentBlt(mdc,700,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 13:
			MyTransparentBlt(mdc,350,50,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 14:
			MyTransparentBlt(mdc,340,120,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 15:
			MyTransparentBlt(mdc,330,200,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
		case 16:
			MyTransparentBlt(mdc,1500,1500,bitmap2.bmWidth,bitmap2.bmHeight/11,bufdc,
				0,bitmap2.bmHeight*jcw/11,bitmap2.bmWidth,bitmap2.bmHeight/11,RGB(0,0,0));//���� 
			break;
	}
	switch(kerxbsd){
		case 0:
			MyTransparentBlt(mdc,300,80,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//����ϣ 
			break;
		case 1:
			MyTransparentBlt(mdc,375,80,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//����ϣ 
			break;
		case 2:
			MyTransparentBlt(mdc,530,160,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//����ϣ 
			break;
		case 3:
			MyTransparentBlt(mdc,530,230,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//����ϣ 
			break;
		case 4:
			MyTransparentBlt(mdc,700,70,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//����ϣ 
			break;
		case 5:
			MyTransparentBlt(mdc,1500,1500,bitmap.bmWidth,bitmap.bmHeight/8,mapdc,
				0,bitmap.bmHeight*kerxw/8,bitmap.bmWidth,bitmap.bmHeight/8,RGB(0,0,0));//����ϣ 
			break;
	}
	if(blood>0)
		MyTransparentBlt(mdc,200,50,bg[2].bmWidth/4,bg[2].bmHeight,bgdc[2],bg[2].bmWidth*docw/4,0,bg[2].bmWidth/4,bg[2].bmHeight,RGB(0,0,0));//��ʿ 
	
	kerxw=(kerxw+1)%8;//����ϣս�� 
//	if()
	jcw=(jcw+1)%11;//����ս�� 
	docw=(docw+1)%4;//Q�沩ʿ�˶� 



////////////////////////////////////////////////////////////////////////////boss
	if(bi>=40&&bi<72&&bosszd==0&&boss_blood!=0){ //boss�ж� 
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

////////////////////////////////////////////////////////////////////////////�����ͼ+ѭ�� 
	BitBlt(hdc,0,0,rect.right,rect.bottom,mdc,0,0,SRCCOPY);//��mdc��hdc����ʾ��fullmap 
	tPre = GetTickCount();     //��¼�˴λ�ͼʱ�䣬���´���Ϸѭ�����ж��Ƿ��Ѿ��ﵽ������²����趨��ʱ������
	num++;

	game_time++;

 	
///////////////////////////////////////////////////////////////////////////��Ա�赲+�������� 
	for(i=0;i<8;i++)//Դʯ�� 
	{
		if(game_time%2==1&&ms_blood[i]>0)
		{	
			if(jcbsd>=13||jcbsd<13&&25-jcbsd*2!=game_time1[i]) 
				game_time1[i]++;
			if(game_time1[i]-game_time1[i+1]<15)
				ms_blood[i]=0;
			if(game_time1[i] == 30){
					blood--;//��ʿ�ܵ�����
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
					blood--;//��ʿ�ܵ�����
					bi=-10000;
					am++; 
				}
	}
/////////////////////////////////////////////////////////////////////////////ʤ���ж�
	if(am+dmm==8&&blood>0&&is_win!=3)
		is_win = 1;
	if(blood<=0&&is_win!=3)
		is_win = 2;
		
	

//////////////////////////////////////////////////////////////////////////��Ϸ���� 
	srand(time(NULL));
	int yy;
	yy=rand()%2; 
	if(is_win == 2&&game_time%5 == 0&&yy==0)
	{
		int result = MessageBox( NULL , TEXT("\t\t����ʧ��\n\n���̣����ǵ�С�ӵ���ģ������������ˣ�û���������õ�ѡ��") ,
		 TEXT("") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:exit(0);break;
			case IDNO:is_win=3;break;
		}
	}
	if(is_win == 2&&game_time%5 == 0&&yy==1)
	{
		int result = MessageBox( NULL , TEXT("\t����ʧ��\n\n����ϣ����ȫ����·�ߣ�׼��������") ,
		 TEXT("") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:exit(0);break;
			case IDNO:is_win=3;break;
		}
	}
	if(is_win == 1&&game_time%3 == 1&&yy==0)
	{
		int result = MessageBox( NULL , TEXT("\t�������\n\n���̣�������ɡ��ǳ���׼��Ч�ʵ�ָ�ӣ���ʿ��") , 
		TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:{
						int result2 = MessageBox( NULL , TEXT("�Ƿ���д�����ս��") , 
						TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
						switch(result2)
						{
							case IDYES:{
//								����
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
		int result = MessageBox( NULL , TEXT("\t�������\n\n����ϣ��Mon3tr���ɼ�������Ҫ����֯������") , 
		TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
		switch(result)
		{
			case IDYES:{
				int result3 = MessageBox( NULL , TEXT("�Ƿ���д�����ս��") , 
							TEXT("result") , MB_ICONINFORMATION|MB_YESNO);
				switch(result3)
						{
							case IDYES:{
//								����
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

//******��Ϣ������*********************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int i;
	RECT rect;
	GetClientRect(hWnd,&rect);
	switch (message)
	{
		case WM_LBUTTONDOWN://����  
			if(LOWORD(lParam)>=960&&LOWORD(lParam)<1043&&HIWORD(lParam)>448){
		  	 	mouse_down=1;
		  	 	kerxan=1; 
			 	}
			if(LOWORD(lParam)>1043&&HIWORD(lParam)>448){
		  	 	mouse_down=1;
		  	 	jcan=1; 
			 	}
			 break;
		case WM_RBUTTONDOWN://����  
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
		case WM_DESTROY:			//���ڽ�����Ϣ
			DeleteDC(mdc);
			DeleteDC(mapdc);
			DeleteDC(bufdc);
			for(i=0;i<2;i++)
				DeleteObject(man[i]);
			ReleaseDC(hWnd,hdc);
			PostQuitMessage(0);
			break;
		default:					//������Ϣ
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


