#pragma once
#include "Engine/AI/MetaAI.h"
#include "Game/Characters/EnemySnakeState.h"
#include "Game/Characters/EnemySnake.h"

//---------------------------------------------------------
//�e�X�e�[�g
//---------------------------------------------------------

/// <summary>
/// �w�r(�G)�̍s���I���X�e�[�g
/// </summary>
class EnemySnakeSelectState
	: public EnemySnakeHierarchicalState
{
private:

public:

	/// <summary>
	/// �w�r(�G)�̍s����I������
	/// </summary>
	/// <param name="snake">�����҂̃|�C���^</param>
	EnemySnakeSelectState(EnemySnake* snake) : EnemySnakeHierarchicalState(snake) {};

	virtual ~EnemySnakeSelectState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

/// <summary>
/// �w�r(�G)�̎󓮂��Ǘ�����X�e�[�g
/// </summary>
class EnemySnakeReactionState
	: public EnemySnakeHierarchicalState
{
private:

public:

	/// <summary>
	/// �w�r(�G)�̎󓮂��Ǘ�����
	/// </summary>
	/// <param name="snake">�����҂̃|�C���^</param>
	EnemySnakeReactionState(EnemySnake* snake) : EnemySnakeHierarchicalState(snake) {};

	virtual ~EnemySnakeReactionState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

/// <summary>
///	 MetaAI���烁�b�Z�[�W����M�����Ƃ��̃X�e�[�g
/// </summary>
class EnemySnakeReceiveState
	:public EnemySnakeHierarchicalState
{
private:

public:

	/// <summary>
	/// MetaAI���烁�b�Z�[�W����M�����Ƃ�
	/// </summary>
	/// <param name="snake">�����҂̃|�C���^</param>
	EnemySnakeReceiveState(EnemySnake* snake) : EnemySnakeHierarchicalState(snake) {};

	virtual ~EnemySnakeReceiveState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

//-------------------------------------
//�q�X�e�[�g
//--------------------------------------

//Select�̎q�m�[�h
class EnemySnakeMoveState
	:public EnemySnakeState
{
private:

public:

	/// <summary>
	/// 1�}�X�ړ�����
	/// </summary>
	/// <param name="snake"></param>
	EnemySnakeMoveState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeMoveState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//Select�̎q�m�[�h
class EnemySnakeAttackState
	:public EnemySnakeState
{
private:

public:

	/// <summary>
	/// �w�r(�G)�̓G���U������
	/// </summary>
	/// <param name="snake"></param>
	EnemySnakeAttackState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeAttackState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//Select�̎q�m�[�h
class EnemySnakeAbilityState
	:public EnemySnakeState
{
private:

public:

	EnemySnakeAbilityState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeAbilityState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//Select�̎q�m�[�h
class EnemySnakeUseItemState
	:public EnemySnakeState
{
private:

public:

	EnemySnakeUseItemState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeUseItemState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//Reaction�̎q�m�[�h
class EnemySnakeDamagedState
	:public EnemySnakeState
{
private:

public:

	EnemySnakeDamagedState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeDamagedState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//reaction�̎q�m�[�h
class EnemySnakeDeathState
	:public EnemySnakeState
{
private:

public:

	EnemySnakeDeathState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeDeathState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//Receive�̎q�m�[�h
class EnemySnakeWaitState
	:public EnemySnakeState
{
private:

public:

	EnemySnakeWaitState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeWaitState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};

//Receive�̎q�m�[�h
class EnemySnakeCalledState
	:public EnemySnakeState
{
private:

public:

	EnemySnakeCalledState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeCalledState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

};