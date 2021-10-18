#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>

class Rasterizer
{
public:
	Rasterizer(ID3D11Device* device, D3D11_FILTER samplerFilter = D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_MODE samplerTextureAddressMode = D3D11_TEXTURE_ADDRESS_BORDER, DirectX::XMFLOAT4 samplerBoarderColour = DirectX::XMFLOAT4(0, 0, 0, 0));
	virtual ~Rasterizer() = default;
	Rasterizer(Rasterizer&) = delete;
	Rasterizer& operator=(Rasterizer&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> embedded_vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> embedded_input_layout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> embedded_pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> embedded_rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> embedded_depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> embedded_sampler_state;

public:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
		DirectX::XMFLOAT4 colour;
	};

	void Blit
	(
		ID3D11DeviceContext* immediateContext,
		ID3D11ShaderResourceView* shaderResourceView,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle/*degree*/,
		float r, float g, float b, float a,
		bool useEmbeddedVertexShader = true,
		bool useEmbeddedPixelShader = true,
		bool useEmbeddedRasterizerState = true,
		bool useEmbeddedDepthstencilState = true,
		bool useEmbeddedSamplerState = true
	) const;
	void Blit(
		ID3D11DeviceContext* immediateContext,
		ID3D11ShaderResourceView* shaderResourceView,
		float dx, float dy, float dw, float dh,
		float angle = 0/*degree*/,
		float r = 1, float g = 1, float b = 1, float a = 1,
		bool useEmbeddedVertexShader = true,
		bool useEmbeddedPixelShader = true,
		bool useEmbeddedRasterizerstate = true,
		bool useEmbeddedDepthStencilState = true,
		bool useEmbeddedSamplerState = true
	) const;
};

//FullscreenQuad'はピクセルシェーダとサンプラーステートを持っていなので、ピクセルシェーダとサンプラーステートを自分で作らないといけない。
class FullscreenQuad
{
public:
	FullscreenQuad(ID3D11Device* device);
	virtual ~FullscreenQuad() = default;
	FullscreenQuad(FullscreenQuad&) = delete;
	FullscreenQuad& operator=(FullscreenQuad&) = delete;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> embedded_vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> embedded_pixel_shader;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> embedded_rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> embedded_depth_stencil_state;

public:
	void blit(ID3D11DeviceContext* immediateContextbool, bool useEmbeddedRasterizerState = true, bool useEmbeddedDepthStencilState = true, bool useEmbeddedPixelShader = false);
};
