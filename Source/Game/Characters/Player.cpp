#include "Game/Characters/Player.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"

Player::Player(RogueLikeDungeon& rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 15.f;
	//�����X�e�[�g

	//position.x = mob_role.position.x;// *Cell_Size;
	//position.z = mob_role.position.y;// *Cell_Size;

	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (rogue_like_dungeon.map_role[y][x].map_data == 2)
			{
				position = DirectX::XMFLOAT3(x * Cell_Size, 0, y * Cell_Size);
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
	//delete model;
}

void Player::Update(float elapsedTime)
{
	position.y = 0.f;

	InputMove(elapsedTime);

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

void Player::DrawDebugGUI()
{
	//���͂��ꂽ�����擾
	GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
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

			// �X�P�[��
			ImGui::InputFloat3("Scale", &this->scale.x);
			//ID
			ImGui::Text(u8"ID:%d", this->id);
			//�Q�[���p�b�h�̃X�e�B�b�N����
			ImGui::Text(u8"GamePadAxis:%f %f", ax, ay);
		}
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
	//Todo�@���^AI����̎�M����

	return false;
}

void Player::OnLanding()
{
}

void Player::OnDamaged()
{
}

void Player::OnDead()
{
}

//bool Player::InputMove(float elapsedTime)
//{
//	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
//	Camera& camera = Camera::Instance();
//	DirectX::XMFLOAT3 camera_right = camera.GetRight();
//	DirectX::XMFLOAT3 camera_front = camera.GetFront();
//
//	// �i�s�x�N�g���擾
//	DirectX::XMFLOAT3 move_vec = GetMoveVec(camera_right, camera_front);
//
//	// �ړ�����
//	Move(move_vec.x, move_vec.z, 1.0f);
//	// �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
//	return move_vec.x != 0.0f || move_vec.y != 0.0f || move_vec.z != 0.0f;
//}