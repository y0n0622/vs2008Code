// PinTu.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PinTu.h"
#include <time.h>

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������


#define BLOCKCOUNTOFW    3   //ͼƬ���ֳ� BLOCKCOUNTOFW ��
#define BLOCKCOUNTOFH    3   //ͼƬ���ֳ� BLOCKCOUNTOFH ��

//���ָ��ľ�������, ˳����
RECT gRect[BLOCKCOUNTOFW * BLOCKCOUNTOFH]; 

//ͼƬ���
HBITMAP gBitMapHandle;

//ͼƬ�Ŀ�͸�
int gBitMapWidth;
int gBitMapHeight;

//ÿ��С����Ŀ�͸�
int gBlockWidth;
int gBlockHeight;

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//��������
void InitializeGame(wchar_t* bmpPath);
void RandomRect();

//��ȡ��ɫ������߷����������
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

	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	//��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PINTU, szWindowClass, MAX_LOADSTRING);


	MyRegisterClass(hInstance);   //ע�ᴰ����, ���ڵ���ʽ

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))   //��������
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PINTU));

	// ����Ϣѭ��:  ���е��ⲿ��Ϣ������������̰��£�  
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


ATOM MyRegisterClass(HINSTANCE hInstance)   //������������˴��ڵ���ʽ��
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;   //ע��ص�����������ϵͳ���ҵ���Ϣ���������������
	//������Ϣ����(�������꣩��ʱ���鷳������һ��
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


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)     //�����ڴ�������
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);  //�����ڽ���

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);   //��ʾ����
	UpdateWindow(hWnd);

	return TRUE;
}


