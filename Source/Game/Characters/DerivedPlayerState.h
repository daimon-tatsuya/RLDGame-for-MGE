#pragma once
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/StateBase.h"

#include "Game/Characters/Player.h"


//-------------------------------------
//親ステート
//--------------------------------------

/// <summary>
/// プレイヤーの入力を受け付ける
/// </summary>
class EntryState :
	public HierarchicalStateBase
{
private:
public:
	/// <summary>
	/// プレイヤーの入力を受け付けるステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	EntryState(Character* character) : HierarchicalStateBase(character) {}
	virtual ~EntryState() ;

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

/// <summary>
/// プレイヤーの受動を管理する
/// </summary>
class ReactionState :
	public HierarchicalStateBase
{
private:
public:
	/// <summary>
	/// プレイヤーの受動を管理するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	ReactionState(Character* character) : HierarchicalStateBase(character) {}
	virtual ~ReactionState() ;

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

/// <summary>
///	 MetaAIからメッセージを受信したとき
/// </summary>
class ReceiveState :
	public HierarchicalStateBase
{
private:
public:
	/// <summary>
	///	 MetaAIからメッセージを受信したときのステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	ReceiveState(Character* character) : HierarchicalStateBase(character) {}
	virtual ~ReceiveState() ;

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

//-------------------------------------
//子ステート
//--------------------------------------

//Inputの子ステート
class SelectState :
	public StateBase
{
private:
public:
	/// <summary>
	///	 入力を受け付け、それに応じてステートを遷移させるステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	SelectState(Character* character) : StateBase(character) {}
	virtual ~SelectState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//Inputの子ステート
class WayChangeState
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>方向を変えるステート</para>
	/// このステートはMoveに遷移しないがAttackには遷移できる
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	WayChangeState(Character* character) : StateBase(character) {}
	virtual ~WayChangeState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Inputの子ステート
class MoveState
	:public StateBase
{
private:
public:
	/// <summary>
	/// 1マス移動するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	MoveState(Character* character) : StateBase(character) {}
	virtual ~MoveState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Inputの子ステート
class AttackState
	:public StateBase
{
private:
public:
	/// <summary>
	/// プレイヤーの前方を攻撃するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	AttackState(Character* character) : StateBase(character) {}
	virtual ~AttackState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Inputの子ステート
class MenuState
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>メニューを開いているステート</para>
	///  <para>閉じたときSelectに遷移する</para>
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	MenuState(Character* character) : StateBase(character) {}
	virtual ~MenuState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};

//Reactionの子ステート
class DamagedState
	:public StateBase
{
private:
public:
	/// <summary>
	/// プレイヤーの前方を攻撃するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	DamagedState(Character* character) : StateBase(character) {}
	virtual ~DamagedState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Reactionの子ステート
class DeathState
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>メニューを開いているステート</para>
	///  <para>閉じたときSelectに遷移する</para>
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	DeathState(Character* character) : StateBase(character) {}
	virtual ~DeathState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};

//Receiveの子ステート
class CalledState
	:public StateBase
{
private:
public:
	/// <summary>
	/// <para>MetaAIから送られてくるメッセージによってステートを遷移させる</para>
	///  <para></para>
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	CalledState(Character* character) : StateBase(character) {}
	virtual ~CalledState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};