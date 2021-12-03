//**********************************************************
//
//		CameraController�N���X
//
//**********************************************************

#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"

#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"

#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"



CameraController::CameraController()
{
	position = Camera::Instance().GetEye();
	new_position = Camera::Instance().GetEye();
}

// �X�V����
void CameraController::ActionGameCameraUpdate(float elapsed_time)
{
	//	�t���[�J����
	CharacterManager& character_manager = CharacterManager::Instance();
	Character* pl = character_manager.GetCharacterFromId(static_cast<int>(Meta::Identity::Player));
	this->new_target = pl->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// �J�����̉�]���x
	float speed = roll_speed * elapsed_time;


	// �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
	//angle.x += ay *speed;
	//angle.y += ax *speed;

	// X���̃J������]�𐧌�
	if (angle.x < min_angleX)
	{
		angle.x = min_angleX;
	}
	if (angle.x > max_angleX)
	{
		angle.x = max_angleX;
	}
	// Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	new_position.x = target.x - front.x * range;

	new_position.y = target.y - front.y * range + camera_heght;
	new_position.z = target.z - front.z * range;

	// ���X�ɖڕW�ɋ߂Â���
	static	constexpr	float	Speed = 1.0f / 8.0f;
	position.x += (new_position.x - position.x) * Speed;
	position.y += (new_position.y - position.y) * Speed;
	position.z += (new_position.z - position.z) * Speed;
	target.x += (new_target.x - target.x) * Speed;
	target.y += (new_target.y - target.y) * Speed;
	target.z += (new_target.z - target.z) * Speed;

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::FollowCameraUpdate(float elapsed_time)
{
	//	�t���[�J����
	CharacterManager& character_manager = CharacterManager::Instance();
	Character* pl = character_manager.GetCharacterFromId(static_cast<int>(Meta::Identity::Player));
	this->new_target = pl->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// �J�����̉�]���x
	float speed = roll_speed * elapsed_time;

	DirectX::XMFLOAT3 player_position = pl->GetPosition();
	position.x = player_position.x;
	position.y = camera_heght;
	position.z = player_position.z-10;
	target = pl->GetPosition();

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::DrawDebugGUI()
{
	Camera& camera = Camera::Instance();
	ImGui::SetNextWindowPos(ImVec2(100, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_None))
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
			//�����_
			ImGui::InputFloat3("target", &target.x);

			//���ˉe�J�����Ȃ�s��Ȃ�
			bool OrthMode = camera.GetOrthMode();
			if (OrthMode == false)
			{
				//�J�����̍���
				ImGui::DragFloat("Height", &camera_heght);
				ImGui::Checkbox("ViewMap", &ViewMap);
				if (ViewMap)
				{
					camera_heght = 400.f;
				}
				else
				{
					camera_heght = 50.f;
				}
			}
		}
	}
	ImGui::End();
}