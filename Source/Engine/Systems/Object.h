#pragma once
//**********************************************************
//
//		Object�N���X
//
//**********************************************************

#include <memory>
#include "Engine/Systems/Math.h"
#include "Engine/AI/Telegram.h"

//�O���錾
class Model;
class Shader;

/// <summary>
/// �Q�[�����̃I�u�W�F�N�g�̊��N���X
/// </summary>
class Object
{
private:

protected:
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		angle = { 0, 0, 0 };// ���W�A���p
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4	transform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	DirectX::XMFLOAT3		velocity = { 0, 0, 0 };
	std::shared_ptr<Model> model = nullptr;
	int					id = 0;
	float				radius = 0.5f;// ���a
	float				height = 2.0f;// ����

public:
	float				gravity = -1.0f;
	float				gravity_cut_time = 0;// �d�͂𖳎�����Ƃ��Ɏg��
	bool				is_ground = false;// �n�ʔ���p
	float				invincible_timer = 0.0f;// ���G����
	float				friction = 0.5f;
	float				acceleration = 1.0f;
	float				max_move_speed = 5.0f;
	float				move_vecX = 0.0f;
	float				move_vecZ = 0.0f;
	float				air_control = 0.3f;
	float				step_offset = 1.0f;
	float				slope_rate = 0.0f;
private:

protected:

public:

	Object() = default;

	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	virtual ~Object() = default;

	// �s��X�V����
	void UpdateTransform();
	// �X�V����
	virtual void Update(float elapsed_time) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader) = 0;

	// �f�o�b�O�pGUI�`��
	virtual	void DrawDebugGUI() {}

	// �f�o�b�O�v���~�e�B�u�`��
	virtual	void DrawDebugPrimitive() {}

	//���b�Z�[�W��M����
	virtual bool OnMessage(const Telegram& msg) = 0;

	// �C�ӂ̉�]�p��0~360�ɐ��K��
	float NormalizeAnyAngle(float radian);

	// ��]�p��0~360�ɐ��K��
	void NormalizeAngle();

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

		// �ʒu���擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �ʒu��ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	void SetPositionX(const float x) { this->position.x = x; }
	void SetPositionY(const float y) { this->position.y = y; }
	void SetPositionZ(const float z) { this->position.z = z; }

	//���݂̈ʒu���瓮����
	void AddPosition(const DirectX::XMFLOAT3& position)
	{
		this->position.x += position.x;
		this->position.y += position.y;
		this->position.z += position.z;
	}
	void AddPositionX(const float x) { this->position.x += x; }
	void AddPositionY(const float y) { this->position.y += y; }
	void AddPositionZ(const float z) { this->position.z += z; }

	// ��]�p�x���擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// ��]�p�x��ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	void SetAngleX(const float x) { this->angle.x = x; }
	void SetAngleY(const float y) { this->angle.y = y; }
	void SetAngleZ(const float z) { this->angle.z = z; }

	// �X�P�[�����擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// �X�P�[����ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	void SetScaleX(const float x) { this->scale.x = x; }
	void SetScaleY(const float y) { this->scale.y = y; }
	void SetScaleZ(const float z) { this->scale.z = z; }

	//character���Ƃ�id���Z�b�g���ĊǗ�����
	void	SetId(const int id) { this->id = id; }
	int		GetId() const { return id; }

	//���f�����擾
	void	SetModel(const char* pass);

	//���f����ݒ�
	Model* GetModel() const { return model.get(); }

	//�p���s����擾
	DirectX::XMFLOAT4X4 GetTransform() const { return transform; }

	//�p���s���ݒ�
	void SetTransform(const DirectX::XMFLOAT4X4& transform) { this->transform = transform; }

	// ���a���擾
	float GetRadius() const { return radius; }

	// ���a��ݒ�
	void SetRadius(const float radius) { this->radius = radius; }

	// �������擾
	float GetHeight()const { return height; }

	// ������ݒ�
	void SetHeight(const float height) { this->height = height; }

};
