//**********************************************************
//
//		DebugRenderer�N���X
//
//**********************************************************

#include <cstdio>
#include <memory>
#include "Engine/Systems/Misc.h"
#include "Engine/Systems/DebugRenderer.h"

DebugRenderer::DebugRenderer(ID3D11Device* device)
{
	// ���_�V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\DebugRender_vs.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// ���_�V�F�[�_�[����
		HRESULT hr = device->CreateVertexShader(cso_data.get(), cso_size, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(input_element_desc, ARRAYSIZE(input_element_desc), cso_data.get(), cso_size, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �s�N�Z���V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\DebugRender_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// �s�N�Z���V�F�[�_�[����
		HRESULT hr = device->CreatePixelShader(cso_data.get(), cso_size, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBufferMesh);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �u�����h�X�e�[�g
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

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
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

	// �����b�V���쐬
	CreateSphereMesh(device, 1.0f, 16, 16);

	// �~�����b�V���쐬
	CreateCylinderMesh(device, 1.0f, 1.0f, 0.0f, 1.0f, 16, 1);
}

void DebugRenderer::Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// �V�F�[�_�[�ݒ�
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	device_context->IASetInputLayout(input_layout.Get());

	// �萔�o�b�t�@�ݒ�
	device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//deviceContext->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

	// �����_�[�X�e�[�g�ݒ�
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	device_context->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
	device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	device_context->RSSetState(rasterizer_state.Get());

	// �r���[�v���W�F�N�V�����s��쐬
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;

	// �v���~�e�B�u�ݒ�
	UINT stride = sizeof(DirectX::XMFLOAT3);
	UINT offset = 0;
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// ���`��
	device_context->IASetVertexBuffers(0, 1, sphere_vertex_buffer.GetAddressOf(), &stride, &offset);
	for (const Sphere& sphere : spheres)
	{
		// ���[���h�r���[�v���W�F�N�V�����s��쐬
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sphere.radius, sphere.radius, sphere.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(sphere.center.x, sphere.center.y, sphere.center.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// �萔�o�b�t�@�X�V
		ConstantBufferMesh constant_buffer_mesh{};
		constant_buffer_mesh.color = sphere.color;
		DirectX::XMStoreFloat4x4(&constant_buffer_mesh.wvp, WVP);

		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &constant_buffer_mesh, 0, 0);
		device_context->Draw(sphere_vertex_count, 0);
	}
	spheres.clear();

	// �~���`��
	device_context->IASetVertexBuffers(0, 1, cylinder_vertex_buffer.GetAddressOf(), &stride, &offset);
	for (const Cylinder& cylinder : cylinders)
	{
		// ���[���h�r���[�v���W�F�N�V�����s��쐬
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cylinder.radius, cylinder.height, cylinder.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cylinder.position.x, cylinder.position.y, cylinder.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;

		// �萔�o�b�t�@�X�V
		ConstantBufferMesh constant_buffer_mesh{};
		constant_buffer_mesh.color = cylinder.color;
		DirectX::XMStoreFloat4x4(&constant_buffer_mesh.wvp, WVP);

		device_context->UpdateSubresource(constant_buffer.Get(), 0, nullptr, &constant_buffer_mesh, 0, 0);
		device_context->Draw(cylinder_vertex_count, 0);
	}
	cylinders.clear();
}

void DebugRenderer::DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color)
{
	Sphere sphere{};
	sphere.center = center;
	sphere.radius = radius;
	sphere.color = color;
	spheres.emplace_back(sphere);
}

void DebugRenderer::DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
	Cylinder cylinder{};
	cylinder.position = position;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.color = color;
	cylinders.emplace_back(cylinder);
}

//void DebugRenderer::CreateCubeMesh(ID3D11Device* device)
//{
//	cube_vertex_count;
//}

void DebugRenderer::CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks)
{
	sphere_vertex_count = stacks * slices * 2 + slices * stacks * 2;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(sphere_vertex_count);

	float phi_step = DirectX::XM_PI / stacks;
	float theta_step = DirectX::XM_2PI / slices;

	DirectX::XMFLOAT3* p = vertices.get();

	for (int i = 0; i < stacks; i++)
	{
		float phi = i * phi_step;
		float y = radius * cosf(phi);
		float r = radius * sinf(phi);

		for (int j = 0; j < slices; j++)
		{
			float theta = j * theta_step;
			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;

			theta += theta_step;

			p->x = r * sinf(theta);
			p->y = y;
			p->z = r * cosf(theta);
			p++;
		}
	}

	theta_step = DirectX::XM_2PI / stacks;
	for (int i = 0; i < slices; i++)
	{
		DirectX::XMMATRIX M = DirectX::XMMatrixRotationY(i * theta_step);
		for (int j = 0; j < stacks; j++)
		{
			float theta = j * theta_step;
			DirectX::XMVECTOR V1 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			DirectX::XMVECTOR P1 = DirectX::XMVector3TransformCoord(V1, M);
			DirectX::XMStoreFloat3(p++, P1);

			int n = (j + 1) % stacks;
			theta += theta_step;

			DirectX::XMVECTOR V2 = DirectX::XMVectorSet(radius * sinf(theta), radius * cosf(theta), 0.0f, 1.0f);
			DirectX::XMVECTOR P2 = DirectX::XMVector3TransformCoord(V2, M);
			DirectX::XMStoreFloat3(p++, P2);
		}
	}

	// ���_�o�b�t�@
	{
		D3D11_BUFFER_DESC desc{};
		D3D11_SUBRESOURCE_DATA subresource_data{};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * sphere_vertex_count);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	//D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresource_data.pSysMem = vertices.get();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresource_data, sphere_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
}

void DebugRenderer::CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks)
{
	cylinder_vertex_count = 2 * slices * (stacks + 1) + 2 * slices;
	std::unique_ptr<DirectX::XMFLOAT3[]> vertices = std::make_unique<DirectX::XMFLOAT3[]>(cylinder_vertex_count);

	DirectX::XMFLOAT3* p = vertices.get();

	float stack_height = height / stacks;
	float radius_step = (radius2 - radius1) / stacks;

	// �����O�̒��_
	float d_theta = DirectX::XM_2PI / slices;

	for (int i = 0; i < slices; i++)
	{
		int n = (i + 1) % slices;

		float c1 = cosf(i * d_theta);
		float s1 = sinf(i * d_theta);

		float c2 = cosf(n * d_theta);
		float s2 = sinf(n * d_theta);

		for (int j = 0; j <= stacks; j++)
		{
			float y = start + j * stack_height;
			float r = radius1 + j * radius_step;

			p->x = r * c1;
			p->y = y;
			p->z = r * s1;
			p++;

			p->x = r * c2;
			p->y = y;
			p->z = r * s2;
			p++;
		}

		p->x = radius1 * c1;
		p->y = start;
		p->z = radius1 * s1;
		p++;

		p->x = radius2 * c1;
		p->y = start + height;
		p->z = radius2 * s1;
		p++;
	}

	// ���_�o�b�t�@
	{
		D3D11_BUFFER_DESC desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		desc.ByteWidth = static_cast<UINT>(sizeof(DirectX::XMFLOAT3) * cylinder_vertex_count);
		desc.Usage = D3D11_USAGE_IMMUTABLE;	// D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		subresource_data.pSysMem = vertices.get();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&desc, &subresource_data, cylinder_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
}