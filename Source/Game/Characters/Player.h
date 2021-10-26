#pragma once
#include "Engine/Systems/PlayerBase.h"
#include "Engine/Systems/DungeonMake.h"
class Player :
	public PlayerBase
{
private:

public:

	Player(RogueLikeDungeon& rogue_like_dungeon);
	Player();
	~Player()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI()override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	//���b�Z�[�W��M����
	bool OnMessage(const Telegram& msg) override;

	// ���n�������ɌĂ΂��
	void OnLanding() override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;

	// ���S�������ɌĂ΂��
	void OnDead() override;


	//Todo ���E�㉺�΂߂̕����Ɉ��̑傫���i��
	//bool InputMove(float elapsedTime) override;

private:

public:
};
