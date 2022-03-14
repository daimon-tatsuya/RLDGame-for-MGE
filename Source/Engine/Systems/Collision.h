#pragma once
//**********************************************************
//
//		Collision�N���X
//
//**********************************************************

#include "Engine/Systems/Math.h"

//�O���錾
class Model;

// �q�b�g����
struct HitResult
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };	// ���C�ƃ|���S���̌�_
	DirectX::XMFLOAT3		normal = { 0, 0, 0 };		// �Փ˂����|���S���̖@���x�N�g��
	DirectX::XMFLOAT3		rotation = { 0, 0, 0 };	// ��]��
	float								distance = 0.0f; 			// ���C�̎n�_�����_�܂ł̋���
	int									material_index = -1; 	// �Փ˂����|���S���̃}�e���A���ԍ�
};

///<summary>
///AABB�̃N���X
///</summary>
class AxisAlignedBoundingBox
{
private:
public:
	//�I�u�W�F�N�g�p
	DirectX::XMFLOAT3 position_min{};			// �ő�l
	DirectX::XMFLOAT3 position_max{};			// �ŏ��l
	DirectX::XMFLOAT3 object_old_position{}; //�O�̍��W�l
	//�͈͗p
	DirectX::XMFLOAT3 center{};
	DirectX::XMFLOAT3 radius{};	// ���a

private:
public:
	AxisAlignedBoundingBox() = default;
	~AxisAlignedBoundingBox() = default;

	AxisAlignedBoundingBox(const DirectX::XMFLOAT3 min_value, const DirectX::XMFLOAT3 max_value, DirectX::XMFLOAT3 object_pos);
};

/// <summary>
/// �����蔻��N���X
/// </summary>
class Collision
{
public:

	/// <summary>
	/// ���Ƌ��̌�������
	/// </summary>
	/// <param name="positionA">SphereA�̈ʒu</param>
	/// <param name="radiusA">SphereA�̔��a</param>
	/// <param name="positionB">SphereB�̈ʒu</param>
	/// <param name="radiusB">SphereB�̔��a</param>
	/// <param name="outPositionA">������A�̈ʒu</param>
	/// <param name="outPositionB">������B�̈ʒu</param>
	/// <returns>�������Ă���Ȃ�true</returns>
	static bool IntersectSphereToSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionA,
		DirectX::XMFLOAT3& outPositionB
	);

	/// <summary>
	/// ���C�ƃ��f���̌�������
	/// </summary>
	/// <param name="start">���C�̎n�_</param>
	/// <param name="end">���C�̏I�_</param>
	/// <param name="model">���肷��model</param>
	/// <param name="result">�q�b�g����</param>
	/// <returns>�������Ă���Ȃ�true</returns>
	static bool IntersectRayToModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

	// �J�v�Z���ƃJ�v�Z���̌�������

	// �J�v�Z���Ƌ��̌�������

	// AABB��AABB�̌�������
	static bool IntersectAxisAlignedBoundingBox(const AxisAlignedBoundingBox& box1, const AxisAlignedBoundingBox& box2);
};
