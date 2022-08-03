#pragma once
//**********************************************************
//
//		LambertShaderクラス
//
//**********************************************************

#include "Liblary/Systems/Shader.h"

/// <summary>
///	 拡散反射
/// </summary>
class LambertShader : public Shader
{
private:

public:

	LambertShader(ID3D11Device* device);

	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~LambertShader() = default;

	// 描画開始
	void Activate(ID3D11DeviceContext* context, const RenderContext& render_context)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;

private:
public:
};
