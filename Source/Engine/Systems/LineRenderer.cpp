//**********************************************************
//
//		LineRenderer�N���X
//
//**********************************************************

#include <stdio.h>
#include <memory>
#include "Engine/Systems/Misc.h"
#include "Engine/Systems/LineRenderer.h"

LineRenderer::LineRenderer(ID3D11Device* device, UINT vertexCount)
	: capacity(vertexCount)
{
	//���_�V�F�[�_�[
	{
		//�t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\LineRender_vs.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		//�t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		//��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		//���_�V�F�[�_�[����
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

		//���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	//�s�N�Z���V�F�[�_�[
	{
		//�t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\LineRender_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		//��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		//�s�N�Z���V�F�[�_�[����
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	//�萔�o�b�t�@
	{
		//�V�[���p�o�b�t�@
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBuffer);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	//�u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
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
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	//�[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	//���X�^���C�U�[�X�e�[�g
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
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	//���_�o�b�t�@
	{
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(Vertex) * vertexCount;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, nullptr, vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
}

//�`��J�n
void LineRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	//�V�F�[�_�[�ݒ�
	deviceContext->VSSetShader(vertex_shader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixel_shader.Get(), nullptr, 0);
	deviceContext->IASetInputLayout(input_layout.Get());

	//�萔�o�b�t�@�ݒ�
	deviceContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//deviceContext->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	//�����_�[�X�e�[�g�ݒ�
	const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	deviceContext->OMSetBlendState(blend_state.Get(), blendFactor, 0xFFFFFFFF);
	deviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	deviceContext->RSSetState(rasterizer_state.Get());

	//�v���~�e�B�u�ݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//�萔�o�b�t�@�X�V
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;
	ConstantBuffer data;
	DirectX::XMStoreFloat4x4(&data.wvp, VP);
	deviceContext->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);

	//�`��
	UINT totalVertexCount = static_cast<UINT>(vertices.size());
	UINT start = 0;
	UINT count = (totalVertexCount < capacity) ? totalVertexCount : capacity;

	while (start < totalVertexCount)
	{
		D3D11_MAPPED_SUBRESOURCE mappedVB;
		HRESULT hr = deviceContext->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVB);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));

		memcpy(mappedVB.pData, &vertices[start], sizeof(Vertex) * count);

		deviceContext->Unmap(vertex_buffer.Get(), 0);

		deviceContext->Draw(count, 0);

		start += count;
		if ((start + count) > totalVertexCount)
		{
			count = totalVertexCount - start;
		}
	}
	vertices.clear();
}

//���_�ǉ�
void LineRenderer::AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
{
	Vertex v;
	v.position = position;
	v.color = color;
	vertices.emplace_back(v);
}