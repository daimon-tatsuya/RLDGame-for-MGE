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
class Camera final
{
private:
	DirectX::XMFLOAT4X4 view{};			// ビュー行列
	DirectX::XMFLOAT4X4 projection{};	// 投影行列

	DirectX::XMFLOAT3	eye{ 0,1,10 };		// カメラの位置
	DirectX::XMFLOAT3	focus{ 0,0,0 };	// カメラの方向
	DirectX::XMFLOAT3	up{ 0,1,0 };		// カメラの上方向、通常は<0.0f、1.0f、0.0f>
	DirectX::XMFLOAT3	front{};		// カメラの前方方向
	DirectX::XMFLOAT3	right{};		// カメラの右方向

	float width{};
	float height{};
	float aspect{};
	float fovY{ DirectX::XM_PI / 6.0f };
	float nearZ{ 0.1f };
	float farZ{ 1000.f };

	bool  OrthoMode = false;// 平行投影カメラモード

private:

	Camera();
	~Camera() = default;

public:

	// 唯一のインスタンス
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	// カメラをアクティブにする
	void ActivateCamera();

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

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
	/// オルソ(平衡投影)の設定
	/// </summary>
	/// <param name="width">幅</param>
	/// <param name="height">高さ</param>
	/// <param name="nearZ">近くのクリッピング平面までの距離(ゼロより大きく)</param>
	/// <param name="farZ">遠いクリッピング平面までの距離(ゼロより大きく)</param>
	void SetOrthoFov(float width, float height, float nearZ, float farZ);

	// ビュー行列の取得
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	// プロジェクション行列の取得
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

	// 視点の取得
	const DirectX::XMFLOAT3& GetEye() const { return eye; }

	// 注視点の取得
	const DirectX::XMFLOAT3& GetFocus() const { return focus; }

	// 上方向の取得
	const DirectX::XMFLOAT3& GetUp() const { return up; }

	// 前方向の取得
	const DirectX::XMFLOAT3& GetFront() const { return front; }

	// 右方向の取得
	const DirectX::XMFLOAT3& GetRight() const { return right; }

	//幅の設定
	void SetWidth(const float width) { this->width = width; }

	//幅の取得
	float GetWidth() const { return  width; }

	//高さの設定
	void SetHeight(const float height) { this->height = height; }

	//高さの取得
	float GetHeight() const { return  height; }

	//アスペクト比の設定
	void SetAspect(const float width, const float height) { SetAspect(width / height); }
	void SetAspect(const float aspect) { this->aspect = aspect; }

	//アスペクト比の取得
	float GetAspect() const { return  aspect; }

	//視野の設定
	void SetFov(const float fov) { this->fovY = fov; }

	//視野の取得
	float GetFov() const { return  fovY; }

	//nearの設定
	void SetNear(const float nearZ) { this->nearZ = nearZ; }

	//nearの取得
	float GetNear() const { return  nearZ; }

	//farの設定
	void SetFar(const float farZ) { this->farZ = farZ; }

	//farの取得
	float GetFar() const { return  farZ; }

	// 平行投影カメラモード取得
	bool GetOrthMode() { return OrthoMode; }

	// 平行投影カメラモード設定
	void SetOrthMode(bool mode) { OrthoMode = mode; }
};
