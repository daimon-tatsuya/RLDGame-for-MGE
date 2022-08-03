#include <Windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include "Liblary/Systems/Framework.h"
#include"CharacterManager.h"

//ここは授業を参考にしました
const static LONG SCREEN_WIDTH = 1920;
const static LONG SCREEN_HEIGHT = 1080;

LRESULT CALLBACK fnWndProc(HWND handle_window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Framework* f = reinterpret_cast<Framework*>(GetWindowLongPtr(handle_window, GWLP_USERDATA));
	return f ? f->HandleMessage(handle_window, msg, wparam, lparam) : DefWindowProc(handle_window, msg, wparam, lparam);
}

INT WINAPI	wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)//Debugならメモリーリークを検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = fnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = _T("Alphonse");
	wcex.hIconSm = nullptr;
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	const HWND handle_window = CreateWindow(_T("Alphonse"), _T(""),
	                                        WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
	                                        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL,
	                                        NULL, instance, NULL);
	ShowWindow(handle_window, cmd_show);


	Framework f(handle_window);
	SetWindowLongPtr(handle_window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));
	return f.Run();
}