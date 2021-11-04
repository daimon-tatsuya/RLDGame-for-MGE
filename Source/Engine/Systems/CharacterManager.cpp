#include <imgui.h>
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Math.h"

CharacterManager::~CharacterManager()
{
	this->Clear();
}


// 更新処理
void CharacterManager::Update(float elapsed_time)
{
	for (auto& character : characteres)
	{
		character->Update(elapsed_time);
	}

	// 破棄処理
	//    characteresの範囲for文中でerase()すると不具合が発生してしまうため、
	// 　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (auto& character : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Character*>::iterator it = std::find(characteres.begin(), characteres.end(), character);
		if (it != characteres.end())
		{
			characteres.erase(it);
		}

		// 削除
		delete character;
	}
	// 破棄リストをクリア
	removes.clear();

	// キャラクター同士の衝突処理
	CollisionCharacterToCharacter();
}

// 描画処理
void CharacterManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader)
{
	for (auto& character : characteres)
	{
		character->Render(context, shader);
	}
}

// デバッグプリミティブ描画
void CharacterManager::DrawDebugPrimitive()
{
	for (auto& character : characteres)
	{
		character->DrawDebugPrimitive();
	}
}

// デバッグ用GUI描画
void CharacterManager::DrawDebugGUI()
{
	for (auto& character : characteres)
	{
		character->DrawDebugGUI();
	}
}

// キャラクターの登録
void CharacterManager::Register(Character* character, int character_type)
{
	if (character_type >= static_cast<int>(Meta::Identity::Player))
	{
		// IDを設定
		character->SetId(player_identity + static_cast<int>(Meta::Identity::Player));

		player_identity++;//設定したらインクリメントする

		// 登録
		characteres.emplace_back(character);
	}

	//登録するキャラクターが敵なら
	if (character_type >= static_cast<int>(Meta::Identity::Enemy))
	{
		// IDを設定
		character->SetId(enemy_identity + static_cast<int>(Meta::Identity::Enemy));

		enemy_identity++;//設定したらインクリメントする

		// 登録
		characteres.emplace_back(character);
		enemies.emplace_back(static_cast<EnemyBase*>(character));
	}
}

// キャラクターの全削除
void CharacterManager::Clear()
{
	for (auto& character : characteres)
	{
		if (character->GetId()==static_cast<int>(Meta::Identity::Player))
		{
			continue;
		}
		delete character;
	}
	characteres.clear();
	player_identity = 0;
	enemy_identity = 0;
}

// キャラクターの削除
void CharacterManager::Remove(Character* character)
{
	// 破棄リストにすでにあれば弾く
	for (auto& it : removes)
	{
		if (it == character)
			break;
	}
	// 破棄リストに追加
	removes.emplace_back(character);
}

// IDからキャラクターを取得する
Character* CharacterManager::GetCharacterFromId(int id)
{
	for (auto& character : characteres)
	{
		if (character->GetId() == id)
			return character;
	}
	return nullptr;
}

// キャラクター同士の衝突処理
void CharacterManager::CollisionCharacterToCharacter()
{
	size_t count = characteres.size();
	for (int i = 0; i < count; i++)
	{
		Character* characterA = characteres.at(i);
		for (int j = i + 1; j < count; j++)
		{
			Character* characterB = characteres.at(j);

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