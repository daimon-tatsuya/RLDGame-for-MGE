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

	// �C���X�^���X�擾
	static Graphics& Instance() { return *instance; }

	// �~���[�e�b�N�X�擾
	std::mutex& GetMutex() { return mutex; }

	// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return immediate_context.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain* GetSwapChain() const { return swapchain.Get(); }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// �X�N���[�����擾
	float GetScreenWidth() const { return screenWidth; }

	// �X�N���[�������擾
	float GetScreenHeight() const { return screenHeight; }

	// �f�o�b�O�����_���擾
	DebugRenderer* GetDebugRenderer() const { return debug_renderer.get(); }

	// ���C�������_���擾
	LineRenderer* GetLineRenderer() const { return line_renderer.get(); }

	// ImGui�����_���擾
	ImGuiRenderer* GetImGuiRenderer() const { return imgui_renderer.get(); }

	// �V�F�[�_�[�}�l�[�W���[�擾
	ShaderManager* GetShaderManager() const { return shader_manager.get(); }
	//�t�H���g�̎擾
	Sprite* GetFont() const { return font.get(); }

	////�f�o�b�O���[�h�̐ݒ�
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
