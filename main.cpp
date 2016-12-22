#include "stdafx.h"
#include "ForDZ2.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MyDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FORDZ2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FORDZ2));

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FORDZ2));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FORDZ2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; 

	hWnd = CreateWindow(szWindowClass, "Shifr-Deshifr", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

unsigned sdvigleft(unsigned a, int n, int l)
{
	unsigned maska = 0;
	for (int i = 0; i < l; i++)
	{
		maska <<= 3;
		maska += 3;
	}

	maska <<= (n - l);
	unsigned b = a & maska;
	b >>= (n - l);
	a <<= l;
	return a | b;
}
unsigned sdvigright(unsigned a, int n, int l)
{

	unsigned maska = 0;
	for (unsigned int i = 0; i < l; ++i)
	{
		maska <<= 3;
		maska += 3;
	}
	unsigned b = a & maska;
	b <<= (n - l);
	a >>= l;
	return a | b;
}

void shifrDeshifr(char *in, char *out, int n, int key, int reg)
{
	srand(key);
	unsigned short a, b, gamma; 
	int l; 
	for (int i = 0; i < n; i += 2)
	{
		if (n % 2 != 0 && i == n - 1) 
		{
			a = 0;
			l = 1;
		}
		else l = 2;
		memcpy(&a, in + i, l); 
		if (reg == 2) 
			a = sdvigright(a, l * 8, 3); 
		gamma = rand();
		b = gamma ^ a;
		if (reg == 1) b = sdvigleft(b, l * 8, 3); 
		memcpy(out + i, &b, l); 

	}
}

char Str[512];
int len = 0;
int key; 

OPENFILENAME ofn; 
TCHAR NameFile[256] = "yourtext.shf"; 

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		unsigned int KEY;
	case WM_KEYDOWN:
		KEY = wParam;
		if ((KEY == VK_BACK) && (len > 0))
		{
			Str[len - 1] = 0;
			len--;
			hdc = BeginPaint(hWnd, &ps);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		if (KEY == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		break;
	case WM_CHAR: 
		if ((wParam != VK_BACK) && (wParam != VK_ESCAPE)) {
			Str[len] = wParam;
			len++; 
			Str[len] = 0; 
			hdc = GetDC(hWnd); 
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, 40, 40, Str, len); 
			ReleaseDC(hWnd, hdc); 
		}
		break;
	case WM_COMMAND: 
		wmId = LOWORD(wParam); 
		wmEvent = HIWORD(wParam);
		switch (wmId) 
		{
		case VK_BACK:
		case IDM_ABOUT: 
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT: 
			DestroyWindow(hWnd);
			break;
		case ID_32771: 
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = NameFile; 
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 255;
			ofn.lpstrFilter = "Шифрованные файлы (*.shf)\0*.shf\0Все файлы\0*.*\0";
			ofn.nFilterIndex = 0;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = "shf";

			if (GetOpenFileName(&ofn)) 
			{
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDialog) == IDOK)
				{
					FILE *pF1;
					char Shifr[512]; 
					pF1 = fopen(NameFile, "rb");
					if (pF1 == 0)
					{
						MessageBox(hWnd, "Ошибка открытия файла. Возможно файл не существует", "Сообщение об ошибке", MB_OK | MB_ICONHAND);
						return TRUE;
					}
					fread(&len, sizeof(int), 1, pF1);
					fread(Shifr, sizeof(char), len, pF1);
					fclose(pF1);
					shifrDeshifr(Shifr, Str, len, key, 2); 
					Str[len] = 0; 
					InvalidateRect(hWnd, 0, TRUE);
				}
				else MessageBox(hWnd, "Данные не прочитаны. Ключ не введен!!", "Сообщение", MB_OK | MB_ICONHAND);
			}


			break;
		case ID_32772:
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd; 
			ofn.lpstrFilter = "Шифрованные файлы (*.shf)\0*.shf\0Все файлы\0*.*\0";
			ofn.lpstrFile = NameFile;
			ofn.nMaxFile = 255;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
			ofn.lpstrTitle = NULL;
			ofn.lpstrDefExt = "shf"; 
			if (GetSaveFileName(&ofn)) 
			{
				if (DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, MyDialog) == IDOK)
				{
					char Shifr[512];
					shifrDeshifr(Str, Shifr, len, key, 1);

					FILE *pF1; 
					pF1 = fopen(NameFile, "wb"); 
					if (pF1 == 0)
					{
						MessageBox(hWnd, "Ошибка открытия файла.", "Сообщение об ошибке", MB_OK | MB_ICONHAND);
						return TRUE;
					}
					fwrite(&len, sizeof(int), 1, pF1);
					fwrite(Shifr, sizeof(char), len, pF1);
					fclose(pF1);
					MessageBox(hWnd, "Данные успешно сохранены.", "Сообщение", MB_OK | MB_ICONINFORMATION);
				}
				else MessageBox(hWnd, "Данные не сохранены. Ключ не введен!!", "Сообщение", MB_OK | MB_ICONHAND);
			}

			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, 40, 40, Str, len);
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

INT_PTR CALLBACK MyDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK)
			{
				char Str[32];
				SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, 31, (LPARAM)Str); 
				if (sscanf(Str, "%d", &key) < 1) 
				{
					MessageBox(hDlg, "Неверный формат ключа. Ключ должен быть целым числом", "Сообщение о неверном формате ключа", MB_OK | MB_ICONHAND);
					return (INT_PTR)TRUE; 

				}

			}
			EndDialog(hDlg, LOWORD(wParam)); 
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
