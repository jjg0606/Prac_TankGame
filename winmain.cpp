#include "WndBase.h"
#include "ProgramCore.h"
#include "EnumMouse.h"

HINSTANCE g_hinst;

#pragma comment(lib, "msimg32.lib")

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	// window handle
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hinst = hInstance;
	ProgramCore::instance.setGinst(hInstance);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int mouseXpos;
	static int mouseYpos;
	ProgramCore::instance.SetHWND(hWnd);
	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		mouseXpos = LOWORD(lParam);
		mouseYpos = HIWORD(lParam);
		ProgramCore::instance.SetMouseClick(mouseXpos, mouseYpos, MOUSE_LEFT_BTN);
		return 0;
	case WM_RBUTTONDOWN:
		mouseXpos = LOWORD(lParam);
		mouseYpos = HIWORD(lParam);
		ProgramCore::instance.SetMouseClick(mouseXpos, mouseYpos, MOUSE_RIGHT_BTN);
		return 0;
	case WM_MOUSEMOVE:
		mouseXpos = LOWORD(lParam);
		mouseYpos = HIWORD(lParam);
		ProgramCore::instance.SetMouseMove(mouseXpos, mouseYpos);
		return 0;
	case WM_KEYDOWN:
		ProgramCore::instance.SetKeyDown(wParam);
		return 0;
	case WM_KEYUP:
		ProgramCore::instance.SetKeyUp(wParam);
		return 0;
	case WM_CHAR:
		ProgramCore::instance.SetInput(wParam);
		return 0;
	case WM_TIMER:
		ProgramCore::instance.Update();
		return 0;
	case WM_CREATE:
		ProgramCore::instance.SetHWND(hWnd);
		ProgramCore::instance.Init();
		ProgramCore::instance.Main();
		return 0;
	case WM_DESTROY:
		ProgramCore::instance.Release();
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		ProgramCore::instance.DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		ProgramCore::instance.SetMenuMessage(wParam);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}