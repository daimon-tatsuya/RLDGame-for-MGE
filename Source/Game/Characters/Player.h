#pragma once
//**********************************************************
//
//		Playerクラス
//
//**********************************************************

#include "Engine/Systems/Character.h"
#include "Engine/Systems/StateMachine.h"

///<summary>
///プレイヤークラス
///</summary>
class Player :
	public Character
{
private:
	// 親ステート
	enum class ParentState
	{
		Entry = 0,
		Reaction,
		Receive,

		StateEnd
	};

	// 子ステート
	enum class Entry
	{
		Select = 0,
		Attack,
		WayChange,
		Move,
		// 今後作る予定のステート
		//Map,
		//Ability,
		//MessageLog,
		//Inventory,
		//View,
		//Step,

		StateEnd
	};

	// 子ステート
	enum class Reaction
	{
		ReactionSelect = 0,
		Damaged,
		Death,

		StateEnd
	};

	// 子ステート
	enum class Receive
	{
		Wait,
	//	Call,

		StateEnd
	};

	StateMachine<ParentState, void, const float> player_state_machine;	// プレイヤーの親ステート

	using PlayerEntryState = StateMachine<Entry, void, const float>; // プレイヤーの子ステート
	using PlayerReactionState = StateMachine<Reaction, void, const float>; // プレイヤーの子ステート
	using PlayerReceiveState = StateMachine<Receive, void, const float>; // プレイヤーの子ステート
	PlayerEntryState      player_entry_state;   // 行動(入力)ステート
	PlayerReactionState  player_reaction_state;// HP関係の反応ステート
	PlayerReceiveState   player_receive_state; // MetaAIからの指示待ちステート

	std::vector<std::vector<std::string>> player_states_string;//ImGui用ステート名コンテナ
	std::vector<std::string> player_entry_string;
	std::vector<std::string> player_reaction_string;
	std::vector<std::string> player_receive_string;

public:

private:

	//--------------------------
	//親ステート
	//--------------------------

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

	//--------------------------
	//子ステート
	//--------------------------

	//?EntryState

	/// <summary>
	/// <para>行動選択ステート</para>
	/// <para>入力に応じて遷移するステートを変える</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void	SelectState(const float elapsed_time);

	/// <summary>
	/// <para>方向転換ステート</para>
	/// <para>ボタンと移動の同時入力で方向を変える</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void	WayChangeState(const float elapsed_time);

	/// <summary>
	/// <para>移動ステート</para>
	/// <para>移動の入力で移動</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void	MoveState(const float elapsed_time);

	/// <summary>
	/// <para>攻撃ステート</para>
	/// <para>プレイヤーの角度から攻撃方向</para>
	/// <para> または、プレイヤーの前方を攻撃するステート </para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void	AttackState(const float elapsed_time);

	/// <summary>
	/// <para>オープンメニューステート</para>
	/// <para>操作をメニューウィンドウに移す</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	//void	MenuState(const float elapsed_time);

	//?ReactionState

	/// <summary>
	/// <para>ダメージを受けた結果を判別する</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void ReactionSelectState(const float elapsed_time);

	/// <summary>
	/// <para>被ダメージステート</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void DamagedState(const float elapsed_time);

	/// <summary>
	/// <para>死亡ステート</para>
	/// <para>HPが0以下ならシーン遷移</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void DeathState(const float elapsed_time);

	//?ReceiveState

	/// <summary>
	/// <para>待機ステート</para>
	/// <para>メッセージ受信待ちのステート</para>
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void WaitState(const float elapsed_time);

public:

	//コンストラクタ
	Player();

	//デストラクタ
	~Player() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void Update(float elapsed_time) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dc"></param>
	/// <param name="shader">描画方法</param>
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) override;

	// 有限ステートマシンの初期化
	void FiniteStateMachineInitialize() override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged() override;

	// 死亡した時に呼ばれる
	void OnDead() override;

	/// <summary>
	/// メッセージ受信処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns>受信の有無</returns>
	bool OnMessage(const Telegram& telegram) override;

 	//メタAIにメッセージを送信する
	void SendMessaging(MESSAGE_TYPE msg) override;

	// デバッグ用GUI描画
	void DrawDebugGUI() override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	//キャラクターマネージャーのリストから自身を消去
	void Destroy() override;

	/// <summary>
	/// 移動しているかをチェック
	/// </summary>
	/// <returns>移動していたらtrue</returns>
	bool IsMoved();

	/// <summary>
	/// <para> ロード明けなど</para>
	/// <para>ゲーム開始時の最初の位置</para>
	/// </summary>
	void SetFirstPos();


};
