//**********************************************************
//
//		Object�N���X
//
//**********************************************************

#include "Liblary/Systems/Object.h"
#include "Liblary/Systems/Math.h"
#include "Liblary/Objects/Model.h"
#include "Liblary/Systems/StageManager.h"
#include "Liblary/Systems/Collision.h"
#include "Liblary/Systems/Logger.h"

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
	switch (msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:
		LOG("\n")
		break;
	case MESSAGE_TYPE::END_ENEMY_TURN:
		LOG("\n")
		break;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:
		LOG("\n")
		break;
	case MESSAGE_TYPE::GO_MAX_FLOOR:
		LOG("\n")
		break;
	default:
		LOG("\n")
		break;
	}
}

void	Object::SetModel(const char* pass)
{
	model = std::make_shared<Model>(pass);
}

void Object::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
#if 0
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
#endif
	// �ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

// ���͏����X�V
void Object::UpdateVelocity(float elapsedTime)
{
	// �o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);

	// �������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);

	// �����ړ��X�V����
	UpdateVerticalMove(elapsedTime);

	// �����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);
}

// ���G���ԍX�V
void Object::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsedTime;
	}
}

// �������͍X�V����
void Object::UpdateVerticalVelocity(float elapsedFrame)
{
	// �d�͏���
	gravity_cut_time -= elapsedFrame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsedFrame;
	}
}

// �����ړ��X�V����
void Object::UpdateVerticalMove(float elapsedTime)
{
	// ���������̈ړ���
	float my = velocity.y * elapsedTime;

	// �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	slope_rate = 0.0f;

	// ������
	if (my < 0.0f)
	{
		// ���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// �n�ʂɐڒn���Ă���
			position = hit.position;

			// �X�Η��̌v�Z
			float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slope_rate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

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

// �������͍X�V����
void Object::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ���ʂ̑��͂���������
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsedFrame;

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

	// XZ���ʂ̑��͂���������
	if (length <= max_move_speed)
	{
		// �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf(move_vec_x * move_vec_x + move_vec_z * move_vec_z);
		if (moveVecLength > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsedFrame;

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
				velocity.y -= length * slope_rate * elapsedFrame;
			}
		}
	}
	// �ړ��x�N�g�������Z�b�g
	move_vec_x = 0.0f;
	move_vec_z = 0.0f;
}

// �����ړ��X�V����
void Object::UpdateHorizontalMove(float elapsedTime)
{
	// �������͗ʌv�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		// ���C�̊J�n�ʒu�ƏI�_�ʒu
		DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + step_offset, position.z + mz };

		// ���C�L���X�g�ɂ��ǔ���
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// �ǂ܂ł̃x�N�g��
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// �ǂ̖@��
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			// ���˃x�N�g����@���Ɏˉe
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// �␳�ʒu�̌v�Z
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			// �ǂ�������փ��C�L���X�g
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				// �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
				position.x = collectPosition.x;
				position.z = collectPosition.z;
			}
			else
			{
				position.x = hit2.position.x;
				position.z = hit2.position.z;
			}
		}
		else
		{
			// �ړ�
			position.x += mx;
			position.z += mz;
		}
	}
}