//�ص������ɳ���Ա���ã����oϵͳ���ã� ϵͳ���������ⲿ�¼�������ʱ��
//��Ϣ����������Ϣ������꣬���̣����硣������
//���е���Ϣ���������ú���
//�ǳ���Ҫ�ĺ���
//��Ϣ������

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)  //�ж�ʲô���͵���Ϣ, ���ݲ�ͬ��Ϣ������ͬ�Ĵ���
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			RandomRect();
			InvalidateRect(hWnd, 0, 1);
			break;

			//�ڳ����ʼ����ʱ�򣬽�ͼƬ����

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
			int index = GetWhiteBlockIndex();  //��ɫ������

			switch (wParam)   //����ͬ�İ���
			{
			case VK_DOWN:   //����������¼�ͷ, virtual key
				{
					int dBlock = GetDownBlockIndex(index);  //�ɰ�ɫ���������ţ��õ���߷����������

					if (dBlock != -1)   //���Խ���
					{
						SwitchBlock(index, dBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}
				break;
			case VK_UP:   //����������¼�ͷ, virtual key
				{
					int uBlock = GetUpBlockIndex(index);  //�ɰ�ɫ���������ţ��õ���߷����������

					if (uBlock != -1)   //���Խ���
					{
						SwitchBlock(index, uBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}
				break;
			case VK_LEFT:   //����������¼�ͷ, virtual key
				{
					int lBlock = GetLeftBlockIndex(index);  //�ɰ�ɫ���������ţ��õ���߷����������

					if (lBlock != -1)   //���Խ���
					{
						SwitchBlock(index, lBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}

				break;
			case VK_RIGHT:   //����������¼�ͷ, virtual key
				{
					int rBlock = GetRightBlockIndex(index);  //�ɰ�ɫ���������ţ��õ���߷����������

					if (rBlock != -1)   //���Խ���
					{
						SwitchBlock(index, rBlock);
						InvalidateRect(hWnd, 0, 1);
					}
				}
				break;
			}
		}
		break;

	case WM_CREATE:   //���ڴ�����ʱ�򣬼���ͼƬ
		{
			InitializeGame(L"E:/2.bmp");
		}
	case WM_PAINT:   //�����ƴ��ڵ�ʱ�򣬼��ص�ͼƬ
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		{
			HDC memDc = CreateCompatibleDC(hdc);  //����һ���ڴ�DC�� �û���ͼƬ
			SelectObject(memDc, gBitMapHandle);  //����Ҫ���Ƶ�ͼƬѡ��dc

			for (int i = 0; i < BLOCKCOUNTOFW; i++)
			{
				for (int j = 0; j < BLOCKCOUNTOFH; j++)
				{
					if (gRect[i * BLOCKCOUNTOFH + j].left == -1 && gRect[i * BLOCKCOUNTOFH + j].top == -1)
						continue;

					BitBlt(hdc, gBlockWidth * j, gBlockHeight * i, gBlockWidth, gBlockHeight,
						memDc, gRect[i * BLOCKCOUNTOFH + j].left, gRect[i * BLOCKCOUNTOFH + j].top, SRCCOPY);  //��ͼ����
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

//�Զ��庯��,���ڼ��㱻�ָ�ͼƬ�ľ�������
//bmpWidth  ͼƬ�Ŀ�
//bmpHeigth ͼƬ�ĸ�
//blocksW ͼƬ�ֳɼ���
//blocksH ͼƬ�ֳɼ���

void InitializeBlockRect(int bmpWidth, int bmpHeigth, int blocksW, int blocksH)
{
	//����ÿ������Ŀ�͸�
	int width = bmpWidth / blocksH;
	int heigth = bmpHeigth / blocksW;

	gBlockWidth = width;
	gBlockHeight = heigth;

	for (int i = 0; i < blocksW; i++)
	{
		for (int j = 0; j < blocksH; j++)
		{
			//��������
			//gRect[i * blocksW + j]; //�ǲ����ܱ�ʾÿһ����������
			gRect[i * blocksH + j].left = width * j;
			gRect[i * blocksH + j].top = heigth * i;
			gRect[i * blocksH + j].right = width * (j + 1);
			gRect[i * blocksH + j].bottom = heigth * (i + 1);
		}
	}

	//ֻ�����ڱ�����һ�������
	gRect[blocksH * blocksW - 1].left = -1;
	gRect[blocksH * blocksW - 1].top = -1;
}

//����

//����ͼƬ, ��ʼ����Ϸ
//bmpPath : ͼƬ��·��
void InitializeGame(wchar_t* bmpPath)
{
	gBitMapHandle = (HBITMAP)LoadImage(0, bmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//ͼƬ�Ŀ�͸�
	BITMAP bmp;
	GetObject(gBitMapHandle, sizeof(BITMAP), &bmp);   //�õ���ͼƬ����

	gBitMapWidth = bmp.bmWidth;
	gBitMapHeight = bmp.bmHeight;

	//��ͼƬ�ָ�
	InitializeBlockRect(gBitMapWidth, gBitMapHeight, BLOCKCOUNTOFW, BLOCKCOUNTOFH);
}


//��������ľ��������������
void RandomRect()
{
	RECT rc;
	for (int i = 0; i < BLOCKCOUNTOFW * BLOCKCOUNTOFH; i++)
	{
		int rd = rand() % (BLOCKCOUNTOFW * BLOCKCOUNTOFH - 1);     //��ȡһ�����
		rc = gRect[i];
		gRect[i] = gRect[rd];
		gRect[rd] = rc;
	}
}

//�õ���ɫ������������е�������
int GetWhiteBlockIndex()
{
	for (int i = 0; i < BLOCKCOUNTOFW * BLOCKCOUNTOFH; i++)
	{
		if (gRect[i].left == -1 && gRect[i].top == -1)  //�����ǰ�ɫ�����ʱ��
		{
			return i;
		}
	}

	return 0;
}

//�õ���ɫ������ߵķ�������
//index ��ɫ�����������
int GetLeftBlockIndex(int index)
{
	if (index % BLOCKCOUNTOFH == 0)  //�жϰ�ɫ�����ǲ����������
		return -1;

	return index - 1;
}

int GetUpBlockIndex(int index)
{
	if (index - BLOCKCOUNTOFH < 0)  //�жϰ�ɫ�����ǲ����������
		return -1;

	return index - BLOCKCOUNTOFH;
}

int GetRightBlockIndex(int index)
{
	if ((index + 1) % BLOCKCOUNTOFH == 0)  //����������Ŵ�0��ʼ
		return -1;

	return index + 1;
}

int GetDownBlockIndex(int index)
{
	if (index + BLOCKCOUNTOFH >=  BLOCKCOUNTOFH * BLOCKCOUNTOFW)  //�жϰ�ɫ�����ǲ����������
		return -1;

	return index + BLOCKCOUNTOFH;
}

//����2��С����
void SwitchBlock(int r, int l)
{
	RECT rc = gRect[r];
	gRect[r] = gRect[l];
	gRect[l] = rc;
}