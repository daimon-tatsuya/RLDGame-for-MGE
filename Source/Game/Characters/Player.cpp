//**********************************************************
//
//		Player�N���X
//
//**********************************************************

//#include <string>

#include  "Engine/AI/MetaAI.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Objects/Model.h"
#include "Engine/AI/DungeonMake.h"

#include "Game/Characters/Player.h"

#include "Engine/Systems/CharacterManager.h"

const float cos45 = cosf(DirectX::XMConvertToRadians(45.f));

//�R���X�g���N�^
Player::Player()
{
	SetModel("Assets/FBX/Animals/BlackWidow.bin");

	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	SetPositionY(0.f);//	����̃Q�[����y���ɓ����Ȃ�

	SetExist(true);

	// �X�e�[�g�}�V���̏�����
	//�R���X�g���N�^���̉��z�֐��̌Ăяo���̓R���p�C�����ɉ��������
	//�Ȃ̂ŃR���X�g���N�^���̉��z�֐��̌Ăяo�������I�Ɏw�肷��
	Player::FiniteStateMachineInitialize();

	//�ʒu�̐ݒ�
	SetFirstPos();

}

//�f�X�g���N�^
Player::~Player()
{
	LOG(" Executed : Player's destructor\n")
}

//	�X�V����
void Player::Update(const float elapsed_time)
{
	SetPositionY(0.f);//�W�����v�����Ȃ��̂ŌŒ�

	//�X�e�[�g�}�V���̍X�V
	player_state_machine.Update(elapsed_time);

	// ��]�p�̐��K��
	NormalizeAngle();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V����
	GetModel()->UpdateAnimation(elapsed_time);

	// ���f���s��X�V
	GetModel()->UpdateTransform(GetTransform());

	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	//�v���C���[�ƊK�i�̈ʒu���d�Ȃ�����
	if (static_cast<int>(GetPosition().x / CellSize) == rogue_like_dungeon.stairs_pos.x &&
		static_cast<int>(GetPosition().z) / CellSize == rogue_like_dungeon.stairs_pos.y)
	{
		SendMessaging(MESSAGE_TYPE::GO_NEXT_FLOOR);
	}
}

//�`�揈��
void Player::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, GetModel());
}

//�X�e�[�g�}�V�̏�����
void Player::FiniteStateMachineInitialize()
{
	//--------------------------
	//�e�X�e�[�g�̒ǉ�
	//--------------------------
	player_state_machine.AddState
	(
		ParentState::Entry,
		[this](const float elapsed_time) { EntryState(elapsed_time); }
	);

	player_state_machine.AddState
	(
		ParentState::Reaction,
		[this](const float elapsed_time) { ReactionState(elapsed_time); }
	);

	player_state_machine.AddState
	(
		ParentState::Receive,
		[this](const float elapsed_time) { ReceiveState(elapsed_time); }
	);
	//--------------------------
	//�q�X�e�[�g�̒ǉ�
	//--------------------------

	//EntryState�̎q�X�e�[�g

	player_entry_state.AddState
	(
		Entry::Select,
		[this](const float elapsed_time) {SelectState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Attack,
		[this](const float elapsed_time) {AttackState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::WayChange,
		[this](const float elapsed_time) {WayChangeState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Move,
		[this](const float elapsed_time) {MoveState(elapsed_time); }
	);

	//ReactionState�̎q�X�e�[�g

	player_reaction_state.AddState
	(
		Reaction::ReactionSelect,
		[this](const float elapsed_time) {ReactionSelectState(elapsed_time); }
	);

	player_reaction_state.AddState
	(
		Reaction::Damaged,
		[this](const float elapsed_time) {DamagedState(elapsed_time); }
	);

	player_reaction_state.AddState
	(
		Reaction::Death,
		[this](const float elapsed_time) {DeathState(elapsed_time); }
	);

	//Receive�̎q�X�e�[�g

	player_receive_state.AddState
	(
		Receive::Wait,
		[this](const float elapsed_time) {WaitState(elapsed_time); }
	);

	// �e�X�e�[�g�}�V���̏����X�e�[�g�̐ݒ�

	player_state_machine.SetState(ParentState::Entry);

	player_entry_state.SetState(Entry::Select);
	player_reaction_state.SetState(Reaction::ReactionSelect);
	player_receive_state.SetState(Receive::Wait);


	//ImGui
	player_entry_string.emplace_back("Select");
	player_entry_string.emplace_back("Attack");
	player_entry_string.emplace_back("WayChange");
	player_entry_string.emplace_back("Move");

	player_reaction_string.emplace_back("ReactionSelect");
	player_reaction_string.emplace_back("Damaged");
	player_reaction_string.emplace_back("Death");

	player_receive_string.emplace_back("Wait");

	player_states_string.emplace_back(player_entry_string);
	player_states_string.emplace_back(player_reaction_string);
	player_states_string.emplace_back(player_receive_string);
}

//	���^AI�o�R��M����
bool Player::OnMessage(const Telegram & telegram)
{
	const RogueLikeDungeon rogue_like_dungeon = RogueLikeDungeon::Instance();
	// ���^AI����̎�M����
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		LOG(" Error : No Function | Player.cpp\n")
			return false;

	case MESSAGE_TYPE::END_ENEMY_TURN:
		//�v���C���[
		player_state_machine.SetState(ParentState::Entry);

		return true;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		SetPositionY(0.f);
		const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x) * CellSize, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CellSize);
		SetPosition(pos);
		return true;
	default:

		LOG(" Error : No Message  | Player.cpp\n ")

			return false;

	}
}

