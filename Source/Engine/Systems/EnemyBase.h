#pragma once

#include "Engine/Systems/Character.h"

//Todo EnemyBase �R�����g
//�G�N���X�̊��N���X
class EnemyBase :public Character
{
private:

protected:
	bool attack_flg = false;
	float search_range = 0.0f;
	float attack_range = 0.0f;
public:

private:

public:

	EnemyBase() {};
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	~EnemyBase()override {};

	// �j��
	void Destroy();


	// �U���t���O�擾
	bool	GetAttackFlg() { return attack_flg; }

	// �U���t���O�Z�b�g
	void	SetAttackFlg(bool flg) { attack_flg = flg; };

	//�G���m�̌�������
	void CollisionEnemyToEnemy();

};
