// ScreenCapture.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include <Windows.h>
#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HDC g_srcMemDc;
HDC g_grayMemDc;
int screenW;
int screenH;

RECT rect = {0};   //��ͼ�ľ�������
bool isDrawing = false;
bool isSelect = false;


void GetScreenCapture();
void CovertToGrayBitmap(HBITMAP hSourceBmp,HDC sourceDc);
void WriteDatatoClipBoard();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPTSTR    lpCmdLine,
					   _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SCREENCAPTURE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENCAPTURE));

	// ����Ϣѭ��:
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



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SCREENCAPTURE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_SCREENCAPTURE);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_MAXIMIZE);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	LOGBRUSH brush;
	brush.lbStyle=BS_NULL;
	HBRUSH hBrush=CreateBrushIndirect(&brush);

	LOGPEN pen;
	POINT penWidth;
	penWidth.x=2;
	penWidth.y=2;
	pen.lopnColor=0x0000FFFF;
	pen.lopnStyle=PS_SOLID;
	pen.lopnWidth=penWidth;
	HPEN hPen=CreatePenIndirect(&pen);


	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CREATE:
		GetScreenCapture();
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			HDC memDc = CreateCompatibleDC(hdc);
			HBITMAP bmp = CreateCompatibleBitmap(hdc, screenW, screenH);
			SelectObject(memDc, bmp);

			BitBlt(memDc, 0, 0, screenW,screenH, g_grayMemDc, 0, 0, SRCCOPY);
			SelectObject(memDc, hBrush);
			SelectObject(memDc, hPen);

			if (isDrawing || isSelect)
			{

				BitBlt(memDc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, g_srcMemDc, rect.left, rect.top, SRCCOPY);
				Rectangle(memDc, rect.left, rect.top, rect.right, rect.bottom);
			}

			BitBlt(hdc, 0, 0, screenW, screenH, memDc, 0, 0, SRCCOPY);

			DeleteObject(bmp);
			DeleteObject(memDc);

			EndPaint(hWnd, &ps);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if (!isSelect)
			{
				POINT pt;
				GetCursorPos(&pt);
				rect.left = pt.x;
				rect.top = pt.y;
				rect.right =  pt.x;
				rect.bottom = pt.y;

				isDrawing = true;
				InvalidateRgn(hWnd, 0, false);
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			if (isDrawing && !isSelect)
			{
				isDrawing = false;
				POINT pt;
				GetCursorPos(&pt);
				rect.right = pt.x;
				rect.bottom = pt.y;

				isSelect = true;

				InvalidateRgn(hWnd, 0, false);
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			if (isDrawing&& !isSelect)
			{
				POINT pt;
				GetCursorPos(&pt);
				rect.right = pt.x;
				rect.bottom = pt.y;
				InvalidateRgn(hWnd, 0, false);
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if (isSelect)
			{
				WriteDatatoClipBoard();
				InvalidateRgn(hWnd, 0, false);
				ShowWindow(hWnd, SW_MINIMIZE);
			}
			isSelect = false;

		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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


void GetScreenCapture()
{
	HDC disDc = ::CreateDC(L"DISPLAY", 0, 0, 0);  //������Ļ��ص�DC
	screenW = GetDeviceCaps(disDc, HORZRES);//ˮƽ�ֱ���
	screenH = GetDeviceCaps(disDc, VERTRES);//��ֱ�ֱ���

	g_srcMemDc = CreateCompatibleDC(disDc);  //��������Ļ���ݵ�DC���ڴ�DC��
	HBITMAP hbMap = CreateCompatibleBitmap(disDc, screenW, screenH);  //ģ��һ�Ż�����������û�����ݵ�
	SelectObject(g_srcMemDc, hbMap);   //����ͼѡ���ڴ�DC�����л���û�����ݵ�
	BitBlt(g_srcMemDc, 0, 0, screenW, screenH, disDc, 0, 0, SRCCOPY);  //����Ļ��dc�еĻ�ͼ���������ڴ�DC��

	//��ȡ��Ļ�ĻҶ�ͼƬ
	g_grayMemDc = CreateCompatibleDC(disDc);
	HBITMAP grayMap = CreateCompatibleBitmap(disDc, screenW, screenH);  //ģ��һ�Ż�����������û�����ݵ�
	SelectObject(g_grayMemDc, grayMap);   //����ͼѡ���ڴ�DC�����л���û�����ݵ�
	BitBlt(g_grayMemDc, 0, 0, screenW, screenH, disDc, 0, 0, SRCCOPY);  //����Ļ��dc�еĻ�ͼ���������ڴ�DC��

	CovertToGrayBitmap(grayMap, g_grayMemDc);  //����ɫͼƬת���Ҷ�ͼƬ

	DeleteObject(hbMap);
	DeleteObject(grayMap);
	DeleteObject(disDc);
}

void CovertToGrayBitmap(HBITMAP hSourceBmp,HDC sourceDc)
{
	HBITMAP retBmp=hSourceBmp;
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo,sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	GetDIBits(sourceDc,retBmp,0,0,NULL,&bmpInfo,DIB_RGB_COLORS);

	BYTE* bits=new BYTE[bmpInfo.bmiHeader.biSizeImage];
	GetBitmapBits(retBmp,bmpInfo.bmiHeader.biSizeImage,bits);

	int bytePerPixel=4;//Ĭ��32λ
	if(bmpInfo.bmiHeader.biBitCount==24)
	{
		bytePerPixel=3;
	}
	for(DWORD i=0;i<bmpInfo.bmiHeader.biSizeImage;i+=bytePerPixel)
	{
		BYTE r=*(bits+i);
		BYTE g=*(bits+i+1);
		BYTE b=*(bits+i+2);
		*(bits+i)=*(bits+i+1)=*(bits+i+2)=(r+b+g)/3;
	}
	SetBitmapBits(hSourceBmp,bmpInfo.bmiHeader.biSizeImage,bits);
	delete[] bits;
}

void WriteDatatoClipBoard()
{
	HDC hMemDc,hScrDc;
	HBITMAP hBmp,hOldBmp;
	int width,height;
	width=rect.right-rect.left;
	height=rect.bottom-rect.top;

	hScrDc=CreateDC(L"DISPLAY",NULL,NULL,NULL);
	hMemDc=CreateCompatibleDC(hScrDc);
	hBmp=CreateCompatibleBitmap(hScrDc,width,height);

	hOldBmp=(HBITMAP)SelectObject(hMemDc,hBmp);
	BitBlt(hMemDc,0,0,width,height,g_srcMemDc,rect.left,rect.top,SRCCOPY);
	hBmp=(HBITMAP)SelectObject(hMemDc,hOldBmp);
	DeleteDC(hMemDc);
	DeleteDC(hScrDc);
	//���Ƶ�������
	if(OpenClipboard(0))
	{
		EmptyClipboard();
		SetClipboardData(CF_BITMAP,hBmp);
		CloseClipboard();
	}

	DeleteObject(hBmp);
	DeleteObject(hMemDc);
	DeleteObject(hScrDc);

}