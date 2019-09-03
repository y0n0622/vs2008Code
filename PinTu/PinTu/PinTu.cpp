// PinTu.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PinTu.h"
#include <time.h>

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名


#define BLOCKCOUNTOFW    3   //图片被分成 BLOCKCOUNTOFW 行
#define BLOCKCOUNTOFH    3   //图片被分成 BLOCKCOUNTOFH 列

//被分割后的矩形坐标, 顺序存放
RECT gRect[BLOCKCOUNTOFW * BLOCKCOUNTOFH]; 

//图片句柄
HBITMAP gBitMapHandle;

//图片的宽和高
int gBitMapWidth;
int gBitMapHeight;

//每个小方块的宽和高
int gBlockWidth;
int gBlockHeight;

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//函数声明
void InitializeGame(wchar_t* bmpPath);
void RandomRect();

//获取白色方块左边方块的索引号
int GetLeftBlockIndex(int index);
int GetDownBlockIndex(int index);
int GetUpBlockIndex(int index);
int GetRightBlockIndex(int index);

int GetWhiteBlockIndex();
void SwitchBlock(int r, int l);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((unsigned int)time(0));

	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	//初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PINTU, szWindowClass, MAX_LOADSTRING);


	MyRegisterClass(hInstance);   //注册窗口类, 窗口的样式

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))   //创建窗口
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PINTU));

	// 主消息循环:  所有的外部消息（鼠标点击，键盘按下）  
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)   //这个函数决定了窗口的样式，
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;   //注册回调函数，告诉系统，我的消息处理函数，就是这个
	//当有消息产生(点击了鼠标）的时候，麻烦您调用一下
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PINTU));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PINTU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)     //将窗口创建出来
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);  //将窗口建立

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);   //显示窗口
	UpdateWindow(hWnd);

	return TRUE;
}


