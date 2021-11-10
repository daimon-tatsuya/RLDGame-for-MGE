#pragma once

#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/Character.h"

//Todo PlayerBase �R�����g
class PlayerBase : public Character
{
private:

public:

	PlayerBase() {};
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	~PlayerBase()override {};
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


public:
};
