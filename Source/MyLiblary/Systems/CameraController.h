#pragma once
//**********************************************************
//
//	CameraControllerクラス
//
//**********************************************************

<<<<<<< HEAD:Source/Liblary/Systems/CameraController.h
#include "Liblary/Systems/Math.h"
=======
#include "MyLiblary/Systems/Math.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Systems/CameraController.h


 const static float	CAMERA_MOVE_SPEED = 1.0f / 8.0f;

//前方宣言
class AxisAlignedBoundingBox;

//視錐台
struct Frustum final
{
	float									shortest_direction[6]{};			// 原点(0,0,0)からの最短距離
	DirectX::XMFLOAT3			normal[6]{};							// 視錐台の各面の法線
	DirectX::XMFLOAT3			near_vertices_position[4]{};	// Nearの四角形の４頂点の座標
	DirectX::XMFLOAT3			far_vertices_position[4]{};		// Farの四角形の４頂点の座標
	DirectX::XMFLOAT3	        out_line_norm[4];
	bool										collision_state = false; // AABBが視錐台の内側にいるか
	//DirectX::XMFLOAT3                vertex[8]{};						//頂点
};

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
	DirectX::XMFLOAT3	position{};// 位置
	DirectX::XMFLOAT3	target{};// 注視点
	DirectX::XMFLOAT3	angle{};// 各軸の回転角(ラジアン)
	DirectX::XMFLOAT3	new_position{};//次フレームの位置
	DirectX::XMFLOAT3	new_target{};//次フレームの注視点

	float				roll_speed = DirectX::XMConvertToRadians(90);// 回転速度
	float				range = 10.0f;
	float				max_angleX = DirectX::XMConvertToRadians(+45);// 最大回転角
	float				min_angleX = DirectX::XMConvertToRadians(-45);// 最小回転角
	float              camera_height = 50.f;// カメラの高さ

	Frustum			frustum;

	// ImGui
	bool               view_map = false;
public:

private:

public:

	CameraController();
	~CameraController() = default;

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

	/// <summary>
	/// <para>視錐台の計算</para>
	/// <para>仮</para>
	/// </summary>
	void CalculateFrustum();

	bool IntersectFrustum(AxisAlignedBoundingBox* aabb);

	// デバッグ用GUI描画
	void DrawDebugGUI();
};
