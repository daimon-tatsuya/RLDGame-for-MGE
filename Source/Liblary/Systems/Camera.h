#pragma once
//**********************************************************
//
//		Camera�N���X
//
//**********************************************************

#include "Liblary/Systems/Math.h"

const float CAMERA_NEAR = 0.1f;
const float CAMERA_FAR = 100.f;

/// <summary>
/// �J�����������N���X
/// </summary>
class Camera final
{
private:
	DirectX::XMFLOAT4X4 view{};			// �r���[�s��
	DirectX::XMFLOAT4X4 projection{};	// ���e�s��

	DirectX::XMFLOAT3	eye{ 0,1,10 };		// �J�����̈ʒu
	DirectX::XMFLOAT3	focus{ 0,0,0 };	// �J�����̕���
	DirectX::XMFLOAT3	up{ 0,1,0 };		// �J�����̏�����A�ʏ��<0.0f�A1.0f�A0.0f>
	DirectX::XMFLOAT3	front{};		// �J�����̑O������
	DirectX::XMFLOAT3	right{};		// �J�����̉E����
	DirectX::XMFLOAT2	current_mouse_pos{};	// ���݂̃}�E�X�̈ʒu
	DirectX::XMFLOAT2	old_mouse_pos{};	// �O�t���[���}�E�X�̈ʒu

	float width{};
	float height{};
	float aspect{};
	float fovY{ DirectX::XM_PI / 6.0f };
	float nearZ{ 0.1f };
	float farZ{ 1000.f };

	bool  ortho_mode = false;// ���s���e�J�������[�h

private:
	//�R���X�g���N�^
	Camera();
	//�f�X�g���N�^
	~Camera() = default;

public:

	// �B��̃C���X�^���X
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}

	// �J�������A�N�e�B�u�ɂ���
	void ActivateCamera();

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

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
	/// �I���\(���s���e)�̐ݒ�
	/// </summary>
	/// <param name="width">��</param>
	/// <param name="height">����</param>
	/// <param name="nearZ">�߂��̃N���b�s���O���ʂ܂ł̋���(�[�����傫��)</param>
	/// <param name="farZ">�����N���b�s���O���ʂ܂ł̋���(�[�����傫��)</param>
	void SetOrthoFov(float width, float height, float nearZ, float farZ);

	// �r���[�s��̎擾
	const DirectX::XMFLOAT4X4& GetView() const { return view; }

	// �v���W�F�N�V�����s��̎擾
	const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

	// ���_�̎擾
	const DirectX::XMFLOAT3& GetEye() const { return eye; }

	// �����_�̎擾
	const DirectX::XMFLOAT3& GetFocus() const { return focus; }

	// ������̎擾
	const DirectX::XMFLOAT3& GetUp() const { return up; }

	// �O�����̎擾
	const DirectX::XMFLOAT3& GetFront() const { return front; }

	// �E�����̎擾
	const DirectX::XMFLOAT3& GetRight() const { return right; }

	//���̐ݒ�
	void SetWidth(const float width) { this->width = width; }

	//���̎擾
	float GetWidth() const { return  width; }

	//�����̐ݒ�
	void SetHeight(const float height) { this->height = height; }

	//�����̎擾
	float GetHeight() const { return  height; }

	//�A�X�y�N�g��̐ݒ�
	void SetAspect(const float width, const float height) { SetAspect(width / height); }
	void SetAspect(const float aspect) { this->aspect = aspect; }

	//�A�X�y�N�g��̎擾
	float GetAspect() const { return  aspect; }

	//����̐ݒ�
	void SetFov(const float fov) { this->fovY = fov; }

	//����̎擾
	float GetFov() const { return  fovY; }

	//near�̐ݒ�
	void SetNear(const float nearZ) { this->nearZ = nearZ; }

	//near�̎擾
	float GetNear() const { return  nearZ; }

	//far�̐ݒ�
	void SetFar(const float farZ) { this->farZ = farZ; }

	//far�̎擾
	float GetFar() const { return  farZ; }

	//���݂̃}�E�X�̈ʒu���擾
	void SetCurrentMousePos(DirectX::XMFLOAT2 pos) {current_mouse_pos = pos;}

	//���݂̃}�E�X�̈ʒu���擾
	DirectX::XMFLOAT2 GetCurrentMousePos() { return current_mouse_pos; }

	//�O�t���[���̃}�E�X�̈ʒu���擾
	void SetOldMousePos(DirectX::XMFLOAT2 pos) { old_mouse_pos = pos; }

	//�O�t���[���̃}�E�X�̈ʒu���擾
	DirectX::XMFLOAT2 GetOldMousePos() { return old_mouse_pos; }

	// ���s���e�J�������[�h�擾
	bool GetOrthMode() { return ortho_mode; }

	// ���s���e�J�������[�h�ݒ�
	void SetOrthMode(bool mode) { ortho_mode = mode; }
};
