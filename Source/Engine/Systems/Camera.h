#pragma once
//**********************************************************
//
//		Camera�N���X
//
//**********************************************************

#include "Engine/Systems/Math.h"

/// <summary>
/// �J�����������N���X
/// </summary>
class Camera
{
private:
	DirectX::XMFLOAT4X4 view = // �r���[�s��
	{
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};
	DirectX::XMFLOAT4X4 projection = // ���e�s��
	{
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};

	DirectX::XMFLOAT3	eye = { 0,0,0 };		// �J�����̈ʒu
	DirectX::XMFLOAT3	focus = { 0,0,0 };	// �J�����̕���
	DirectX::XMFLOAT3	up = { 0,0,0 };		// �J�����̏�����A�ʏ��<0.0f�A1.0f�A0.0f>
	DirectX::XMFLOAT3	front = { 0,0,0 };	// �J�����̑O������
	DirectX::XMFLOAT3	right = { 0,0,0 };	// �J�����̉E����

	bool  OrthMode = false;// ���ˉe�J�������[�h

private:

	Camera() = default;
	~Camera() = default;

public:

	// �B��̃C���X�^���X
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	/// <summary>
	/// �w�����������
	/// </summary>
	/// <param name="eye">�J�����̈ʒu</param>
	/// <param name="focus">�����_</param>
	/// <param name="up">�J�����̏��</param>
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

	/// <summary>
	/// �p�[�X�y�N�e�B�u�̐ݒ�
	/// </summary>
	/// <param name="fovY">���W�A���p�̎���p</param>
	/// <param name="aspect">�A�X�y�N�g��</param>
	/// <param name="nearZ">�߂��̃N���b�s���O���ʂ܂ł̋���(�[�����傫��)</param>
	/// <param name="farZ">�����N���b�s���O���ʂ܂ł̋���(�[�����傫��)</param>
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	/// <summary>
	/// �I���\�̐ݒ�
	/// </summary>
	/// <param name="width">��</param>
	/// <param name="height">����</param>
	/// <param name="nearZ">�߂��̃N���b�s���O���ʂ܂ł̋���(�[�����傫��)</param>
	/// <param name="farZ">�����N���b�s���O���ʂ܂ł̋���(�[�����傫��)</param>
	void SetOrthFov(float width, float height, float nearZ, float farZ);

	//------------------------------------------------
	//
	// Getter
	//
	//------------------------------------------------

	// �r���[�s��擾
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	// �v���W�F�N�V�����s��擾
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

	// ���_�擾
	const DirectX::XMFLOAT3& GetEye() const { return eye; }

	// �����_�擾
	const DirectX::XMFLOAT3& GetFocus() const { return focus; }

	// ������擾
	const DirectX::XMFLOAT3& GetUp() const { return up; }

	// �O�����擾
	const DirectX::XMFLOAT3& GetFront() const { return front; }

	// �E�����擾
	const DirectX::XMFLOAT3& GetRight() const { return right; }

	// ���ˉe�J�������[�h�擾
	 bool GetOrthMode() { return OrthMode; }

	// ���ˉe�J�������[�h�ݒ�
	void SetOrthMode(bool mode) {  OrthMode= mode; }
};
