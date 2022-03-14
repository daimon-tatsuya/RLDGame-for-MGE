#pragma once
//**********************************************************
//
//		Character.h
//
//**********************************************************

#include <utility>

#include "Engine/AI/HeuristicSearch.h"
#include "Engine/Systems/Object.h"

// �Q�[���ɋ��ʂ�����̂����c���A���Ƃ͂��̃N���X���p�����āA�A�N�V�����Q�[���̃L�����N�^�[�x�[�X�ƕ�����d�l�ɂ���\��
// �g��Ȃ��֐������邪�A�����܂ŃA�N�V�����Q�[���x�[�X�Ȃ̂ŏ����Ȃ��ł���
/*
 �g���Ă��Ȃ��֐�
 UpdateVerticalVelocity
 UpdateVerticalMove
 UpdateHorizontalVelocity
 UpdateHorizontalMove
 UpdateVelocity
 UpdateInvincibleTimer
 AddImpulse
 AirDash
 ApplyDamage
 Jump
 Move
 Turn
 OnLanding
 OnDamaged
 OnDead
 IsGround
 MoveCheck
*/
//�O���錾
class RogueLikeDungeon;

struct  ShortestPath;

/// <summary>
/// �L�����N�^�[(�G�A�v���C���[)�Ȃǂ̊��N���X
/// </summary>
class Character : public Object
{
private:

public:
	int	 current_health = 5;//���O��ς��邩�� health��life,HP
	int	 max_health = 5;

	RogueLikeDungeon* stage_information = nullptr; // �}�b�v���@(�l���R�s�[�������̂ɂ��邩������)

	ShortestPath* shortest_path{};//�ŒZ�o�H

private:

	/// <summary>
	/// �L���X�e�[�g�}�V���̏�����
	/// </summary>
	virtual void FiniteStateMachineInitialize() {}

	/// <summary>
	/// �������͍X�V����
	/// </summary>
	/// <param name="elapsed_frame">�o�ߎ���</param>
	virtual void UpdateVerticalVelocity(float elapsed_frame);

	/// <summary>
	///  �����ړ��X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	virtual  void UpdateVerticalMove(float elapsed_time);

	/// <summary>
	///  �������͍X�V����
	/// </summary>
	/// <param name="elapsed_frame">�o�ߎ���</param>
	virtual  void UpdateHorizontalVelocity(float elapsed_frame);

	/// <summary>
	///  �����ړ��X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	virtual  void UpdateHorizontalMove(float elapsed_time);

	/// <summary>
	///  ���͍X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void UpdateVelocity(float elapsed_time);

	/// <summary>
	/// ���G���ԍX�V
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void UpdateInvincibleTimer(float elapsed_time);

public:

	Character() = default;

	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	~Character() override = default;

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
	virtual  void AirDash(float vx, float vz, float gravity_cut_time);

	/// <summary>
	/// �_���[�W��^����
	/// </summary>
	/// <param name="damage">hp�֌��Z�����</param>
	/// <param name="invincible_time">���G����</param>
	bool ApplyDamage(int damage, float invincible_time);

	/// <summary>
	/// �j��
	/// </summary>
	//virtual  void Destroy();

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
	int GetHealth() const { return current_health; }

	// ���N��Ԃ�ݒ�
	void SetHealth(const int health) { this->current_health = health; }

	// �n�ʂɐڒn���Ă��邩
	bool IsGround() const { return is_ground; }

	//�v���C���[���ړ��ł��邩�`�F�b�N����
	void MoveCheck(float mx, float mz);

	// �ő匒�N��Ԃ��擾
	int GetMaxHealth() const { return max_health; }

	// �ő匒�N��Ԃ�ݒ�
	void SetMaxHealth() { this->max_health = max_health; }

	// �}�b�v�����擾
	RogueLikeDungeon* GetStageInformation() const { return stage_information; }

	//�ŒZ�o�H��ݒ�
	void SetShortestPath(std::vector<int> path) const { shortest_path->path = std::move(path); }
};
