
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Math.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"

CharacterManager::~CharacterManager()
{
	this->Clear();
}



void CharacterManager::Update(float elapsed_time)
{
	for (const auto& character : characteres)
	{
		character->Update(elapsed_time);
	}

	// �j������
	// characteres�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
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
//x	CollisionCharacterToCharacter();
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
	// �o�^����L�����N�^�[��	�v���C���[�Ȃ�
	if (character_type >= static_cast<int>(Meta::Identity::Player))
	{
		// ID��ݒ�
		character->SetId(player_number + static_cast<int>(Meta::Identity::Player));

		player_number++;// �ݒ肵����C���N�������g����

		// �o�^
		characteres.emplace_back(character);
	}

	// �o�^����L�����N�^�[���G�Ȃ�
	if (character_type >= static_cast<int>(Meta::Identity::Enemy))
	{
		// ID��ݒ�
		character->SetId(enemy_number + static_cast<int>(Meta::Identity::Enemy));

		enemy_number++;// �ݒ肵����C���N�������g����

		// �o�^
		characteres.emplace_back(character);
		enemy_manager.Register(character);
	}
}


void CharacterManager::Clear()
{
	for (const auto& character : characteres)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Player))
		{
			continue;
		}


		delete character;
	}
	characteres.clear();
	player_number = 0;
	enemy_number = 0;
}


void CharacterManager::Remove(Character* character)
{
	// �j�����X�g�ɂ��łɂ���Βe��
	for (const auto& it : removes)
	{
		if (it == character)
			break;
	}
	// �j�����X�g�ɒǉ�
	removes.emplace_back(character);
}


Character* CharacterManager::GetCharacterFromId(int id)
{
	for (const auto& character : characteres)
	{
		if (character->GetId() == id)
			return character;
	}
	return nullptr;
}

Character* CharacterManager::GetPlayer(int number)
{
	for (const auto& character : characteres)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Player) + number)
			return character;
	}
	return nullptr;
}


void CharacterManager::CollisionCharacterToCharacter()
{
	const size_t count = characteres.size();
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