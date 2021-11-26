#pragma once
//**********************************************************
//
//		WireLambertShader
//
//**********************************************************
#include "Engine/Systems/Shader.h"

/// <summary>
/// ワイヤーフレームで表示
/// </summary>
class WireLambertShader : public Shader
{
public:
	WireLambertShader(ID3D11Device* device);
	~WireLambertShader() {};
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;
	// 描画開始
	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// 描画終了
	void Deactivate(ID3D11DeviceContext* context) override;
private:
};
