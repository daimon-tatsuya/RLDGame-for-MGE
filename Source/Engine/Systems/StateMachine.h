#pragma once

#include <vector>
#include <memory>
#include "Engine/Systems/StateBase.h"

//ToDo StateMachine �R�����g

//�O���錾
class Character;

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
	// �X�e�[�g�Z�b�g
	void SetState(int setState);
	// �X�e�[�g�ύX
	void ChangeState(int newState);
	// �X�e�[�g�o�^(�o�^����͈̂�w��)
	void RegisterState(HierarchicalStateBase* state);
	//���݂̃X�e�[�g�擾
	HierarchicalStateBase* GetState() { return current_state; }
	// �T�u�X�e�[�g�o�^��ǉ�
	void StateMachine::RegisterSubState(int index, StateBase* subState);
	// ���݂̃X�e�[�g�ԍ��擾
	int GetStateIndex();

private:
	Character* owner = nullptr; // ������
	HierarchicalStateBase* current_state = nullptr;	// ���݂̃X�e�[�g
	std::vector<HierarchicalStateBase*> state_pool;	// �e�X�e�[�g��ێ�����z��
protected:

public:
};
