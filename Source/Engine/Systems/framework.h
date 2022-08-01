#pragma once

//**********************************************************
//
//		Frameworkクラス
//
//**********************************************************

#include <Windows.h>
#include <tchar.h>

#include <d3d11.h>
#include <wrl.h>

#include "Engine/Systems/HighResolutionTimer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"

class Framework final
{
private:

	const HWND				handle_window;
	HighResolutionTimer	timer;	// フレーム管理クラス
	Graphics					graphics;// 描画周り管理クラス
	Input							input;		// 入力管理クラス

public:

private:
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void Update(float elapsed_time/*最後のフレームからの経過秒数*/);

	/// <summary>
	///  描画処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void Render(float elapsed_time/*最後のフレームからの経過秒数*/) const;

	// 1 秒あたりの平均フレーム数と、1 つのフレームを描画するのにかかる平均時間の計算
	void CalculateFrameStates() const;

public:

	Framework(HWND handle_window);

	~Framework() = default;

	//Q WIN32メッセージハンドラー
	LRESULT CALLBACK HandleMessage(HWND handle_window, UINT msg, WPARAM wparam, LPARAM lparam);

	int Run();
};
