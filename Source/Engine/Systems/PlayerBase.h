#pragma once
#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/Character.h"
//Todo�@�R�����g
class PlayerBase : public Character
{
private:

public:

	PlayerBase() {};
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	~PlayerBase()override {};

	// �X�V����
	virtual void Update(float elapsedTime)override = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override = 0;

	// �X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec(const DirectX::XMFLOAT3& cameraRight, const DirectX::XMFLOAT3& cameraFront) const;

	// �ړ����͏���
	virtual bool InputMove(float elapsedTime);

	// �W�����v���͏���
	bool InputJump(float elapsedTime);

	// �U�����͏���
	bool InputAttack();

	// �v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
	void CollisionPlayerToEnemis();

private:

	float				move_speed = 5.0f;
	float				turn_speed = DirectX::XMConvertToRadians(720);
	bool				attack_flag = false;
	float				jump_speed = 20.0f;
	int					jump_count = 0;
	float				jump_wait = 10.0f;
	float				jump_wait_max = 10.0f;
	int					jump_limit = 2;

	//LockonState			lockonState = LockonState::NotLocked;
	//float							lockonTargetChangeTime = 0;
	//float							lockonTargetChangeTimeMax = 8;
	////Todo�@character��������Ȃ����̑��̂��̂ɂ��ł���悤�ɂ���
	//Character* lockonCharacter = nullptr;
	//DirectX::XMFLOAT3	lockDirection = { 0,0,0 };
public:
};
