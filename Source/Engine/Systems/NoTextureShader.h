#pragma once
//**********************************************************
//
//		NoTextureShader�N���X
//
//**********************************************************
#include "Engine/Systems/Shader.h"


/// <summary>
/// �X�^�e�B�b�N���b�V���p
/// </summary>
class NoTextureShader :
	public Shader
{
public:

	NoTextureShader(ID3D11Device* device);
	~NoTextureShader() {};
	// �`��J�n
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;

	// �`��I��
	void Deactivate(ID3D11DeviceContext* context) override;
};
