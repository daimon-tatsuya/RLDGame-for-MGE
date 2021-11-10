#include <imgui.h>
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/EnemyManager.h"


EnemyManager::~EnemyManager()
{
	this->Clear();
}
// 更新処理
void EnemyManager::Update(float elapsed_time)
{
	for (auto& enemy : enemies)
	{
		enemy->Update(elapsed_time);
	}

	// 破棄処理
	//    characteresの範囲for文中でerase()すると不具合が発生してしまうため、
	// 　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (auto& enemy : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<EnemyBase*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// 削除
		delete enemy;
	}
	// 破棄リストをクリア
	removes.clear();

	// キャラクター同士の衝突処理
	CollisionCharacterToCharacter();
}

// 描画処理
void EnemyManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader)
{
	for (auto& enemies : enemies)
	{
		enemies->Render(context, shader);
	}
}

// デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

// デバッグ用GUI描画
void EnemyManager::DrawDebugGUI()
{
	for (auto& enemy: enemies)
	{
		enemy->DrawDebugGUI();
	}
}

// キャラクターの登録
void EnemyManager::Register(EnemyBase* character, int character_type)
{
	//登録するキャラクターが敵なら
	if (character_type >= static_cast<int>(Meta::Identity::Enemy))
	{
		// IDを設定
		character->SetId(enemy_number + static_cast<int>(Meta::Identity::Enemy));

		enemy_number++;//設定したらインクリメントする

		// 登録
		enemies.emplace_back(static_cast<EnemyBase*>(character));
	}
}

// キャラクターの全削除
void EnemyManager::Clear()
{
	for (auto& enemy : enemies)
	{
		if (enemy->GetId()==static_cast<int>(Meta::Identity::Player))
		{
			continue;
		}
		delete enemy;
	}
	enemies.clear();
	enemy_number = 0;
}

// キャラクターの削除
void EnemyManager::Remove(EnemyBase* character)
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
EnemyBase* EnemyManager::GetEnemyFromId(int id)
{
	for (auto& enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy;
	}
	return nullptr;
}


// キャラクター同士の衝突処理
void EnemyManager::CollisionCharacterToCharacter()
{
	size_t count = enemies.size();
	for (int i = 0; i < count; i++)
	{
		Character* characterA = enemies.at(i);
		for (int j = i + 1; j < count; j++)
		{
			Character* characterB = enemies.at(j);

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