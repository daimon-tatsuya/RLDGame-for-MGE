#pragma once

#include <wrl.h>
#include <d3d11.h>


#include "Engine/Systems/Math.h"


//ToDo GeometricPrimitive �R�����g
class GeometricPrimitive
{
private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader>      vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>        pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>        input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>     rasterizer_states[2];       //���`��
	//Microsoft::WRL::ComPtr<ID3D11RasterizerState>     fill_rasterizer_state;        //�h��Ԃ��`��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;

public:

private:

public:
	GeometricPrimitive(ID3D11Device* device);    //�R���X�g���N�^
	~GeometricPrimitive() {}                               //�f�X�g���N�^

	//���_�t�H�[�}�b�g
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};
	//�萔�o�b�t�@
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;    //���[���h�E�r��-�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 material_color;
		DirectX::XMFLOAT4 light_direction;
	};
	void Render(
		ID3D11DeviceContext* immediateContext,
		const DirectX::XMFLOAT4X4& worldViewProjection,
		const DirectX::XMFLOAT4X4& world,
		const DirectX::XMFLOAT4& lightDirection,
		const DirectX::XMFLOAT4& materialColor,
		bool  wireframe = false
	);

protected:
	void CreateBuffers(ID3D11Device* device, Vertex* vertices, int numVertices, u_int* indices, int numIndices);
};

class GeometricCube : public GeometricPrimitive
{
public:
	GeometricCube(ID3D11Device* device);
	virtual ~GeometricCube() {}
};

class GeometricCylinder : public GeometricPrimitive
{
public:
	GeometricCylinder(ID3D11Device* device, u_int slices);
	virtual ~GeometricCylinder() {}
};

class GeometricSphere : public GeometricPrimitive
{
public:
	GeometricSphere(ID3D11Device* device, u_int slices, u_int stacks);
	virtual ~GeometricSphere() {}
};
