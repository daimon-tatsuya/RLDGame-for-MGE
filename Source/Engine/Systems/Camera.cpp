//**********************************************************
//
//	!	Cameraクラス
//
//**********************************************************

#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Graphics.h"

Camera::Camera()
{
	const Graphics& graphics = Graphics::Instance();
	width = graphics.GetScreenWidth();
	height = graphics.GetScreenHeight();
	aspect = width / height;
}

void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
	// 視点、注視点、上方向からビュー行列を作成
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	// ビューを逆行列化し、ワールド行列に戻す
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world{};
	DirectX::XMStoreFloat4x4(&world, World);

	// カメラの方向を取り出す
	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;

	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	// 視点、注視点を保存
	this->eye = eye;
	this->focus = focus;
}

void Camera::SetPerspectiveFov(const float fovY, const float aspect, const float nearZ, const float farZ)
{
	this->fovY = fovY;
	this->aspect = aspect;
	this->nearZ = nearZ;
	this->farZ = farZ;
	OrthoMode = false;
}

void Camera::SetOrthoFov(const float width, const float height, const float nearZ, const float farZ)
{
	this->width = width;
	this->height = height;
	this->nearZ = nearZ;
	this->farZ = farZ;
	OrthoMode = true;
}

void Camera::ActivateCamera()
{
	const DirectX::XMVECTOR eye_position = DirectX::XMVectorSet(eye.x, eye.y, eye.z, 0);
	const DirectX::XMVECTOR focus_position = DirectX::XMVectorSet(focus.x, focus.y, focus.z, 0);
	const DirectX::XMVECTOR up_direction = DirectX::XMVectorSet(up.x, up.y, up.z, 0);

	const DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(eye_position, focus_position, up_direction);
	DirectX::XMStoreFloat4x4(&view, View);

	DirectX::XMMATRIX Projection{};
	if (OrthoMode == false)
	{
		//画角、画面比率、クリップ距離からプロジェクション行列を作成
		Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
		XMStoreFloat4x4(&projection, Projection);
	}
	else
	{
		// 画面幅高さ、クリップ距離からプロジェクション行列を作成
		Projection = DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ);
		XMStoreFloat4x4(&projection, Projection);
	}
}