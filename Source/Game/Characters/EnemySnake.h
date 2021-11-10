#pragma once

#include "Engine/Systems/EnemyBase.h"
//Todo EnemyBase �R�����g
class EnemySnake :
	public EnemyBase
{
private:


protected:


public:
	//�e�X�e�[�g
	//�q�X�e�[�g
private:


protected:


public:

	EnemySnake(RogueLikeDungeon* rogue_like_dungeon);
	EnemySnake();
	virtual ~EnemySnake();

	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	// �j��
	void Destroy();

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI()override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;



	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged()override;

	// ���S�������ɌĂ΂��
	void OnDead()override;

	//���b�Z�[�W��M����
	virtual bool OnMessage(const Telegram& msg) override;

};
