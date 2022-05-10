//**********************************************************
//
//		NoTextureShaderクラス
//
//**********************************************************

#include "Engine/Systems/Misc.h"
#include "Engine/Systems/NoTextureShader.h"
#include "Engine/Objects/ModelResource.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/RenderContext.h"

NoTextureShader::NoTextureShader(ID3D11Device* device) :Shader(device)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\NoTexture_vs.cso", "rb");
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
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(input_element_desc,
			ARRAYSIZE(input_element_desc), cso_data.get(), cso_size, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\NoTexture_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixel_shader.GetAddressOf());
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
		desc.ByteWidth = sizeof(ConstantBufferScene);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// メッシュ用バッファ
		desc.ByteWidth = sizeof(ConstantBufferMesh);

		hr = device->CreateBuffer(&desc, 0, mesh_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// サブセット用バッファ
		desc.ByteWidth = sizeof(ConstantBufferSubset);

		hr = device->CreateBuffer(&desc, 0, subset_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
	// ブレンドステート
	{
		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
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
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
}

void NoTextureShader::Activate(ID3D11DeviceContext* context, const RenderContext& rc)
{
	context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	context->GSSetShader(geometry_shader.Get(), nullptr, 0);
	context->HSSetShader(hull_shader.Get(), nullptr, 0);
	context->DSSetShader(domain_shader.Get(), nullptr, 0);

	context->IASetInputLayout(input_layout.Get());

	ID3D11Buffer* constantBuffers[] =
	{
		scene_constant_buffer.Get(),
		mesh_constant_buffer.Get(),
		subset_constant_buffer.Get()
	};
	context->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	context->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	context->RSSetState(rasterizer_state.Get());
	context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());

	// シーン用定数バッファ更新
	ConstantBufferScene constant_buffer_scene{};
	DirectX::XMFLOAT4X4 view_projection{};
	{
		DirectX::XMMATRIX VP{}, V{}, P{};
		V = DirectX::XMLoadFloat4x4(&rc.view);
		P = DirectX::XMLoadFloat4x4(&rc.projection);
		VP = V * P;
		DirectX::XMStoreFloat4x4(&view_projection, VP);
	}
	constant_buffer_scene.view_projection = view_projection;
	constant_buffer_scene.light_direction = rc.light_direction;
	context->UpdateSubresource(scene_constant_buffer.Get(), 0, 0, &constant_buffer_scene, 0, 0);
}

void NoTextureShader::Draw(ID3D11DeviceContext* context, const Model* model)
{
	const ModelResource* resource = model->GetResource();
	const std::vector<Model::Node>& nodes = model->GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// メッシュ用定数バッファ更新
		ConstantBufferMesh constant_buffer_mesh = {};
		if (!mesh.node_indices.empty())
		{
			for (size_t i = 0; i < mesh.node_indices.size(); i++)
			{
				DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.node_indices.at(i)).world_transform);
				DirectX::XMMATRIX offset_transform = DirectX::XMLoadFloat4x4(&mesh.inverse_transforms.at(i));
				DirectX::XMMATRIX bone_transform = offset_transform * world_transform;
				DirectX::XMStoreFloat4x4(&constant_buffer_mesh.bone_transforms[i], bone_transform);
			}
		}
		else
		{
			constant_buffer_mesh.bone_transforms[0] = nodes.at(mesh.node_index).world_transform;
		}
		context->UpdateSubresource(mesh_constant_buffer.Get(), 0, 0, &constant_buffer_mesh, 0, 0);

		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			ConstantBufferSubset constant_buffer_subset{};
			constant_buffer_subset.material_color = subset.material->color;
			context->UpdateSubresource(subset_constant_buffer.Get(), 0, 0, &constant_buffer_subset, 0, 0);
			context->PSSetShaderResources(0, 1, subset.material->shader_resource_view.GetAddressOf());
			context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
			context->DrawIndexed(subset.index_count, subset.start_index, 0);
		}
	}
}

void NoTextureShader::Deactivate(ID3D11DeviceContext* context)
{
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->HSSetShader(nullptr, nullptr, 0);
	context->DSSetShader(nullptr, nullptr, 0);
}