#pragma once

//**********************************************************
//
//		EnemyManagerクラス
//
//**********************************************************


#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"


class EnemyManager
{
private:

	std::vector<Character*>	 enemies;// エネミーだけを格納する
	std::vector<Character*>	 removes; // 削除するEnemyBaseを格納するして, characteresのindexを指定して直接削除するのを回避
public:

private:

	EnemyManager() {}
	~EnemyManager();

public:

	//EnemyManagerの生成と削除は全てCharacterManagerから行う
	 friend class CharacterManager;

	// キャラクター同士の衝突処理
	//void CollisionEnemyToEnemy();

	//// 更新処理
	//void Update(float elapsed_time);

	//// 描画処理
	//void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// 敵のを全削除
	void Clear();

	//// デバッグプリミティブ描画
	//void DrawDebugPrimitive();

	//// デバッグ用GUI描画
	//void DrawDebugGUI();

	// 敵を登録
	void Register(Character* enemy_base);

	// 敵を削除
	void Remove(Character* enemy_base);

	// 敵を数取得
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// 敵配列を取得
	std::vector<Character*> GetEnemis() { return enemies; }

	// 敵を取得
	Character* GetEnemy(int index) { return enemies.at(index); }

	// IDから敵を取得
	Character* GetEnemyFromId(int id);

};
