#pragma once
//**********************************************************
//
//		LineRendererクラス
//
//**********************************************************

#include <vector>
#include <wrl.h>
#include <d3d11.h>

#include "Engine/Systems/Math.h"

/// <summary>
/// ラインを描画するクラス
/// </summary>
class LineRenderer
{
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
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>				blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	std::vector<Vertex>			vertices;
	UINT									capacity = 0;

public:

private:

public:

	LineRenderer(ID3D11Device* device, UINT vertex_count);
	~LineRenderer() = default;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="device_context"></param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	void Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	// 頂点追加
	void AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color);
};
