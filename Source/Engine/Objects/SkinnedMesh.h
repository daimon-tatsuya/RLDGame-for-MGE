#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>

#include "Engine/Systems/Math.h"

#define MAX_BONE_INFLUENCES 4

#define MAX_BONES 32

//ToDo SkinnedMesh ÉRÉÅÉìÉg

class SkinnedMesh
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;

		FLOAT bone_weights[MAX_BONE_INFLUENCES] = { 1, 0, 0, 0 };
		INT bone_indices[MAX_BONE_INFLUENCES] = {};
	};
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;
		DirectX::XMFLOAT4X4 world_inverse_transpose;
		DirectX::XMFLOAT4 material_color;
		DirectX::XMFLOAT4 light_direction;

		DirectX::XMFLOAT4X4 bone_transforms[MAX_BONES] =
		{ { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
	};

	struct Material
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f }; // w channel is used as shininess by only specular.
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	};

	struct Subset
	{
		u_int index_start = 0;	// start number of index buffer
		u_int index_count = 0;	// number of vertices (indices)
		Material diffuse;
	};

	struct Bone
	{
		DirectX::XMFLOAT4X4 transform;
	};

	typedef std::vector<Bone> skeletal;
	struct SkeletalAnimation : public std::vector<skeletal>
	{
		float sampling_time = 1 / 24.0f;
		float animation_tick = 0.0f;
	};

	struct Mesh
	{
		std::vector<Subset> subsets;

		DirectX::XMFLOAT4X4 global_transform =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		SkeletalAnimation skeletal_animation;
	};
	std::vector<Mesh> meshes;

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[2];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;

public:
	SkinnedMesh(ID3D11Device* device, const char* fbxFilename);
	virtual ~SkinnedMesh() {}
	void CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices);
	void Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& worldViewProjection, const DirectX::XMFLOAT4X4& worldInverseTranspose, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor, bool wireframe, float elapsed_time/*UNIT.23*/);

protected:

	DirectX::XMFLOAT4X4 coordinate_conversion =
	{ 1, 0, 0, 0,
		0, 0, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1
	};
};
