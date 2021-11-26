#pragma once
//**********************************************************
//
//		LambertShaderƒNƒ‰ƒX
//
//**********************************************************

#include "Engine/Systems/Shader.h"

/// <summary>
///	 ŠgU”½Ë
/// </summary>
class LambertShader : public Shader
{
private:

public:

	LambertShader(ID3D11Device* device);
	virtual ~LambertShader() {};

	// •`‰æŠJn
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// •`‰æI—¹
	void Deactivate(ID3D11DeviceContext* context) override;

private:
public:
};
