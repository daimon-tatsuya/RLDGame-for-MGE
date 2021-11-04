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
	//参照用のため解放しない
	Player* owner = nullptr;// 所有者

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
//親ステート
//--------------------------------------

/// <summary>
/// プレイヤーの入力を受け付ける
/// </summary>
class EntryState :
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	/// プレイヤーの入力を受け付けるステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	EntryState(Player* player) : PlayerHierarchicalState(player) {}
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
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	/// プレイヤーの受動を管理するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	ReactionState(Player* player) : PlayerHierarchicalState(player) {}
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
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	///	 MetaAIからメッセージを受信したときのステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	ReceiveState(Player* player) : PlayerHierarchicalState(player) {}
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
	public PlayerState
{
private:
public:
	/// <summary>
	///	 入力を受け付け、それに応じてステートを遷移させるステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	SelectState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>方向を変えるステート</para>
	/// このステートはMoveに遷移しないがAttackには遷移できる
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	WayChangeState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// 1マス移動するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	MoveState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// プレイヤーの前方を攻撃するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	AttackState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>メニューを開いているステート</para>
	///  <para>閉じたときSelectに遷移する</para>
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	MenuState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// プレイヤーの前方を攻撃するステート
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	DamagedState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>メニューを開いているステート</para>
	///  <para>閉じたときSelectに遷移する</para>
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	DeathState(Player* player) : PlayerState(player) {}
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
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>MetaAIから送られてくるメッセージによってステートを遷移させる</para>
	///  <para></para>
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	CalledState(Player* player) : PlayerState(player) {}
	virtual ~CalledState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};