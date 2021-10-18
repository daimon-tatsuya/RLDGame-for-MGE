#include "Engine/Systems/misc.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Objects/GeometricPrimitive.h"

GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	CreateVertexShaderFromCso(device, "Shaders/geometric_primitive_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	CreatePixelShaderFromCso(device, "Shaders/geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

	// create rasterizer state : solid mode //���`��
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// create rasterizer state : wireframe mode //�h��Ԃ��`��
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[1].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// create depth stencil state
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		depth_stencil_desc.DepthEnable = TRUE;     //�[�x�e�X�g�̐^�U        depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; //�[�x�X�e���V���o�b�t�@�[�̏������݃I�����I�t��
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS; //�[�x�f�[�^�������̐[�x�f�[�^�Ɣ�r���� //����̓\�[�X�f�[�^������f�[�^�����������ꍇ�A��r�͐������܂��B
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.StencilEnable = FALSE;  //�X�e���V���e�X�g�̐^�U
		depth_stencil_desc.StencilReadMask = 0xFF; //�X�e���V���f�[�^�ǂݎ��
		depth_stencil_desc.StencilWriteMask = 0xFF;//�X�e���V���f�[�^�������
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// create constant buffer
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(ConstantBuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT; //�ǂݎ�菑�����g�p������̗\�z//�����GPU�œǂݎ��A�������݂ł���
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//3d�̂��߃R���X�^���g���g�p
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
	// create a mesh for a cube
	//vertex vertices[24] = {}; //���@�̖̂ʁ~���_�̐� //�ʂ̒��_
	//u_int indices[36] = {};   //���@�̖̂ʂ��O�p�`�ŕ������Ƃ��̒��_�~�����̖̂� //�ʂ��O�p�`�œ�ɂ����Ƃ��̒��_

	//int face = 0;
	////top side
	//// 0-------1
	//// |   -y   |
	//// 2-------3
	//face = 0;//�l�̕ς��ē���钸�_�ʒu�����炷

	//vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);     //�����̂̒��_�ɒl�����Ă�
	//vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);    //
	//vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	//vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	//vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);     //�����̖̂@���ɒl�����Ă�
	//vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	//vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	//vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	////   0-------------1
	////   |            /    3
	////   |        /         |
	////  2     /            |
	////     5--------------4

	//indices[face * 6 + 0] = face * 4 + 0;       //�O�p�`�̒��_
	//indices[face * 6 + 1] = face * 4 + 1;
	//indices[face * 6 + 2] = face * 4 + 2;
	//indices[face * 6 + 3] = face * 4 + 1;
	//indices[face * 6 + 4] = face * 4 + 3;
	//indices[face * 6 + 5] = face * 4 + 2;

	////bottom side
	//// 4-------5
	//// |   -y   |
	//// 6-------7
	//face += 1;
	//vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	//vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	//vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	//vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	//vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	//vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	//vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	//vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	////   6-------------8
	////   |            /    9
	////   |        /        |
	////   7     /           |
	////    10--------------11
	//indices[face * 6 + 0] = face * 4 + 0;
	//indices[face * 6 + 1] = face * 4 + 2;
	//indices[face * 6 + 2] = face * 4 + 1;
	//indices[face * 6 + 3] = face * 4 + 1;
	//indices[face * 6 + 4] = face * 4 + 2;
	//indices[face * 6 + 5] = face * 4 + 3;

	////flont side
	//// 8-------9
	//// |   +z  |
	////10-------11
	//face += 1;
	//vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	//vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	//vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	//vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	//vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	//vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	//vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	//vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	////   12-------------13
	////   |            /    15
	////   |        /         |
	////  14     /           |
	////    17-------------16
	//indices[face * 6 + 0] = face * 4 + 0;
	//indices[face * 6 + 1] = face * 4 + 1;
	//indices[face * 6 + 2] = face * 4 + 2;
	//indices[face * 6 + 3] = face * 4 + 1;
	//indices[face * 6 + 4] = face * 4 + 3;
	//indices[face * 6 + 5] = face * 4 + 2;

	////back side
	//// 12------13
	//// |   +z   |
	//// 14------15
	//face += 1;
	//vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	//vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	//vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	//vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	//vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	//vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	//vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	//vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	////   18-------------20
	////   |            /    21
	////   |        /         |
	////  19     /           |
	////    22-------------23
	//indices[face * 6 + 0] = face * 4 + 0;
	//indices[face * 6 + 1] = face * 4 + 2;
	//indices[face * 6 + 2] = face * 4 + 1;
	//indices[face * 6 + 3] = face * 4 + 1;
	//indices[face * 6 + 4] = face * 4 + 2;
	//indices[face * 6 + 5] = face * 4 + 3;

	////right side
	//// 16------17
	//// |   -x   |
	//// 18------19
	//face += 1;
	//vertices[face * 4 + 0].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	//vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	//vertices[face * 4 + 2].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	//vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	//vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	//vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	//vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	//vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	////   24-------------25
	////   |            /    27
	////   |        /         |
	////  26     /           |
	////    29-------------28
	//indices[face * 6 + 0] = face * 4 + 0;
	//indices[face * 6 + 1] = face * 4 + 1;
	//indices[face * 6 + 2] = face * 4 + 2;
	//indices[face * 6 + 3] = face * 4 + 1;
	//indices[face * 6 + 4] = face * 4 + 3;
	//indices[face * 6 + 5] = face * 4 + 2;

	////left side
	//// 20------21
	//// |   -x   |
	//// 22------23
	//face += 1;
	//vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	//vertices[face * 4 + 1].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	//vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	//vertices[face * 4 + 3].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	//vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	//vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	//vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	//vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	////   30-------------32
	////   |            /    33
	////   |        /         |
	////  31     /           |
	////    35-------------34
	//indices[face * 6 + 0] = face * 4 + 0;
	//indices[face * 6 + 1] = face * 4 + 2;
	//indices[face * 6 + 2] = face * 4 + 1;
	//indices[face * 6 + 3] = face * 4 + 1;
	//indices[face * 6 + 4] = face * 4 + 2;
	//indices[face * 6 + 5] = face * 4 + 3;

	//create_buffers(device, vertices, 24, indices, 36);
}

void GeometricPrimitive::CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices)
{
	HRESULT hr;
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(Vertex) * numVertices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE; // �ǂݎ�菑�����g�p������̗\�z//�����GPU�̂ݓǂݎ���A�������݂͂ł��Ȃ�
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���_�o�b�t�@�[����̓A���Z���u���[�X�e�[�W�Ƀo�C���h
		buffer_desc.CPUAccessFlags = 0;//�ǂݎ�肩�A������肩�g��񂯂�΂O
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = vertices; //�������f�[�^�ւ̃|�C���^�B
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.mm  //�e�N�X�`����1�̍s�̐擪���玟�̍s�܂ł̋����i�o�C�g�P�ʁj�B
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers. //����[�x���x���̐擪���玟�̐[�x���x���܂ł̋����i�o�C�g�P�ʁj�B

		hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(u_int) * numIndices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = indices;
		subresource_data.SysMemPitch = 0; //Not use for index buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
}

void GeometricPrimitive::Render(ID3D11DeviceContext* immediateContext, const DirectX::XMFLOAT4X4& worldViewProjection,

	const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& lightDirection, const DirectX::XMFLOAT4& materialColor,
	bool  wireframe)
{
	ConstantBuffer data;
	data.world_view_projection = worldViewProjection;
	data.world = world;
	data.light_direction = lightDirection;
	data.material_color = materialColor;
	immediateContext->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0); //CPU�́A����������}�b�s���O�s�\�ȃ������ɍ쐬���ꂽ�T�u���\�[�X�Ƀf�[�^���R�s�[���܂��B//(�������\�[�X�ւ̃|�C���^�[�A�[���x�[�X�C���f�b�N�X�A���\�[�X�f�[�^���R�s�[���鈶��T�u���\�[�X�̕������`����{�b�N�X�ւ̃|�C���^-,���������̃\�[�X�f�[�^�ւ̃|�C���^,�\�[�X�f�[�^��1�s�̃T�C�Y,�\�[�X�f�[�^��1�̐[�x�X���C�X�̃T�C�Y)
	immediateContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf()); //���_�V�F�[�_�[�p�C�v���C���X�e�[�W�Ŏg�p�����萔�o�b�t�@�[��ݒ肵�܂��B

	u_int stride = sizeof(Vertex);
	u_int offset = 0;
	immediateContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset); //���_�o�b�t�@�[�̔z�����̓A�Z���u���[�X�e�[�W�Ƀo�C���h���܂��B
	immediateContext->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0); //�C���f�b�N�X�o�b�t�@����̓A�Z���u���X�e�[�W�Ƀo�C���h���܂��B
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  //�v���~�e�B�u�^�C�v�Ɋւ���o�C���h���A����ѓ��̓A�Z���u���X�e�[�W�̓��̓f�[�^���L�q����f�[�^�����B
	immediateContext->IASetInputLayout(input_layout.Get()); //input-layout�I�u�W�F�N�g��input-assembler�X�e�[�W�Ƀo�C���h���܂��B

	immediateContext->VSSetShader(vertex_shader.Get(), nullptr, 0); //���_�V�F�[�_�[���f�o�C�X�ɐݒ肵�܂��B
	immediateContext->PSSetShader(pixel_shader.Get(), nullptr, 0); //�s�N�Z���V�F�[�_�[���f�o�C�X�ɐݒ肵�܂�

	immediateContext->OMSetDepthStencilState(depth_stencil_state.Get(), 1); //�o�̓}�[�W�X�e�[�W�̐[�x�X�e���V����Ԃ�ݒ肵�܂��B
	if (wireframe)
	{
		immediateContext->RSSetState(rasterizer_states[1].Get());  //�p�C�v���C���̃��X�^���C�U�X�e�[�W�̃��X�^���C�U��Ԃ�ݒ肵�܂��B
	}
	else
	{
		immediateContext->RSSetState(rasterizer_states[0].Get());  //�p�C�v���C���̃��X�^���C�U�X�e�[�W�̃��X�^���C�U��Ԃ�ݒ肵�܂��B
	}

	D3D11_BUFFER_DESC buffer_desc;
	index_buffer->GetDesc(&buffer_desc);//�o�b�t�@���\�[�X�̃v���p�e�B���擾���܂��B
	immediateContext->DrawIndexed(buffer_desc.ByteWidth / sizeof(u_int), 0, 0);  //�C���f�b�N�X�t���̃C���X�^���X������Ă��Ȃ��v���~�e�B�u��`�悵�܂��B
}

