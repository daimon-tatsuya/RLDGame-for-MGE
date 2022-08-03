//**********************************************************
//
//		Player�N���X
//
//**********************************************************

#include "Game/Characters/Player.h"

<<<<<<< HEAD
#include  "Liblary/AI/MetaAI.h"
#include "Liblary/Systems/Logger.h"
#include "Liblary/Systems/Input.h"
#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/Shader.h"
#include "Liblary/Systems/ShaderManager.h"
#include "Liblary/Systems/DebugRenderer.h"
#include "Liblary/Systems/ImGuiRenderer.h"
#include "Liblary/Objects/Model.h"
#include "Liblary/AI/DungeonMake.h"



#include "Liblary/Systems/CharacterManager.h"
=======
#include  "MyLiblary/AI/MetaAI.h"
#include "MyLiblary/Systems/Logger.h"
#include "MyLiblary/Systems/Input.h"
#include "MyLiblary/Systems/Graphics.h"
#include "MyLiblary/Systems/Shader.h"
#include "MyLiblary/Systems/ShaderManager.h"
#include "MyLiblary/Systems/DebugRenderer.h"
#include "MyLiblary/Systems/ImGuiRenderer.h"
#include "MyLiblary/Objects/Model.h"
#include "MyLiblary/AI/DungeonMake.h"



#include "MyLiblary/Systems/CharacterManager.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3

