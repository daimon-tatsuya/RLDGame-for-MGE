#pragma once

#include "Engine/AI/MetaAI.h"
#include "Game/Characters/Player.h"
#include "Game/Characters/PlayerState.h"


//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

/// <summary>
/// �v���C���[�̓��͂��󂯕t����X�e�[�g
/// </summary>
class PlayerEntryState :
	public PlayerHierarchicalState
{
private:

public:

	/// <summary>
	/// �v���C���[�̓��͂��󂯕t����
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	PlayerEntryState(Player* player) : PlayerHierarchicalState(player) {}

	virtual ~PlayerEntryState() ;

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;

	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;

	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

/// <summary>
/// �v���C���[�̎󓮂��Ǘ�����X�e�[�g
/// </summary>
class PlayerReactionState :
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	/// �v���C���[�̎󓮂��Ǘ�����
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerReactionState(Player* player) : PlayerHierarchicalState(player) {}

	virtual ~PlayerReactionState() ;

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
class PlayerReceiveState :
	public PlayerHierarchicalState
{
private:

public:
	/// <summary>
	///	 MetaAI���烁�b�Z�[�W����M�����Ƃ�
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerReceiveState(Player* player) : PlayerHierarchicalState(player) {}

	virtual ~PlayerReceiveState() ;

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

//Input�̎q�X�e�[�g
class PlayerSelectState :
	public PlayerState
{
private:

public:
	/// <summary>
	///	 ���͂��󂯕t���A����ɉ����ăX�e�[�g��J�ڂ�����X�e�[�g
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerSelectState(Player* player) : PlayerState(player) {}
	virtual ~PlayerSelectState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//Input�̎q�X�e�[�g
class PlayerWayChangeState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>������ς���X�e�[�g</para>
	/// ���̃X�e�[�g��Move�ɑJ�ڂ��Ȃ���Attack�ɂ͑J�ڂł���
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerWayChangeState(Player* player) : PlayerState(player) {}
	virtual ~PlayerWayChangeState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Input�̎q�X�e�[�g
class PlayerMoveState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// 1�}�X�ړ�����X�e�[�g
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerMoveState(Player* player) : PlayerState(player) {}
	virtual ~PlayerMoveState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Input�̎q�X�e�[�g
class PlayerAttackState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// �v���C���[�̑O�����U������X�e�[�g
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerAttackState(Player* player) : PlayerState(player) {}
	virtual ~PlayerAttackState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Input�̎q�X�e�[�g
class PlayerMenuState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>���j���[���J���Ă���X�e�[�g</para>
	///  <para>�����Ƃ�Select�ɑJ�ڂ���</para>
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerMenuState(Player* player) : PlayerState(player) {}
	virtual ~PlayerMenuState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};

//Reaction�̎q�X�e�[�g
class PlayerDamagedState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// �v���C���[�̑O�����U������X�e�[�g
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerDamagedState(Player* player) : PlayerState(player) {}
	virtual ~PlayerDamagedState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Reaction�̎q�X�e�[�g
class PlayerDeathState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>���j���[���J���Ă���X�e�[�g</para>
	///  <para>�����Ƃ�Select�ɑJ�ڂ���</para>
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerDeathState(Player* player) : PlayerState(player) {}
	virtual ~PlayerDeathState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};

//Receive�̎q�X�e�[�g
class PlayerWaitState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>MetaAI���瑗���Ă��郁�b�Z�[�W��҂X�e�[�g</para>
	///  <para></para>
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerWaitState(Player* player) : PlayerState(player) {}
	virtual ~PlayerWaitState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};

//Receive�̎q�X�e�[�g
class PlayerCalledState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>MetaAI���瑗���Ă��郁�b�Z�[�W�ɂ���ăX�e�[�g��J�ڂ�����</para>
	///  <para></para>
	/// </summary>
	/// <param name="player">�����҂̃|�C���^</param>
	PlayerCalledState(Player* player) : PlayerState(player) {}
	virtual ~PlayerCalledState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};