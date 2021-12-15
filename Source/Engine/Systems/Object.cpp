//**********************************************************
//
//		Object�N���X
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/Systems/Math.h"
#include "Engine/Objects/Model.h"

void Object::UpdateTransform()
{
	// �X�P�[���s����쐬
	const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
#if 0
	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else

	const DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	const DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	const DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	const DirectX::XMMATRIX R = Y * X * Z;

#endif
	// �ʒu�s����쐬
	const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	const DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
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