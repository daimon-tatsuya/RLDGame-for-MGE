#pragma once
//**********************************************************
//
//		WireLambertShader
//
//**********************************************************

#include "Liblary/Systems/Shader.h"

/// <summary>
/// ���C���[�t���[���ŕ\��
/// </summary>
class WireLambertShader : public Shader
{
public:
	WireLambertShader(ID3D11Device* device);

	~WireLambertShader() override = default;

	// �`��J�n
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;

	// �`��I��
	void Deactivate(ID3D11DeviceContext* context) override;
private:
};
