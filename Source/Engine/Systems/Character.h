#pragma once

#include "Engine/Systems/Object.h"
// �L�����N�^�[
//Todo �R�����g
class Character : public Object
{
private:
	// �������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);

	// �������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateHorizontalMove(float elapsedTime);
protected:
	// ���n�������ɌĂ΂��
	virtual void OnLanding() {}

	// �_���[�W���󂯂����ɌĂ΂��
	virtual void OnDamaged() {}

	// ���S�������ɌĂ΂��
	virtual void OnDead() {}

	// �ړ�����
	void Move(float vx, float vz, float speed);

	// ���񏈗�
	void Turn(float elapsed_time, float vx, float vz, float speed);

	// �W�����v����
	void Jump(float speed);

	// �󒆃_�b�V������
	void AirDush(float vx, float vz, float gravity_cut_time);

	// ���͍X�V����
	void UpdateVelocity(float elapsed_time);

	// ���G���ԍX�V
	void UpdateInvincibleTimer(float elapsed_time);

	// �ڕW�n�_�ֈړ�
	//void MoveToTarget(float elapsed_time, float speed_rate);

public:
	Character() {}
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	virtual ~Character() {}

	// �n�ʂɐڒn���Ă��邩
	bool IsGround() const { return is_ground; }

	// ���N��Ԃ��擾
	int GetHealth() const { return health; }

	// ���N��Ԃ�ݒ�
	void SetHealth(const int health) { this->health = health; }

	// �ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return max_health; }

	// �ő匒�N��Ԃ��擾
	void SetMaxHealth() { this->max_health = max_health; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincible_time);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);


private:

public:

protected:
	DirectX::XMFLOAT3	velocity = { 0, 0, 0 };

	float				gravity = -1.0f;
	float				gravity_cut_time = 0;//�d�͂𖳎�����Ƃ��Ɏg��
	bool				is_ground = false;
	int					health = 5;
	int					max_health = 5;
	float				invincible_timer = 0.0f;//���G����
	float				friction = 0.5f;
	float				acceleration = 1.0f;
	float				max_move_speed = 5.0f;
	float				move_vecX = 0.0f;
	float				move_vecZ = 0.0f;
	float				air_control = 0.3f;
	float				step_offset = 1.0f;
	float				slope_rate = 0.0f;
};
