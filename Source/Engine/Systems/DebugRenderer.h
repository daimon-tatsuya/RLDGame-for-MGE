#pragma once
//**********************************************************
//
//		DebugRendererクラス
//
//**********************************************************

#include <vector>
#include <wrl.h>
#include <d3d11.h>

#include "Engine/Systems/Math.h"

class DebugRenderer final
{
private:
	struct ConstantBufferMesh
	{
		DirectX::XMFLOAT4X4	wvp;
		DirectX::XMFLOAT4		color;
	};

	//x struct Cube
	//x {
	//x 	DirectX::XMFLOAT4	color;
	//x 	DirectX::XMFLOAT3	center;
	//x };
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
	//x Microsoft::WRL::ComPtr<ID3D11Buffer>			cude_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>				blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	//x std::vector<Cylinder>	cubes;
	std::vector<Sphere>		spheres;
	std::vector<Cylinder>	cylinders;

	//x UINT	cube_vertex_count = 0;
	UINT	sphere_vertex_count = 0;
	UINT	cylinder_vertex_count = 0;

public:

private:

	// 箱メッシュ作成
	//x void CreateCubeMesh(ID3D11Device* device);

	// 球メッシュ作成
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);

	// 円柱メッシュ作成
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks);

	// カプセルメッシュ作成

public:
	DebugRenderer(ID3D11Device* device);
	~DebugRenderer() = default;

	// 描画処理
	void Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// 球描画
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);

	// 円柱描画
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);

	// カプセル描画
};
