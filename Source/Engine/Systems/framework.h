#pragma once

//**********************************************************
//
//		Frameworkクラス
//
//**********************************************************

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <memory>

#include <d3d11.h>
#include <wrl.h>

#include "Engine/Systems/HighResolutionTimer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"



class Framework
{

private:

	const HWND				hwnd;
	HighResolutionTimer	timer;//フレーム管理クラス
	Graphics					graphics;//描画周り管理クラス
	Input							input;//入力管理クラス

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
	void Render(float elapsed_time/*最後のフレームからの経過秒数*/);

	//1 秒あたりの平均フレーム数と、1 つのフレームを描画するのにかかる平均時間の計算
	void CalculateFrameStats();

public:

	Framework(HWND hwnd);

	~Framework() {}

	// WIN32メッセージハンドラー
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	int Run();

};
