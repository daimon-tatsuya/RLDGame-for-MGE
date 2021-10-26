#include "EnemyBase.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"

void EnemyBase::Destroy()
{
	CharacterManager::Instance().Remove(this);
}

//void EnemyBase::DrawDebugPrimitive()
//{
//	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
//
//	// �Փ˔���p�f�o�b�O����`��
//	debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
//}

void EnemyBase::CollisionEnemyToEnemy()
{
	CharacterManager& character_manager = CharacterManager::Instance();

	// �S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemy_count = character_manager.GetEnemyCount();
	for (int i = 0; i < enemy_count; ++i)
	{
		Character* enemy = character_manager.GetEnemy(i);

		// �Փˏ���
		DirectX::XMFLOAT3 out_enemy1_position, out_enemy2_position;
		if (Collision::IntersectSphereToSphere(
			position,
			radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			out_enemy1_position,
			out_enemy2_position)
			)
		{
			// �����o����̈ʒu�ݒ�
			enemy->SetPosition(out_enemy2_position);
			this->SetPosition(out_enemy1_position);
		}
	}
}
