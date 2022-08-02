//**********************************************************
//
//		Mathクラス
//
//**********************************************************

#include "Engine/AI/MetaAI.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DungeonSystem.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/SceneManager.h"

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

	const DungeonSystem dungeon_system = DungeonSystem::Instance();

	//経過ターンが最大数を超えたら
	if (dungeon_system.ExceededMaxTurn())
	{
		LOG("Success : elapsed_turn ecceed max_turn | MetaAI.cpp  Update\n")
	}
}

//	キャラクタークラスにメッセージ送信する
void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		// 受信できなかったときの処理
		LOG("\n error:Character Receive Failed | MetaAI.cpp  Character Discharge\n")
	}
}

//
void Meta::Discharge(int identity, const Telegram& telegram)
{
	if (identity == static_cast<int>(Identity::CharacterManager))
	{
		CharacterManager& character_manager = CharacterManager::Instance();
		if (!character_manager.OnMessage(telegram))
		{
			// 受信できなかったときの処理
			LOG("\n error:CharacterManager Receive Failed | MetaAI.cpp  CharacterManager Discharge\n")
		}
	}
	if (identity == static_cast<int>(Identity::SceneManager))
	{
		const SceneManager& scene_manager = SceneManager::Instance();
		if (!scene_manager.GetCurrentScene()->OnMessage(telegram))
		{
			// 受信できなかったときの処理
			LOG("\n error:CharacterManager Receive Failed | MetaAI.cpp  SceneManager Discharge\n")
		}
	}
	else
	{
		// 受信できなかったときの処理
		LOG("\n error:All Receive Failed | MetaAI.cpp  identity Discharge\n")
	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// 受信できなかったときの処理
		LOG("\n error: Receive Failed | MetaAI.cpp  Discharge\n")
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
	const int player_id = character_manager.GetPlayer()->GetId();
	DungeonSystem& dungeon_system = DungeonSystem::Instance();

	// ステートマシンにできるかも？
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:	// プレイヤーのターンが終わった

		//敵宛
		for (int i = 0; i < character_manager.GetEnemyCount(); ++i)
		{
			this->SendMessaging(
				static_cast<int>(Identity::Meta),
				character_manager.GetEnemy(i)->GetId(),
				MESSAGE_TYPE::END_PLAYER_TURN
			);

		}

		return true;
	case MESSAGE_TYPE::END_ENEMY_TURN:	// 敵のターンが終わった


		//	プレイヤー宛
		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			player_id,
			MESSAGE_TYPE::END_ENEMY_TURN
		);

		//ターンの経過
		dungeon_system.ElapseTurns();

		return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		//------------------------------敵宛--------------------------------------
		for (auto& enemy : character_manager.GetCharacters())
		{
			if (enemy->GetId() == static_cast<int>(Identity::Player))
			{
				continue;
			}
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				enemy->GetId(),
				MESSAGE_TYPE::GO_NEXT_FLOOR
			);

		}
		//-------------------------------------------------------------------------
		//---------------------------シーンマネージャー宛-----------------------
		//現在の階数が最大数に到達したら
		if (dungeon_system.BreakingThroughTopFloor())
		{
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				static_cast<int>(Identity::SceneManager),
				MESSAGE_TYPE::GO_MAX_FLOOR
			);
			LOG("Success : current_floor reach max_floor | MetaAI.cpp  Update\n")
		}
		else
		{
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				static_cast<int>(Identity::SceneManager),
				MESSAGE_TYPE::GO_NEXT_FLOOR
			);
		}
		//-------------------------------------------------------------------------

		//ターンの経過
		//メッセージを送信した後で階を進ませる
		dungeon_system.ElapseCurrentFloor();

		return true;
	default:
		break;

	}

	return false;
}

void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	// キャラクターマネージャー
	const CharacterManager& character_manager = CharacterManager::Instance();


	//--------------------メッセージがMetaAI宛----------------------------
	if (receiver == static_cast<int>(Identity::Meta))
	{
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(telegram);
	}
	//------------------------------------------------------------------------


	//メッセージがMetaAIから他オブジェクト宛

	//---------------------キャラクターマネージャー宛----------------------
	else if (receiver == static_cast<int>(Identity::CharacterManager))
	{
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receiver, telegram);
	}
	//------------------------------------------------------------------------

	//---------------------シーンマネージャー宛----------------------------
	if (receiver == static_cast<int>(Identity::SceneManager))
	{
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(static_cast<int>(Identity::SceneManager), telegram);
	}
	//------------------------------------------------------------------------

	// ---------------------------プレイヤー宛---------------------------
	else if (receiver == static_cast<int>(Identity::Player))
	{
		// 受信者のポインタを取得
		Character* receive_player = character_manager.GetCharacterFromId(receiver);
		// レシーバー居ないとき関数を終了する
		if (receive_player == nullptr)
		{
			return;
		}
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receive_player, telegram);
	}
	//---------------------------------------------------------------------


	//------------------------------敵宛--------------------------------------
	else if (receiver >= static_cast<int>(Identity::Enemy))
	{
		// 受信者のポインタを取得
		Character* receive_enemy = character_manager.GetCharacterFromId(receiver);
		// レシーバー居ないとき関数を終了する
		if (receive_enemy == nullptr)
		{
			return;
		}
		// メッセージデータを作成
		const Telegram telegram(sender, receiver, msg);
		// ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(receive_enemy, telegram);
	}
	//-------------------------------------------------------------------------
}

void Meta::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("MetaAI", nullptr, ImGuiWindowFlags_None))
	{

		if (ImGui::CollapsingHeader("DungeonSystem", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text(" elapsed_turn : %d", DungeonSystem::Instance().GetMaxTurn());
			ImGui::Text(" elapsed_turn : %d", DungeonSystem::Instance().GetElapsedTurn());
			ImGui::Text(" current_floor : %d", DungeonSystem::Instance().GetMaxFloor());
			ImGui::Text(" current_floor : %d", DungeonSystem::Instance().GetCurrentFloor());
		}

	}
	ImGui::End();
}