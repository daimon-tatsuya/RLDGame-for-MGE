#pragma once
//**********************************************************
//
//		Shaderクラス
//
//**********************************************************
#include <d3d11.h>
#include "Engine/Systems/RenderContext.h"
#include "Engine/Objects/Model.h"


 HRESULT CreateVertexShaderFromCso(ID3D11Device* device, const char* csoname, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements);
 HRESULT CreatePixelShaderFromCso(ID3D11Device* device, const char* csoname, ID3D11PixelShader** pixelShader);

/// <summary>
/// シェーダーの基底クラス
/// </summary>
class Shader
{
public:
	Shader(ID3D11Device* device) {};
	~Shader() {};
	// 描画開始
	virtual void Activate(ID3D11DeviceContext* deviceContext, const RenderContext& rc) = 0;
	// 描画開始
	virtual void Draw(ID3D11DeviceContext* deviceContext, const Model* model) = 0;
	// 描画終了
	virtual void Deactivate(ID3D11DeviceContext* deviceContext) = 0;

private:
	static const int MaxBones = 256;

protected:
	struct CbScene
	{
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	bone_transforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	material_color;
		DirectX::XMFLOAT4  vertex_color;
 	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11HullShader>			hull_shader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		domain_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
};
