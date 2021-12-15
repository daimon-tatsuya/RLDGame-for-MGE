//**********************************************************
//
//		Mathクラス
//
//**********************************************************

#include "Engine/Systems/Logger.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Character.h"
#include "Engine/AI/MetaAI.h"


Meta* Meta::instance = nullptr;

Meta& Meta::Instance()
{
	return *instance;
}

Meta::Meta(CharacterManager* character_manager)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}
void Meta::Update()
{
	// メタAIが監視している処理

}

void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		// 受信できなかったときの処理
		LOG("\n error:Enemy Receive Failed");


	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// 受信できなかったときの処理
		LOG("\n error: Receive Failed");
	}
}

bool Meta::HandleMessage(const Telegram& msg)
{
	//メッセージを受信した時
	if (OnMessage(msg))
	{
		return true;
	}
	return false;
}

bool Meta::OnMessage(const Telegram& telegram)
{
	std::vector<Character*> enemis;
	enemis.clear();
	CharacterManager& chracter_manager = CharacterManager::Instance();
	int enemy_count = character_manager->GetEnemyManager().GetEnemyCount();
	// ステートマシンにできるかも？
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:// プレイヤーのターンが終わった
		for (int i = 0; i < enemy_count; i++)
		{
			const int enemy_id = character_manager->GetEnemyManager().GetEnemy(i)->GetId();

			// エネミーにメッセージを送る
			this->SendMessaging(
				static_cast<int>(Meta::Identity::Meta),
				enemy_id,
				MESSAGE_TYPE::MSG_END_PLAYER_TURN);
		}
		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		const int player_id = character_manager->GetPlayer()->GetId();

		// エネミーにメッセージを送る
		this->SendMessaging(
			static_cast<int>(Meta::Identity::Meta),
			player_id,
			MESSAGE_TYPE::MSG_END_ENEMY_TURN);
		return true;
	}
	return false;
}


void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	// キャラクターマネージャー
	CharacterManager& chracter_manager = CharacterManager::Instance();

	// メッセージがMetaAI宛
	if (receiver == static_cast<int>(Meta::Identity::Meta))
	{
		// メッセージデータを作成
		Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(telegram);
	}

	//メッセージがMetaAIから他オブジェクト宛

	// プレイヤー宛
	else if (receiver == static_cast<int>(Meta::Identity::Player))
	{
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(telegram);
	}

	// 敵宛
	else if (receiver >= static_cast<int>(Meta::Identity::Enemy))
	{

		// 受信者のポインタを取得
		Character* receive_enemy = chracter_manager.GetEnemyManager().GetEnemyFromId(receiver);
		// レシーバー居ないとき関数を終了する
		if (receive_enemy == nullptr)
		{ return; }
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receive_enemy, telegram);
	}
}