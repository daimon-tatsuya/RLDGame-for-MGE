//**********************************************************
//
//		PlayerBase�N���X
//
//**********************************************************
#include "Engine/Systems/PlayerBase.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"

void PlayerBase::CollisionPlayerToEnemis()
{
	//CharacterManager& character_manager = CharacterManager::Instance();

	//// �S�Ă̓G�Ƒ�������ŏՓˏ���
	//int enemy_count = character_manager.GetEnemyCount();
	//for (int i = 0; i < enemy_count; ++i)
	//{
	//	Character* enemy = character_manager.GetEnemy(i);

	//	// �Փˏ���
	//	DirectX::XMFLOAT3 out_enemy_position, out_player_position;
	//	if (Collision::IntersectSphereToSphere(
	//		this->position,
	//		this->radius,
	//		enemy->GetPosition(),
	//		enemy->GetRadius(),
	//		out_player_position,
	//		out_enemy_position)
	//		)
	//	{
	//		// �����o����̈ʒu�ݒ�
	//		enemy->SetPosition(out_enemy_position);
	//		this->SetPosition(out_player_position);
	//	}
	//}
}

//DirectX::XMFLOAT3 PlayerBase::GetMoveVec(const DirectX::XMFLOAT3& camera_right, const DirectX::XMFLOAT3& camera_front) const
//{
//	//���͂��ꂽ�����擾
//	GamePad& game_pad = Input::Instance().GetGamePad();
//	float ax = game_pad.GetAxisLX();
//	float ay = game_pad.GetAxisLY();
//
//	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���Ȃ�悤�ɂ���
//
//	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
//	float camera_rightX = camera_right.x;
//	float camera_rightZ = camera_right.z;
//	float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
//	if (camera_right_length > 0.0f)
//	{
//		// �P�ʃx�N�g����
//		camera_rightX /= camera_right_length;
//		camera_rightZ /= camera_right_length;
//	}
//
//	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
//	float camera_frontX = camera_front.x;
//	float camera_frontZ = camera_front.z;
//	float camera_front_length = sqrtf(camera_frontX * camera_frontX + camera_frontZ * camera_frontZ);
//	if (camera_front_length > 0.0f)
//	{
//		// �P�ʃx�N�g����
//		camera_frontX /= camera_front_length;
//		camera_frontZ /= camera_front_length;
//	}
//
//	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
//	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
//	// �i�s�x�N�g�����v�Z����
//	DirectX::XMFLOAT3 vec;
//	vec.x = (camera_rightX * ax) + (camera_frontX * ay);
//	vec.z = (camera_rightZ * ax) + (camera_frontZ * ay);
//	// Y�������ɂ͈ړ����Ȃ�
//	vec.y = 0.0f;
//
//	return vec;
//}
//
//bool PlayerBase::InputMove(float elapsedTime)
//{
//	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
//	Camera& camera = Camera::Instance();
//	DirectX::XMFLOAT3 camera_right = camera.GetRight();
//	DirectX::XMFLOAT3 camera_front = camera.GetFront();
//	//if (lockonState != LockonState::NotLocked)
//	//{
//	//	//	���b�N�I�����͓G�ւ̌����ōl����
//	//	cameraFront = lockDirection;
//	//	DirectX::XMVECTOR	z = DirectX::XMLoadFloat3(&lockDirection);
//	//	DirectX::XMVECTOR	y = DirectX::XMVectorSet(0, 1, 0, 0);
//	//	DirectX::XMVECTOR	x = DirectX::XMVector3Cross(y, z);
//	//	DirectX::XMStoreFloat3(&cameraRight, x);
//	//}
//	// �i�s�x�N�g���擾
//	DirectX::XMFLOAT3 move_vec = GetMoveVec(camera_right, camera_front);
//
//	// �ړ�����
//	Move(move_vec.x, move_vec.z, this->move_speed);
//
//	// ���񏈗�
//	//if (jumpCount == 0)
//	//{
//		//if (lockonState != LockonState::NotLocked)
//		//{
//		//	//	���b�N�I���������̓��b�N�I���ΏۂɌ�����
//		//	Turn(elapsedTime, cameraFront.x, cameraFront.z, turnSpeed);
//		//}
//		//else
//		//{
//	Turn(elapsedTime, move_vec.x, move_vec.z, this->turn_speed);
//	//}
////}
//
//// �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
//	return move_vec.x != 0.0f || move_vec.y != 0.0f || move_vec.z != 0.0f;
//}
//
//// �W�����v���͏���
//bool PlayerBase::InputJump(float elapsedTime)
//{
//	// �o�߃t���[��
//	float elapsedFrame = 60.0f * elapsedTime;
//	jump_wait -= elapsedFrame;
//	if (jump_wait > 0)
//		return false;
//
//	// �W�����v�񐔐���
//	if (jump_count < jump_limit)
//	{
//		GamePad& gamePad = Input::Instance().GetGamePad();
//		if (gamePad.GetButtonDown() & GamePad::BTN_A)
//		{
//			float ax = gamePad.GetAxisLX();
//			float ay = gamePad.GetAxisLY();
//			if (ax * ax + ay * ay < 0.3f * 0.3f || jump_count == 0)
//			{
//				// �W�����v
//				Jump(jump_speed);
//			}
//			else
//			{
//				// �󒆃_�b�V��
//				// ���g�̉�]�l����O���������߂Ă��̕����֐i�܂���
//				float frontX = sinf(angle.y) * 12;
//				float frontZ = cosf(angle.y) * 12;
//				AirDush(frontX, frontZ, 15.0f);
//			}
//			jump_count++;
//			jump_wait = jump_wait_max;
//			// �W�����v���͂���
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool PlayerBase::InputAttack()
//{
//	return false;
//}
