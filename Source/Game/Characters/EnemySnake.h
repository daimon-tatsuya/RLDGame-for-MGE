#pragma once
//**********************************************************
//
//		EnemySnakeクラス
//
//**********************************************************

#include "MyLiblary/Systems/Character.h"
#include "MyLiblary//Systems/StateMachine.h"

/// <summary>
/// 敵クラス(ヘビ)
/// </summary>
class EnemySnake :
	public Character
{
private:

public:

	//親ステート
	enum class ParentState :int
	{
		Entry = 0,
		Reaction,
		Receive,

		StateEnd
	};

	//子ステート
	enum class Entry :int
	{
		Select = 0,
		Approach,
		Explore,
		Attack,
		Ability,
		//UseItem,

		StateEnd
	};

	//子ステート
	enum class Reaction :int
	{
		ReactionSelect = 0,
		Damaged,
		Death,

		StateEnd
	};

	//子ステート
	enum class Receive
	{
		//		Wait,
		Called,
		StateEnd
	};

	StateMachine<ParentState, void, const float> enemy_snake_state_machine;	// 親ステート

	using EnemySnakeEntryState = StateMachine<Entry, void, const float>; // 行動ステートの宣言
	using EnemySnakeReactionState = StateMachine<Reaction, void, const float>; // 反応ステートの宣言
	using EnemySnakeReceiveState = StateMachine<Receive, void, const float>; // MetaAIからの指示待ちステートの宣言
	EnemySnakeEntryState      enemy_snake_entry_state;   // 行動ステート
	EnemySnakeReactionState enemy_snake_reaction_state;//反応ステート
	EnemySnakeReceiveState   enemy_snake_receive_state; // MetaAIからの指示待ちステート

private:

	//親ステート

	/// <summary>
	/// 行動(入力)ステート
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void EntryState(const float elapsed_time);

	/// <summary>
	/// HP関係の反応ステート
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void ReactionState(const float elapsed_time);

	/// <summary>
	///  MetaAIからの指示待ちステート
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void ReceiveState(const float elapsed_time);

	//子ステート

	//? EntryState

	/// <summary>
	/// <para>行動選択ステート</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void	SelectState(const float elapsed_time);

	/// <summary>
	/// <para>対象に近づくステート</para>
	/// <para>対象が近くにいるとき(5*5の範囲)簡単なアルゴリズムにする</para>
	/// <para>その他はA*</para>
	/// <param name="elapsed_time">経過時間</param>
	/// <summary>
	void ApproachState(const float elapsed_time);

	/// <summary>
	/// <para>マップを探索するステート</para>
	/// <para>通路は通路に沿って、A*</para>
	/// <para>部屋は出口に向かって、A*</para>
	/// </summary>
	/// <param name="elapsed_time"></param>
	void ExploreState(const float elapsed_time);

	/// <summary>
	/// <para>対象に攻撃するステート</para>
	/// </summary>
	/// <param name="elapsed_time"></param>
	void AttackState(const float elapsed_time);

	/// <summary>
	/// <para>対象にアビリティ発動するステート</para>
	///<para>基本AttackStateかAbilityStateが選択される(8対2の割合)</para>
	/// </summary>
	/// <param name="elapsed_time"></param>
	void AbilityState(const float elapsed_time);

	//? ReactionState

	/// <summary>
	/// <para>ダメージを受けた結果を判別する</para>
	/// <para>仮置き</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void ReactionSelectState(const float elapsed_time);

	/// <summary>
	/// <para>被ダメージステート</para>
	/// <para></para>
	/// <para></para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void DamagedState(const float elapsed_time);

	/// <summary>
	/// <para>死亡ステート</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void DeathState(const float elapsed_time);

	//? ReceiveState

	///// <summary>
	///// <para>待機ステート</para>
	///// </summary>
	///// <param name="elapsed_time">経過時間</param>
	void WaitState(const float elapsed_time);

	/// <summary>
	/// <para>オープンメニューステート</para>
	/// <para>MetaAIから送られてくる命令(メッセージ)によってステートを遷移させる</para>
	/// <para>仮置き</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>k
	void CalledState(const float elapsed_time);

protected:

public:

	//EnemySnake(RogueLikeDungeon* rogue_like_dungeon);

	EnemySnake();

	~EnemySnake() override;

	//更新処理
	void Update(float elapsed_time)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//有限ステートマシンの初期化
	void FiniteStateMachineInitialize() override;

	//キャラクターマネージャーのリストから自身を消去
	void Destroy() override;

	// デバッグエネミー情報表示
	void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged()override;

	// 死亡した時に呼ばれる
	void OnDead()override;

	//メッセージ受信処理
	bool OnMessage(const Telegram& telegram) override;

	//メタAIメタAIにターンの終了を伝える
	void SendMessaging(MESSAGE_TYPE msg) override;
};
