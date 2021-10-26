#pragma once
#include "Engine/Systems/Character.h"
//Todo�@�R�����g
//�G�N���X�̊��N���X
class EnemyBase :public Character
{
private:

public:

	EnemyBase() {};
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	~EnemyBase()override {};

	// �X�V����
	virtual void Update(float elapsedTime) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) = 0;

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


private:

protected:
	int	 state = 0;
	bool attack_flg = false;
	int id = 0;
	float search_range = 0.0f;
	float attack_range = 0.0f;
public:
};
