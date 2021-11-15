#pragma once
#include "Engine/AI/MetaAI.h"
#include "Game/Characters/EnemySnakeState.h"
#include "Game/Characters/EnemySnake.h"

//---------------------------------------------------------
//親ステート
//---------------------------------------------------------

/// <summary>
/// ヘビ(敵)の行動選択ステート
/// </summary>
class EnemySnakeSelectState
	: public EnemySnakeHierarchicalState
{
private:

public:

	/// <summary>
	/// ヘビ(敵)の行動を選択する
	/// </summary>
	/// <param name="snake">所持者のポインタ</param>
	EnemySnakeSelectState(EnemySnake* snake) : EnemySnakeHierarchicalState(snake) {};

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
	: public EnemySnakeHierarchicalState
{
private:

public:

	/// <summary>
	/// ヘビ(敵)の受動を管理する
	/// </summary>
	/// <param name="snake">所持者のポインタ</param>
	EnemySnakeReactionState(EnemySnake* snake) : EnemySnakeHierarchicalState(snake) {};

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
	:public EnemySnakeHierarchicalState
{
private:

public:

	/// <summary>
	/// MetaAIからメッセージを受信したとき
	/// </summary>
	/// <param name="snake">所持者のポインタ</param>
	EnemySnakeReceiveState(EnemySnake* snake) : EnemySnakeHierarchicalState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	/// <summary>
	/// 1マス移動する
	/// </summary>
	/// <param name="snake"></param>
	EnemySnakeMoveState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	/// <summary>
	/// ヘビ(敵)の敵を攻撃する
	/// </summary>
	/// <param name="snake"></param>
	EnemySnakeAttackState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	EnemySnakeAbilityState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	EnemySnakeUseItemState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	EnemySnakeDamagedState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	EnemySnakeDeathState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	EnemySnakeWaitState(EnemySnake* snake) : EnemySnakeState(snake) {};

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
	:public EnemySnakeState
{
private:

public:

	EnemySnakeCalledState(EnemySnake* snake) : EnemySnakeState(snake) {};

	virtual ~EnemySnakeCalledState() {};

	// ステートに入った時のメソッド
	void Enter()override;

	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;

	// ステートから出ていくときのメソッド
	void Exit()override;

};