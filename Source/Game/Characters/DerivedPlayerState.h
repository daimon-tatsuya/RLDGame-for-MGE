#pragma once
#include "Game/Characters/Player.h"
#include "Engine/Systems/StateBase.h"
class ParentState :
	public HierarchicalStateBase
{
private:
public:
	// コンストラクタ
	ParentState(Character* character) : HierarchicalStateBase(character) {}
	virtual ~ParentState() {}

	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

class ChildState :
	public StateBase
{
private:
public:
	// コンストラクタ
	ChildState(Character* character) : StateBase(character) {}
	virtual ~ChildState() {}

	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};
