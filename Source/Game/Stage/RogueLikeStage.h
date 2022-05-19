#pragma once
//**********************************************************
//
//		RogueLikeStage�N���X
//
//**********************************************************

#include "Engine/Systems/Stage.h"

//�O���錾


class RogueLikeStages
	: public Stage
{
private:
public:
	RogueLikeStages();
	~RogueLikeStages() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) override;

	// ���C�L���X�g
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

	//���b�Z�[�W��M����
	bool OnMessage(const Telegram& telegram) override;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI()override;

	//�I�u�W�F�N�g�z�u
	void SetStageObject();

	//�X�e�[�W�̍폜
	void Clear();

private:
protected:
	std::vector <std::shared_ptr<Stage>> stage_chip;
	//imgui
public:
};