const float COS45 = cosf(DirectX::XMConvertToRadians(45.f));

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
	//�K��i�߂�
	if (static_cast<int>(GetPosition().x / CELL_SIZE) == rogue_like_dungeon.stairs_pos.x &&
		static_cast<int>(GetPosition().z) / CELL_SIZE == rogue_like_dungeon.stairs_pos.y)
	{
		SendMessaging(MESSAGE_TYPE::GO_NEXT_FLOOR);
	}
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
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
		Entry::Wait,
		[this](const float elapsed_time) {WaitState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Move,
		[this](const float elapsed_time) {MoveState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Stop,
		[this](const float elapsed_time) {StopState(elapsed_time); }
	);


	player_entry_state.AddState
	(
		Entry::Jump,
		[this](const float elapsed_time) {JumpState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Landing,
		[this](const float elapsed_time) {JumpState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Avoid,
		[this](const float elapsed_time) {AvoidState((elapsed_time)); }
	);

	player_entry_state.AddState
	(
		Entry::Attack,
		[this](const float elapsed_time) {AttackState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Counter,
		[this](const float elapsed_time) {CounterState(elapsed_time); }
	);
	//ReactionState�̎q�X�e�[�g

	player_reaction_state.AddState
	(
		Reaction::ReactionSelect,
		[this](const float elapsed_time) {ReactionSelectState(elapsed_time); }
	);

	player_reaction_state.AddState
	(
		Reaction::Countered,
		[this](const float elapsed_time) {CounteredState(elapsed_time); }
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
		Receive::WaitReaction,
		[this](const float elapsed_time) {WaitReactionState(elapsed_time); }
	);

	// �e�X�e�[�g�}�V���̏����X�e�[�g�̐ݒ�

	player_state_machine.SetState(ParentState::Entry);

	player_entry_state.SetState(Entry::Wait);
	player_reaction_state.SetState(Reaction::ReactionSelect);
	player_receive_state.SetState(Receive::WaitReaction);


	//ImGui
	player_entry_string.emplace_back("Wait");
	player_entry_string.emplace_back("Move");
	player_entry_string.emplace_back("Stop");
	player_entry_string.emplace_back("Jump");
	player_entry_string.emplace_back("Avoid");
	player_entry_string.emplace_back("Attack");
	player_entry_string.emplace_back("Counter");

	player_reaction_string.emplace_back("ReactionSelect");
	player_reaction_string.emplace_back("Countered");
	player_reaction_string.emplace_back("Damaged");
	player_reaction_string.emplace_back("Death");

	player_receive_string.emplace_back("WaitReaction");

	player_states_string.emplace_back(player_entry_string);
	player_states_string.emplace_back(player_reaction_string);
	player_states_string.emplace_back(player_receive_string);
}

//	���^AI�o�R��M����
bool Player::OnMessage(const Telegram& telegram)
{
	const RogueLikeDungeon rogue_like_dungeon = RogueLikeDungeon::Instance();
	// ���^AI����̎�M����
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		LOG(" Error : END_PLAYER_TURN is No Function | Player.cpp OnMessage Method\n")
			return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:
		//�v���C���[
		player_state_machine.SetState(ParentState::Entry);

		return true;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		SetPositionY(0.f);

		const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x) * CELL_SIZE, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CELL_SIZE);

		SetPosition(pos);

		return true;

	case MESSAGE_TYPE::GO_MAX_FLOOR:

		LOG(" Error : GO_MAX_FLOOR is No Function  | Player.cpp OnMessage Method\n ")

			return true;

	default:

		LOG(" Error : No Message  | Player.cpp  OnMessage Method\n ")

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
		player_entry_state.SetState(Entry::Wait);
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
		ImGui::Text("Character ID : %d", GetId());
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

			ImGui::Text("Parent State Name : %s", parent_state_name);
			ImGui::Text("Current State Name : %s", current_state_name);
		}
		if (ImGui::CollapsingHeader("Game Pad Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �Q�[���p�b�h�̃X�e�B�b�N����
			ImGui::Text("Game Pad Axis: x:%f y:%f", ax, ay);
			// �Q�[���p�b�h�̃X�e�B�b�N���͂̃X�e�b�v
			if (ax > 0.f)
			{
				ax = Math::StepAnyFloat(game_pad.GetAxisLX(), COS45, (COS45 / 2.f), 1.f - (COS45 / 2.f));
			}
			else if (ax < 0.f)
			{
				ax = Math::StepAnyFloat(game_pad.GetAxisLX(), COS45, -1.f + (COS45 / 2.f), (COS45 / 2.f), true);
			}

			if (ay > 0.f)
			{
				ay = Math::StepAnyFloat(game_pad.GetAxisLY(), COS45, (COS45 / 2.f), 1.f - (COS45 / 2.f)); ;
			}
			else if (ay < 0.f)
			{
				ay = Math::StepAnyFloat(game_pad.GetAxisLY(), COS45, -1.f + (COS45 / 2.f), (COS45 / 2.f), true);
			}
			ImGui::Text("Game Pad Axis On Step: x : %f y : %f", ax, ay);
		}
		if (ImGui::CollapsingHeader("Player Map Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// ���͂̃}�b�v���
			RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
			const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / CELL_SIZE, GetPosition().z / CELL_SIZE);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
			const size_t up_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			const size_t down_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			const size_t right_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			const size_t left_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����

			ImGui::Text("Omni Attribute");
			ImGui::Text("            up : %zu", up_data);
			ImGui::Text("  left : %zu          right : %zu ", left_data, right_data);
			ImGui::Text("           down : %zu", down_data);
			ImGui::Text("");
			if (rogue_like_dungeon.map_room_player >= rogue_like_dungeon.dungeon_map_role.map_room_count ||
				rogue_like_dungeon.map_room_player < 0)
			{
				ImGui::Text("Be RoomNumber : -1");
			}
			else
			{
				ImGui::Text("Be RoomNumber : %zu", rogue_like_dungeon.map_room_player);
			}

		}
		if (ImGui::CollapsingHeader("Player initialize Position", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//�v���C���[�̃}�b�v����ł̏����ʒu
			ImGui::Text("Player Map Position : x : %d  y : %d",
				RogueLikeDungeon::Instance().player_pos.x, RogueLikeDungeon::Instance().player_pos.y);

			//�v���C���[�̏����ʒu
			const int player_positionX = RogueLikeDungeon::Instance().player_pos.x * CELL_SIZE;
			const int player_positionY = RogueLikeDungeon::Instance().player_pos.y * CELL_SIZE;

			ImGui::Text("Player Position : x : %d  y : %d", player_positionX, player_positionY);
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
	const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x) * CELL_SIZE, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CELL_SIZE);
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
	if (player_state_machine.IsStateFirstTime())//��x�������s
	{
		// �q�X�e�[�g�̂̏�����
		if (player_entry_state.GetState() != static_cast<int>(Entry::Wait))
		{
			player_entry_state.SetState(Entry::Wait);

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
	if (player_state_machine.IsStateFirstTime())//��x�������s
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
	if (player_state_machine.IsStateFirstTime())//��x�������s
	{
		// �q�X�e�[�g�̂̏�����
		if (player_receive_state.GetState() != static_cast<int>(Receive::WaitReaction))
		{
			player_receive_state.SetState(Receive::WaitReaction);
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
//	WaitState
//----------------------------------------------------
void	Player::WaitState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//��x�������s
	{
	}
}

//----------------------------------------------------
//	MoveState
//----------------------------------------------------
void	Player::MoveState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//��x�������s
	{
	}
}

//----------------------------------------------------
//	StopState
//----------------------------------------------------
void Player::StopState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//��x�������s
	{

	}
}

//----------------------------------------------------
//	JumpState
//----------------------------------------------------
void	Player::JumpState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//��x�������s
	{

	}
}

void Player::LandingState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
	{

	}
}

void Player::AvoidState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
	{

	}
}

//----------------------------------------------------
//	AttackState
//----------------------------------------------------
void	Player::AttackState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//��x�������s
	{

	}
}


void Player::CounterState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
	{

	}
}

//? ReactionState

//----------------------------------------------------
//	ReactionSelectState
//----------------------------------------------------
void Player::ReactionSelectState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
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



void Player::CounteredState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
	{

	}
}

//----------------------------------------------------
//	DamagedState
//----------------------------------------------------
void Player::DamagedState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
	{

	}
	OnDamaged();
}

//----------------------------------------------------
//	DeathState
//----------------------------------------------------
void Player::DeathState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//��x�������s
	{
	}
	OnDead();
}

//? ReceiveState

//----------------------------------------------------
//	WaitReactionState
//----------------------------------------------------
void Player::WaitReactionState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())//��x�������s
	{

	}
	//�������Ȃ�
}

