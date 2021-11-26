#pragma once

//**********************************************************
//
//		EnemyManagerクラス
//
//**********************************************************


#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/EnemyBase.h"

class EnemyManager
{
private:

	std::vector<EnemyBase*>	 enemies;// エネミーだけを格納する
	std::vector<EnemyBase*>	 removes; // 削除するEnemyBaseを格納するして, characteresのindexを指定して直接削除するのを回避

	int enemy_number = 0;	// 付与するIDの値(この値にMetaAI::Identity::Enemyを加算してIdを付与する)

public:

private:
	EnemyManager() {}
	~EnemyManager();
public:
	//EnemyManagerの生成と削除は全てCharacterManagerから行う
	 friend class CharacterManager;

	// キャラクター同士の衝突処理
	void CollisionEnemyToEnemy();

	// 更新処理
	void Update(float elapsed_time);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// 敵のを全削除
	void Clear();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

	// 敵を登録
	void Register(EnemyBase* enemy_base);

	// 敵を削除
	void Remove(EnemyBase* enemy_base);

	// 敵を数取得
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// 敵配列を取得
	std::vector<EnemyBase*> GetEnemis() { return enemies; }

	// 敵を取得
	EnemyBase* GetEnemy(int index) { return enemies.at(index); }

	// IDから敵を取得
	EnemyBase* GetEnemyFromId(int id);

	/// <summary>
	/// プレイヤーの取得
	/// </summary>
	/// <param name="number">プレイヤーの番号:0~3</param>
	/// <returns>number番目のPlayer</returns>
	//Character* GetPlayer(int number = 0);
};
