#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <string>

#include "Engine/Systems/Math.h"

//ToDo Sprite コメント

// スプライト
class Sprite
{
public:
	Sprite();
	Sprite(const wchar_t* filename);
	~Sprite() {}

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// 描画実行
	void Render(ID3D11DeviceContext* dc,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// テクスチャ幅取得
	int GetTextureWidth() const { return texture_width; }

	// テクスチャ高さ取得
	int GetTextureHeight() const { return texture_height; }

	void TextOut(ID3D11DeviceContext* immediateContext, std::string s, float x, float y, float w, float h, float r = 1, float g = 1, float b = 1, float a = 1) const;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>					vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
	D3D11_TEXTURE2D_DESC									texture2d_desc;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;

	int texture_width = 0;
	int texture_height = 0;
};

class SpriteBatch
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;

	const size_t MAX_INSTANCES = 256;
	struct Instance
	{
		DirectX::XMFLOAT4X4 ndc_transform;
		DirectX::XMFLOAT4 texcoord_transform;
		DirectX::XMFLOAT4 color;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_buffer;

public:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
	};

	SpriteBatch(ID3D11Device* device, const wchar_t* filename, size_t maxInstance = 256);

	void Begin(ID3D11DeviceContext* immediateContext);
	void Render(ID3D11DeviceContext* immediateContext, float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle/*degree*/, float r, float g, float b, float a);
	void End(ID3D11DeviceContext* immediateContext);

private:
	D3D11_VIEWPORT viewport;

	UINT count_instance = 0;
	Instance* instances = nullptr;
};
