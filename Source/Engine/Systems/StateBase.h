#pragma once
//**********************************************************
//
//		 StateBase�N���X
//		 HierarchicalStateBase�N���X
//
//**********************************************************
#include <memory>
#include <vector>

//�O���錾
class Character;

/// <summary>
/// �S�ăX�e�[�g�N���X�̊��N���X
/// </summary>
class StateBase
{
private:

protected:

public:
	// �R���X�g���N�^
	StateBase() {}
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
//�p����ŏ����҂̃|�C���^��ݒ肷��

public:
};


/// <summary>
/// �e�K�w�p�̃X�e�[�g�N���X
/// </summary>
class HierarchicalStateBase : public StateBase
{
public:

	// �R���X�g���N�^
	HierarchicalStateBase() {}

	// �f�X�g���N�^
	virtual ~HierarchicalStateBase() {}

	// �T�u�X�e�[�g�ύX�֐�
	 void ChangeSubState(int newState);

	// �T�u�X�e�[�g�o�^�֐�
	 void RegisterSubState(StateBase* state);

	// �T�u�X�e�[�g�ԍ��擾�֐�
	 int GetSubStateIndex();

	// �T�u�X�e�[�g�擾�֐�
	 StateBase* GetSubState() { return sub_state; }

	// �T�u�X�e�[�g�ݒ�֐�
	 void SetSubState(int setState);

protected:

	// �e�T�u�X�e�[�g��ێ�����z��
	std::vector<StateBase*> sub_state_pool;

	// �T�u�X�e�[�g
	StateBase* sub_state = nullptr;
};