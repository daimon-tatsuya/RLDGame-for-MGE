#pragma once
//**********************************************************
//
//	CameraController�N���X
//
//**********************************************************

<<<<<<< HEAD:Source/Liblary/Systems/CameraController.h
#include "Liblary/Systems/Math.h"
=======
#include "MyLiblary/Systems/Math.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Systems/CameraController.h


 const static float	CAMERA_MOVE_SPEED = 1.0f / 8.0f;

//�O���錾
class AxisAlignedBoundingBox;

//������
struct Frustum final
{
	float									shortest_direction[6]{};			// ���_(0,0,0)����̍ŒZ����
	DirectX::XMFLOAT3			normal[6]{};							// ������̊e�ʂ̖@��
	DirectX::XMFLOAT3			near_vertices_position[4]{};	// Near�̎l�p�`�̂S���_�̍��W
	DirectX::XMFLOAT3			far_vertices_position[4]{};		// Far�̎l�p�`�̂S���_�̍��W
	DirectX::XMFLOAT3	        out_line_norm[4];
	bool										collision_state = false; // AABB��������̓����ɂ��邩
	//DirectX::XMFLOAT3                vertex[8]{};						//���_
};

/// <summary>
/// �J�����̋������s���N���X
/// </summary>
class CameraController
{
private:
	// ���[�h
	enum class Mode
	{
		FreeCamera,		// �t���[�J����
		LockOnCamera,	// ���b�N�I���J����
		MotionCamera,	// ���[�V�����J����
		DebugCamera,    // �f�o�b�N�J����
	};
	DirectX::XMFLOAT3	position{};// �ʒu
	DirectX::XMFLOAT3	target{};// �����_
	DirectX::XMFLOAT3	angle{};// �e���̉�]�p(���W�A��)
	DirectX::XMFLOAT3	new_position{};//���t���[���̈ʒu
	DirectX::XMFLOAT3	new_target{};//���t���[���̒����_

	float				roll_speed = DirectX::XMConvertToRadians(90);// ��]���x
	float				range = 10.0f;
	float				max_angleX = DirectX::XMConvertToRadians(+45);// �ő��]�p
	float				min_angleX = DirectX::XMConvertToRadians(-45);// �ŏ���]�p
	float              camera_height = 50.f;// �J�����̍���

	Frustum			frustum;

	// ImGui
	bool               view_map = false;
public:

private:

public:

	CameraController();
	~CameraController() = default;

	/// <summary>
	/// �A�N�V�����Q�[���p�X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void ActionGameCameraUpdate(float elapsed_time);

	/// <summary>
	/// �ǔ��J�����X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void FollowCameraUpdate(float elapsed_time);

	/// <summary>
	/// <para>������̌v�Z</para>
	/// <para>��</para>
	/// </summary>
	void CalculateFrustum();

	bool IntersectFrustum(AxisAlignedBoundingBox* aabb);

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();
};
