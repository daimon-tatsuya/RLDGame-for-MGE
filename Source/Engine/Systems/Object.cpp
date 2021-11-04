#include "Engine/Systems/Object.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Math.h"


void Object::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
#if 0
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
#endif
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

float Object::NormalizeAnyAngle(float radian)
{
	return Math::NormalizeRadianAngle(radian);
}

void Object::NormalizeAngle()
{
	angle.x=NormalizeAnyAngle(angle.x);
	angle.y=NormalizeAnyAngle(angle.y);
	angle.z=NormalizeAnyAngle(angle.z);
}
