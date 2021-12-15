#pragma once
//**********************************************************
//
//		Stage�N���X
//
//**********************************************************
#include "Engine/Systems/Object.h"

struct HitResult;

/// <summary>
/// �X�e�[�W�̊��N���X
/// </summary>
class Stage
: public Object
{
public:
	Stage() = default;

	Stage(const char* filename, DirectX::XMFLOAT3& pos, const int id)
	{
		SetModel(filename);
		SetPosition(pos);
		SetAngle({ 0,0,0 });
		SetScale({ 1,1,1 });
		SetId(id);
	}

	~Stage() override = default;

	// �X�V����
	void Update(float elapsed_time) override {}

	// �`�揈��
	void Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader) override {}

	// ���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) { return false; }

	//���b�Z�[�W��M����
	 bool OnMessage(const Telegram& msg) override { return false; }

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI()override {}
};
