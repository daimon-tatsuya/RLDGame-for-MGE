#pragma once
//**********************************************************
//
//		RogueLikeStage�N���X
//
//**********************************************************

<<<<<<< HEAD
#include "Liblary/Systems/Stage.h"
=======
#include "MyLiblary/Systems/Stage.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3

//�O���錾


class RogueLikeStage
	: public Stage
{
private:
public:
	RogueLikeStage();
	RogueLikeStage(const char* filename, DirectX::XMFLOAT3& pos, const int id):Stage(filename,pos,id){}
	RogueLikeStage(const char* filename, DirectX::XMFLOAT3& pos,DirectX::XMFLOAT3 scale, const int id) :Stage(filename, pos, scale,id) {}
	~RogueLikeStage() override;

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

	// �X�e�[�W�o�^
	void Register(Stage stage);

private:
protected:
	std::vector <Stage> stage_chip;
	//imgui
public:
};
