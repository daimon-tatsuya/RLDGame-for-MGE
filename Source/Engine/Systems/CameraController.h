#pragma once
#include <windows.h>
#include <DirectXMath.h>

class CameraController
{
public:
	CameraController();
	~CameraController();

	void Update(float elapsed_time);

	void DrawDebugGUI();
private:
	// モード
	enum class Mode
	{
		FreeCamera,		// フリーカメラ
		LockonCamera,	// ロックオンカメラ
		MotionCamera,	// モーションカメラ
		DebugCamera,    //デバックカメラ
	};
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	DirectX::XMFLOAT3	new_position = { 0, 0, 0 };
	DirectX::XMFLOAT3	new_target = { 0, 0, 0 };

	float				roll_speed = DirectX::XMConvertToRadians(90);
	float				range = 10.0f;
	float				max_angleX = DirectX::XMConvertToRadians(+45);
	float				min_angleX = DirectX::XMConvertToRadians(-45);
};
