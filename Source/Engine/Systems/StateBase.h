#pragma once
#include "Engine/Systems/Character.h"
//�S�ăX�e�[�g�N���X�̊��N���X
class StateBase
{
private:

protected:

public:
	// �R���X�g���N�^
	StateBase(Character* character) : owner(character) {}
	virtual ~StateBase() {}

	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;

private:

protected:

	Character* owner = nullptr;// ���L��

public:
};

// �e�K�w�p�̃X�e�[�g
class HierarchicalStateBase : public StateBase
{
public:
	// �R���X�g���N�^
	HierarchicalStateBase(Character* character) : StateBase(character) {}
	// �f�X�g���N�^
	virtual ~HierarchicalStateBase() {}
	// �T�u�X�e�[�g�ύX�֐�
	virtual void ChangeSubState(int newState);
	// �T�u�X�e�[�g�o�^�֐�
	virtual void RegisterSubState(StateBase* state);
	// �T�u�X�e�[�g�擾�֐�
	virtual StateBase* GetSubState() { return sub_state; }
	// �T�u�X�e�[�g�ԍ��擾�֐�
	virtual int GetSubStateIndex();
	// �T�u�X�e�[�g�ݒ�֐�
	virtual void SetSubState(int setState);
protected:
	// �e�T�u�X�e�[�g��ێ�����z��
	std::vector<StateBase*> sub_state_pool;
	// �T�u�X�e�[�g
	StateBase* sub_state = nullptr;
};