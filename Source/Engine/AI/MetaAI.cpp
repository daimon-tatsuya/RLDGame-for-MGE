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

Meta::Meta()
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
		LOG("\n error:Character Receive Failed")
	}
}

void Meta::Discharge(int identity, const Telegram& telegram)
{
	if (identity == static_cast<int>(Identity::CharacterManager))
	{
		CharacterManager& character_manager = CharacterManager::Instance();
		if (!character_manager.OnMessage(telegram))
		{
			// 受信できなかったときの処理
			LOG("\n error:CharacterManager Receive Failed")
		}
	}
	else
	{
		// 受信できなかったときの処理
		LOG("\n error:All Receive Failed")
	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// 受信できなかったときの処理
		LOG("\n error: Receive Failed")
	}
}

bool Meta::HandleMessage(const Telegram& telegram)
{
	//メッセージを受信した時
	if (OnMessage(telegram))
	{
		return true;
	}
	return false;
}

bool Meta::OnMessage(const Telegram& telegram)
{
	const CharacterManager& character_manager = CharacterManager::Instance();

	// ステートマシンにできるかも？
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:	// プレイヤーのターンが終わった
		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			static_cast<int>(Identity::CharacterManager),
			MESSAGE_TYPE::MSG_END_PLAYER_TURN
		);

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:	// 敵のターンが終わった

		const int player_id = character_manager.GetPlayer()->GetId();

		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			static_cast<int>(Identity::CharacterManager),
			MESSAGE_TYPE::MSG_END_ENEMY_TURN
		);
		return true;
	}

	return false;
}

void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	// キャラクターマネージャー
	const CharacterManager& character_manager = CharacterManager::Instance();

	// メッセージがMetaAI宛
	if (receiver == static_cast<int>(Identity::Meta))
	{
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(telegram);
	}

	//メッセージがMetaAIから他オブジェクト宛

	//キャラクターマネージャー宛
	else if (receiver == static_cast<int>(Identity::CharacterManager))
	{
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receiver,telegram);
	}

	//// プレイヤー宛
	//else if (receiver == static_cast<int>(Identity::Player))
	//{
	//	// メッセージデータを作成
	//	const Telegram telegram(sender, receiver, msg);
	//	// ディレイ無しメッセージ（即時配送メッセージ）
	//	Discharge(telegram);
	//}

	//// 敵宛
	//else if (receiver >= static_cast<int>(Identity::Enemy))
	//{
	//	// 受信者のポインタを取得
	//	Character* receive_enemy = character_manager.GetEnemy(receiver);
	//	// レシーバー居ないとき関数を終了する
	//	if (receive_enemy)
	//	{ return; }
	//	// メッセージデータを作成
	//	const Telegram telegram(sender, receiver, msg);
	//	// ディレイ無しメッセージ（即時配送メッセージ）
	//	Discharge(receive_enemy, telegram);
	//}
}