//���^AI�Ƀ��b�Z�[�W�𑗐M����
void Player::SendMessaging(MESSAGE_TYPE msg)
{
	Meta& meta = Meta::Instance();

	switch (msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		//���^AI�Ƀ^�[���̏I����`����
		meta.SendMessaging(GetId(),
			static_cast<int>(Identity::Meta),
			MESSAGE_TYPE::END_PLAYER_TURN);

		//�X�e�[�g�}�V���̐ݒ�
		player_entry_state.SetState(Entry::Select);
		player_state_machine.SetState(ParentState::Receive);
		break;
	case MESSAGE_TYPE::END_ENEMY_TURN:

		LOG(" Error : No Function | Player.cpp  MESSAGE_TYPE::END_ENEMY_TURN\n")
			break;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		meta.SendMessaging(
			GetId(),
			static_cast<int>(Identity::Meta),
			MESSAGE_TYPE::GO_NEXT_FLOOR
		);


		//�X�e�[�g�}�V���̐ݒ�
		player_state_machine.SetState(ParentState::Receive);

		break;

	default:
		LOG(" Error : No Message | Player.cpp   SendMessaging\n ")
			break;

	}
}

//�_���[�W���󂯂��Ƃ��ɌĂ΂��
void Player::OnDamaged()
{
}

//�_���[�W���󂯁Ahelath��0�ȉ��̎��ɌĂ΂��Ƃ��ɌĂ΂��
void Player::OnDead()
{
}

