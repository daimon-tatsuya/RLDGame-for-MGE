//**********************************************************
//
//		Inputクラス
//
//**********************************************************
#include <d3d11.h>
#include <memory>
#include "Liblary/Systems/Input.h"

Input* Input::instance = nullptr;

// コンストラクタ
Input::Input(HWND hWnd)
	: mouse(hWnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

// 更新処理
void Input::Update()
{
	game_pad.Update();
	mouse.Update();
}