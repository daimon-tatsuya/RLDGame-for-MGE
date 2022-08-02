#pragma once
//**********************************************************
//
//	Mathクラス
//
//**********************************************************

#include <set>

#include "MyLiblary/AI/Telegram.h"

// 前方宣言
class CharacterManager;
class Character;

//IDの列挙
	enum class Identity : int
	{
		Meta = 0,
		Stage,
		Camera,
		SceneManager,
		CharacterManager,

		Player,					// プレイヤーID
		Team,					// 仲間のID
		Enemy=8				// 以降の値は全て敵のID
	};

/// <summary>
/// メタAIクラス
/// </summary>
class Meta final
{
private:
	// プレイヤーや敵などとの通信機能(Messaging)
	// std::setは順序付けされたデータを複数保持することができる順序付集合
	std::set<Telegram> priority;

	static Meta* instance;

public:
private:

public:

	//コンストラクタ
	Meta();

	//デストラクタ
	~Meta() = default;

	// 唯一のインスタンス取得
	static Meta& Instance() { return *instance; }

	// 更新処理
	void Update();

	/// <summary>
	/// Characterを継承するクラスのメッセージレシーバーの処理を呼び出す。
	/// </summary>
	/// <param name="receiver">命令先</param>
	/// <param name="telegram">命令</param>
	void Discharge(Character* receiver, const Telegram& telegram);

	/// <summary>
	/// Identityから特定のクラスのメッセージレシーバーの処理を呼び出す。
	/// </summary>
	/// <param name="identity"></param>
	/// <param name="telegram">命令</param>
	void Discharge(int identity, const Telegram& telegram);

	/// <summary>
	/// MetaAI用のレシーブ処理を指定
	/// </summary>
	/// <param name="telegram">命令</param>
	void Discharge(const Telegram& telegram);

	/// <summary>
	///  メッセージ受信しているか
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns></returns>
	bool HandleMessage(const Telegram& telegram);

	/// <summary>
	/// メッセージ受信したときの処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns></returns>
	bool OnMessage(const Telegram& telegram);

	/// <summary>
	///  メッセージ送信関数
	/// </summary>
	/// <param name="sender">送信者</param>
	/// <param name="receiver">受信者</param>
	/// <param name="msg">命令</param>
	void SendMessaging(int sender, int receiver, MESSAGE_TYPE msg);

	// デバッグ用GUI描画
	void DrawDebugGUI() ;


};