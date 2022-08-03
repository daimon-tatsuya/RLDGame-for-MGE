//**********************************************************
//
//	   CharacterManagerクラス
//
//**********************************************************
#include "Liblary/Systems/CharacterManager.h"
#include "Liblary/Systems/Collision.h"
#include "Liblary/AI/MetaAI.h"
#include "Liblary/Systems/Math.h"
#include "Liblary/Systems/Shader.h"
#include "Liblary/Systems/Character.h"
#include "Liblary/Systems/Logger.h"

void CharacterManager::Update(float elapsed_time)
{
	for (const auto& character : characters)
	{
			character->Update(elapsed_time);
	}

	// ※enemiesの範囲for文の中でerase()すると不具合が発生してしまうため、
	// 更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (auto& character : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		auto it = std::ranges::find(characters, character);
		if (it != characters.end())
		{
			characters.erase(it);
		}

		// 削除
		delete character.get();
	}
	// 破棄リストをクリア
	removes.clear();
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
	if (character_type == static_cast<int>(Identity::Player))
	{
		// IDを設定
		character->SetId(team_number + static_cast<int>(Identity::Player));

		//team_number++;// 設定したらインクリメントする
	}

	// 登録するキャラクターが敵なら
	if (character_type == static_cast<int>(Identity::Enemy))
	{
		// IDを設定
		character->SetId(enemy_number + static_cast<int>(Identity::Enemy));

		enemy_number++;// 設定したらインクリメントする
	}
	// 登録
	characters.emplace_back(character);
}

bool CharacterManager::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:



		return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:



		return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:



		return true;

	case MESSAGE_TYPE::GO_MAX_FLOOR:



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
		if (character->GetId() == static_cast<int>(Identity::Player))
		{
			continue;
		}
		character.reset();
	}
	characters.clear();
	characters.shrink_to_fit();
	team_number = 0;
	enemy_number = 0;

	LOG(" Executed : CharacterManager's Clear Method\n")
}

void CharacterManager::Remove(Character* character)
{
	// 破棄リストにすでにあれば弾く
	for (const auto& it : removes)
	{
		if (it.get() == character)
			break;
	}
	// 破棄リストに追加
	removes.emplace_back(character);
}

void CharacterManager::RemoveEnemy() const
{
	for(const auto& enemy :characters)
	{
		//IDが敵でなければスキップ
		if (enemy->GetId()< static_cast<int>(Identity::Enemy)){continue;}

		//敵を削除用コンテナに入れる
		enemy->Destroy();
	}
}

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
		if (character->GetId() == static_cast<int>(Identity::Player))
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetEnemy(int index) const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Identity::Enemy) + index)
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