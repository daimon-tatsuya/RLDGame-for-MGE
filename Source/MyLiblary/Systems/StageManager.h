#pragma once
//**********************************************************
//
//		StageManager�N���X
//
//**********************************************************
#include <vector>
#include <memory>
#include <d3d11.h>
#include "Engine/Systems/Math.h"

//�O���錾
class Stage;
class Shader;
struct HitResult;

/// <summary>
/// Stage���Ǘ�����N���X
/// </summary>
class StageManager final
{
private:
	std::vector<std::shared_ptr<Stage>>		stages;

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
};
