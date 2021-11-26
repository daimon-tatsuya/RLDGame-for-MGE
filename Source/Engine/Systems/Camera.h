#pragma once
//**********************************************************
//
//		Cameraクラス
//
//**********************************************************

#include "Engine/Systems/Math.h"

/// <summary>
/// カメラ情報を持つクラス
/// </summary>
class Camera
{
private:
	DirectX::XMFLOAT4X4 view =//ビュー行列
	{
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};
	DirectX::XMFLOAT4X4 projection =//投影行列
	{
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	DirectX::XMFLOAT3	eye = { 0,0,0 };//カメラの位置
	DirectX::XMFLOAT3	focus = { 0,0,0 };//カメラの方向
	DirectX::XMFLOAT3	up = { 0,0,0 };//カメラの上方向、通常は<0.0f、1.0f、0.0f>
	DirectX::XMFLOAT3	front = { 0,0,0 };//カメラの前方方向
	DirectX::XMFLOAT3	right = { 0,0,0 };//カメラの右方向
public:

private:

	Camera() {};
	~Camera() {};

public:

	// 唯一のインスタンス
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	/// <summary>
	/// 指定方向を向く
	/// </summary>
	/// <param name="eye">カメラの位置</param>
	/// <param name="focus">注視点</param>
	/// <param name="up">カメラの上方</param>
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

	/// <summary>
	/// パースペクティブの設定
	/// </summary>
	/// <param name="fovY">ラジアン角の視野角</param>
	/// <param name="aspect">アスペクト比</param>
	/// <param name="nearZ">近くのクリッピング平面までの距離(ゼロより大きく)</param>
	/// <param name="farZ">遠いクリッピング平面までの距離(ゼロより大きく)</param>
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	/// <summary>
	/// オルソの設定
	/// </summary>
	/// <param name="fovY">ラジアン角の視野角</param>
	/// <param name="aspect">アスペクト比</param>
	/// <param name="nearZ">近くのクリッピング平面までの距離(ゼロより大きく)</param>
	/// <param name="farZ">遠いクリッピング平面までの距離(ゼロより大きく)</param>
	void SetOrthFov(float fovY, float aspect, float nearZ, float farZ);

	//------------------------------------------------
	//
	// Getter
	//
	//------------------------------------------------

	// ビュー行列取得
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	// プロジェクション行列取得
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

	// 視点取得
	const DirectX::XMFLOAT3& GetEye() const { return eye; }

	// 注視点取得
	const DirectX::XMFLOAT3& GetFocus() const { return focus; }

	// 上方向取得
	const DirectX::XMFLOAT3& GetUp() const { return up; }

	// 前方向取得
	const DirectX::XMFLOAT3& GetFront() const { return front; }

	// 右方向取得
	const DirectX::XMFLOAT3& GetRight() const { return right; }

};
