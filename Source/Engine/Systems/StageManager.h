#pragma once

#include <vector>
#include "Engine/Systems/Stage.h"
#include "Engine/Systems/DungeonMake.h"
// �X�e�[�W�}�l�[�W���[
class StageManager
{
private:
	StageManager() {}
	~StageManager();

public:
	// �B��̃C���X�^���X�擾
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// �X�e�[�W�o�^
	void Register(Stage* stage);

	// �X�e�[�W�S�폜
	void Clear();

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

private:
	std::vector<Stage*>		stages;

};
