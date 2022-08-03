//**********************************************************
//
//		Character�N���X
//
//**********************************************************
#include "Liblary/Systems/Character.h"
#include "Liblary/Systems/CharacterManager.h"
//�f�X�g���N�^
Character::~Character()
{
	//�ŒZ�o�H�̔j��
	if (shortest_path!=nullptr)
	{
		shortest_path->Clear();
		delete shortest_path;
	}
}

void Character::Destroy()
{
	//�L�����N�^�[�}�l�[�W���[�̏������X�g�Ɏ��M��ǉ�
	CharacterManager::Instance().Remove(this);
}

bool Character::ApplyDamage(int damage, float invincible_time)
{
	// �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage == 0) return false;

	// ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (GetCurrentHealth() <= 0) return false;

	// ���G���Ԓ��̓_���[�W��^���Ȃ�
	if (invincible_timer > 0.0f) return false;

	// ���G���Ԑݒ�
	invincible_timer = invincible_time;

	// �_���[�W����
	AddCurrentHealth(-1 * damage);

	// ���S�ʒm
	if (GetCurrentHealth() <= 0)
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

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// ���͂ɗ͂�������
	AddVelocity(impulse);
}

void Character::Move(float vx, float vz, float speed)
{
	// �ړ������x�N�g����ݒ�
	move_vec_x = vx;
	move_vec_z = vz;

	// �ő呬�x�ݒ�
	max_move_speed= speed;
}
// �A�N�V�����Q�[���p�Ȃ̂Ō�� https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;

	// �P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(GetAngle().y);
	float frontZ = cosf(GetAngle().y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (frontX * vx) + (frontZ * vz);

	// ���ϒl��-1.0�`1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontX * vz) - (frontZ * vx);

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		AddAngleY(rot);
	}
	else
	{
		AddAngleY(-1 * rot);
	}
}

void Character::Jump(float speed)
{
	SetVelocityY(speed);
}

void Character::AirDush(float vx, float vz, float gravityCutTime)
{
}
