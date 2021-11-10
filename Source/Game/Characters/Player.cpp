#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"

#include "Game/Characters/Player.h"
#include "Game/Characters/DerivedPlayerState.h"

const float cos45 = cosf(DirectX::XMConvertToRadians(45.f));


Player::Player(RogueLikeDungeon* rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 3.f;

	//�����X�e�[�g
	FSMInitialize();

	stage_informations = rogue_like_dungeon;

	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_informations->map_role[y][x].map_data == 2)
			{
				float pos_x = static_cast<float>(x * Cell_Size);
				float pos_z = static_cast<float> (y * Cell_Size);

				position = DirectX::XMFLOAT3(pos_x, 0, pos_z);
			}
		}
	}
}

Player::Player()
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	scale.x = scale.y = scale.z = 1.f;
	position = DirectX::XMFLOAT3(0, 3, 0);

}

Player::~Player()
{
}

void Player::Update(float elapsedTime)
{
	position.y = 0.f;

	state_machine->Update(elapsedTime);
	{
		//--�ǉ�---------------------------------------
		// AABB
		//
		//-----------------------------------------------
	}

	//--�����\��----------------------------------
	// ���͍X�V����
	UpdateVelocity(elapsedTime);

	// ���G���ԍX�V
	UpdateInvincibleTimer(elapsedTime);
	//------------------------------------------------

	//��]�p�̐��K��
	NormalizeAngle();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
}

void Player::FSMInitialize()
{
	state_machine = std::make_unique<StateMachine>(this);
	//�e�X�e�[�g
	state_machine->RegisterState(new EntryState(this));
	state_machine->RegisterState(new ReactionState(this));
	state_machine->RegisterState(new  ReceiveState(this));

	////�q�X�e�[�g
	//Entry
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Entry), new SelectState(this));
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Entry), new WayChangeState(this));
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Entry), new MoveState(this));
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Entry), new AttackState(this));
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Entry), new MenuState(this));

	//Reaction
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Reaction), new DamagedState(this));
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Reaction), new DeathState(this));

	//Receive
	state_machine->RegisterSubState(static_cast<int>(Player::ParentState::Receive), new CalledState(this));

	state_machine->SetState(static_cast<int>(Player::ParentState::Entry));
}

void Player::DrawDebugGUI()
{
	//���͂��ꂽ�����擾
	GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Appearing);
	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::InputFloat3("Position", &this->position.x);
			// ��]
			DirectX::XMFLOAT3 a;
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
		//ID
		ImGui::Text(u8"ID:%d", this->id);
		//�Q�[���p�b�h�̃X�e�B�b�N����
		ImGui::Text(u8"GamePadAxis: x:%f y:%f", ax, ay);
		//�Q�[���p�b�h�̃X�e�B�b�N���͂̃X�e�b�v
		if (ax > 0.f)
		{
			ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
		}
		else if (ax < 0.f)
		{
			ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
		}

		if (ay > 0.f)
		{
			ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
		}
		else if (ay < 0.f)
		{
			ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
		}
		ImGui::Text(u8"GamePadAxisOnStep: x:%f y:%f", ax, ay);
		DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / Cell_Size, GetPosition().z / Cell_Size);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
		size_t up_data		= GetStageInformations()->map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
		size_t down_data = GetStageInformations()->map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		size_t right_data	= GetStageInformations()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
		size_t left_data	= GetStageInformations()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		ImGui::Text(u8"OmniMapData");
		ImGui::Text(u8"            up:%d", up_data);
		ImGui::Text(u8"  left:%d          right:%d ", left_data,right_data);
		ImGui::Text(u8"           down:%d", down_data);
	}
	ImGui::End();
}

void Player::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debug_renderer->DrawSphere(this->position, this->radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

bool Player::OnMessage(const Telegram& msg)
{
	//ToDo ���^AI����̎�M����

	return false;
}

void Player::OnDamaged()
{
}

void Player::OnDead()
{
}

