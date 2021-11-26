//**********************************************************
//
//		EnemyBaseクラス
//
//**********************************************************

#include "EnemyBase.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"

void EnemyBase::Destroy()
{
	CharacterManager::Instance().Remove(this);
}

//今回は使わない
void EnemyBase::CollisionEnemyToEnemy()
{

	//CharacterManager& character_manager = CharacterManager::Instance();

	//// 全ての敵と総当たりで衝突処理
	//int enemy_count = character_manager.GetEnemyCount();
	//for (int i = 0; i < enemy_count; ++i)
	//{
	//	Character* enemy = character_manager.GetEnemy(i);

	//	// 衝突処理
	//	DirectX::XMFLOAT3 out_enemy1_position, out_enemy2_position;
	//	if (Collision::IntersectSphereToSphere(
	//		position,
	//		radius,
	//		enemy->GetPosition(),
	//		enemy->GetRadius(),
	//		out_enemy1_position,
	//		out_enemy2_position)
	//		)
	//	{
	//		// 押し出し後の位置設定
	//		enemy->SetPosition(out_enemy2_position);
	//		this->SetPosition(out_enemy1_position);
	//	}
	//}
}
