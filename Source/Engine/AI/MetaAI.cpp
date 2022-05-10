//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include "Engine/Systems/Logger.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Character.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/DungeonSystem.h"
#include "Engine/Systems/Scene.h"

Meta* Meta::instance = nullptr;

Meta::Meta()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}
void Meta::Update()
{
	// ���^AI���Ď����Ă��鏈��

	const DungeonSystem dungeon_system = DungeonSystem::Instance();

	//���݂̌o�߃^�[�����ő�o�߃^�[���𒴂�����
	if (dungeon_system.GetElapsedTurn() > dungeon_system.GetMaxTurn())
	{
		LOG("Success : elapsed_turn is  over the max_turn | MetaAI.cpp")
	}
}

//	�L�����N�^�[�N���X�Ƀ��b�Z�[�W���M����
void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:Character Receive Failed | MetaAI.cpp")
	}
}

//
void Meta::Discharge(int identity, const Telegram& telegram)
{
	if (identity == static_cast<int>(Identity::CharacterManager))
	{
		CharacterManager& character_manager = CharacterManager::Instance();
		if (!character_manager.OnMessage(telegram))
		{
			// ��M�ł��Ȃ������Ƃ��̏���
			LOG("\n error:CharacterManager Receive Failed | MetaAI.cpp")
		}
	}
	if (identity == static_cast<int>(Identity::SceneManager))
	{
		const SceneManager& scene_manager = SceneManager::Instance();
		if (!scene_manager.GetCurrentScene()->OnMessage(telegram))
		{
			// ��M�ł��Ȃ������Ƃ��̏���
			LOG("\n error:CharacterManager Receive Failed | MetaAI.cpp")
		}
	}
	else
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:All Receive Failed | MetaAI.cpp")
	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error: Receive Failed | MetaAI.cpp")
	}
}

bool Meta::HandleMessage(const Telegram& telegram)
{
	//���b�Z�[�W����M������
	if (OnMessage(telegram))
	{
		return true;
	}
	return false;
}

bool Meta::OnMessage(const Telegram& telegram)
{
	const CharacterManager& character_manager = CharacterManager::Instance();
	const int player_id = character_manager.GetPlayer()->GetId();
	DungeonSystem& dungeon_system = DungeonSystem::Instance();

	// �X�e�[�g�}�V���ɂł��邩���H
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:	// �v���C���[�̃^�[�����I�����

		for (int i = 0; i < character_manager.GetEnemyCount(); ++i)
		{
			this->SendMessaging(
				static_cast<int>(Identity::Meta),
				character_manager.GetEnemy(i)->GetId(),
				MESSAGE_TYPE::END_PLAYER_TURN
			);

		}

		return true;
	case MESSAGE_TYPE::END_ENEMY_TURN:	// �G�̃^�[�����I�����



		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			player_id,
			MESSAGE_TYPE::END_ENEMY_TURN
		);

		//�^�[���̌o��
		dungeon_system.ElapseTurns();

		return true;

	case MESSAGE_TYPE::GOING_TO_NEXT_FLOOR:


	
		for (auto& enemy :character_manager.GetCharacters())
		{
			if (enemy->GetId() == static_cast<int>(Identity::Player))
			{
				continue;
			}
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				enemy->GetId(),
				MESSAGE_TYPE::GOING_TO_NEXT_FLOOR
			);

		}

		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			static_cast<int>(Identity::SceneManager),
			MESSAGE_TYPE::GOING_TO_NEXT_FLOOR
		);
		//�^�[���̌o��
		dungeon_system.ElapseTurns();
		return true;
	}

	return false;
}

void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	// �L�����N�^�[�}�l�[�W���[
	const CharacterManager& character_manager = CharacterManager::Instance();


	//--------------------���b�Z�[�W��MetaAI��----------------------------
	if (receiver == static_cast<int>(Identity::Meta))
	{
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}
	//------------------------------------------------------------------------


	//���b�Z�[�W��MetaAI���瑼�I�u�W�F�N�g��

	//---------------------�L�����N�^�[�}�l�[�W���[��----------------------
	else if (receiver == static_cast<int>(Identity::CharacterManager))
	{
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receiver, telegram);
	}
	//------------------------------------------------------------------------

	//---------------------�V�[���}�l�[�W���[��----------------------------
	if (receiver == static_cast<int>(Identity::SceneManager))
	{
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(static_cast<int>(Identity::SceneManager),telegram);
	}
	//------------------------------------------------------------------------

	// ---------------------------�v���C���[��---------------------------
	else if (receiver == static_cast<int>(Identity::Player))
	{
		// ��M�҂̃|�C���^���擾
		Character* receive_player = character_manager.GetCharacterFromId(receiver);
		// ���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receive_player == nullptr)
		{
			return;
		}
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_player, telegram);
	}
	//---------------------------------------------------------------------


	//------------------------------�G��--------------------------------------
	else if (receiver >= static_cast<int>(Identity::Enemy))
	{
		// ��M�҂̃|�C���^���擾
		Character* receive_enemy = character_manager.GetCharacterFromId(receiver);
		// ���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receive_enemy == nullptr)
		{
			return;
		}
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_enemy, telegram);
	}
	//------------------------------�G��---------------------------------------
}