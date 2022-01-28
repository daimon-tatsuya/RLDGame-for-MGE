//**********************************************************
//
//		Player�N���X
//
//**********************************************************


#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Objects/Model.h"
#include "Engine/AI/DungeonMake.h"

#include "Game/Characters/Player.h"
#include "Game/Characters/DerivedPlayerState.h"


const float cos45 = cosf(DirectX::XMConvertToRadians(45.f));

Player::Player(RogueLikeDungeon* rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	//model = std::make_shared<Model>("Assets/FBX/Animals/Rattlesnake.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 0.f;

	// �����X�e�[�g
	Player::FiniteStateMachineInitialize();

	stage_information = rogue_like_dungeon;

	// �I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_information->map_role[y][x].map_data == static_cast<size_t>(Attribute::Player))
			{
				const float pos_x = static_cast<float>(x * CellSize);
				const float pos_z = static_cast<float> (y * CellSize);

				position = DirectX::XMFLOAT3(pos_x, 0, pos_z);
			}
		}
	}
}

Player::~Player()
= default;


void Player::Update(const float elapsed_time)
{
	position.y = 0.f;

	player_state_machine.Update(elapsed_time);

	{
		//--�ǉ�---------------------------------------
		// AABB
		//
		//-----------------------------------------------
	}


	// ��]�p�̐��K��
	NormalizeAngle();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapsed_time);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
}

void Player::FiniteStateMachineInitialize()
{

	//�e�X�e�[�g�̒ǉ�
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

	//�q�X�e�[�g�̒ǉ�

	//Entry
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

	//player_entry_state.AddState
	//(
	//	 Entry::Menu,
	//	[this](const float elapsed_time) {MenuState(elapsed_time); }
	//);

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

	//Reaction

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


	//Recieve

	player_receive_state.AddState
	(
		Receive::Wait,
		[this](const float elapsed_time) {WaitState(elapsed_time); }
	);

	player_receive_state.AddState
	(
		Receive::Called,
		[this](const float elapsed_time) {CalledState(elapsed_time); }
	);

	// �e�����X�e�[�g�̐ݒ�
	player_state_machine.SetState(ParentState::Entry);

	player_entry_state.SetState(Entry::Select);
	player_reaction_state.SetState(Reaction::ReactionSelect);
	player_receive_state.SetState(Receive::Wait);

}

bool Player::OnMessage(const Telegram & telegram)
{
	// ���^AI����̎�M����

	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		player_state_machine.SetState(ParentState::Entry);

		return true;
	}
	return false;
}
void Player::OnDamaged()
{
}

void Player::OnDead()
{
}


void Player::DrawDebugGUI()
{
	// ���͂��ꂽ�����擾
	GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();


	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::InputFloat3("Position", &this->position.x);
			// ��]
			DirectX::XMFLOAT3 a{};
			a.x = DirectX::XMConvertToDegrees(this->angle.x);
			a.y = DirectX::XMConvertToDegrees(this->angle.y);
			a.z = DirectX::XMConvertToDegrees(this->angle.z);
			ImGui::InputFloat3("Radian", &this->angle.x);
			ImGui::InputFloat3("Degree", &a.x);
			this->angle.x = DirectX::XMConvertToRadians(a.x);
			this->angle.y = DirectX::XMConvertToRadians(a.y);
			this->angle.z = DirectX::XMConvertToRadians(a.z);
			// �X�P�[��
			ImGui::InputFloat3("Scale", &this->scale.x);
		}
		// ID
		ImGui::Text("ID:%d", this->id);
		// �Q�[���p�b�h�̃X�e�B�b�N����
		ImGui::Text("GamePadAxis: x:%f y:%f", ax, ay);
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
		ImGui::Text("GamePadAxisOnStep: x:%f y:%f", ax, ay);
		// ���͂̃}�b�v���
		const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / CellSize, GetPosition().z / CellSize);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
		const size_t up_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
		const size_t down_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		const size_t right_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
		const size_t left_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����

		ImGui::Text("OmniAttribute");
		ImGui::Text("            up:%zu", up_data);
		ImGui::Text("  left:%zu          right:%zu ", left_data, right_data);
		ImGui::Text("           down:%zu", down_data);

	}
	ImGui::End();

}

