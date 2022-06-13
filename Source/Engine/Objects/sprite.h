#pragma once
//**********************************************************
//
//		Spriteクラス
//
//**********************************************************
#include <d3d11.h>
#include <wrl.h>
#include <string>

#include "Engine/Systems/Math.h"

/// <summary>
/// テクスチャーを描画するクラス
/// </summary>
class Sprite final
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>				pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>						vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>				blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_state;
	D3D11_TEXTURE2D_DESC											texture2d_desc;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;

	float texture_width = 0;
	float texture_height = 0;
public:



private:

public:
	Sprite();
	Sprite(const wchar_t* filename);
	~Sprite() = default;

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	/// <summary>
	/// 読み込んだテクスチャを描画する
	/// </summary>
	/// <param name="device_context">"DirectX context"</param>
	/// <param name="dx">"X座標"</param>
	/// <param name="dy">"Y座標"</param>
	/// <param name="dw">"描画幅"</param>
	/// <param name="dh">"描画高さ"</param>
	/// <param name="sx">"画像切り取りX座標"</param>
	/// <param name="sy">"画像切り取りY座標"</param>
	/// <param name="sw">"画像切り取り幅"</param>
	/// <param name="sh">"画像切り取り高さ"</param>
	/// <param name="angle">角度(degree)</param>
	/// <param name="r">赤</param>
	/// <param name="g">緑</param>
	/// <param name="b">青</param>
	/// <param name="a">透明度</param>
	void Render(ID3D11DeviceContext* device_context,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;
	//void Render(ID3D11DeviceContext* device_context, DirectX::XMFLOAT2 );
	// テクスチャ幅取得
	float GetTextureWidth() const { return texture_width; }

	// テクスチャ高さ取得
	float GetTextureHeight() const { return texture_height; }



	void TextOut(ID3D11DeviceContext* device_context, std::string s, float x, float y, float w, float h, float r = 1, float g = 1, float b = 1, float a = 1) const;
};

class SpriteBatch
{
private:
	D3D11_VIEWPORT viewport;

	UINT count_instance = 0;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				  vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>				sampler_state;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depth_stencil_state;

	const size_t max_instances = 256;
	struct Instance
	{
		DirectX::XMFLOAT4X4 ndc_transform;
		DirectX::XMFLOAT4	 texcoord_transform;
		DirectX::XMFLOAT4	 color;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> instance_buffer;
	Instance* instances = nullptr;
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
	};

	SpriteBatch(ID3D11Device* device, const wchar_t* filename, size_t max_instance = 256);

	void Begin(ID3D11DeviceContext* device_context);
	void Render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh, float sx, float sy, float sw, float sh, float angle/*degree*/, float r, float g, float b, float a);
	void End(ID3D11DeviceContext* device_context);
};
