#pragma once
#include "Engine/Systems/Stage.h"

class StageMain :
	public Stage
{
private:

public:

	StageMain();
	~StageMain() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

	//���b�Z�[�W��M����
	bool OnMessage(const Telegram& msg) override;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI()override;

private:


public:
};