void Player::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debug_renderer->DrawSphere(this->position, this->radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//----------------------------------------------------------------
//
// �X�e�[�g
//
//----------------------------------------------------------------

//---------------------
// �e�X�e�[�g
//---------------------

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

// EntryState

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

	//�����ɏ������������Ȃ��̂� else if��1��true�Ȃ����Ȃ��悤�ɂ���

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

void	Player::AttackState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}
	//��
	player_entry_state.SetState(Entry::Select);
	//player_state.SetState(ParentState::Receive);
}

//void	Player::MenuState(const float elapsed_time)
//{
//	if (player_entry_state.IsStateFirstTime())
//	{
//
//	}
//
//	// �X�e�[�g�̏I��
//	if (false)
//	{
//
//	}
//}

void	Player::WayChangeState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}

	GamePad& game_pad = Input::Instance().GetGamePad();
	//Y�{�^���𒷉������Ă����
	if (game_pad.GetButtonUp() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		player_entry_state.SetState(Entry::Select);
	}
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	//�X�e�b�v�����ā@���X�e�B�b�N�̓��͂�0, 1 , -1 , 1 / ��2(��0.71)�ɂ���8������\��
	//���X�e�B�b�N��x���̃X�e�b�v
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
	}

	//���X�e�B�b�N��y���̃X�e�b�v
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
	if (ax == 0.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(0));
	}
	//��
	else if (ax == 0.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(180));
	}
	//�E
	else if (ax == 1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(90));
	}
	//��
	else if (ax == -1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(270));
	}

	//�E��
	if (ax == 1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//����
	else if (ax == -1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(315));
	}
	//����
	else if (ax == -1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(225));
	}
	//�E��
	else if (ax == 1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(135));
	}

	//�U��
	else	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attack�X�e�[�g�ɑJ�ڂ���
		player_entry_state.SetState(Entry::Attack);
	}

	// �X�e�[�g�̏I��
	if (false)
	{

	}
}

void	Player::MoveState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}

	GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	DirectX::XMFLOAT2 player_pos =//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
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
	if (ax == 1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(45));
		size_t map_data_diagonal = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂��E��̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;


		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(CellSize);
			AddPositionX(CellSize);
		}
	}
	//����
	else if (ax == -1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(315));
		size_t map_data_diagonal = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂�����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(CellSize);
			AddPositionX(-CellSize);
		}
	}
	//����
	else if (ax == -1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(225));
		size_t map_data_diagonal = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂������̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(-CellSize);
			AddPositionX(-CellSize);
		}
	}
	//�E��
	else if (ax == 1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(135));
		size_t map_data_diagonal = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂��E���̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(-CellSize);
			AddPositionX(CellSize);
		}
	}

	//��
	if (ax == 0.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(0));
		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionZ(CellSize);
		}
	}
	//��
	else if (ax == 0.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(180));
		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionZ(-CellSize);
		}
	}
	//�E
	else if (ax == 1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(90));
		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
	//�ǂ��G�łȂ��Ȃ�
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionX(CellSize);
		}
	}
	//��
	else if (ax == -1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(270));

		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionX(-CellSize);
		}
	}
	//��
	player_entry_state.SetState(Entry::Select);
	//player_state.SetState(ParentState::Receive);

}

// ReactionState

void Player::ReactionSelectState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
		if (current_health > 0)
		{
			player_reaction_state.SetState(Reaction::Damaged);
		}
		else
		{
			player_reaction_state.SetState(Reaction::Death);
		}
	}

}

void Player::DamagedState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
		//Meta& meta = Meta::Instance();
		//meta.SendMessaging(GetId(),
		//static_cast<int>(Meta::Identity::Meta),
		//MESSAGE_TYPE::MSG_END_PLAYER_TURN);
	}
	//player_state.SetState(ParentState::Entry);

}

void Player::DeathState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{

	}

	// �X�e�[�g�̏I��
	if (false)
	{

	}
}

// ReceiveState

void Player::WaitState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())
	{
		//���^AI�Ƀ^�[���̏I����`����
		//Meta& meta = Meta::Instance();
		//meta.SendMessaging(owner->GetId(),
		//	static_cast<int>(Meta::Identity::Meta),
		//	MESSAGE_TYPE::MSG_END_PLAYER_TURN);
	}

	// �X�e�[�g�̏I��
	if (false)
	{

	}
}

void Player::CalledState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())
	{

	}

}
