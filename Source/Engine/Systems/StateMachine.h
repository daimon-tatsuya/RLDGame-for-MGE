#pragma once

#include <vector>
#include <memory>
#include "Engine/Systems/StateBase.h"

//ToDo StateMachine コメント

//前方宣言
class Character;

class StateMachine
{
private:

protected:

public:
	// コンストラクタ
	StateMachine(Character* character) :owner(character) {}
	// デストラクタ
	~StateMachine();

	/// <summary>
	///  更新処理
	/// </summary>
	/// <param name="elapsedTime">経過時間</param>
	void Update(float elapsedTime);
	// ステートセット
	void SetState(int setState);
	// ステート変更
	void ChangeState(int newState);
	// ステート登録(登録するのは一層目)
	void RegisterState(HierarchicalStateBase* state);
	//現在のステート取得
	HierarchicalStateBase* GetState() { return current_state; }
	// サブステート登録を追加
	void StateMachine::RegisterSubState(int index, StateBase* subState);
	// 現在のステート番号取得
	int GetStateIndex();

private:
	Character* owner = nullptr; // 所持者
	HierarchicalStateBase* current_state = nullptr;	// 現在のステート
	std::vector<HierarchicalStateBase*> state_pool;	// 各ステートを保持する配列
protected:

public:
};
