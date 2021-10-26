#pragma once
#include "Engine/Systems/EnemyBase.h"
//Todo�@�R�����g
class EnemySnake :
	public EnemyBase
{
private:
protected:
public:
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	// �j��
	void Destroy();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI()override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged()override;

	// ���S�������ɌĂ΂��
	void OnDead()override;

	//���b�Z�[�W��M����
	virtual bool OnMessage(const Telegram& msg) override;
private:
protected:
public:
};
