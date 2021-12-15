//#pragma once
//
////**********************************************************
////
////		EnemySnake��State�̔h���N���X
////
////**********************************************************
//
//#include "Engine/AI/MetaAI.h"
//#include "Game/Characters/EnemyState.h"
//#include "Game/Characters/EnemySnake.h"
//
//
//
////---------------------------------------------------------
////�e�X�e�[�g
////---------------------------------------------------------
//
///// <summary>
///// �w�r(�G)�̍s���I���X�e�[�g
///// </summary>
//class EnemySnakeEntryState
//	: public EnemyBaseHierarchicalState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// �w�r(�G)�̍s����I������
//	/// </summary>
//	/// <param name="snake">�����҂̃|�C���^</param>
//	EnemySnakeEntryState(EnemySnake* snake) : EnemyBaseHierarchicalState(snake) {};
//
//	virtual ~EnemySnakeEntryState();
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
///// <summary>
///// �w�r(�G)�̎󓮂��Ǘ�����X�e�[�g
///// </summary>
//class EnemySnakeReactionState
//	: public EnemyBaseHierarchicalState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// �w�r(�G)�̎󓮂��Ǘ�����
//	/// </summary>
//	/// <param name="snake">�����҂̃|�C���^</param>
//	EnemySnakeReactionState(EnemySnake* snake) : EnemyBaseHierarchicalState(snake) {};
//
//	virtual ~EnemySnakeReactionState();
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
///// <summary>
/////	 MetaAI���烁�b�Z�[�W����M�����Ƃ��̃X�e�[�g
///// </summary>
//class EnemySnakeReceiveState
//	:public EnemyBaseHierarchicalState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// MetaAI���烁�b�Z�[�W����M�����Ƃ�
//	/// </summary>
//	/// <param name="snake">�����҂̃|�C���^</param>
//	EnemySnakeReceiveState(EnemySnake* snake) : EnemyBaseHierarchicalState(snake) {};
//
//	virtual ~EnemySnakeReceiveState();
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//};
//
////-------------------------------------
////�q�X�e�[�g
////--------------------------------------
//
////Entry�̎q�m�[�h
//class EnemySnakeSelectState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// �s���I��
//	/// </summary>
//	/// <param name="snake"></param>
//	EnemySnakeSelectState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeSelectState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
//
////Entry�̎q�m�[�h
//class EnemySnakeApproachState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// �Ώۂɋ߂Â�
//	/// </summary>
//	/// <param name="snake"></param>
//	EnemySnakeApproachState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeApproachState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Entry�̎q�m�[�h
//class EnemySnakeExploreState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// �����̏o���֌����������Ȃ�ɐi��
//	/// </summary>
//	/// <param name="snake"></param>
//	EnemySnakeExploreState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeExploreState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Entry�̎q�m�[�h
//class EnemySnakeAttackState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	/// <summary>
//	/// �w�r(�G)�̓G���U������
//	/// </summary>
//	/// <param name="snake"></param>
//	EnemySnakeAttackState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeAttackState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Entry�̎q�m�[�h
//class EnemySnakeAbilityState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	EnemySnakeAbilityState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeAbilityState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Entry�̎q�m�[�h
//class EnemySnakeUseItemState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	EnemySnakeUseItemState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeUseItemState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Reaction�̎q�m�[�h
//class EnemySnakeDamagedState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	EnemySnakeDamagedState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeDamagedState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////reaction�̎q�m�[�h
//class EnemySnakeDeathState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	EnemySnakeDeathState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeDeathState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Receive�̎q�m�[�h
//class EnemySnakeWaitState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	EnemySnakeWaitState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeWaitState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};
//
////Receive�̎q�m�[�h
//class EnemySnakeCalledState
//	:public EnemyBaseState
//{
//private:
//
//public:
//
//	EnemySnakeCalledState(EnemySnake* snake) : EnemyBaseState(snake) {};
//
//	virtual ~EnemySnakeCalledState() {};
//
//	// �X�e�[�g�ɓ��������̃��\�b�h
//	void Enter()override;
//
//	// �X�e�[�g�Ŏ��s���郁�\�b�h
//	void Execute(float elapsedTime)override;
//
//	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
//	void Exit()override;
//
//};