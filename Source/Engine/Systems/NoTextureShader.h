#pragma once

#include "Engine/Systems/Shader.h"

//ToDo NoTextureShaderが行うことを書く

class NoTextureShader :
	public Shader
{
public:
	NoTextureShader(ID3D11Device* device);
	~NoTextureShader() {};
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;
	// 描画開始
	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;
};