//ImGui�\���p�֐�
void Player::DrawDebugGUI()
{
	// ���͂��ꂽ�����擾
	const GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// ID
		ImGui::Text("Character ID:%d", GetId());
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			DirectX::XMFLOAT3 pos_for_imgui = GetPosition();
			DirectX::XMFLOAT3 old_pos_for_imgui = GetOldPosition();
			DirectX::XMFLOAT3 angle_for_imgui = GetAngle();
			DirectX::XMFLOAT3 scale_for_imgui = GetScale();

			// �ʒu
			ImGui::InputFloat3("Position", &pos_for_imgui.x);
			// �O�t���[���̈ʒu
			ImGui::InputFloat3("OldPosition", &old_pos_for_imgui.x);
			// ��]
			DirectX::XMFLOAT3 a{};
			a.x = DirectX::XMConvertToDegrees(angle_for_imgui.x);
			a.y = DirectX::XMConvertToDegrees(angle_for_imgui.y);
			a.z = DirectX::XMConvertToDegrees(angle_for_imgui.z);
			ImGui::InputFloat3("Radian", &angle_for_imgui.x);
			ImGui::InputFloat3("Degree", &a.x);
			//SetAngle(a);
			// �X�P�[��
			ImGui::InputFloat3("Scale", &scale_for_imgui.x);

		}

		//���݂̃X�e�[�g��\��
		if (ImGui::CollapsingHeader("Current State", ImGuiTreeNodeFlags_DefaultOpen))
		{
			char parent_state_name[64] = "empty";
			char current_state_name[64] = "empty";

			//	player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at
			//	(static_cast<u_int>(player_entry_state.GetState()));

			//!�֐���

			if (player_state_machine.GetState() == static_cast<int>(ParentState::Entry))
			{
				//"empty"�̕�������폜
				memset(current_state_name, '\0', sizeof(current_state_name));
				memset(parent_state_name, '\0', sizeof(parent_state_name));

				//�X�e�[�g��������
				strcat_s(parent_state_name, sizeof(parent_state_name), "Entry");
				const char* state_name = player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at(static_cast<u_int>(player_entry_state.GetState())).c_str();
				strcat_s(current_state_name, sizeof(current_state_name), state_name);
			}

			if (player_state_machine.GetState() == static_cast<int>(ParentState::Reaction))
			{
				//"empty"�̕�������폜
				memset(current_state_name, '\0', sizeof(current_state_name));
				memset(parent_state_name, '\0', sizeof(parent_state_name));

				//�X�e�[�g��������
				strcat_s(parent_state_name, sizeof(parent_state_name), "Reaction");
				const char* state_name = player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at(static_cast<u_int>(player_reaction_state.GetState())).c_str();
				strcat_s(current_state_name, sizeof(current_state_name), state_name);
			}

			if (player_state_machine.GetState() == static_cast<int>(ParentState::Receive))
			{
				//"empty"�̕�������폜
				memset(current_state_name, '\0', sizeof(current_state_name));
				memset(parent_state_name, '\0', sizeof(parent_state_name));

				//�X�e�[�g��������
				strcat_s(parent_state_name, sizeof(parent_state_name), "Receive");
				const char* state_name = player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at(static_cast<u_int>(player_receive_state.GetState())).c_str();
				strcat_s(current_state_name, sizeof(current_state_name), state_name);
			}

			ImGui::Text("Parent State Name:%s", parent_state_name);
			ImGui::Text("Current State Name:%s", current_state_name);
		}
		if (ImGui::CollapsingHeader("Game Pad Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �Q�[���p�b�h�̃X�e�B�b�N����
			ImGui::Text("Game Pad Axis: x:%f y:%f", ax, ay);
			// �Q�[���p�b�h�̃X�e�B�b�N���͂̃X�e�b�v
			if (ax > 0.f)
			{
				ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f));
			}
			else if (ax < 0.f)
			{
				ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
			}

			if (ay > 0.f)
			{
				ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
			}
			else if (ay < 0.f)
			{
				ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
			}
			ImGui::Text("Game Pad Axis On Step: x:%f y:%f", ax, ay);
		}
		if (ImGui::CollapsingHeader("Player Omni Attribute", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// ���͂̃}�b�v���
			RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
			const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / CellSize, GetPosition().z / CellSize);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
			const size_t up_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			const size_t down_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			const size_t right_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			const size_t left_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����

			ImGui::Text("Omni Attribute");
			ImGui::Text("            up:%zu", up_data);
			ImGui::Text("  left:%zu          right:%zu ", left_data, right_data);
			ImGui::Text("           down:%zu", down_data);
		}
		if (ImGui::CollapsingHeader("Player initialize Position", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�v���C���[�̃}�b�v����ł̏����ʒu
			ImGui::Text("Player Map Position: 	%d %d",
				RogueLikeDungeon::Instance().player_pos.x, RogueLikeDungeon::Instance().player_pos.y);

			//�v���C���[�̏����ʒu
			const int player_positionX = RogueLikeDungeon::Instance().player_pos.x * CellSize;
			const int player_positionY = RogueLikeDungeon::Instance().player_pos.y * CellSize;

			ImGui::Text("Player Position:	 %d %d", player_positionX, player_positionY);
		}
	}
	ImGui::End();
}

//
void Player::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debug_renderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

bool Player::IsMoved()
{
	if (Math::Comparison(GetPosition().x, GetOldPosition().x) || Math::Comparison(GetPosition().z, GetOldPosition().z))//true�Ȃ瓮���Ă��Ȃ�
	{
		return false;
	}
	return true;
}

void Player::SetFirstPos()
{
	const RogueLikeDungeon rogue_like_dungeon = RogueLikeDungeon::Instance();
	const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x) * CellSize, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CellSize);
	SetPosition(pos);
}

void Player::Destroy()
{
	CharacterManager::Instance().Remove(this);
}

//	*********************************************
//
// �X�e�[�g
//
//	*********************************************

//---------------------
// �e�X�e�[�g
//---------------------