GeometricCube::GeometricCube(ID3D11Device* device) : GeometricPrimitive(device)
{
	Vertex vertices[24] = {}; //���@�̖̂ʁ~���_�̐� //�ʂ̒��_
	u_int indices[36] = {};   //���@�̖̂ʂ��O�p�`�ŕ������Ƃ��̒��_�~�����̖̂� //�ʂ��O�p�`�œ�ɂ����Ƃ��̒��_

	int face = 0;
	//top side
	// 0-------1
	// |   +y  |
	// 2-------3
	//face = 0;

	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);     //�����̂̒��_�ɒl�����Ă�
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);    //
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);     //�����̖̂@���ɒl�����Ă�
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
	//   0----------------1
	//   |            �^   3
	//   |        �^       |
	//   2  �^             |
	//   5----------------4

	indices[face * 6 + 0] = face * 4 + 0;       //�O�p�`�̒��_
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;

	//bottom side
	// 4-------5
	// |   -y   |
	// 6-------7
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
	//   6-------------8
	//   |            /    9
	//   |        /        |
	//   7     /           |
	//    10--------------11
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	//flont side
	// 8-------9
	// |   +z  |
	//10-------11
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
	//   12-------------13
	//   |            /    15
	//   |        /         |
	//  14     /           |
	//    17-------------16
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;

	//back side
	// 12------13
	// |   +z   |
	// 14------15
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
	//   18-------------20
	//   |            /    21
	//   |        /         |
	//  19     /           |
	//    22-------------23
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	//right side
	// 16------17
	// |   -x   |
	// 18------19
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
	//   24-------------25
	//   |            /    27
	//   |        /         |
	//  26     /           |
	//    29-------------28
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;

	//left side
	// 20------21
	// |   -x   |
	// 22------23
	face += 1;
	vertices[face * 4 + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[face * 4 + 1].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	vertices[face * 4 + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[face * 4 + 3].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
	vertices[face * 4 + 0].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 1].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 2].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	vertices[face * 4 + 3].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
	//   30-------------32
	//   |            /    33
	//   |        /         |
	//  31     /           |
	//    35-------------34
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	CreateBuffers(device, vertices, 24, indices, 36);
}

