#pragma once
//**********************************************************
//
//	CameraControllerクラス
//
//**********************************************************

#include "Engine/Systems/Math.h"


/// <summary>
/// カメラの挙動を行うクラス
/// </summary>
class CameraController
{
private:
	// モード
	enum class Mode
	{
		FreeCamera,		// フリーカメラ
		LockOnCamera,	// ロックオンカメラ
		MotionCamera,	// モーションカメラ
		DebugCamera,    // デバックカメラ
	};
	DirectX::XMFLOAT3	position = { 0, 0, 0 };// 位置
	DirectX::XMFLOAT3	target = { 0, 0, 0 };// 注視点
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };// 各軸の回転角(ラジアン)
	DirectX::XMFLOAT3	new_position = { 0, 0, 0 };//
	DirectX::XMFLOAT3	new_target = { 0, 0, 0 };

	float				roll_speed = DirectX::XMConvertToRadians(90);// 回転速度
	float				range = 10.0f;
	float				max_angleX = DirectX::XMConvertToRadians(+45);// 最大回転角
	float				min_angleX = DirectX::XMConvertToRadians(-45);// 最小回転角
	float              camera_height = 50.f;// カメラの高さ
	// ImGui
	bool               ViewMap = false;
public:

private:

public:

	CameraController();
	~CameraController() {};

	/// <summary>
	/// アクションゲーム用更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void ActionGameCameraUpdate(float elapsed_time);

	/// <summary>
	/// 追尾カメラ更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void FollowCameraUpdate(float elapsed_time);

	// デバッグ用GUI描画
	void DrawDebugGUI();

};
