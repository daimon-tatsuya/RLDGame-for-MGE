#pragma once
//**********************************************************
//
//	Graphicsクラス
//
//**********************************************************

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>

//前方宣言
class Shader;
class DebugRenderer;
class LineRenderer;
class ImGuiRenderer;
class ShaderManager;
class Sprite;

/// <summary>
/// 描画周り管理クラス
/// </summary>
class Graphics
{
private:
	static Graphics* instance;
	std::mutex			mutex;

	Microsoft::WRL::ComPtr<ID3D11Device>					device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;

	std::unique_ptr<DebugRenderer>								debug_renderer;
	std::unique_ptr<LineRenderer>									line_renderer;
	std::unique_ptr<ImGuiRenderer>								imgui_renderer;
	std::unique_ptr<ShaderManager>								shader_manager;
	std::unique_ptr<Sprite>												font;

	// Shader
	std::shared_ptr<Shader>											lambert_shader;
	std::shared_ptr<Shader>											wireframe_lambert_shader;
	std::shared_ptr<Shader>											no_texture_shader;

	float	screen_width;
	float	screen_height;

public:

private:

public:
	Graphics(HWND hWnd);
	~Graphics();

	//唯一のインスタンス取得
	static Graphics& Instance() { return *instance; }

	//------------------------------------------------
	//
	//	Getter Setter
	//
	//------------------------------------------------

		// デバイス取得
	ID3D11Device* GetDevice() const { return device.Get(); }

	// デバイスコンテキスト取得
	ID3D11DeviceContext* GetDeviceContext() const { return device_context.Get(); }

	// スワップチェーン取得
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// ミューテックス取得
	std::mutex& GetMutex() { return mutex; }

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	//1 デプスステンシルビュー取得
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// スクリーン幅取得
	float GetScreenWidth() const { return screen_width; }

	// スクリーン高さ取得
	float GetScreenHeight() const { return screen_height; }

	// デバッグレンダラ取得
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	// ラインレンダラ取得
	LineRenderer* GetLineRenderer() const { return line_renderer.get(); }

	// ImGuiレンダラ取得
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// シェーダーマネージャー取得
	ShaderManager* GetShaderManager() const { return shader_manager.get(); }

	// フォントの取得
	Sprite* GetFont() const { return font.get(); }
};