//----------------------------------------------------
//	EntryState
//----------------------------------------------------
void Player::EntryState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())
	{
		// �q�X�e�[�g�̂̏�����
		if (player_entry_state.GetState() != static_cast<int>(Entry::Select))
		{
			player_entry_state.SetState(Entry::Select);

		}
	}
	// �T�u�X�e�[�g�̎��s
	player_entry_state.Update(elapsed_time);
}

//----------------------------------------------------
//	ReactionState
//----------------------------------------------------
void Player::ReactionState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())
	{
		// �q�X�e�[�g�̂̏�����
		if (player_reaction_state.GetState() != static_cast<int>(Reaction::ReactionSelect))
		{
			player_reaction_state.SetState(Reaction::ReactionSelect);
		}
	}

	// �T�u�X�e�[�g�̎��s
	player_reaction_state.Update(elapsed_time);
}

//----------------------------------------------------
//	ReceiveState
//----------------------------------------------------
void Player::ReceiveState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())
	{
		// �q�X�e�[�g�̂̏�����
		if (player_receive_state.GetState() != static_cast<int>(Receive::Wait))
		{
			player_receive_state.SetState(Receive::Wait);
		}
	}

	// �T�u�X�e�[�g�̎��s
	player_receive_state.Update(elapsed_time);
}

//---------------------
//�q�X�e�[�g
//---------------------

//? EntryState


//----------------------------------------------------
//	SelectState
//----------------------------------------------------
void	Player::SelectState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}

	GamePad& game_pad = Input::Instance().GetGamePad();

	//�U��
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attack�X�e�[�g�ɑJ�ڂ���
		player_entry_state.SetState(Entry::Attack);
	}

	/*�����ɏ������������Ȃ��̂� else if�ő���if������Ȃ��悤�ɂ���*/

	//���j���[
	//else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	//{
	//	//Menu�X�e�[�g�ɑJ�ڂ���
	//	player_entry_state.SetState( Entry::Menu);
	//}

	//�����]��
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		//WayChange�X�e�[�g�ɑJ�ڂ���
		player_entry_state.SetState(Entry::WayChange);
	}

	//	�ړ�
	//���X�e�B�b�N��������DPad�����͂��ꂽ�Ƃ�
	else if (game_pad.GetAxisLX() != 0 || game_pad.GetAxisLY() != 0)
	{
		//Move�X�e�[�g�ɑJ�ڂ���
		player_entry_state.SetState(Entry::Move);
	}
}

//----------------------------------------------------
//	AttackState
//----------------------------------------------------
void	Player::AttackState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{
	}

}

