//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include "Engine/AI/MetaAI.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DungeonSystem.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/SceneManager.h"

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

	//�o�߃^�[�����ő吔�𒴂�����
	if (dungeon_system.ExceededMaxTurn())
	{
		LOG("Success : elapsed_turn ecceed max_turn | MetaAI.cpp  Update\n")
	}
}

//	�L�����N�^�[�N���X�Ƀ��b�Z�[�W���M����
void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:Character Receive Failed | MetaAI.cpp  Character Discharge\n")
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
			LOG("\n error:CharacterManager Receive Failed | MetaAI.cpp  CharacterManager Discharge\n")
		}
	}
	if (identity == static_cast<int>(Identity::SceneManager))
	{
		const SceneManager& scene_manager = SceneManager::Instance();
		if (!scene_manager.GetCurrentScene()->OnMessage(telegram))
		{
			// ��M�ł��Ȃ������Ƃ��̏���
			LOG("\n error:CharacterManager Receive Failed | MetaAI.cpp  SceneManager Discharge\n")
		}
	}
	else
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error:All Receive Failed | MetaAI.cpp  identity Discharge\n")
	}
}

void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		// ��M�ł��Ȃ������Ƃ��̏���
		LOG("\n error: Receive Failed | MetaAI.cpp  Discharge\n")
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

		//�G��
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


		//	�v���C���[��
		this->SendMessaging
		(
			static_cast<int>(Identity::Meta),
			player_id,
			MESSAGE_TYPE::END_ENEMY_TURN
		);

		//�^�[���̌o��
		dungeon_system.ElapseTurns();

		return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		//------------------------------�G��--------------------------------------
		for (auto& enemy : character_manager.GetCharacters())
		{
			if (enemy->GetId() == static_cast<int>(Identity::Player))
			{
				continue;
			}
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				enemy->GetId(),
				MESSAGE_TYPE::GO_NEXT_FLOOR
			);

		}
		//-------------------------------------------------------------------------
		//---------------------------�V�[���}�l�[�W���[��-----------------------
		//���݂̊K�����ő吔�ɓ��B������
		if (dungeon_system.BreakingThroughTopFloor())
		{
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				static_cast<int>(Identity::SceneManager),
				MESSAGE_TYPE::GO_MAX_FLOOR
			);
			LOG("Success : current_floor reach max_floor | MetaAI.cpp  Update\n")
		}
		else
		{
			this->SendMessaging
			(
				static_cast<int>(Identity::Meta),
				static_cast<int>(Identity::SceneManager),
				MESSAGE_TYPE::GO_NEXT_FLOOR
			);
		}
		//-------------------------------------------------------------------------

		//�^�[���̌o��
		//���b�Z�[�W�𑗐M������ŊK��i�܂���
		dungeon_system.ElapseCurrentFloor();

		return true;
	default:
		break;

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
		Discharge(static_cast<int>(Identity::SceneManager), telegram);
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
	//-------------------------------------------------------------------------
}

void Meta::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("MetaAI", nullptr, ImGuiWindowFlags_None))
	{

		if (ImGui::CollapsingHeader("DungeonSystem", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text(" elapsed_turn : %d", DungeonSystem::Instance().GetMaxTurn());
			ImGui::Text(" elapsed_turn : %d", DungeonSystem::Instance().GetElapsedTurn());
			ImGui::Text(" current_floor : %d", DungeonSystem::Instance().GetMaxFloor());
			ImGui::Text(" current_floor : %d", DungeonSystem::Instance().GetCurrentFloor());
		}

	}
	ImGui::End();
}