#pragma once
//**********************************************************
//
//		LambertShader�N���X
//
//**********************************************************

#include "Engine/Systems/Shader.h"

/// <summary>
///	 �g�U����
/// </summary>
class LambertShader : public Shader
{
private:

public:

	LambertShader(ID3D11Device* device);
	virtual ~LambertShader() {};

	// �`��J�n
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// �`��I��
	void Deactivate(ID3D11DeviceContext* context) override;

private:
public:
};
