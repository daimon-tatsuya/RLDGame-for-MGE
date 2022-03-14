#pragma once
//**********************************************************
//
//	Graphics�N���X
//
//**********************************************************

#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <mutex>

//�O���錾
class Shader;
class DebugRenderer;
class LineRenderer;
class ImGuiRenderer;
class ShaderManager;
class Sprite;

/// <summary>
/// �`�����Ǘ��N���X
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

	//�B��̃C���X�^���X�擾
	static Graphics& Instance() { return *instance; }

	//------------------------------------------------
	//
	//	Getter Setter
	//
	//------------------------------------------------

		// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return device_context.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// �~���[�e�b�N�X�擾
	std::mutex& GetMutex() { return mutex; }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	//1 �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// �X�N���[�����擾
	float GetScreenWidth() const { return screen_width; }

	// �X�N���[�������擾
	float GetScreenHeight() const { return screen_height; }

	// �f�o�b�O�����_���擾
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	// ���C�������_���擾
	LineRenderer* GetLineRenderer() const { return line_renderer.get(); }

	// ImGui�����_���擾
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// �V�F�[�_�[�}�l�[�W���[�擾
	ShaderManager* GetShaderManager() const { return shader_manager.get(); }

	// �t�H���g�̎擾
	Sprite* GetFont() const { return font.get(); }
};
