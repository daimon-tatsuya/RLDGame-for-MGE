//**********************************************************
//
//	   CharacterManager�N���X
//
//**********************************************************
#include "Liblary/Systems/CharacterManager.h"
#include "Liblary/Systems/Collision.h"
#include "Liblary/AI/MetaAI.h"
#include "Liblary/Systems/Math.h"
#include "Liblary/Systems/Shader.h"
#include "Liblary/Systems/Character.h"
#include "Liblary/Systems/Logger.h"

void CharacterManager::Update(float elapsed_time)
{
	for (const auto& character : characters)
	{
			character->Update(elapsed_time);
	}

	// ��enemies�͈̔�for���̒���erase()����ƕs����������Ă��܂����߁A
	// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (auto& character : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		auto it = std::ranges::find(characters, character);
		if (it != characters.end())
		{
			characters.erase(it);
		}

		// �폜
		delete character.get();
	}
	// �j�����X�g���N���A
	removes.clear();
}

void CharacterManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader) const
{
	for (const auto& character : characters)
	{
		character->Render(context, shader);
	}
}

void CharacterManager::DrawDebugPrimitive() const
{
	for (auto& character : characters)
	{
		character->DrawDebugPrimitive();
	}
}

void CharacterManager::DrawDebugGUI() const
{
	for (const auto& character : characters)
	{
		character->DrawDebugGUI();
	}
}

void CharacterManager::Register(Character* character, int character_type)
{
	// �o�^����L�����N�^�[��	�v���C���[�Ȃ�
	if (character_type == static_cast<int>(Identity::Player))
	{
		// ID��ݒ�
		character->SetId(team_number + static_cast<int>(Identity::Player));

		//team_number++;// �ݒ肵����C���N�������g����
	}

	// �o�^����L�����N�^�[���G�Ȃ�
	if (character_type == static_cast<int>(Identity::Enemy))
	{
		// ID��ݒ�
		character->SetId(enemy_number + static_cast<int>(Identity::Enemy));

		enemy_number++;// �ݒ肵����C���N�������g����
	}
	// �o�^
	characters.emplace_back(character);
}

bool CharacterManager::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:



		return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:



		return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:



		return true;

	case MESSAGE_TYPE::GO_MAX_FLOOR:



		return true;

	default:
		break;
	}
	return false;
}

void CharacterManager::Clear()
{
	for (auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Identity::Player))
		{
			continue;
		}
		character.reset();
	}
	characters.clear();
	characters.shrink_to_fit();
	team_number = 0;
	enemy_number = 0;

	LOG(" Executed : CharacterManager's Clear Method\n")
}

void CharacterManager::Remove(Character* character)
{
	// �j�����X�g�ɂ��łɂ���Βe��
	for (const auto& it : removes)
	{
		if (it.get() == character)
			break;
	}
	// �j�����X�g�ɒǉ�
	removes.emplace_back(character);
}

void CharacterManager::RemoveEnemy() const
{
	for(const auto& enemy :characters)
	{
		//ID���G�łȂ���΃X�L�b�v
		if (enemy->GetId()< static_cast<int>(Identity::Enemy)){continue;}

		//�G���폜�p�R���e�i�ɓ����
		enemy->Destroy();
	}
}

Character* CharacterManager::GetCharacterFromId(int id) const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == id)
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetPlayer() const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Identity::Player))
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetEnemy(int index) const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Identity::Enemy) + index)
			return character.get();
	}
	return nullptr;
}

void CharacterManager::CollisionCharacterToCharacter() const
{
	const size_t count = characters.size();
	for (int i = 0; i < count; i++)
	{
		Character* characterA = characters.at(i).get();
		for (int j = i + 1; j < count; j++)
		{
			Character* characterB = characters.at(j).get();

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