#pragma once
//**********************************************************
//
//		Mathクラス
//
//**********************************************************

#include <set>
#include "Engine/AI/Telegram.h"
#include "Engine/Systems/EnemyBase.h"
#include "Game/Characters/Player.h"

//前方宣言
class CharacterManager;

/// <summary>
/// メタAIクラス
/// </summary>
class Meta final
{
private:
	// PlayerやEnemyとの通信機能(Messaging)
	// std::setは順序付けされたデータを複数保持することができる順序付集合
	std::set<Telegram> priority;

	static Meta* instance;

public:
	CharacterManager* character_manager = nullptr;

	enum class Identity : int
	{
		Meta = 0,
		Stage,
		Player,			// プレイヤーIDは2
		Team,			//仲間のIDは3~5
		Camera = 6,
		Enemy = 7		// 7以降は全て敵
	};
private:

public:

	// コンストラクタ
	Meta(CharacterManager* c_manager);
	~Meta() {}

	// 唯一のインスタンス取得
	static Meta& Instance();

	// MetaAIの機能を追加していく
	// 更新処理
	void Update();

	// このメソッドはtelegramクラスを作成して
	// Characterを継承するクラスのメッセージレシーバーの処理を呼び出す。
	void Discharge(Character* receiver, const Telegram& telegram);

	// MetaAI用のレシーブ処理を指定
	void Discharge(const Telegram& telegram);

	// メッセージ受信しているか
	bool HandleMessage(const Telegram& msg);
	// メッセージ受信したときの処理

	bool OnMessage(const Telegram& msg);
	// メッセージ送信関数
	void SendMessaging(int sender, int receiver, MESSAGE_TYPE msg);

};