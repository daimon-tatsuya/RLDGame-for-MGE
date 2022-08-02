//**********************************************************
//
//		CameraController�N���X
//
//**********************************************************

#include "Engine/Systems/Input.h"

#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/Camera.h"
#include  "Engine/Systems/Graphics.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/Math.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"

CameraController::CameraController()
	: position(Camera::Instance().GetEye()),
	  new_position(Camera::Instance().GetEye()), frustum()
{
}

// �X�V����
void CameraController::ActionGameCameraUpdate(const float elapsed_time)
{
	// �t���[�J����
	const CharacterManager& character_manager = CharacterManager::Instance();
	const Character* player = character_manager.GetCharacterFromId(static_cast<int>(Identity::Player));
	this->new_target = player->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// �J�����̉�]���x
	float speed = roll_speed * elapsed_time;

	// �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
	//angle.x += ay *speed;
	//angle.y += ax *speed;

	//! X���̃J������]�𐧌�
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
	DirectX::XMFLOAT3 front{};
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	new_position.x = target.x - front.x * range;

	new_position.y = target.y - front.y * range + camera_height;
	new_position.z = target.z - front.z * range;

	// ���X�ɖڕW�ɋ߂Â���

	position.x += (new_position.x - position.x) * CAMERA_MOVE_SPEED;
	position.y += (new_position.y - position.y) * CAMERA_MOVE_SPEED;
	position.z += (new_position.z - position.z) * CAMERA_MOVE_SPEED;
	target.x += (new_target.x - target.x) * CAMERA_MOVE_SPEED;
	target.y += (new_target.y - target.y) * CAMERA_MOVE_SPEED;
	target.z += (new_target.z - target.z) * CAMERA_MOVE_SPEED;

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::FollowCameraUpdate(float elapsed_time)
{
	// �t���[�J����
	CharacterManager& character_manager = CharacterManager::Instance();
	const Character* player = character_manager.GetCharacterFromId(static_cast<int>(Identity::Player));
	this->new_target = player->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// �J�����̉�]���x
	float speed = roll_speed * elapsed_time;

	const DirectX::XMFLOAT3 player_position = player->GetPosition();
	position.x = player_position.x;
	position.y = camera_height;
	position.z = player_position.z - 10;
	target = player->GetPosition();

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::CalculateFrustum()
{
	const Camera& camera = Camera::Instance();

	// �r���[�v���W�F�N�V�����s����擾����
	DirectX::XMMATRIX matrix = {};
	const DirectX::XMMATRIX view_matrix = DirectX::XMLoadFloat4x4(&camera.GetView());
	const DirectX::XMMATRIX projction_matrix = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	matrix = view_matrix * projction_matrix;

	// �r���[�v���W�F�N�V�����s��̋t�s��
	DirectX::XMMATRIX inverse_matrix = DirectX::XMMatrixInverse(nullptr, matrix);

	// �r���[�v���W�F�N�V�������̒��_�Z�o�p�ʒu�x�N�g��
	DirectX::XMVECTOR vertexs[8] =
	{
		// near plane corners
		{ -1, -1, 0 },	// [0]:����
		{  1, -1, 0 },	// [1]:�E��
		{  1,  1, 0 },	// [2]:�E��
		{ -1,  1 ,0 },	// [3]:����

		// far plane corners.
		{ -1, -1, 1 },	// [4]:����
		{  1, -1, 1 },	// [5]:�E��
		{  1,  1, 1 },	// [6]:�E��
		{ -1,  1, 1 } 	// [7]:����
	};

	// �r���[�v���W�F�N�V�����s��̋t�s���p���āA�e���_���Z�o����
	for (int i = 0; i < 8; i++)
	{
		vertexs[i] = DirectX::XMVector3TransformCoord(vertexs[i], inverse_matrix);
		if (i < 4)
		{
			DirectX::XMStoreFloat3(&frustum.near_vertices_position[i], vertexs[i]);
		}
		else
		{
			DirectX::XMStoreFloat3(&frustum.far_vertices_position[i - 4], vertexs[i]);
		}
	}

	DirectX::XMFLOAT4X4 matrix4X4 = {};
	DirectX::XMStoreFloat4x4(&matrix4X4, matrix);

	// ������i�t���X�^���j���\������U���ʂ��Z�o����
	// 0:������, 1:�E����, 2:������, 3:�㑤��, 4:������, 5:��O����
	// �S�Ă̖ʂ̖@���͓����������悤�ɐݒ肷�邱��

	// ������
	frustum.normal[0].x = matrix4X4._14 + matrix4X4._11;
	frustum.normal[0].y = matrix4X4._24 + matrix4X4._21;
	frustum.normal[0].z = matrix4X4._34 + matrix4X4._31;
	frustum.normal[0] = Math::Normalize(frustum.normal[0]);
	frustum.shortest_direction[0] = Math::Dot(frustum.normal[0], frustum.near_vertices_position[0]);

	// �E����
	frustum.normal[1].x = matrix4X4._14 - matrix4X4._11;
	frustum.normal[1].y = matrix4X4._24 - matrix4X4._21;
	frustum.normal[1].z = matrix4X4._34 - matrix4X4._31;
	frustum.normal[1] = Math::Normalize(frustum.normal[1]);
	frustum.shortest_direction[1] = Math::Dot(frustum.normal[1], frustum.near_vertices_position[1]);

	// ������
	frustum.normal[2].x = matrix4X4._14 + matrix4X4._12;
	frustum.normal[2].y = matrix4X4._24 + matrix4X4._22;
	frustum.normal[2].z = matrix4X4._34 + matrix4X4._32;
	frustum.normal[2] = Math::Normalize(frustum.normal[2]);
	frustum.shortest_direction[2] = Math::Dot(frustum.normal[2], frustum.near_vertices_position[0]);

	// �㑤��
	frustum.normal[3].x = matrix4X4._14 - matrix4X4._12;
	frustum.normal[3].y = matrix4X4._24 - matrix4X4._22;
	frustum.normal[3].z = matrix4X4._34 - matrix4X4._32;
	frustum.normal[3] = Math::Normalize(frustum.normal[3]);
	frustum.shortest_direction[3] = Math::Dot(frustum.normal[3], frustum.near_vertices_position[2]);

	// ������
	frustum.normal[4].x = -matrix4X4._14 - matrix4X4._13;
	frustum.normal[4].y = -matrix4X4._24 - matrix4X4._23;
	frustum.normal[4].z = -matrix4X4._34 - matrix4X4._33;
	frustum.normal[4] = Math::Normalize(frustum.normal[4]);
	frustum.shortest_direction[4] = Math::Dot(frustum.normal[4], frustum.far_vertices_position[0]);

	// ��O����
	frustum.normal[5].x = -matrix4X4._14 + matrix4X4._13;
	frustum.normal[5].y = -matrix4X4._24 + matrix4X4._23;
	frustum.normal[5].z = -matrix4X4._34 + matrix4X4._33;
	frustum.normal[5] = Math::Normalize(frustum.normal[5]);
	frustum.shortest_direction[5] = Math::Dot(frustum.normal[5], frustum.near_vertices_position[0]);

	//�e���E��outLineNorm��ʂ̖@���̊O�ς��狁�߂Đ��K������
	// �������E�� ( frustum[0]�~frustum[2] )
	frustum.out_line_norm[0] = Math::Normalize(Math::Cross(frustum.normal[0], frustum.normal[2]));
	// �E�����E�� ( frustum[2]�~frustum[1] )
	frustum.out_line_norm[1] = Math::Normalize(Math::Cross(frustum.normal[2], frustum.normal[1]));
	// �E�㋫�E�� ( frustum[1]�~frustum[3] )
	frustum.out_line_norm[2] = Math::Normalize(Math::Cross(frustum.normal[1], frustum.normal[3]));
	// ���㋫�E�� ( frustum[3]�~frustum[0] )
	frustum.out_line_norm[3] = Math::Normalize(Math::Cross(frustum.normal[3], frustum.normal[0]));
}

bool CameraController::IntersectFrustum(AxisAlignedBoundingBox* aabb)
{
	// ������i�t���X�^���j�Ƃ̌����E���O����
	// AABB��������̓����ɂ���			�Fcollision_state = 0
	// AABB��������̋��E�ƌ�����Ă���Fcollision_state = 1
	// AABB��������̊O���ɂ���			�Fcollision_state = 2

	// ������������ɂ���Ƃ��Ă���
	frustum.collision_state = true;

	for (int i = 0; i < 6; i++)//�������6��
	{
		//�C�e���ʂ̖@���̐�����p����AABB�̂W���_�̒�����ŋߓ_�ƍŉ��_�����߂�
		DirectX::XMFLOAT3 nega_pos = aabb->center;	// �ŋߓ_
		DirectX::XMFLOAT3 posi_pos = aabb->center;	// �ŉ��_
		if (frustum.normal[i].x > 0.0f)
		{
			nega_pos.x -= aabb->radius.x;
			posi_pos.x += aabb->radius.x;
		}
		else
		{
			nega_pos.x += aabb->radius.x;
			posi_pos.x -= aabb->radius.x;
		}
		if (frustum.normal[i].y > 0.0f)
		{
			nega_pos.y -= aabb->radius.y;
			posi_pos.y += aabb->radius.y;
		}
		else
		{
			nega_pos.y += aabb->radius.y;
			posi_pos.y -= aabb->radius.y;
		}
		if (frustum.normal[i].z > 0.0f)
		{
			nega_pos.z -= aabb->radius.z;
			posi_pos.z += aabb->radius.z;
		}
		else
		{
			nega_pos.z += aabb->radius.z;
			posi_pos.z -= aabb->radius.z;
		}

		//  �e���ʂƂ̓��ς��v�Z���A�����E���O������s��
		//  �O���ƕ�����Ώ�����break���m�肳����
		//  ������Ԃł���΁A�X�e�[�^�X��ύX���Ă��玟�̕��ʂƂ̃`�F�b�N�ɑ�����
		//  �����ł���΁A���̂܂܎��̕��ʂƂ̃`�F�b�N�ɑ�����
		const float dot_negative = Math::Dot(frustum.normal[i], nega_pos);
		const float dot_positive = Math::Dot(frustum.normal[i], posi_pos);

		if (dot_negative < frustum.shortest_direction[i] && dot_positive < frustum.shortest_direction[i])
		{
			frustum.collision_state = false;
			break;
		}
		else if ((dot_negative - frustum.shortest_direction[i]) * (dot_positive - frustum.shortest_direction[i]) < 0.0f)
		{
			frustum.collision_state = true;
		}
	}

	// ������Ԃ̏ڍה���
	// AABB�̖ʂ̒��S�_�Ɩ@�������A���S�_���王����̂W���_�ɑ΂��ĕ\��������s���B
	// �W���_�S�Ă��\���ɂ���΁A�������Ă��Ȃ��������Ƃ��m�肷��̂�collisionState���X�V����return����

	if (frustum.collision_state == 1)	// AABB��������̋��E�ƌ�����Ă���ꍇ�A�ڍה�����s��
	{
		DirectX::XMFLOAT3 aabb_face_center = {};	// �ʂ̒��S�_
		DirectX::XMFLOAT3 normal = {};			// �ʂ̖@���i�P�ʉ��j

		for (int i = 0; i < 6; i++)	// AABB�̂U�ʕ����[�v
		{
			switch (i)
			{
			case 0:
				normal = { 1,0,0 };
				aabb_face_center = { aabb->center.x + aabb->radius.x, aabb->center.y, aabb->center.z };
				break;
			case 1:
				normal = { 0,1,0 };
				aabb_face_center = { aabb->center.x, aabb->center.y + aabb->radius.y, aabb->center.z };
				break;
			case 2:
				normal = { 0,0,1 };
				aabb_face_center = { aabb->center.x, aabb->center.y, aabb->center.z + aabb->radius.z };
				break;
			case 3:
				normal = { -1,0,0 };
				aabb_face_center = { aabb->center.x - aabb->radius.x, aabb->center.y, aabb->center.z };
				break;
			case 4:
				normal = { 0,-1,0 };
				aabb_face_center = { aabb->center.x, aabb->center.y - aabb->radius.y, aabb->center.z };
				break;
			case 5:
				normal = { 0,0,-1 };
				aabb_face_center = { aabb->center.x, aabb->center.y, aabb->center.z - aabb->radius.z };
				break;
			default:
				break;
			}
			for (int j = 0; j < 8; j++)	// ������̂W���_�����[�v
			{
				DirectX::XMFLOAT3 vec_near = {};
				if (j < 4)
				{
					vec_near = { frustum.near_vertices_position[j].x - aabb_face_center.x, frustum.near_vertices_position[j].y - aabb_face_center.y, frustum.near_vertices_position[j].z - aabb_face_center.z };
				}
				else
				{
					vec_near = { frustum.far_vertices_position[j - 4].x - aabb_face_center.x, frustum.far_vertices_position[j - 4].y - aabb_face_center.y, frustum.far_vertices_position[j - 4].z - aabb_face_center.z };
				}

				// �\���`�F�b�N�B��ł���������΁A���̖ʂł͌������Ă���ʒu�֌W�ɂ���
				if (Math::Dot(vec_near, normal) < 0)
				{
					break;
				}

				// �W���_���S�ĕ\���ɂ���΁A�������Ă��Ȃ����Ƃ��m�肷�邽��collisionState��ύX����return
				if (j == 7)
				{
					frustum.collision_state = false;
					return frustum.collision_state;
				}
			}
		}
	}
	return frustum.collision_state;
}

void CameraController::DrawDebugGUI()
{
	Camera& camera = Camera::Instance();
	const Graphics& graphics = Graphics::Instance();
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
			DirectX::XMFLOAT3 a{};
			a.x = DirectX::XMConvertToDegrees(this->angle.x);
			a.y = DirectX::XMConvertToDegrees(this->angle.y);
			a.z = DirectX::XMConvertToDegrees(this->angle.z);
			ImGui::InputFloat3("Radian", &this->angle.x);
			ImGui::InputFloat3("Degree", &a.x);
			// �����_
			ImGui::InputFloat3("target", &target.x);
		}
		DirectX::XMFLOAT4X4 view{};			// �r���[�s��
		DirectX::XMFLOAT4X4 projection{};	// ���e�s��

		DirectX::XMFLOAT3	eye{ 0,1,10 };		// �J�����̈ʒu
		DirectX::XMFLOAT3	focus{ 0,0,0 };	// �J�����̕���
		DirectX::XMFLOAT3	up{ 0,1,0 };		// �J�����̏�����A�ʏ��<0.0f�A1.0f�A0.0f>
		DirectX::XMFLOAT3	front{};		// �J�����̑O������
		DirectX::XMFLOAT3	right{};		// �J�����̉E����

		ImGui::Text("width:%f", camera.GetWidth());
		ImGui::Text("height:%f", camera.GetHeight());
		ImGui::Text("aspect:%f", camera.GetAspect());
		ImGui::Text("fovY:%f", camera.GetFov());
		ImGui::Text("nearZ:%f", camera.GetNear());
		ImGui::Text("farZ:%f", camera.GetFar());

		// ���s���e�J����
		if (camera.GetOrthMode() == true)
		{
			ImGui::Checkbox("ViewMap", &view_map);
			if (view_map)
			{
				camera.SetWidth(graphics.GetScreenWidth() / 5);
				camera.SetHeight(graphics.GetScreenHeight() / 5);
			}
			else
			{
				camera.SetWidth(graphics.GetScreenWidth() / 30);
				camera.SetHeight(graphics.GetScreenHeight() / 30);
			}
		}
		else	//�������e�J����
		{
			// �J�����̍���
			ImGui::DragFloat("Height", &camera_height);
			ImGui::Checkbox("ViewMap", &view_map);
			if (view_map)
			{
				camera_height = 400.f;
			}
			else
			{
				camera_height = 50.f;
			}
		}
	}
	ImGui::End();
}