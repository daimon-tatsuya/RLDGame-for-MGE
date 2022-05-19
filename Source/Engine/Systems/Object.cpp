//**********************************************************
//
//		Object�N���X
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/Systems/Math.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Collision.h"

void Object::UpdateTransform()
{
	// �X�P�[���s����쐬
	const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
#if 0
	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else

	const DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	const DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	const DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	const DirectX::XMMATRIX R = Y * X * Z;

#endif
	// �ʒu�s����쐬
	const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	const DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

void Object::Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader)
{
	shader->Draw(device_context, this->GetModel());
}

float Object::NormalizeAnyAngle(float radian)
{
	return Math::NormalizeRadianAngle(radian);
}

void Object::NormalizeAngle()
{
	angle.x = NormalizeAnyAngle(angle.x);
	angle.y = NormalizeAnyAngle(angle.y);
	angle.z = NormalizeAnyAngle(angle.z);
}

void Object::SendMessaging(const MESSAGE_TYPE msg)
{
	//switch (msg)
	//{
	//case MESSAGE_TYPE::END_PLAYER_TURN:

	//	break;
	//case MESSAGE_TYPE::END_ENEMY_TURN:

	//	break;
	//default:
	//	break;
	//}
}

void	Object::SetModel(const char* pass)
{
	model = std::make_shared<Model>(pass);
}

void Object::UpdateVelocity(float elapsed_time)
{
	// �o�߃t���[��
	const float elapsed_frame = 60.0f * elapsed_time;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsed_frame);

	// �������͍X�V����
	UpdateHorizontalVelocity(elapsed_frame);

	// �����ړ��X�V����
	UpdateVerticalMove(elapsed_time);

	// �����ړ��X�V����
	UpdateHorizontalMove(elapsed_time);
}

void Object::UpdateInvincibleTimer(float elapsed_time)
{
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

void Object::UpdateVerticalVelocity(float elapsed_frame)
{
	// �d�͏���
	gravity_cut_time -= elapsed_frame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsed_frame;
	}
}

void Object::UpdateVerticalMove(float elapsed_time)
{
	// ���������̈ړ���
	float my = velocity.y * elapsed_time;

	// �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	slope_rate = 0.0f;

	// ������
	if (my < 0.0f)
	{
		// ���C�̊J�n�ʒu�͑�����菭����
		const DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		const DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// �n�ʂɐڒn���Ă���
			position = hit.position;

			// �X�Η��̌v�Z
			const float normal_lengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slope_rate = 1.0f - (hit.normal.y / (normal_lengthXZ + hit.normal.y));

			// �@���x�N�g���擾
			normal = hit.normal;

			// ��]
			angle.y += hit.rotation.y;

			// ���n����
			if (!is_ground)
			{
				OnLanding();
			}
			is_ground = true;
			velocity.y = 0.0f;
		}
		else
		{
			// �󒆂ɕ����Ă���
			position.y += my;
			is_ground = false;
		}
	}
	// �㏸��
	else if (my > 0.0f)
	{
		position.y += my;
		is_ground = false;
	}

	// �n�ʂ̌����ɉ����悤��XZ����]
	{
		// Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
		float ax = atan2f(normal.z, normal.y);
		float az = -atan2f(normal.x, normal.y);

		// ���`�⊮�Ŋ��炩�ɉ�]����
		angle.x = Math::LerpFloat(angle.x, ax, 0.2f);
		angle.z = Math::LerpFloat(angle.z, az, 0.2f);
	}
}

void Object::UpdateHorizontalVelocity(float elapsed_frame)
{
	//! XZ���ʂ̑��͂���������
	const float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsed_frame;

		// �󒆂ɂ���Ƃ��͖��C�͂����炷
		if (!is_ground) friction *= air_control;

		// ���C�ɂ�鉡�����̌�������
		if (length > friction)
		{
			// �P�ʃx�N�g����
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		// �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	//! XZ���ʂ̑��͂���������
	if (length <= max_move_speed)
	{
		// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		const float move_vec_length = sqrtf(move_vec_x * move_vec_x + move_vec_z * move_vec_z);
		if (move_vec_length > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsed_frame;

			// �󒆂ɂ���Ƃ��͉����͂����炷
			if (!is_ground) acceleration *= air_control;

			// �ړ��x�N�g���ɂ���������
			velocity.x += move_vec_x * acceleration;
			velocity.z += move_vec_z * acceleration;

			// �ő呬�x����
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > max_move_speed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * max_move_speed;
				velocity.z = vz * max_move_speed;
			}

			// �����ŃK�^�K�^���Ȃ��悤�ɂ���
			if (is_ground && slope_rate > 0.0f)
			{
				velocity.y -= length * slope_rate * elapsed_frame;
			}
		}
	}
	// �ړ��x�N�g�������Z�b�g
	move_vec_x = 0.0f;
	move_vec_z = 0.0f;
}

void Object::UpdateHorizontalMove(float elapsed_time)
{
	// �������͗ʌv�Z
	const float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocity_lengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsed_time;
		float mz = velocity.z * elapsed_time;

		//MoveCheck(mx, mz);
	}
}