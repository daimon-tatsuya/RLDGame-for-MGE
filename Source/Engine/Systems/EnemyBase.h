#pragma once

//**********************************************************
//
//		EnemyBase�N���X
//
//**********************************************************

#include "Engine/Systems/Character.h"

//�G�N���X�̊��N���X
class EnemyBase :public Character
{
private:

protected:

public:


private:

public:

	EnemyBase() {};
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	virtual ~EnemyBase()override {};

	// �j��
	void Destroy();

	//�G���m�̌�������
	void CollisionEnemyToEnemy();

};
