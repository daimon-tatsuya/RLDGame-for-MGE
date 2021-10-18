#include "Engine/Systems/misc.h"
#include "Engine/Systems/NoTextureShader.h"

NoTextureShader::NoTextureShader(ID3D11Device* device) :Shader(device)
{
	// ���_�V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\NoTexture_vs.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ���_�V�F�[�_�[����
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES",    0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc,
			ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// �s�N�Z���V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\NoTexture_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// �s�N�Z���V�F�[�_�[����
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbScene);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, scene_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

		// ���b�V���p�o�b�t�@
		desc.ByteWidth = sizeof(CbMesh);

		hr = device->CreateBuffer(&desc, 0, mesh_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

		// �T�u�Z�b�g�p�o�b�t�@
		desc.ByteWidth = sizeof(CbSubset);

		hr = device->CreateBuffer(&desc, 0, subset_constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
	// �u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
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
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = true;
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
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// �T���v���X�e�[�g
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
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
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
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

	// �V�[���p�萔�o�b�t�@�X�V
	CbScene cbScene;
	DirectX::XMFLOAT4X4 viewProjection;
	{
		DirectX::XMMATRIX VP, V, P;
		V = DirectX::XMLoadFloat4x4(&rc.view);
		P = DirectX::XMLoadFloat4x4(&rc.projection);
		VP = V * P;
		DirectX::XMStoreFloat4x4(&viewProjection, VP);
	}
	cbScene.view_projection = viewProjection;
	cbScene.light_direction = rc.lightDirection;
	context->UpdateSubresource(scene_constant_buffer.Get(), 0, 0, &cbScene, 0, 0);
}

void NoTextureShader::Draw(ID3D11DeviceContext* context, const Model* model)
{
	const ModelResource* resource = model->GetResource();
	const std::vector<Model::Node>& nodes = model->GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// ���b�V���p�萔�o�b�t�@�X�V
		CbMesh cbMesh;
		::memset(&cbMesh, 0, sizeof(cbMesh));
		if (mesh.node_indices.size() > 0)
		{
			for (size_t i = 0; i < mesh.node_indices.size(); i++)
			{
				DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.node_indices.at(i)).world_transform);
				DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.inverse_transforms.at(i));
				DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
				DirectX::XMStoreFloat4x4(&cbMesh.bone_transforms[i], boneTransform);
			}
		}
		else
		{
			cbMesh.bone_transforms[0] = nodes.at(mesh.node_index).world_transform;
		}
		context->UpdateSubresource(mesh_constant_buffer.Get(), 0, 0, &cbMesh, 0, 0);

		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			CbSubset cbSubset;
			cbSubset.material_color = subset.material->color;
			context->UpdateSubresource(subset_constant_buffer.Get(), 0, 0, &cbSubset, 0, 0);
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