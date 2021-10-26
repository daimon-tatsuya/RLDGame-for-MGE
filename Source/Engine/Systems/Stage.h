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
		SetModel(filename);
		SetPosition(pos);
		SetAngle({ 0,0,0 });
		SetScale({ 1,1,1 });
		SetId(id);
		//this->model = stModel(filename);
		//this->position = pos;
		//this->angle = { 0,0,0 };
		//this->scale = { 1,1,1 };
		//this->id = id;
	}
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
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
