//**********************************************************
//
//		Mathクラス
//
//**********************************************************

#include "Engine/Systems/Logger.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Character.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/DungeonSystem.h"

Meta* Meta::instance = nullptr;

Meta::Meta()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}
void Meta::Update()
{
	// メタAIが監視している処理
	const DungeonSystem dungeon_system=DungeonSystem::Instance();

	//現在の経過ターンが最大経過ターンを超えたら
	if (dungeon_system.GetElapsedTurn() > dungeon_system.GetMaxTurn())
	{
		LOG("Success : elapsed_turn is  over the max_turn")
	}
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

		for (int i=0;i <character_manager.GetEnemyCount();++i )
		{
			this->SendMessaging(
				static_cast<int>(Identity::Meta),
				character_manager.GetEnemy(i)->GetId(),
				MESSAGE_TYPE::MSG_END_PLAYER_TURN
			);

		}

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:	// 敵のターンが終わった

		const int player_id = character_manager.GetPlayer()->GetId();

		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			player_id,
			MESSAGE_TYPE::MSG_END_ENEMY_TURN
		);
		//ターンの経過
		DungeonSystem& dungeon_system= DungeonSystem::Instance();
		dungeon_system.TurnsElapse();

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
	else if (receiver == static_cast<int>(Identity::Player))
	{
		// 受信者のポインタを取得
		Character* receive_player = character_manager.GetCharacterFromId(receiver);
		// レシーバー居ないとき関数を終了する
		if (receive_player == nullptr)
		{return;}
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receive_player,telegram);
	}

	// 敵宛
	else if (receiver >= static_cast<int>(Identity::Enemy))
	{
		// 受信者のポインタを取得
		Character* receive_enemy = character_manager.GetCharacterFromId(receiver);
		// レシーバー居ないとき関数を終了する
		if (receive_enemy==nullptr)
		{ return; }
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receive_enemy, telegram);
	}
}