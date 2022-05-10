//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include "Engine/Systems/Logger.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Character.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/DungeonSystem.h"

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
	const DungeonSystem dungeon_system=DungeonSystem::Instance();

	//���݂̌o�߃^�[�����ő�o�߃^�[���𒴂�����
	if (dungeon_system.GetElapsedTurn() > dungeon_system.GetMaxTurn())
	{
		LOG("Success : elapsed_turn is  over the max_turn")
	}
}

void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:Character Receive Failed")
	}
}

void Meta::Discharge(int identity, const Telegram& telegram)
{
	if (identity == static_cast<int>(Identity::CharacterManager))
	{
		CharacterManager& character_manager = CharacterManager::Instance();
		if (!character_manager.OnMessage(telegram))
		{
			// ��M�ł��Ȃ������Ƃ��̏���
			LOG("\n error:CharacterManager Receive Failed")
		}
	}
	else
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:All Receive Failed")
	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error: Receive Failed")
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

	// �X�e�[�g�}�V���ɂł��邩���H
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:	// �v���C���[�̃^�[�����I�����

		for (int i=0;i <character_manager.GetEnemyCount();++i )
		{
			this->SendMessaging(
				static_cast<int>(Identity::Meta),
				character_manager.GetEnemy(i)->GetId(),
				MESSAGE_TYPE::MSG_END_PLAYER_TURN
			);

		}

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:	// �G�̃^�[�����I�����

		const int player_id = character_manager.GetPlayer()->GetId();

		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			player_id,
			MESSAGE_TYPE::MSG_END_ENEMY_TURN
		);
		//�^�[���̌o��
		DungeonSystem& dungeon_system= DungeonSystem::Instance();
		dungeon_system.TurnsElapse();

		return true;
	}

	return false;
}

void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	// �L�����N�^�[�}�l�[�W���[
	const CharacterManager& character_manager = CharacterManager::Instance();

	// ���b�Z�[�W��MetaAI��
	if (receiver == static_cast<int>(Identity::Meta))
	{
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}

	//���b�Z�[�W��MetaAI���瑼�I�u�W�F�N�g��

	//�L�����N�^�[�}�l�[�W���[��
	else if (receiver == static_cast<int>(Identity::CharacterManager))
	{
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receiver,telegram);
	}

	//// �v���C���[��
	else if (receiver == static_cast<int>(Identity::Player))
	{
		// ��M�҂̃|�C���^���擾
		Character* receive_player = character_manager.GetCharacterFromId(receiver);
		// ���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receive_player == nullptr)
		{return;}
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_player,telegram);
	}

	// �G��
	else if (receiver >= static_cast<int>(Identity::Enemy))
	{
		// ��M�҂̃|�C���^���擾
		Character* receive_enemy = character_manager.GetCharacterFromId(receiver);
		// ���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receive_enemy==nullptr)
		{ return; }
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_enemy, telegram);
	}
}