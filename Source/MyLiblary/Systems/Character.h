#pragma once
//**********************************************************
//
//		Character�N���X
//
//**********************************************************

<<<<<<< HEAD:Source/Liblary/Systems/Character.h
#include "Liblary/Systems/Object.h"
#include "Liblary/AI/HeuristicSearch.h"
=======
#include "MyLiblary/Systems/Object.h"
#include "MyLiblary/AI/HeuristicSearch.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Systems/Character.h

class Character :
	public Object
{
private:
	//	���O��ς��邩�� health��life,HP
	int	current_health = 5;//���݂̗̑͒l
	int	max_health = 5;//�ő�̗͒l

	bool	is_decide_pos = false;//�ʒu�����܂�������break���邽�߂Ɏg��




public:
	//-�x�������邽�߂Ɏg���ϐ�-
	float delay_time = 1.5f;
	float counted_time = 0.f;
	//-----------------------------
	ShortestPath* shortest_path{};//�ŒZ�o�H

private:

	/// <summary>
/// �L���X�e�[�g�}�V���̏�����
/// </summary>
	virtual void FiniteStateMachineInitialize() {};

public:

	Character() = default;
	~Character() override;

	//�L�����N�^�[�}�l�[�W���[�̃��X�g���玩�g������
	virtual void Destroy();

	// �̗͒l���擾
	int GetCurrentHealth() const { return current_health; }

	// �̗͒l��ݒ�
	void SetCurrentHealth(const int health) { this->current_health = health; }

	// �̗͒l�𑝉�
	void AddCurrentHealth(const int health) { this->current_health += health; }

	// �ő�̗͒l���擾
	int GetMaxHealth() const { return max_health; }

	// �ő�̗͒l��ݒ�
	void SetMaxHealth(const int health) { this->max_health = health; }

	// �ő�̗͒l�𑝉�
	void AddMaxHealth(const int health) { this->max_health += health; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincible_time);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// �ړ�����
	void Move(float vx, float vz, float speed);

	// ���񏈗�
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// �W�����v����
	void Jump(float speed);

	// �󒆃_�b�V������
	void AirDush(float vx, float vz, float gravityCutTime);

	//�ŒZ�o�H��ݒ�
	void SetShortestPath(std::vector<int> path) const { shortest_path->path_information = std::move(path); }

	//�ʒu�����肵�Ă��邩�̃t���O���擾
	bool GetIsDecidePos() const { return  is_decide_pos; }

	//�ʒu�����肵�Ă��邩�̃t���O�̐ݒ�
	void SetIsDecidePos(const bool set) { is_decide_pos = set; }

	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram) override{return false;}
};