//----------------------------------------------------
//	WayChangeState
//----------------------------------------------------
void	Player::WayChangeState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{
	}

	const GamePad& game_pad = Input::Instance().GetGamePad();
	//Y�{�^���𒷉������Ă����
	if (game_pad.GetButtonUp() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		player_entry_state.SetState(Entry::Select);
	}
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	/*�X�e�b�v���������č��X�e�B�b�N�̓��͂�0, 1, -1, 1 / ��2(��0.71)�ɂ���*/

	//���X�e�B�b�N��x���̃X�e�b�v����
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
	}

	//���X�e�B�b�N��y���̃X�e�b�v����
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
	}

	//�����]��
	//��
	if (Math::Comparison(ax, 0.f) && Math::Comparison(ay, 1.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(0));
	}
	//��
	else if (Math::Comparison(ax, 0.f) && Math::Comparison(ay, -1.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(180));
	}
	//�E
	else if (Math::Comparison(ax, 1.f) && Math::Comparison(ay, 0.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(90));
	}
	//��
	else if (Math::Comparison(ax, -1.f) && Math::Comparison(ay, 0.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(270));
	}

	//�E��
	if (Math::Comparison(ax, 1.f) && Math::Comparison(ay, 1.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//����
	else if (Math::Comparison(ax, -1.f) && Math::Comparison(ay, 1.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(315));
	}
	//����
	else if (Math::Comparison(ax, -1.f) && Math::Comparison(ay, -1.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(225));
	}
	//�E��
	else if (Math::Comparison(ax, 1.f) && Math::Comparison(ay, -1.f))
	{
		SetAngleY(Math::ConvertToRadianAngle(135));
	}

	//�U��
	else	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attack�X�e�[�g�ɑJ�ڂ���
		player_entry_state.SetState(Entry::Attack);
	}

}

//----------------------------------------------------
//	MoveState
//----------------------------------------------------
void	Player::MoveState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//��x�������s
	{
		SetOldPosition();//�O��̈ʒu��ۑ�
	}
	//SetOldPosition();//�O��̈ʒu��ۑ�

	const GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	const DirectX::XMFLOAT2 player_pos =//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
		DirectX::XMFLOAT2(GetPosition().x / CellSize, GetPosition().z / CellSize);

	//�X�e�b�v�����ā@���X�e�B�b�N�̓��͂�0, 1 , -1 , 1 / ��2(��0.71)�ɂ���
	//���X�e�B�b�N��x���̃X�e�b�v
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//���X�e�B�b�N��y���̃X�e�b�v
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//�ړ�����

	//�E��
	if (Math::Comparison(ax,1.f) && Math::Comparison(ay, 1.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(45));

		const size_t map_data_diagonal = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂��E��̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//�E��Ɉړ�
			AddPositionZ(CellSize);
			AddPositionX(CellSize);

			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//����
	else if (Math::Comparison(ax, -1.f) && Math::Comparison(ay, 1.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(315));

		const size_t map_data_diagonal = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂�����̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//�i�s�������ړ��\�ȑ��������`�F�b�N
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//����Ɉړ�
			AddPositionZ(CellSize);
			AddPositionX(-CellSize);
			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//����-
	else if (Math::Comparison(ax, 1.f) && Math::Comparison(ay, -1.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(225));

		const size_t map_data_diagonal = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂������̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//�i�s�������ړ��\�ȑ��������`�F�b�N
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//�����Ɉړ�
			AddPositionZ(-CellSize);
			AddPositionX(-CellSize);

			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//�E��
	else if (Math::Comparison(ax, 1.f) && Math::Comparison(ay, -1.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(135));

		const size_t map_data_diagonal = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂��E���̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//�i�s�������ړ��\�ȑ��������`�F�b�N
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//�E���Ɉړ�
			AddPositionZ(-CellSize);
			AddPositionX(CellSize);

			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}

	//��
	if (Math::Comparison(ax, 0.f) && Math::Comparison(ay, 1.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(0));

		const size_t map_data = rogue_like_dungeon.
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����

			//�i�s�������ړ��\�ȑ��������`�F�b�N
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//��Ɉړ�
			AddPositionZ(CellSize);

			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//��
	else if (Math::Comparison(ax, 0.f) && Math::Comparison(ay, -1.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();


		SetAngleY(Math::ConvertToRadianAngle(180));
		const size_t map_data = rogue_like_dungeon.
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����

			//�i�s�������ړ��\�ȑ��������`�F�b�N
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//���Ɉړ�
			AddPositionZ(-CellSize);
			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//�E
	else if (Math::Comparison(ax, 1.f) && Math::Comparison(ay, 0.f))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(90));

		const size_t map_data = rogue_like_dungeon.
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����

		//�i�s�������ړ��\�ȑ��������`�F�b�N
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//�E�Ɉړ�
			AddPositionX(CellSize);
			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//��
	else if ((Math::Comparison(ax, -1.f) && Math::Comparison(ay, 0.f)))
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(270));

		const size_t map_data = rogue_like_dungeon.
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����

		//�i�s�������ړ��\�ȑ��������`�F�b�N
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//���Ɉړ�
			AddPositionX(-CellSize);
			//�v���C���[�̍s�����I������
			SendMessaging(MESSAGE_TYPE::END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}

	}
	player_entry_state.SetState(Entry::Select);
}


//? ReactionState

//----------------------------------------------------
//	ReactionSelectState
//----------------------------------------------------
void Player::ReactionSelectState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
		if (GetCurrentHealth() > 0)
		{
			player_reaction_state.SetState(Reaction::Damaged);
		}
		else
		{
			player_reaction_state.SetState(Reaction::Death);
		}
	}
}

//----------------------------------------------------
//	DamagedState
//----------------------------------------------------
void Player::DamagedState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{

	}
	OnDamaged();
}

//----------------------------------------------------
//	DeathState
//----------------------------------------------------
void Player::DeathState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
	}
	OnDead();
}

//? ReceiveState

//----------------------------------------------------
//	WaitState
//----------------------------------------------------
void Player::WaitState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())
	{

	}
	//�������Ȃ�
}

