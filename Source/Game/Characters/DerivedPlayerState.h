#pragma once
#include "Game/Characters/Player.h"
#include "Engine/Systems/StateBase.h"
class ParentState :
	public HierarchicalStateBase
{
private:
public:
	// �R���X�g���N�^
	ParentState(Character* character) : HierarchicalStateBase(character) {}
	virtual ~ParentState() {}

	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

class ChildState :
	public StateBase
{
private:
public:
	// �R���X�g���N�^
	ChildState(Character* character) : StateBase(character) {}
	virtual ~ChildState() {}

	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
