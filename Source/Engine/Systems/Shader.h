#pragma once
//**********************************************************
//
//		Shaderクラス
//
//**********************************************************
#include <d3d11.h>
#include <wrl.h>
#include  "Engine/Systems/Math.h"

HRESULT CreateVertexShaderFromCso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader, ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);
HRESULT CreatePixelShaderFromCso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);

//前方宣言
class Model;
struct RenderContext;

/// <summary>
/// シェーダーの基底クラス
/// </summary>
class Shader
{
public:
	explicit Shader(ID3D11Device* device) {}
	virtual ~Shader() = default;
	// 描画開始
	virtual void Activate(ID3D11DeviceContext* device_context, const RenderContext& rc) = 0;
	// 描画開始
	virtual void Draw(ID3D11DeviceContext* device_context, const Model* model) = 0;
	// 描画終了
	virtual void Deactivate(ID3D11DeviceContext* device_context) = 0;

private:
	const static int MAX_BONES = 256;

protected:
	struct ConstantBufferScene
	{
		DirectX::XMFLOAT4X4	view_projection;
		DirectX::XMFLOAT4	light_direction;
	};

	struct ConstantBufferMesh
	{
		DirectX::XMFLOAT4X4	bone_transforms[MAX_BONES];
	};

	struct ConstantBufferSubset
	{
		DirectX::XMFLOAT4	material_color;
		DirectX::XMFLOAT4  vertex_color;
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>			scene_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mesh_constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subset_constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>				pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>		geometry_shader;
	Microsoft::WRL::ComPtr<ID3D11HullShader>				hull_shader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		domain_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>				blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
};
