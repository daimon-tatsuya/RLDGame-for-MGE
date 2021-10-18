#pragma once
#include <DirectXMath.h>
#include "Engine/Systems/Shader.h"
#include "Engine/AI/Telegram.h"
#include "Engine/Objects/Model.h"
class Object
{
private:

protected:

public:

	Object() {}
	virtual ~Object() {}

	// �s��X�V����
	void UpdateTransform();
	// �X�V����
	virtual void Update(float elapsed_time) = 0;

	// �`�揈��
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) = 0;

	// �f�o�b�O�pGUI�`��
	virtual	void DrawDebugGUI() {}

	// �f�o�b�O�v���~�e�B�u�`��
	virtual	void DrawDebugPrimitive() {}

	// �ʒu���擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// �ʒu��ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// ��]�p�x���擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// ��]�p�x��ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// �X�P�[�����擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// �X�P�[����ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//���b�Z�[�W��M����
	virtual bool OnMessage(const Telegram& msg) = 0;

	//character���Ƃ�id���Z�b�g���ĊǗ�����
	void	SetId(int id) { this->id = id; }
	int		GetId() { return id; }

	//���f�����擾
	void	SetModel(char* pass) { this->model = new Model(pass); }
	//���f����ݒ�
	Model* GetModel() { return model; }

	//�p���s��̎擾
	 DirectX::XMFLOAT4X4 GetTransform() { return transform; }

private:

protected:
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	DirectX::XMFLOAT3	scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4	transform =
	{ 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	int									id = 0;
	Model* model = nullptr;
public:
};
