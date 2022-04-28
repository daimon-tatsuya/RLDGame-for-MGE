//**********************************************************
//
//	   CharacterManagerクラス
//
//**********************************************************
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Math.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Game/Characters/EnemySnake.h"
#include "Game/Characters/Player.h"

void CharacterManager::Update(float elapsed_time) const
{
	for (const auto& character : characters)
	{
			character->Update(elapsed_time);
	}
}

void CharacterManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader) const
{
	for (const auto& character : characters)
	{
		character->Render(context, shader);
	}
}

void CharacterManager::DrawDebugPrimitive() const
{
	for (auto& character : characters)
	{
		character->DrawDebugPrimitive();
	}
}

void CharacterManager::DrawDebugGUI() const
{
	for (const auto& character : characters)
	{
		character->DrawDebugGUI();
	}
}

void CharacterManager::Register(Character* character, int character_type)
{
	// 登録するキャラクターが	プレイヤーなら
	if (character_type == static_cast<int>(Meta::Identity::Player))
	{
		// IDを設定
		character->SetId(team_number + static_cast<int>(Meta::Identity::Player));

		//team_number++;// 設定したらインクリメントする
	}

	// 登録するキャラクターが敵なら
	if (character_type == static_cast<int>(Meta::Identity::Enemy))
	{
		// IDを設定
		character->SetId(enemy_number + static_cast<int>(Meta::Identity::Enemy));

		enemy_number++;// 設定したらインクリメントする
	}
	// 登録
	characters.emplace_back(character);
}

bool CharacterManager::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:


		return true;
	default:
		break;
	}
	return false;
}

void CharacterManager::Clear()
{
	for (auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Player))
		{
			continue;
		}
		character.reset();
	}
	characters.clear();
	team_number = 0;
	enemy_number = 0;
}

//void CharacterManager::Remove(RogueLikeGameCharacter* character)
//{
//	// 破棄リストにすでにあれば弾く
//	for (const auto& it : removes)
//	{
//		if (it.get() == character)
//			break;
//	}
//	// 破棄リストに追加
//	removes.emplace_back(character);
//}

Character* CharacterManager::GetCharacterFromId(int id) const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == id)
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetPlayer() const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Player))
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetEnemy(int index) const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Enemy) + index)
			return character.get();
	}
	return nullptr;
}

void CharacterManager::CollisionCharacterToCharacter() const
{
	const size_t count = characters.size();
	for (int i = 0; i < count; i++)
	{
		Character* characterA = characters.at(i).get();
		for (int j = i + 1; j < count; j++)
		{
			Character* characterB = characters.at(j).get();

			DirectX::XMFLOAT3 out_positionA, out_positionB;

			if (Collision::IntersectSphereToSphere(
				characterA->GetPosition(),
				characterA->GetRadius(),
				characterB->GetPosition(),
				characterB->GetRadius(),
				out_positionA,
				out_positionB))
			{
				characterA->SetPosition(out_positionA);
				characterB->SetPosition(out_positionB);
			}
		}
	}
}