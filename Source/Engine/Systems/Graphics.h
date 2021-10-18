#pragma once
#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/LineRenderer.h"
#include "Engine/Objects/Sprite.h"

class Graphics
{
private:

public:
	Graphics(HWND hWnd);
	~Graphics();

	// インスタンス取得
	static Graphics& Instance() { return *instance; }

	// ミューテックス取得
	std::mutex& GetMutex() { return mutex; }

	// デバイス取得
	ID3D11Device* GetDevice() const { return device.Get(); }

	// デバイスコンテキスト取得
	ID3D11DeviceContext* GetDeviceContext() const { return immediate_context.Get(); }

	// スワップチェーン取得
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	// デプスステンシルビュー取得
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// スクリーン幅取得
	float GetScreenWidth() const { return screenWidth; }

	// スクリーン高さ取得
	float GetScreenHeight() const { return screenHeight; }

	// デバッグレンダラ取得
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	// ラインレンダラ取得
	LineRenderer* GetLineRenderer() const { return line_renderer.get(); }

	// ImGuiレンダラ取得
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// シェーダーマネージャー取得
	ShaderManager* GetShaderManager() const { return shader_manager.get(); }
	//フォントの取得
	Sprite* GetFont() const { return font.get(); }

	////デバッグモードの設定
	//bool GetDebugMode() { return debug_mode; }

	//void SetDebugMode(bool setMode) { debug_mode = setMode; }
private:
	static Graphics* instance;
	std::mutex			mutex;

	Microsoft::WRL::ComPtr<ID3D11Device>					device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		immediate_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				depth_stencil_buffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;

	std::unique_ptr<DebugRenderer>								debug_renderer;
	std::unique_ptr<LineRenderer>									line_renderer;
	std::unique_ptr<ImGuiRenderer>								imgui_renderer;
	std::unique_ptr<ShaderManager>								shader_manager;
	std::unique_ptr<Sprite>											font;

	std::shared_ptr<Shader>											lambert_shader;
	std::shared_ptr<Shader>											wireframe_lambert_shader;
	std::shared_ptr<Shader>											no_texture_shader;
	float	screenWidth;
	float	screenHeight;
	//	bool debug_mode = false;
public:
};
