#pragma once

#include "Engine/AI/MetaAI.h"
#include "Game/Characters/Player.h"
#include "Game/Characters/PlayerState.h"


//-------------------------------------
//親ステート
//--------------------------------------

/// <summary>
/// プレイヤーの入力を受け付けるステート
/// </summary>
class PlayerEntryState :
	public PlayerHierarchicalState
{
private:

public:

	/// <summary>
	/// プレイヤーの入力を受け付ける
	/// </summary>
	/// <param name="character">所持者のポインタ</param>
	PlayerEntryState(Player* player) : PlayerHierarchicalState(player) {}

	virtual ~PlayerEntryState() ;

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;
};

/// <summary>
/// プレイヤーの受動を管理するステート
/// </summary>
class PlayerReactionState :
	public PlayerHierarchicalState
{
private:
public:
	/// <summary>
	/// プレイヤーの受動を管理する
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerReactionState(Player* player) : PlayerHierarchicalState(player) {}

	virtual ~PlayerReactionState() ;

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;
};

/// <summary>
///	 MetaAIからメッセージを受信したときのステート
/// </summary>
class PlayerReceiveState :
	public PlayerHierarchicalState
{
private:

public:
	/// <summary>
	///	 MetaAIからメッセージを受信したとき
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerReceiveState(Player* player) : PlayerHierarchicalState(player) {}

	virtual ~PlayerReceiveState() ;

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
class PlayerSelectState :
	public PlayerState
{
private:

public:
	/// <summary>
	///	 入力を受け付け、それに応じてステートを遷移させるステート
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerSelectState(Player* player) : PlayerState(player) {}
	virtual ~PlayerSelectState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
//Inputの子ステート
class PlayerWayChangeState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>方向を変えるステート</para>
	/// このステートはMoveに遷移しないがAttackには遷移できる
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerWayChangeState(Player* player) : PlayerState(player) {}
	virtual ~PlayerWayChangeState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Inputの子ステート
class PlayerMoveState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// 1マス移動するステート
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerMoveState(Player* player) : PlayerState(player) {}
	virtual ~PlayerMoveState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Inputの子ステート
class PlayerAttackState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// プレイヤーの前方を攻撃するステート
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerAttackState(Player* player) : PlayerState(player) {}
	virtual ~PlayerAttackState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Inputの子ステート
class PlayerMenuState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>メニューを開いているステート</para>
	///  <para>閉じたときSelectに遷移する</para>
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerMenuState(Player* player) : PlayerState(player) {}
	virtual ~PlayerMenuState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};

//Reactionの子ステート
class PlayerDamagedState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// プレイヤーの前方を攻撃するステート
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerDamagedState(Player* player) : PlayerState(player) {}
	virtual ~PlayerDamagedState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};
//Reactionの子ステート
class PlayerDeathState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>メニューを開いているステート</para>
	///  <para>閉じたときSelectに遷移する</para>
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerDeathState(Player* player) : PlayerState(player) {}
	virtual ~PlayerDeathState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};

//Receiveの子ステート
class PlayerWaitState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>MetaAIから送られてくるメッセージを待つステート</para>
	///  <para></para>
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerWaitState(Player* player) : PlayerState(player) {}
	virtual ~PlayerWaitState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};

//Receiveの子ステート
class PlayerCalledState
	:public PlayerState
{
private:
public:
	/// <summary>
	/// <para>MetaAIから送られてくるメッセージによってステートを遷移させる</para>
	///  <para></para>
	/// </summary>
	/// <param name="player">所持者のポインタ</param>
	PlayerCalledState(Player* player) : PlayerState(player) {}
	virtual ~PlayerCalledState() {};

	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;


};