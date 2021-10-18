#pragma once
#include "Engine/Systems/Shader.h"

class WireLambertShader : public Shader
{
public:
	WireLambertShader(ID3D11Device* device);
	~WireLambertShader() {};
	void Activate(ID3D11DeviceContext* context, const RenderContext& rc)override;
	// �`��J�n
	void Draw(ID3D11DeviceContext* context, const Model* model) override;
	// �`��I��
	void Deactivate(ID3D11DeviceContext* context) override;
private:
};
