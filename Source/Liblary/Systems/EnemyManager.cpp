//**********************************************************
//
//	EnemyManagerクラス
//
//**********************************************************

#include "Liblary/Systems/EnemyManager.h"
#include "Liblary/Systems/Character.h"


EnemyManager::~EnemyManager()
{
	this->Clear();
}

// キャラクターの登録
void EnemyManager::Register(Character* character)
{
		// 登録
		enemies.emplace_back(character);
}

// キャラクターの全削除
void EnemyManager::Clear()
{
	for (const auto& enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

// キャラクターの削除
void EnemyManager::Remove(Character* character)
{
	// 破棄リストにすでにあれば弾く
	for (const auto& it : removes)
	{
		if (it == character)
			break;
	}
	// 破棄リストに追加
	removes.emplace_back(character);
}

// IDからキャラクターを取得する
Character* EnemyManager::GetEnemyFromId(int id)
{
	for (const auto& enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy;
	}
	return nullptr;
}
