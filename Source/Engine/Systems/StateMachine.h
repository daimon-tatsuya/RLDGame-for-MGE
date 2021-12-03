#pragma once
//**********************************************************
//
//		StateMachine�N���X
//
//**********************************************************
#include <vector>
#include <memory>
#include "Engine/Systems/StateBase.h"


//�O���錾
class Character;

/// <summary>
/// �L���X�e�[�g�}�V���N���X
/// </summary>
class StateMachine
{
private:

protected:

public:
	// �R���X�g���N�^
	StateMachine(Character* character) :owner(character) {}
	// �f�X�g���N�^
	~StateMachine();

	/// <summary>
	///  �X�V����
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	void Update(float elapsedTime);

	// �X�e�[�g��ݒ�
	void SetState(int setState);

	// �e�X�e�[�g��ύX
	void ChangeState(int newState);

	// �X�e�[�g�̓o�^(�o�^����͈̂�w��)
	void RegisterState(HierarchicalStateBase* state);

	//���݂̃X�e�[�g�擾
	HierarchicalStateBase* GetState() { return current_state; }

	// �T�u�X�e�[�g�o�^��ǉ�
	void StateMachine::RegisterSubState(int index, StateBase* subState);

	// ���݂̃X�e�[�g�ԍ��擾
	int GetCurrentStateIndex();

private:
	Character* owner = nullptr; // ������
	HierarchicalStateBase* current_state = nullptr;	// ���݂̃X�e�[�g
	std::vector<HierarchicalStateBase*> state_pool;	// �e�X�e�[�g��ێ�����z��
protected:

public:
};
