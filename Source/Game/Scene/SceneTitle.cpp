#include "Engine/Systems/Graphics.h"
#include "Game/Scene/SceneTitle.h"
#include "Engine/Systems/blender.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneGame.h"
SceneTitle::SceneTitle()
{
}
SceneTitle::~SceneTitle()
{
}
void SceneTitle::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	strcpy_s(message, "Title");
}

void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// �Ȃɂ��{�^�����������烍�[�f�B���O�V�[���֐؂�ւ�
	if (gamePad.GetButtonDown() & static_cast<GamePadButton>(GamePad::AnyBTN))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	device_context->ClearRenderTargetView(render_target_view, color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// �`�揈��
	RenderContext render_context;
	render_context.lightDirection = { -0.5f, -1.0f, -0.5f, 0.0f };	// ���C�g�����i�������j

	//// 3D���f���`��
	//{
	//	// ���[���h�s����쐬
	//	DirectX::XMMATRIX W;
	//	{
	//		static float rx = 0;
	//		static float ry = 0;
	//		//rx += DirectX::XMConvertToRadians(0.25f);	// �p�x�����W�A��(��)�ɕϊ�
	//		//ry += DirectX::XMConvertToRadians(0.5f);	// �p�x�����W�A��(��)�ɕϊ�

	//		DirectX::XMFLOAT3 scale(0.5f, 0.5f, 0.5f);
	//		DirectX::XMFLOAT3 rotate(rx, ry, 0);
	//		DirectX::XMFLOAT3 translate(0, -20, 0);

	//		DirectX::XMMATRIX S, R, T;
	//		S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//		R = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);	// ZXY��]
	//		T = DirectX::XMMatrixTranslation(translate.x, translate.y, translate.z);

	//		W = S * R * T;
	//	}

	//	// �r���[�s����쐬
	//	DirectX::XMMATRIX V;
	//	{
	//		static CameraController camera;
	//		camera.Update();

	//		// �J�����̐ݒ�
	//		DirectX::XMVECTOR eye, focus, up;
	//		eye = DirectX::XMVectorSet(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 1.0f);
	//		focus = DirectX::XMVectorSet(camera.GetFocus().x, camera.GetFocus().y, camera.GetFocus().z, 1.0f);
	//		up = DirectX::XMVectorSet(camera.GetUp().x, camera.GetUp().y, camera.GetUp().z, 1.0f);

	//		V = DirectX::XMMatrixLookAtRH(eye, focus, up);
	//	}

	//	// �v���W�F�N�V�����s����쐬
	//	DirectX::XMMATRIX P;
	//	{
	//		// ��ʃT�C�Y�擾�̂��߃r���[�|�[�g���擾
	//		D3D11_VIEWPORT viewport;
	//		UINT num_viewports = 1;
	//		device_context->RSGetViewports(&num_viewports, &viewport);

	//		// �p�x�����W�A��(��)�ɕϊ�
	//		float fov_y = 30 * 0.01745f;	// ��p
	//		float aspect = viewport.Width / viewport.Height;	// ��ʔ䗦
	//		float nearZ = 0.1f;	// �\���ŋߖʂ܂ł̋���
	//		float farZ = 1000.0f;	// �\���ŉ��ʂ܂ł̋���

	//		P = DirectX::XMMatrixPerspectiveFovRH(fov_y, aspect, nearZ, farZ);
	//	}

	//	// �r���[�s��A�v���W�F�N�V�����s����������s��f�[�^�����o���B
	//	DirectX::XMFLOAT4X4 viewProjection;
	//	{
	//		DirectX::XMMATRIX VP;
	//		VP = V * P;
	//		DirectX::XMStoreFloat4x4(&viewProjection, VP);
	//		DirectX::XMStoreFloat4x4(&render_context.projection, P);
	//		DirectX::XMStoreFloat4x4(&render_context.view, V);
	//	}

	//	ShaderManager* shader_manager = graphics.GetShaderManager();
	//	std::shared_ptr<Shader> shader = shader_manager->GetShader(ShaderManager::ShaderName::Lambert);

	//	shader->Activate(device_context,
	//		render_context
	//	);

	//	shader->Deactivate(device_context);
	//}
	// 2D�X�v���C�g�`��
	{
	}

	// 3D�G�t�F�N�g�`��
	{
	}

	// 3D�f�o�b�O�`��
	{
		// �L�����N�^�[�f�o�b�O�`��
		//CharacterManager::Instance().DrawDebugPrimitive();

		//// ���C�������_���`����s
		//graphics.GetLineRenderer()->Render(device_context, render_context.view, render_context.projection);

		//// �f�o�b�O�����_���`����s
		//graphics.GetDebugRenderer()->Render(device_context, render_context.view, render_context.projection);
	}

	// 2D�X�v���C�g�`��
	{
		// HUD�X�V
	//	headUpDisplay->Render(dc);

		Sprite* font = graphics.GetFont();

		// �uTitle�v��`��
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = 32.f;// static_cast<float>(font->GetTextureWidth());
		float textureHeight = 32.f;// static_cast<float>(font->GetTextureHeight());
		float positionX = screenWidth / 3;// - textureWidth;// * (textureWidth / 2);
		float positionY =( screenHeight / 2 )- textureHeight;

		font->TextOutW(device_context, message, positionX, positionY, 128, 128);
	}

	// 2D�f�o�b�OGUI�`��
	{
		CharacterManager::Instance().DrawDebugGUI();
	}

	//�����_�[�^�[�Q�b�g��ς��ĐF�X������
	//frame_buffer[0]->Clear(immediateContext.Get());
	//frame_buffer[0]->Activate(immediateContext.Get());
	//paintar_fullsdreen->blit(immediateContext.Get(), true, true, false);
	////fbx_mesh->Render(immediateContext.Get(), world_view_projection, world, light_direction, material_color, wireframe, elapsedTime);
	//frame_buffer[0]->Deactivate(immediateContext.Get());
	//immediateContext->PSSetShaderResources(0, 1, frame_buffer[0]->render_target_shader_resource_view.GetAddressOf());
	//paintar_fullsdreen->blit(immediateContext.Get(), true, true, true);
}

void SceneTitle::Finalize()
{
}