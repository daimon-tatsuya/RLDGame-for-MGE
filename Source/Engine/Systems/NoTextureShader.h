#pragma once
//**********************************************************
//
//		NoTextureShaderクラス
//
//**********************************************************
#include "Engine/Systems/Shader.h"


/// <summary>
/// スタティックメッシュ用
/// </summary>
class NoTextureShader :
	public Shader
{
public:

	NoTextureShader(ID3D11Device* device);
	~NoTextureShader() {};
	// 描画開始
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;

	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;
};
