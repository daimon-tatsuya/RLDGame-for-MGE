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

	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~NoTextureShader() = default;

	// 描画開始
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;

	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;
};
