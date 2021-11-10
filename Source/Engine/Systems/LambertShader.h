#pragma once

#include "Engine/Systems/Shader.h"

//ToDo LambertShaderが行うことを書く

/// <summary>
///
/// </summary>
class LambertShader : public Shader
{
private:

public:

	LambertShader(ID3D11Device* device);
	virtual ~LambertShader() {};

	// 描画開始
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;

private:
public:
};
