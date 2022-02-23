
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Math.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"

void CharacterManager::Update(float elapsed_time)
{
	for (const auto& character : characteres)
	{
		character->Update(elapsed_time);
	}

	// 破棄処理
	// characteresの範囲for文中でerase()すると不具合が発生してしまうため、
	// 更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (auto& character : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<std::shared_ptr<Character>>::iterator it = std::find(characteres.begin(), characteres.end(), character);
		if (it != characteres.end())
		{
			characteres.erase(it);
		}
		//delete character.get();
		character.reset();
	}
	// 破棄リストをクリア
	removes.clear();

}


void CharacterManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader)
{
	for (const auto& character : characteres)
	{
		character->Render(context, shader);
	}
}


void CharacterManager::DrawDebugPrimitive()
{
	for (auto& character : characteres)
	{
		character->DrawDebugPrimitive();
	}
}

void CharacterManager::DrawDebugGUI()
{
	for (const auto& character : characteres)
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
		character->SetId(player_number + static_cast<int>(Meta::Identity::Player));

		player_number++;// 設定したらインクリメントする

	}

	// 登録するキャラクターが敵なら
	if (character_type == static_cast<int>(Meta::Identity::Enemy))
	{
		// IDを設定
		character->SetId(enemy_number + static_cast<int>(Meta::Identity::Enemy));

		enemy_number++;// 設定したらインクリメントする
	}
	// 登録
	characteres.emplace_back(character);
}


void CharacterManager::Clear()
{
	for(auto& character:characteres)
	{
		if (character->GetId()==static_cast<int>(Meta::Identity::Player))
		{
			continue;
		}
		 character.reset();
	}
	characteres.clear();
	player_number = 0;
	enemy_number = 0;
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


Character* CharacterManager::GetCharacterFromId(int id)
{
	for (const auto& character : characteres)
	{
		if (character->GetId() == id)
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetPlayer(int number)
{
	for (const auto& character : characteres)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Player) + number)
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetEnemy(int index)
{
	for (const auto& character : characteres)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Enemy) + index)
			return character.get();
	}
	return nullptr;
}


void CharacterManager::CollisionCharacterToCharacter()
{
	const size_t count = characteres.size();
	for (int i = 0; i < count; i++)
	{
		Character* characterA = characteres.at(i).get();
		for (int j = i + 1; j < count; j++)
		{
			Character* characterB = characteres.at(j).get();

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
