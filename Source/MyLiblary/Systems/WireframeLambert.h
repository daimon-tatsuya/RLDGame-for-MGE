#pragma once
//**********************************************************
//
//		WireLambertShader
//
//**********************************************************

#include "Liblary/Systems/Shader.h"

/// <summary>
/// ワイヤーフレームで表示
/// </summary>
class WireLambertShader : public Shader
{
public:
	WireLambertShader(ID3D11Device* device);

	~WireLambertShader() override = default;

	// 描画開始
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;

	void Draw(ID3D11DeviceContext* context, const Model* model) override;

	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;
private:
};
