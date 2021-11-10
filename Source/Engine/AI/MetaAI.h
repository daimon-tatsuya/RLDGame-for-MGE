#pragma once

#include <set>
#include "Telegram.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/EnemyBase.h"
#include "Engine/Systems/PlayerBase.h"

//ToDo Metaが行うことを書く

class Meta final
{
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
private:
	// PlayerやEnemyとの通信機能(Messaging)
	// std::setは順序付けされたデータを複数保持することができる順序付集合
	std::set<Telegram> priority;

public:
	CharacterManager* character_manager = nullptr;

	enum class Identity : int
	{
		Meta = 0,
		Stage,
		Player,			// プレイヤーIDは2～5
		Camera = 6,
		Enemy = 7		// 7以降は全て敵
	};
};