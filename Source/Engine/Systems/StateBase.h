#pragma once

#include <memory>
#include <vector>

//前方宣言
class Character;
//全てステートクラスの基底クラス
class StateBase
{
private:

protected:

public:
	// コンストラクタ
	StateBase() {}
	virtual ~StateBase() {}

	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;

private:

protected:
//継承先で所持者のポインタを設定する

public:
};

// 親階層用のステート
class HierarchicalStateBase : public StateBase
{
public:
	// コンストラクタ
	HierarchicalStateBase() {}
	// デストラクタ
	virtual ~HierarchicalStateBase() {}
	// サブステート変更関数
	virtual void ChangeSubState(int newState);
	// サブステート登録関数
	virtual void RegisterSubState(StateBase* state);
	// サブステート取得関数
	virtual StateBase* GetSubState() { return sub_state; }
	// サブステート番号取得関数
	virtual int GetSubStateIndex();
	// サブステート設定関数
	virtual void SetSubState(int setState);
protected:
	// 各サブステートを保持する配列
	std::vector<StateBase*> sub_state_pool;
	// サブステート
	//std::unique_ptr<StateBase>sub_state = nullptr;
	StateBase* sub_state = nullptr;
};