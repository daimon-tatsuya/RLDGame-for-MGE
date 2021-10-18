#pragma once
#include "Engine/Systems/misc.h"
//デバッグ時のみ有効
#if defined( DEBUG ) || defined( _DEBUG )

#define USE_IMGUI

#endif // !_DEBUG

//#ifdef USE_IMGUI
//
//#include <Windows.h>
//#include "../imgui/imgui.h"
//#include "../imgui/imgui_internal.h"
//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern ImWchar glyphRangesJapanese[];
//#endif //USE_IMGUI