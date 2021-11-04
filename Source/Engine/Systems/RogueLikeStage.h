#pragma once
#pragma once

#include "Engine/Systems/Stage.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/DungeonMake.h"

// ���[�O���C�N�X�e�[�W
class RogueLikeStage : public Stage
{
private:
public:
	 RogueLikeStage(RogueLikeDungeon* rogue_like_dungeon);
	 ~RogueLikeStage();

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	//���b�Z�[�W��M����
	bool OnMessage(const Telegram& msg) override;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI()override;

	//�I�u�W�F�N�g�z�u
	void SetStageObject(std::vector<std::vector<RogueLikeMap>> map_role);

	//�X�e�[�W�̍폜
	void Clear();

private:
protected:
	std::vector <Stage> stage_chip;
	RogueLikeDungeon*  db_rogue_like_dungeon;
public:
};
