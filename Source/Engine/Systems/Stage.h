#pragma once

#include "Engine/Systems/Object.h"

#include "Engine/Systems/Collision.h"

// �X�e�[�W
class Stage : public Object
{
public:
	Stage() {}
	Stage(const char* filename, DirectX::XMFLOAT3& pos,int id)
	{
		this->model = new Model(filename);
		this->position = pos;
		this->angle = { 0,0,0 };
		this->scale = { 1,1,1 };
		this->id = id;
	}

	virtual ~Stage() {}

	// �X�V����
	virtual void Update(float elapsedTime) {};

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader){};

	// ���C�L���X�g
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) { return false; };

	//���b�Z�[�W��M����
	virtual bool OnMessage(const Telegram& msg) override { return false; };

	// �f�o�b�O�pGUI�`��
	virtual void DrawDebugGUI()override {};
};
