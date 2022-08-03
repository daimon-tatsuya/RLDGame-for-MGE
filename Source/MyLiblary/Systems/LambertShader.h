#pragma once
//**********************************************************
//
//		LambertShader�N���X
//
//**********************************************************

#include "Liblary/Systems/Shader.h"

/// <summary>
///	 �g�U����
/// </summary>
class LambertShader : public Shader
{
private:

public:

	LambertShader(ID3D11Device* device);

	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	virtual ~LambertShader() = default;

	// �`��J�n
	void Activate(ID3D11DeviceContext* context, const RenderContext& render_context)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// �`��I��
	void Deactivate(ID3D11DeviceContext* context) override;

private:
public:
};
