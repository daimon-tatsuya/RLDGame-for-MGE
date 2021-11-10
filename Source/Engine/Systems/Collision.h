#pragma once

#include "Engine/Systems/Math.h"

#include "Engine/Objects/Model.h"


//ToDo Collision �R�����g

//�q�b�g����
struct HitResult
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };	// ���C�ƃ|���S���̌�_
	DirectX::XMFLOAT3		normal = { 0, 0, 0 };	// �Փ˂����|���S���̖@���x�N�g��
	DirectX::XMFLOAT3		rotation = { 0, 0, 0 };	// ��]��
	float								distance = 0.0f; 		// ���C�̎n�_�����_�܂ł̋���
	int									material_index = -1; 	// �Փ˂����|���S���̃}�e���A���ԍ�
};

class Collision
{
public:
	// ���Ƌ��̌�������
	static bool IntersectSphereToSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionA,
		DirectX::XMFLOAT3& outPositionB
	);

	// ���C�ƃ��f���̌�������
	static bool IntersectRayToModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

	//Todo �J�v�Z���ƃJ�v�Z���̌�������


	//Todo �J�v�Z���Ƌ��̌�������


	//Todo AABB

};
