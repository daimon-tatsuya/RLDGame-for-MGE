#pragma once
//**********************************************************
//
//	ImGuiRendererクラス
//
//**********************************************************

#include <d3d11.h>
#include <imgui.h>
#include <wrl.h>

#include "Liblary/Systems/Math.h"

//デバッグ時のみ有効
#if defined( DEBUG ) || defined( _DEBUG )

#define USE_IMGUI

#endif  !_DEBUG


/// <summary>
/// Imguiを描画を行うクラス
/// </summary>
class ImGuiRenderer final
{
private:

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4		wvp;
	};

	HWND																	hWnd;
	INT64																	time = 0;
	INT64																	ticks_per_second = 0;
	ImGuiMouseCursor													last_mouse_cursor = ImGuiMouseCursor_COUNT;

	Microsoft::WRL::ComPtr<ID3D11Buffer>					vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>					index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>					constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>				pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>				blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_tate;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>				sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;

	int													vertex_count = 0;
	int													index_count = 0;
public:

private:

	// マウスカーソル更新
	bool UpdateMouseCursor();

	// マウス座標更新
	void UpdateMousePos();

public:

	ImGuiRenderer(HWND hWnd, ID3D11Device* device);
	~ImGuiRenderer();

	// フレーム開始処理
	void NewFrame();

	// 描画処理
	void Render(ID3D11DeviceContext* device_context);

	// WIN32メッセージハンドラー
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
