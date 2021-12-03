//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include "Engine/Systems/Logger.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/CharacterManager.h"

Meta* Meta::instance = nullptr;

Meta& Meta::Instance()
{
	return *instance;
}

Meta::Meta(CharacterManager* c_manager)
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}
void Meta::Update()
{
	//���^AI���Ď����Ă��鏈��

}

// ���̃��\�b�h��telegram�N���X���쐬����
// Character���p������N���X�̃��b�Z�[�W���V�[�o�[�̏������Ăяo���B
void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		//��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Enemy Receive Failed");

	}
}

// ���^AI�̃��V�[�o�[�������Ăяo��
void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		//��M�ł��Ȃ������Ƃ��̏���
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}
// ���b�Z�[�W��M���Ă��邩
bool Meta::HandleMessage(const Telegram& msg)
{
	// ���b�Z�[�W����M������
	if (OnMessage(msg))
	{
		return true;
	}
	return false;
}
// ���b�Z�[�W��M�����Ƃ��̏���
bool Meta::OnMessage(const Telegram& telegram)
{
	std::vector<Character*> enemis;
	enemis.clear();
	CharacterManager& chracter_manager = CharacterManager::Instance();
	int enemy_count = character_manager->GetEnemyManager().GetEnemyCount();
	//�X�e�[�g�}�V���ɂł��邩���H
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN://�v���C���[�̃^�[�����I�����
		for (int i = 0; i < enemy_count; i++)
		{

			int enemy_id = character_manager->GetEnemyManager().GetEnemy(i)->GetId();

			//�G�l�~�[�Ƀ��b�Z�[�W�𑗂�
			this->SendMessaging(
				static_cast<int>(Meta::Identity::Meta),
				enemy_id,
				MESSAGE_TYPE::MSG_END_PLAYER_TURN);
		}
		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		int player_id = character_manager->GetPlayer()->GetId();

		//�G�l�~�[�Ƀ��b�Z�[�W�𑗂�
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
	//�L�����N�^�[�}�l�[�W���[
	CharacterManager& chracter_manager = CharacterManager::Instance();

	// ���b�Z�[�W��MetaAI��
	if (receiver == static_cast<int>(Meta::Identity::Meta))
	{
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, msg);
		//�f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}

	//���b�Z�[�W��MetaAI���瑼�I�u�W�F�N�g��
	//�v���C���[��
	else if (receiver == static_cast<int>(Meta::Identity::Player))
	{
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, msg);
		//�f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(telegram);
	}

	//�G��
	else if (receiver >= static_cast<int>(Meta::Identity::Enemy))
	{

		// ��M�҂̃|�C���^���擾
		Character* receiveEnemy = chracter_manager.GetEnemyManager().GetEnemyFromId(receiver);
		//���V�[�o�[���Ȃ��Ƃ��֐����I������
		if (receiveEnemy == nullptr)
		{ return; }
		//���b�Z�[�W�f�[�^���쐬
		Telegram telegram(sender, receiver, msg);
		//�f�B���C�������b�Z�[�W�i�����z�����b�Z�[�W�j
		Discharge(receiveEnemy, telegram);
	}
}