#pragma once
//**********************************************************
//
//		 StateBaseクラス
//		 HierarchicalStateBaseクラス
//
//**********************************************************
#include <memory>
#include <vector>

//前方宣言
class Character;

/// <summary>
/// 全てステートクラスの基底クラス
/// </summary>
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


/// <summary>
/// 親階層用のステートクラス
/// </summary>
class HierarchicalStateBase : public StateBase
{
public:

	// コンストラクタ
	HierarchicalStateBase() {}

	// デストラクタ
	virtual ~HierarchicalStateBase() {}

	// サブステート変更関数
	 void ChangeSubState(int newState);

	// サブステート登録関数
	 void RegisterSubState(StateBase* state);

	// サブステート番号取得関数
	 int GetSubStateIndex();

	// サブステート取得関数
	 StateBase* GetSubState() { return sub_state; }

	// サブステート設定関数
	 void SetSubState(int setState);

protected:

	// 各サブステートを保持する配列
	std::vector<StateBase*> sub_state_pool;

	// サブステート
	StateBase* sub_state = nullptr;
};