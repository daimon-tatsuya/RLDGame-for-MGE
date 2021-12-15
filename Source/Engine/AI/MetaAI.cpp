//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include "Engine/Systems/Logger.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Character.h"
#include "Engine/AI/MetaAI.h"


Meta* Meta::instance = nullptr;

Meta& Meta::Instance()
{
	return *instance;
}

Meta::Meta(CharacterManager* character_manager)
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}
void Meta::Update()
{
	// ���^AI���Ď����Ă��鏈��

}

void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:Enemy Receive Failed");


	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error: Receive Failed");
	}
}

bool Meta::HandleMessage(const Telegram& msg)
{
	//���b�Z�[�W����M������
	if (OnMessage(msg))
	{
		return true;
	}
	return false;
}

bool Meta::OnMessage(const Telegram& telegram)
{
	std::vector<Character*> enemis;
	enemis.clear();
	CharacterManager& chracter_manager = CharacterManager::Instance();
	int enemy_count = character_manager->GetEnemyManager().GetEnemyCount();
	// �X�e�[�g�}�V���ɂł��邩���H
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:// �v���C���[�̃^�[�����I�����
		for (int i = 0; i < enemy_count; i++)
		{
			const int enemy_id = character_manager->GetEnemyManager().GetEnemy(i)->GetId();

			// �G�l�~�[�Ƀ��b�Z�[�W�𑗂�
			this->SendMessaging(
				static_cast<int>(Meta::Identity::Meta),
				enemy_id,
				MESSAGE_TYPE::MSG_END_PLAYER_TURN);
		}
		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		const int player_id = character_manager->GetPlayer()->GetId();

		// �G�l�~�[�Ƀ��b�Z�[�W�𑗂�
		this->SendMessaging(
			static_cast<int>(Meta::Identity::Meta),
			player_id,
			MESSAGE_TYPE::MSG_END_ENEMY_TURN);
		return true;
	}
	return false;
}


void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	// �L�����N�^�[�}�l�[�W���[
	CharacterManager& chracter_manager = CharacterManager::Instance();

	// ���b�Z�[�W��MetaAI��
	if (receiver == static_cast<int>(Meta::Identity::Meta))
	{
		// ���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}

	//���b�Z�[�W��MetaAI���瑼�I�u�W�F�N�g��

	// �v���C���[��
	else if (receiver == static_cast<int>(Meta::Identity::Player))
	{
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}

	// �G��
	else if (receiver >= static_cast<int>(Meta::Identity::Enemy))
	{

		// ��M�҂̃|�C���^���擾
		Character* receive_enemy = chracter_manager.GetEnemyManager().GetEnemyFromId(receiver);
		// ���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receive_enemy == nullptr)
		{ return; }
		// ���b�Z�[�W�f�[�^���쐬
		const Telegram telegram(sender, receiver, msg);
		// �f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receive_enemy, telegram);
	}
}