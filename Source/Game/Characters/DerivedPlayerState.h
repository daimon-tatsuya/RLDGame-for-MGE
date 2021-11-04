#pragma once
#include<memory>
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/StateBase.h"

#include "Game/Characters/Player.h"


class PlayerState :
	public StateBase
{
private:
public:
	PlayerState(Player* player) :owner(player) {};
	virtual ~PlayerState() {};



protected:
	//�Q�Ɨp�̂��߉�����Ȃ�
	Player* owner = nullptr;// ���L��

};
class PlayerHierarchicalState :
	public  PlayerState ,public HierarchicalStateBase
{
private:
public:
	PlayerHierarchicalState(Player*	player):PlayerState(player) {};
	virtual ~PlayerHierarchicalState() {};
protected:

};


//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

/// <summary>
/// �v���C���[�̓��͂��󂯕t����
/// </summary>
class EntryState :
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	/// �v���C���[�̓��͂��󂯕t����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	EntryState(Player* player) : PlayerHierarchicalState(player) {}
	virtual ~EntryState() ;

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

/// <summary>
/// �v���C���[�̎󓮂��Ǘ�����
/// </summary>
class ReactionState :
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	/// �v���C���[�̎󓮂��Ǘ�����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	ReactionState(Player* player) : PlayerHierarchicalState(player) {}
	virtual ~ReactionState() ;

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

/// <summary>
///	 MetaAI���烁�b�Z�[�W����M�����Ƃ�
/// </summary>
class ReceiveState :
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	///	 MetaAI���烁�b�Z�[�W����M�����Ƃ��̃X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	ReceiveState(Player* player) : PlayerHierarchicalState(player) {}
	virtual ~ReceiveState() ;

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
class SelectState :
	public PlayerState
{
private:
public:
	/// <summary>
	///	 ���͂��󂯕t���A����ɉ����ăX�e�[�g��J�ڂ�����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	SelectState(Player* player) : PlayerState(player) {}
	virtual ~SelectState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};
//Input�̎q�X�e�[�g
class WayChangeState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>������ς���X�e�[�g</para>
	/// ���̃X�e�[�g��Move�ɑJ�ڂ��Ȃ���Attack�ɂ͑J�ڂł���
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	WayChangeState(Player* player) : PlayerState(player) {}
	virtual ~WayChangeState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Input�̎q�X�e�[�g
class MoveState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// 1�}�X�ړ�����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	MoveState(Player* player) : PlayerState(player) {}
	virtual ~MoveState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Input�̎q�X�e�[�g
class AttackState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// �v���C���[�̑O�����U������X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	AttackState(Player* player) : PlayerState(player) {}
	virtual ~AttackState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Input�̎q�X�e�[�g
class MenuState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>���j���[���J���Ă���X�e�[�g</para>
	///  <para>�����Ƃ�Select�ɑJ�ڂ���</para>
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	MenuState(Player* player) : PlayerState(player) {}
	virtual ~MenuState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};

//Reaction�̎q�X�e�[�g
class DamagedState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// �v���C���[�̑O�����U������X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	DamagedState(Player* player) : PlayerState(player) {}
	virtual ~DamagedState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};
//Reaction�̎q�X�e�[�g
class DeathState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>���j���[���J���Ă���X�e�[�g</para>
	///  <para>�����Ƃ�Select�ɑJ�ڂ���</para>
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	DeathState(Player* player) : PlayerState(player) {}
	virtual ~DeathState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};

//Receive�̎q�X�e�[�g
class CalledState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>MetaAI���瑗���Ă��郁�b�Z�[�W�ɂ���ăX�e�[�g��J�ڂ�����</para>
	///  <para></para>
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	CalledState(Player* player) : PlayerState(player) {}
	virtual ~CalledState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};