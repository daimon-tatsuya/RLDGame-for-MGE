#pragma once

//**********************************************************
//
//		EnemySnakeのStateの派生クラス
//
//**********************************************************

#include "Engine/AI/MetaAI.h"
#include "Game/Characters/EnemyState.h"
#include "Game/Characters/EnemySnake.h"



//---------------------------------------------------------
//親ステート
//---------------------------------------------------------

/// <summary>
/// ヘビ(敵)の行動選択ステート
/// </summary>
class EnemySnakeSelectState
	: public EnemyBaseHierarchicalState
{
private:

public:

	/// <summary>
	/// ヘビ(敵)の行動を選択する
	/// </summary>
	/// <param name="snake">所持者のポインタ</param>
	EnemySnakeSelectState(EnemySnake* snake) : EnemyBaseHierarchicalState(snake) {};

	virtual ~EnemySnakeSelectState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

/// <summary>
/// ヘビ(敵)の受動を管理するステート
/// </summary>
class EnemySnakeReactionState
	: public EnemyBaseHierarchicalState
{
private:

public:

	/// <summary>
	/// ヘビ(敵)の受動を管理する
	/// </summary>
	/// <param name="snake">所持者のポインタ</param>
	EnemySnakeReactionState(EnemySnake* snake) : EnemyBaseHierarchicalState(snake) {};

	virtual ~EnemySnakeReactionState() {};

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
class EnemySnakeReceiveState
	:public EnemyBaseHierarchicalState
{
private:

public:

	/// <summary>
	/// MetaAIからメッセージを受信したとき
	/// </summary>
	/// <param name="snake">所持者のポインタ</param>
	EnemySnakeReceiveState(EnemySnake* snake) : EnemyBaseHierarchicalState(snake) {};

	virtual ~EnemySnakeReceiveState() {};

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

//Selectの子ノード
class EnemySnakeMoveState
	:public EnemyBaseState
{
private:

public:

	/// <summary>
	/// 1マス移動する
	/// </summary>
	/// <param name="snake"></param>
	EnemySnakeMoveState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeMoveState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//Selectの子ノード
class EnemySnakeAttackState
	:public EnemyBaseState
{
private:

public:

	/// <summary>
	/// ヘビ(敵)の敵を攻撃する
	/// </summary>
	/// <param name="snake"></param>
	EnemySnakeAttackState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeAttackState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//Selectの子ノード
class EnemySnakeAbilityState
	:public EnemyBaseState
{
private:

public:

	EnemySnakeAbilityState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeAbilityState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//Selectの子ノード
class EnemySnakeUseItemState
	:public EnemyBaseState
{
private:

public:

	EnemySnakeUseItemState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeUseItemState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//Reactionの子ノード
class EnemySnakeDamagedState
	:public EnemyBaseState
{
private:

public:

	EnemySnakeDamagedState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeDamagedState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//reactionの子ノード
class EnemySnakeDeathState
	:public EnemyBaseState
{
private:

public:

	EnemySnakeDeathState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeDeathState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//Receiveの子ノード
class EnemySnakeWaitState
	:public EnemyBaseState
{
private:

public:

	EnemySnakeWaitState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeWaitState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};

//Receiveの子ノード
class EnemySnakeCalledState
	:public EnemyBaseState
{
private:

public:

	EnemySnakeCalledState(EnemySnake* snake) : EnemyBaseState(snake) {};

	virtual ~EnemySnakeCalledState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};