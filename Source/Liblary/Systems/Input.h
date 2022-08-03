#pragma once
//**********************************************************
//
//		Inputクラス
//
//**********************************************************

#include "Liblary/Systems/GamePad.h"
#include "Liblary/Systems/Mouse.h"

/// <summary>
/// 入力管理クラス
/// </summary>
class Input final
{
private:
	static Input* instance;
	GamePad	   game_pad;
	Mouse		   mouse;

public:

private:

public:
	Input(HWND hWnd);
	~Input() = default;

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>Inputクラスの唯一のインスタンス</returns>
	static Input& Instance() { return *instance; }

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームパッド取得
	/// </summary>
	/// <returns>ゲームパッドの入力を持つクラスのインスタンス</returns>
	GamePad& GetGamePad() { return game_pad; }

	/// <summary>
	///  マウス取得
	/// </summary>
	/// <returns>マウスの入力を持つクラスのインスタンス</returns>
	Mouse& GetMouse() { return mouse; }
};
