#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Math.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Game/Characters/EnemySnake.h"
#include "Game/Characters/Player.h"

void CharacterManager::Update(float elapsed_time) const
{
	for (const auto& character : characters)
	{
		if( character->GetId() == static_cast<int>(Meta::Identity::Player)&& is_player_turn==true  )//�v���C���[���v���C���[�̃^�[���Ȃ�
		{
			character->Update(elapsed_time);
		}

		if (is_enemy_turn==true && character->GetId()>= static_cast<int>(Meta::Identity::Enemy))//�G�̃^�[�����G�Ȃ�
		{
			character->Update(elapsed_time);
		}

	}

	//// �j������
	//// characteres�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	//// �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	//for (std::shared_ptr<Character> character : removes)
	//{
	//	// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
	//	std::vector<std::shared_ptr<Character>>::iterator it = std::find(characters.begin(), characters.end(), character);
	//	if (it != characters.end())
	//	{
	//		characters.erase(it);
	//	}
	//	//delete character.get();
	//	//character.reset();
	//}
	//// �j�����X�g���N���A
	//removes.clear();
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

void CharacterManager::DrawDebugGUI()
{
	for (const auto& character : characters)
	{
		character->DrawDebugGUI();
	}
}

void CharacterManager::Register(Character* character, int character_type)
{
	// �o�^����L�����N�^�[��	�v���C���[�Ȃ�
	if (character_type == static_cast<int>(Meta::Identity::Player))
	{
		// ID��ݒ�
		character->SetId(team_number + static_cast<int>(Meta::Identity::Player));

		//team_number++;// �ݒ肵����C���N�������g����
	}

	// �o�^����L�����N�^�[���G�Ȃ�
	if (character_type == static_cast<int>(Meta::Identity::Enemy))
	{
		// ID��ݒ�
		character->SetId(enemy_number + static_cast<int>(Meta::Identity::Enemy));

		enemy_number++;// �ݒ肵����C���N�������g����
	}
	// �o�^
	characters.emplace_back(character);
}

bool CharacterManager::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:
		is_player_turn = false;
		is_enemy_turn = true;

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		is_player_turn = true;
		is_enemy_turn = false;

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
		if (character->GetId() == static_cast<int>(Meta::Identity::Player))
		{
			continue;
		}
		character.reset();
	}
	characters.clear();
	team_number = 0;
	enemy_number = 0;
}

//void CharacterManager::Remove(Character* character)
//{
//	// �j�����X�g�ɂ��łɂ���Βe��
//	for (const auto& it : removes)
//	{
//		if (it.get() == character)
//			break;
//	}
//	// �j�����X�g�ɒǉ�
//	removes.emplace_back(character);
//}

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
		if (character->GetId() == static_cast<int>(Meta::Identity::Player))
			return character.get();
	}
	return nullptr;
}

Character* CharacterManager::GetEnemy(int index) const
{
	for (const auto& character : characters)
	{
		if (character->GetId() == static_cast<int>(Meta::Identity::Enemy) + index)
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