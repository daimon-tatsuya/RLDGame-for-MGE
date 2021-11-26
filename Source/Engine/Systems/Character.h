#pragma once
//**********************************************************
//
//		Character.h
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/Systems/StateMachine.h"
#include "Engine/Systems/DungeonMake.h"

// �Q�[���ɋ��ʂ�����̂����c���A���Ƃ͂��̃N���X���p�����āA�A�N�V�����Q�[���̃L�����N�^�[�x�[�X�ƕ�����d�l�ɂ���\��
// �g��Ȃ��֐������邪�A�����܂ŃA�N�V�����Q�[���x�[�X�Ȃ̂ŏ����Ȃ��ł���

/// <summary>
/// �L�����N�^�[(�G�A�v���C���[)�Ȃǂ̊��N���X
/// </summary>
class Character : public Object
{
private:

public:

protected:

	int	 health = 5;

	int	 max_health = 5;

	RogueLikeDungeon* stage_informations = nullptr; // �}�b�v���@(�l���R�s�[�������̂ɂ��邩������)

	std::unique_ptr<StateMachine> state_machine = nullptr;//�X�e�[�g�}�V��

	std::vector<int> shortest_path;//�ŒZ�o�H
private:

	/// <summary>
	/// �L���X�e�[�g�}�V���̏�����
	/// </summary>
	virtual void FSMInitialize() {};

	/// <summary>
	/// �������͍X�V����
	/// </summary>
	/// <param name="elapsedFrame">�o�ߎ���</param>
	virtual void UpdateVerticalVelocity(float elapsedFrame);

	/// <summary>
	///  �����ړ��X�V����
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	virtual  void UpdateVerticalMove(float elapsedTime);

	/// <summary>
	///  �������͍X�V����
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	virtual  void UpdateHorizontalVelocity(float elapsedFrame);

	/// <summary>
	///  �����ړ��X�V����
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	virtual  void UpdateHorizontalMove(float elapsedTime);

	/// <summary>
	///  ���͍X�V����
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	void UpdateVelocity(float elapsed_time);

	/// <summary>
	///  ���G���ԍX�V
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	void UpdateInvincibleTimer(float elapsed_time);

protected:

public:

	Character() {}
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	virtual ~Character() {}

	/// <summary>
	/// �Ռ���^����
	/// </summary>
	 /// <param name="impulse">���x�ɉ��Z����Ռ�</param>
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	/// <summary>
	/// �󒆃_�b�V������
	/// </summary>
	/// <param name="vx">x���ɉ�������傫��</param>
	/// <param name="vz">z���ɉ�������傫��</param>
	/// <param name="gravity_cut_time">�d�͂𖳎����鎞��</param>
	virtual  void AirDush(float vx, float vz, float gravity_cut_time);

	/// <summary>
	/// �_���[�W��^����
	/// </summary>
	/// <param name="damage">hp�֌��Z�����</param>
	/// <param name="invincible_time">���G����</param>
	bool ApplyDamage(int damage, float invincible_time);

	/// <summary>
	/// �W�����v����
	/// </summary>
	/// <param name="speed">y���ɉ�������傫��</param>
	virtual void Jump(float speed);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="vx">x���ɉ�������傫��</param>
	/// <param name="vz">z���ɉ�������傫��</param>
	/// <param name="speed">�ő呬�x</param>
	virtual  void Move(float vx, float vz, float speed);

	/// <summary>
	/// ���񏈗�
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	/// <param name="vx">x���ɉ�������傫��</param>
	/// <param name="vz">z���ɉ�������傫��</param>
	/// <param name="speed">�ő呬�x</param>
	virtual  void Turn(float elapsed_time, float vx, float vz, float speed);

	/// <summary>
	/// ���n�������ɌĂ΂��
	/// </summary>
	virtual void OnLanding() {}

	/// <summary>
	/// �_���[�W���󂯂����ɌĂ΂��
	/// </summary>
	virtual void OnDamaged() {}

	/// <summary>
	/// ���S�������ɌĂ΂��
	/// </summary>
	virtual void OnDead() {}

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// ���N��Ԃ��擾
	int GetHealth() const { return health; }

	// ���N��Ԃ�ݒ�
	void SetHealth(const int health) { this->health = health; }

	// �n�ʂɐڒn���Ă��邩
	bool IsGround() const { return is_ground; }

	// �ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return max_health; }

	//�v���C���[���ړ��ł��邩�`�F�b�N����
	void MoveChack(float mx, float mz);

	// �ő匒�N��Ԃ��擾
	void SetMaxHealth() { this->max_health = max_health; }

	//�X�e�[�g�}�V�����擾
	StateMachine* GetStateMachine() { return state_machine.get(); }

	//�}�b�v�����擾
	RogueLikeDungeon* GetStageInformations() { return stage_informations; }

};