//回调，不由程序员调用，交給系统调用， 系统当发现有外部事件发生的时候
//消息处理，各种消息处理，鼠标，键盘，网络。。。。
//所有的消息，都会进入该函数
//非常重要的函数
//消息处理函数

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)  //判断什么类型的消息, 根据不同消息，做不同的处理
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			RandomRect();
			InvalidateRect(hWnd, 0, 1);
			break;

			//在程序初始化的时候，将图片加载

			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;


	case WM_KEYDOWN:   //wm  window message
		{
			int index = GetWhiteBlockIndex();  //白色索引号

			switch (wParam)   //区别不同的按键
			{
			case VK_DOWN:   //方向键的向下箭头, virtual key
				{
					int dBlock = GetDownBlockIndex(index);  //由白色方块索引号，得到左边方块的索引号

					if (dBlock != -1)   //可以交换
					{
						SwitchBlock(index, dBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}
				break;
			case VK_UP:   //方向键的向下箭头, virtual key
				{
					int uBlock = GetUpBlockIndex(index);  //由白色方块索引号，得到左边方块的索引号

					if (uBlock != -1)   //可以交换
					{
						SwitchBlock(index, uBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}
				break;
			case VK_LEFT:   //方向键的向下箭头, virtual key
				{
					int lBlock = GetLeftBlockIndex(index);  //由白色方块索引号，得到左边方块的索引号

					if (lBlock != -1)   //可以交换
					{
						SwitchBlock(index, lBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}

				break;
			case VK_RIGHT:   //方向键的向下箭头, virtual key
				{
					int rBlock = GetRightBlockIndex(index);  //由白色方块索引号，得到左边方块的索引号

					if (rBlock != -1)   //可以交换
					{
						SwitchBlock(index, rBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}
				break;
			}
		}
		break;

	case WM_CREATE:   //窗口创建的时候，加载图片
		{
			InitializeGame(L"E:/2.bmp");
		}
	case WM_PAINT:   //当绘制窗口的时候，加载的图片
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		{
			HDC memDc = CreateCompatibleDC(hdc);  //创建一个内存DC， 用户画图片
			SelectObject(memDc, gBitMapHandle);  //将需要绘制的图片选入dc

			for (int i = 0; i < BLOCKCOUNTOFW; i++)
			{
				for (int j = 0; j < BLOCKCOUNTOFH; j++)
				{
					if (gRect[i * BLOCKCOUNTOFH + j].left == -1 && gRect[i * BLOCKCOUNTOFH + j].top == -1)
						continue;

					BitBlt(hdc, gBlockWidth * j, gBlockHeight * i, gBlockWidth, gBlockHeight,
						memDc, gRect[i * BLOCKCOUNTOFH + j].left, gRect[i * BLOCKCOUNTOFH + j].top, SRCCOPY);  //绘图函数
				}
			}

			DeleteObject(memDc);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

//自定义函数,用于计算被分割图片的矩形坐标
//bmpWidth  图片的宽
//bmpHeigth 图片的高
//blocksW 图片分成几行
//blocksH 图片分成几列

void InitializeBlockRect(int bmpWidth, int bmpHeigth, int blocksW, int blocksH)
{
	//计算每个方块的宽和高
	int width = bmpWidth / blocksH;
	int heigth = bmpHeigth / blocksW;

	gBlockWidth = width;
	gBlockHeight = heigth;

	for (int i = 0; i < blocksW; i++)
	{
		for (int j = 0; j < blocksH; j++)
		{
			//计算坐标
			//gRect[i * blocksW + j]; //是不是能表示每一个矩形区域
			gRect[i * blocksH + j].left = width * j;
			gRect[i * blocksH + j].top = heigth * i;
			gRect[i * blocksH + j].right = width * (j + 1);
			gRect[i * blocksH + j].bottom = heigth * (i + 1);
		}
	}

	//只是用于标记最后一个方块的
	gRect[blocksH * blocksW - 1].left = -1;
	gRect[blocksH * blocksW - 1].top = -1;
}

//经验

//加载图片, 初始化游戏
//bmpPath : 图片的路径
void InitializeGame(wchar_t* bmpPath)
{
	gBitMapHandle = (HBITMAP)LoadImage(0, bmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//图片的宽和高
	BITMAP bmp;
	GetObject(gBitMapHandle, sizeof(BITMAP), &bmp);   //得到了图片内容

	gBitMapWidth = bmp.bmWidth;
	gBitMapHeight = bmp.bmHeight;

	//将图片分割
	InitializeBlockRect(gBitMapWidth, gBitMapHeight, BLOCKCOUNTOFW, BLOCKCOUNTOFH);
}


//将算出来的矩形区域，随机打乱
void RandomRect()
{
	RECT rc;
	for (int i = 0; i < BLOCKCOUNTOFW * BLOCKCOUNTOFH; i++)
	{
		int rd = rand() % (BLOCKCOUNTOFW * BLOCKCOUNTOFH - 1);     //获取一个随机
		rc = gRect[i];
		gRect[i] = gRect[rd];
		gRect[rd] = rc;
	}
}

//得到白色方块的在数组中的索引号
int GetWhiteBlockIndex()
{
	for (int i = 0; i < BLOCKCOUNTOFW * BLOCKCOUNTOFH; i++)
	{
		if (gRect[i].left == -1 && gRect[i].top == -1)  //发现是白色方块的时候
		{
			return i;
		}
	}

	return 0;
}

//得到白色方块左边的方块索引
//index 白色方块的索引号
int GetLeftBlockIndex(int index)
{
	if (index % BLOCKCOUNTOFH == 0)  //判断白色方块是不是在最左边
		return -1;

	return index - 1;
}

int GetUpBlockIndex(int index)
{
	if (index - BLOCKCOUNTOFH < 0)  //判断白色方块是不是在最左边
		return -1;

	return index - BLOCKCOUNTOFH;
}

int GetRightBlockIndex(int index)
{
	if ((index + 1) % BLOCKCOUNTOFH == 0)  //数组的索引号从0开始
		return -1;

	return index + 1;
}

int GetDownBlockIndex(int index)
{
	if (index + BLOCKCOUNTOFH >=  BLOCKCOUNTOFH * BLOCKCOUNTOFW)  //判断白色方块是不是在最左边
		return -1;

	return index + BLOCKCOUNTOFH;
}

//交换2个小方块
void SwitchBlock(int r, int l)
{
	RECT rc = gRect[r];
	gRect[r] = gRect[l];
	gRect[l] = rc;
}