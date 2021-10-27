#pragma once
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/StateBase.h"

#include "Game/Characters/Player.h"


//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

/// <summary>
/// �v���C���[�̓��͂��󂯕t����
/// </summary>
class EntryState :
	public HierarchicalStateBase
{
private:
public:
	/// <summary>
	/// �v���C���[�̓��͂��󂯕t����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	EntryState(Character* character) : HierarchicalStateBase(character) {}
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
	public HierarchicalStateBase
{
private:
public:
	/// <summary>
	/// �v���C���[�̎󓮂��Ǘ�����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	ReactionState(Character* character) : HierarchicalStateBase(character) {}
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
	public HierarchicalStateBase
{
private:
public:
	/// <summary>
	///	 MetaAI���烁�b�Z�[�W����M�����Ƃ��̃X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	ReceiveState(Character* character) : HierarchicalStateBase(character) {}
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
	public StateBase
{
private:
public:
	/// <summary>
	///	 ���͂��󂯕t���A����ɉ����ăX�e�[�g��J�ڂ�����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	SelectState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>������ς���X�e�[�g</para>
	/// ���̃X�e�[�g��Move�ɑJ�ڂ��Ȃ���Attack�ɂ͑J�ڂł���
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	WayChangeState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// 1�}�X�ړ�����X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	MoveState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// �v���C���[�̑O�����U������X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	AttackState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>���j���[���J���Ă���X�e�[�g</para>
	///  <para>�����Ƃ�Select�ɑJ�ڂ���</para>
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	MenuState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// �v���C���[�̑O�����U������X�e�[�g
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	DamagedState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>���j���[���J���Ă���X�e�[�g</para>
	///  <para>�����Ƃ�Select�ɑJ�ڂ���</para>
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	DeathState(Character* character) : StateBase(character) {}
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
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>MetaAI���瑗���Ă��郁�b�Z�[�W�ɂ���ăX�e�[�g��J�ڂ�����</para>
	///  <para></para>
	/// </summary>
	/// <param name="character">�����҂̃|�C���^</param>
	CalledState(Character* character) : StateBase(character) {}
	virtual ~CalledState() {};

	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;


};