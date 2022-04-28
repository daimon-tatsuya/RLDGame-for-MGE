//**********************************************************
//
//		LineRendererクラス
//
//**********************************************************

#include "Engine/Systems/LineRenderer.h"

#include <cstdio>
#include <memory>

#include "Engine/Systems/Misc.h"

LineRenderer::LineRenderer(ID3D11Device* device, UINT vertex_count)
	: capacity(vertex_count)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\LineRender_vs.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(cso_data.get(), cso_size, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(input_element_desc, ARRAYSIZE(input_element_desc), cso_data.get(), cso_size, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\LineRender_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(cso_data.get(), cso_size, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// 定数バッファ
	{
		// シーン用バッファ
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBuffer);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(Vertex) * vertex_count;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
}

// 描画開始
void LineRenderer::Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// シェーダー設定
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	device_context->IASetInputLayout(input_layout.Get());

	// 定数バッファ設定
	device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// レンダーステート設定
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	device_context->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
	device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	device_context->RSSetState(rasterizer_state.Get());

	// プリミティブ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	// 定数バッファ更新
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;
	ConstantBuffer data{};
	DirectX::XMStoreFloat4x4(&data.wvp, VP);
	device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);

	// 描画
	UINT total_vertex_count = static_cast<UINT>(vertices.size());
	UINT start = 0;
	UINT count = (total_vertex_count < capacity) ? total_vertex_count : capacity;

	while (start < total_vertex_count)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_vertex_buffer{};
		HRESULT hr = device_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		memcpy(mapped_vertex_buffer.pData, &vertices[start], sizeof(Vertex) * count);

		device_context->Unmap(vertex_buffer.Get(), 0);

		device_context->Draw(count, 0);

		start += count;
		if ((start + count) > total_vertex_count)
		{
			count = total_vertex_count - start;
		}
	}
	vertices.clear();
}

void LineRenderer::AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
{
	Vertex v{};
	v.position = position;
	v.color = color;
	vertices.emplace_back(v);
}