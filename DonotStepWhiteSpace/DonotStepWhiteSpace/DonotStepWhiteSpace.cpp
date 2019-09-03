#include <Windows.h>
#include "resource.h"
#include <time.h>
#define  BLOCK	100

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = L"DotnotStepWhiteSpace";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;


	RegisterClass(&wndClass);

	HWND hWnd = CreateWindow(L"DotnotStepWhiteSpace", L"别踩白块 By Fzy v0.1", WS_SYSMENU | WS_CAPTION, 500, 150, BLOCK * 4 + 6, BLOCK * 4 + 30, NULL, NULL, hInstance, 0);

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int bw[4];
	static int t = 0;
	HDC hDC;
	PAINTSTRUCT ps;
	HPEN hPen;
	HBRUSH hBrush;
	POINT point = {0, 0};
	wchar_t szTemp[100];
	RECT rect;
	static int flag;
	static int n = 0;

	switch(uMsg)
	{

	case WM_CREATE:
		srand((unsigned)time(NULL));
		for (int i = 0; i < 4; i++)
		{
			bw[i] = rand() % 4;
		}

		SetTimer(hWnd, 1, 10, NULL);
		break;

	case WM_TIMER:
		t++;
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		


		for (int i = 0; i < 4; i++)
		{
			SetRect(&rect, bw[i] * BLOCK, i * BLOCK, bw[i] * BLOCK + BLOCK, i * BLOCK + BLOCK);
			hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
			SelectObject(hDC, hPen);
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			SelectObject(hDC, hBrush);
			Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
	


		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		SendMessage(hWnd, WM_CHAR, point.x/BLOCK, 0);
		break;

	case WM_CHAR:
		hDC = GetDC(hWnd);
		
		if (flag == -1)
		{
			wsprintf(szTemp, L"游戏结束：你输了！用时：%d.%d秒 格数：%d", t / 100, t - (t / 100) * 100, n);
			MessageBox(NULL, szTemp, L"提示", MB_OK);
			exit(0);
		}
		if (wParam != bw[3])
		{
			flag = -1;
			KillTimer(hWnd, 1);
			wsprintf(szTemp, L"游戏结束：你输了！用时：%d.%d秒 格数：%d", t / 100, t - (t / 100) * 100, n);
			MessageBox(NULL, szTemp, L"提示", MB_OK);
			exit(0);

		}
		for(int i = 3; i >= 1; i--)
		{
			bw[i] = bw[i - 1];
		}
		bw[0] = rand() % 4;
		n++;
		ScrollWindow(hWnd, 0, BLOCK, NULL, NULL);


		SetRect(&rect, bw[0] * BLOCK, 0, bw[0] * BLOCK + BLOCK, BLOCK);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
		SelectObject(hDC, hPen);
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
		DeleteObject(hPen);
		DeleteObject(hBrush);


		ReleaseDC(hWnd, hDC);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY: 
		PostQuitMessage(0); 
		break; 

	default:
		
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}