//**********************************************************
//
//		Objectクラス
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/Systems/Math.h"
#include "Engine/Objects/Model.h"

void Object::UpdateTransform()
{
	// スケール行列を作成
	const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
#if 0
	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else

	const DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	const DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	const DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	const DirectX::XMMATRIX R = Y * X * Z;

#endif
	// 位置行列を作成
	const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	const DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

float Object::NormalizeAnyAngle(float radian)
{
	return Math::NormalizeRadianAngle(radian);
}

void Object::NormalizeAngle()
{
	angle.x = NormalizeAnyAngle(angle.x);
	angle.y = NormalizeAnyAngle(angle.y);
	angle.z = NormalizeAnyAngle(angle.z);
}
void	Object::SetModel(const char* pass)
{
	model = std::make_shared<Model>(pass);
}