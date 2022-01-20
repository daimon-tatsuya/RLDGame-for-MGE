//**********************************************************
//
//		Character�N���X
//
//**********************************************************

#include "Engine/Systems/Character.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/Math.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/CharacterManager.h"

bool Character::ApplyDamage(int damage, float invincible_time)
{
	// �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage == 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (current_health <= 0) return false;

	// ���G���Ԓ��̓_���[�W��^���Ȃ�
	if (invincible_timer > 0.0f) return false;

	// ���G���Ԑݒ�
	invincible_timer = invincible_time;

	// �_���[�W����
	current_health -= damage;

	// ���S�ʒm
	if (current_health <= 0)
	{
		OnDead();
	}
	// �_���[�W�ʒm
	else
	{
		OnDamaged();
	}

	// ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;
}

void Character::Destroy()
{
	CharacterManager& character_manager = CharacterManager::Instance();
	character_manager.Remove(this);
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// ���͂ɗ͂�������
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}


void Character::Move(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	move_vecX = vx;
	move_vecZ = vz;

	// �ő呬�x�ݒ�
	max_move_speed = speed;
}

void Character::MoveCheck(float mx, float mz)
{
	// ���C�̊J�n�ʒu�ƏI�_�ʒu
	DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
	DirectX::XMFLOAT3 end = { position.x + mx, position.y + step_offset, position.z + mz };

	// ���C�L���X�g�ɂ��ǔ���
	HitResult hit;
	if (StageManager::Instance().RayCast(start, end, hit))
	{
		// �ǂ܂ł̃x�N�g��
		DirectX::XMVECTOR v_start = DirectX::XMLoadFloat3(&start);
		DirectX::XMVECTOR v_end = DirectX::XMLoadFloat3(&end);
		DirectX::XMVECTOR v_vec = DirectX::XMVectorSubtract(v_end, v_start);

		// �ǂ̖@��
		DirectX::XMVECTOR v_normal = DirectX::XMLoadFloat3(&hit.normal);

		// ���˃x�N�g����@���Ɏˉe
		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(v_vec), v_normal);

		// �␳�ʒu�̌v�Z
		DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(v_normal, Dot, v_end);
		DirectX::XMFLOAT3 collect_position{};
		DirectX::XMStoreFloat3(&collect_position, CollectPosition);

		// �ǂ�������փ��C�L���X�g
		HitResult hit2;
		if (!StageManager::Instance().RayCast(hit.position, collect_position, hit2))
		{
			// �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
			position.x = collect_position.x;
			position.z = collect_position.z;
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

// �A�N�V�����Q�[���p�Ȃ̂Ō�� https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	const float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;

	// �P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O���������߂�
	const float frontX = sinf(angle.y);
	const float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	const float dot = (frontX * vx) + (frontZ * vz);

	// ���ϒl��-1.0�`1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	const float cross = (frontX * vz) - (frontZ * vx);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

void Character::Jump(float speed)
{
	// ������̗͂�ݒ�
	velocity.y = speed;
}

void Character::AirDash(float vx, float vz, float gravity_cut_time)
{
	velocity.x = vx;
	velocity.z = vz;
	velocity.y = 0;
	this->gravity_cut_time = gravity_cut_time;
}

void Character::UpdateVelocity(float elapsed_time)
{
	// �o�߃t���[��
	float elapsed_frame = 60.0f * elapsed_time;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsed_frame);

	// �������͍X�V����
	UpdateHorizontalVelocity(elapsed_frame);

	// �����ړ��X�V����
	UpdateVerticalMove(elapsed_time);

	// �����ړ��X�V����
	UpdateHorizontalMove(elapsed_time);
}


void Character::UpdateInvincibleTimer(float elapsed_time)
{
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

void Character::UpdateVerticalVelocity(float elapsed_frame)
{
	// �d�͏���
	gravity_cut_time -= elapsed_frame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsed_frame;
	}
}


void Character::UpdateVerticalMove(float elapsed_time)
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


void Character::UpdateHorizontalVelocity(float elapsed_frame)
{
	//! XZ���ʂ̑��͂���������
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
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
		float move_vec_length = sqrtf(move_vecX * move_vecX + move_vecZ * move_vecZ);
		if (move_vec_length > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsed_frame;

			// �󒆂ɂ���Ƃ��͉����͂����炷
			if (!is_ground) acceleration *= air_control;

			// �ړ��x�N�g���ɂ���������
			velocity.x += move_vecX * acceleration;
			velocity.z += move_vecZ * acceleration;

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
	move_vecX = 0.0f;
	move_vecZ = 0.0f;
}

void Character::UpdateHorizontalMove(float elapsed_time)
{
	// �������͗ʌv�Z
	float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocity_lengthXZ > 0.0f)
	{
		// �����ړ��l
		float mx = velocity.x * elapsed_time;
		float mz = velocity.z * elapsed_time;

		MoveCheck(mx, mz);
	}
}

