#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include "Engine/Systems/Math.h"


//ToDo DebugRenderer�R�����g
class DebugRenderer
{
private:
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4	color;
	};

	struct Cube
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	center;
	};
	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	center;
		float				radius;
	};

	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			sphere_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cylinder_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cude_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	std::vector<Cylinder>	cubes;
	std::vector<Sphere>		spheres;
	std::vector<Cylinder>	cylinders;

	UINT	cube_vertex_count = 0;
	UINT	sphere_vertex_count = 0;
	UINT	cylinder_vertex_count = 0;
public:

private:
	// Todo �����b�V���쐬
	// �����b�V���쐬
	void CreateCubeMesh(ID3D11Device* device);

	// �����b�V���쐬
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);

	// �~�����b�V���쐬
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);

	//�J�v�Z�����b�V���쐬

public:
	DebugRenderer(ID3D11Device* device);
	~DebugRenderer() {}

	// �`����s
	void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// ���`��
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);

	// �~���`��
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);

	//Todo �J�v�Z���`��
	//�J�v�Z���`��


};
