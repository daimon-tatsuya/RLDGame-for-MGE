#include <imgui.h>
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/EnemyManager.h"


EnemyManager::~EnemyManager()
{
	this->Clear();
}
// �X�V����
void EnemyManager::Update(float elapsed_time)
{
	for (auto& enemy : enemies)
	{
		enemy->Update(elapsed_time);
	}

	// �j������
	//    characteres�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (auto& enemy : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<EnemyBase*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// �폜
		delete enemy;
	}
	// �j�����X�g���N���A
	removes.clear();

	// �L�����N�^�[���m�̏Փˏ���
	CollisionCharacterToCharacter();
}

// �`�揈��
void EnemyManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader)
{
	for (auto& enemies : enemies)
	{
		enemies->Render(context, shader);
	}
}

// �f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

// �f�o�b�O�pGUI�`��
void EnemyManager::DrawDebugGUI()
{
	for (auto& enemy: enemies)
	{
		enemy->DrawDebugGUI();
	}
}

// �L�����N�^�[�̓o�^
void EnemyManager::Register(EnemyBase* character, int character_type)
{
	//�o�^����L�����N�^�[���G�Ȃ�
	if (character_type >= static_cast<int>(Meta::Identity::Enemy))
	{
		// ID��ݒ�
		character->SetId(enemy_number + static_cast<int>(Meta::Identity::Enemy));

		enemy_number++;//�ݒ肵����C���N�������g����

		// �o�^
		enemies.emplace_back(static_cast<EnemyBase*>(character));
	}
}

// �L�����N�^�[�̑S�폜
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

// �L�����N�^�[�̍폜
void EnemyManager::Remove(EnemyBase* character)
{
	// �j�����X�g�ɂ��łɂ���Βe��
	for (auto& it : removes)
	{
		if (it == character)
			break;
	}
	// �j�����X�g�ɒǉ�
	removes.emplace_back(character);
}

// ID����L�����N�^�[���擾����
EnemyBase* EnemyManager::GetEnemyFromId(int id)
{
	for (auto& enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy;
	}
	return nullptr;
}


// �L�����N�^�[���m�̏Փˏ���
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