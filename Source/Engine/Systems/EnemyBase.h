#pragma once

#include "Engine/Systems/Character.h"

//Todo EnemyBase �R�����g
//�G�N���X�̊��N���X
class EnemyBase :public Character
{
private:

protected:
	int	 state = 0;
	bool attack_flg = false;
	int id = 0;
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

	// �f�o�b�O�v���~�e�B�u�`��
	//virtual void DrawDebugPrimitive() override;

	// �U���t���O�擾
	bool	GetAttackFlg() { return attack_flg; }

	// �U���t���O�Z�b�g
	void	SetAttackFlg(bool flg) { attack_flg = flg; };

	//�G���m�̌�������
	void CollisionEnemyToEnemy();

};
