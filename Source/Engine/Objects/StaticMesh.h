#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "Engine/Systems/Math.h"

// ToDo StaticMesh ÉRÉÅÉìÉg

class StaticMesh
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 material_color;
		DirectX::XMFLOAT4 light_direction;
	};
	struct Subset
	{
		std::wstring usemtl;
		u_int index_start = 0;
		u_int index_count = 0;
	};
	std::vector<Subset> subsets;

	struct Material
	{
		std::wstring newmtl;
		DirectX::XMFLOAT3 Ka = { 0.2f, 0.2f, 0.2f };
		DirectX::XMFLOAT3 Kd = { 0.8f, 0.8f, 0.8f };
		DirectX::XMFLOAT3 Ks = { 1.0f, 1.0f, 1.0f };
		u_int illum = 1;
		std::wstring map_Kd;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	};
	std::vector<Material> materials;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[2];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;

public:
	//static_mesh(ID3D11Device *device, const wchar_t *obj_filename);
	StaticMesh(ID3D11Device* device, const wchar_t* objfilename, bool flippingVertexCoordinates);
	virtual ~StaticMesh();

	void Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& worldViewProjection, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor, bool wireframe = false);

private:
	void CreateBuffers(ID3D11Device* device, Vertex* vertices, int num_vertices, u_int* indices, int num_indices);
};