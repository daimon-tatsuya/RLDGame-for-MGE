#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class LineRenderer
{
public:
	LineRenderer(ID3D11Device* device, UINT vertexCount);
	~LineRenderer() {}

public:
	// ï`âÊé¿çs
	void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// í∏ì_í«â¡
	void AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color);

private:
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4	wvp;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	std::vector<Vertex>			vertices;
	UINT							capacity = 0;
};
