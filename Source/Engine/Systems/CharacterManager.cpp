#include <imgui.h>
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Math.h"

CharacterManager::~CharacterManager()
{
	this->Clear();
}


// �X�V����
void CharacterManager::Update(float elapsed_time)
{
	for (auto& character : characteres)
	{
		character->Update(elapsed_time);
	}

	// �j������
	//    characteres�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (auto& character : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Character*>::iterator it = std::find(characteres.begin(), characteres.end(), character);
		if (it != characteres.end())
		{
			characteres.erase(it);
		}

		// �폜
		delete character;
	}
	// �j�����X�g���N���A
	removes.clear();

	// �L�����N�^�[���m�̏Փˏ���
	CollisionCharacterToCharacter();
}

// �`�揈��
void CharacterManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader)
{
	for (auto& character : characteres)
	{
		character->Render(context, shader);
	}
}

// �f�o�b�O�v���~�e�B�u�`��
void CharacterManager::DrawDebugPrimitive()
{
	for (auto& character : characteres)
	{
		character->DrawDebugPrimitive();
	}
}

// �f�o�b�O�pGUI�`��
void CharacterManager::DrawDebugGUI()
{
	for (auto& character : characteres)
	{
		character->DrawDebugGUI();
	}
}

// �L�����N�^�[�̓o�^
void CharacterManager::Register(Character* character, int character_type)
{
	if (character_type >= static_cast<int>(Meta::Identity::Player))
	{
		// ID��ݒ�
		character->SetId(player_identity + static_cast<int>(Meta::Identity::Player));

		player_identity++;//�ݒ肵����C���N�������g����

		// �o�^
		characteres.emplace_back(character);
	}

	//�o�^����L�����N�^�[���G�Ȃ�
	if (character_type >= static_cast<int>(Meta::Identity::Enemy))
	{
		// ID��ݒ�
		character->SetId(enemy_identity + static_cast<int>(Meta::Identity::Enemy));

		enemy_identity++;//�ݒ肵����C���N�������g����

		// �o�^
		characteres.emplace_back(character);
		enemies.emplace_back(static_cast<EnemyBase*>(character));
	}
}

// �L�����N�^�[�̑S�폜
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

// �L�����N�^�[�̍폜
void CharacterManager::Remove(Character* character)
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
Character* CharacterManager::GetCharacterFromId(int id)
{
	for (auto& character : characteres)
	{
		if (character->GetId() == id)
			return character;
	}
	return nullptr;
}

// �L�����N�^�[���m�̏Փˏ���
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