#include <vector>
GeometricCylinder::GeometricCylinder(ID3D11Device* device, u_int slices) : GeometricPrimitive(device)
{
	std::vector<Vertex> vertices;
	std::vector<u_int> indices;

	float d = 2.0f * DirectX::XM_PI / slices;
	float r = 0.5f;

	Vertex vertex;
	u_int base_index = 0;

	// top cap centre
	vertex.position = DirectX::XMFLOAT3(0.0f, +0.5f, 0.0f);
	vertex.normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);
	vertices.push_back(vertex);
	// top cap ring
	for (u_int i = 0; i < slices; i++)
	{
		float x = r * cosf(i * d);
		float z = r * sinf(i * d);
		vertex.position = DirectX::XMFLOAT3(x, +0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);
		vertices.push_back(vertex);
	}
	base_index = 0;
	for (u_int i = 0; i < slices - 1; i++)
	{
		indices.push_back(base_index + 0);
		indices.push_back(base_index + i + 2);
		indices.push_back(base_index + i + 1);
	}
	indices.push_back(base_index + 0);
	indices.push_back(base_index + 1);
	indices.push_back(base_index + slices);

	// bottom cap centre
	vertex.position = DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f);
	vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices.push_back(vertex);
	// bottom cap ring
	for (u_int i = 0; i < slices; i++)
	{
		float x = r * cosf(i * d);
		float z = r * sinf(i * d);
		vertex.position = DirectX::XMFLOAT3(x, -0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices.push_back(vertex);
	}
	base_index = slices + 1;
	for (u_int i = 0; i < slices - 1; i++)
	{
		indices.push_back(base_index + 0);
		indices.push_back(base_index + i + 1);
		indices.push_back(base_index + i + 2);
	}
	indices.push_back(base_index + 0);
	indices.push_back(base_index + (slices - 1) + 1);
	indices.push_back(base_index + (0) + 1);

	// side rectangle
	for (u_int i = 0; i < slices; i++)
	{
		float x = r * cosf(i * d);
		float z = r * sinf(i * d);

		vertex.position = DirectX::XMFLOAT3(x, +0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(x, 0.0f, z);
		vertices.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(x, -0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(x, 0.0f, z);
		vertices.push_back(vertex);
	}
	base_index = slices * 2 + 2;
	for (u_int i = 0; i < slices - 1; i++)
	{
		indices.push_back(base_index + i * 2 + 0);
		indices.push_back(base_index + i * 2 + 2);
		indices.push_back(base_index + i * 2 + 1);

		indices.push_back(base_index + i * 2 + 1);
		indices.push_back(base_index + i * 2 + 2);
		indices.push_back(base_index + i * 2 + 3);
	}
	indices.push_back(base_index + (slices - 1) * 2 + 0);
	indices.push_back(base_index + (0) * 2 + 0);
	indices.push_back(base_index + (slices - 1) * 2 + 1);

	indices.push_back(base_index + (slices - 1) * 2 + 1);
	indices.push_back(base_index + (0) * 2 + 0);
	indices.push_back(base_index + (0) * 2 + 1);

	CreateBuffers(device, vertices.data(), /*Todo*/vertices.size(), indices.data(), indices.size());
}

GeometricSphere::GeometricSphere(ID3D11Device* device, u_int slices, u_int stacks) : GeometricPrimitive(device)
{
	std::vector<Vertex> vertices;
	std::vector<u_int> indices;

	float r = 0.5f;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex top_vertex;
	top_vertex.position = DirectX::XMFLOAT3(0.0f, +r, 0.0f);
	top_vertex.normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);

	Vertex bottom_vertex;
	bottom_vertex.position = DirectX::XMFLOAT3(0.0f, -r, 0.0f);
	bottom_vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices.push_back(top_vertex);

	float phi_step = DirectX::XM_PI / stacks;
	float theta_step = 2.0f * DirectX::XM_PI / slices;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (u_int i = 1; i <= stacks - 1; i++)
	{
		float phi = i * phi_step;

		// Vertices of ring.
		for (u_int j = 0; j <= slices; j++)
		{
			float theta = j * theta_step;

			Vertex v;

			// spherical to cartesian
			v.position.x = r * sinf(phi) * cosf(theta);
			v.position.y = r * cosf(phi);
			v.position.z = r * sinf(phi) * sinf(theta);

			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&v.position);
			DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));

			vertices.push_back(v);
		}
	}

	vertices.push_back(bottom_vertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//
	for (UINT i = 1; i <= slices; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.//���S�̖�
	u_int base_index = 1;
	u_int ring_vertex_count = slices + 1;
	for (u_int i = 0; i < stacks - 2; i++)
	{
		for (u_int j = 0; j < slices; j++)
		{
			indices.push_back(base_index + i * ring_vertex_count + j);
			indices.push_back(base_index + i * ring_vertex_count + j + 1);
			indices.push_back(base_index + (i + 1) * ring_vertex_count + j);

			indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
			indices.push_back(base_index + i * ring_vertex_count + j + 1);
			indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//
	//�����S�̃|�[��
	// South pole vertex was added last.
	u_int south_pole_index = (u_int)vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	base_index = south_pole_index - ring_vertex_count;

	for (u_int i = 0; i < slices; i++)
	{
		indices.push_back(south_pole_index);
		indices.push_back(base_index + i);
		indices.push_back(base_index + i + 1);
	}

	CreateBuffers(device, vertices.data(), /*Todo*/vertices.size(), indices.data(), indices.size());
}