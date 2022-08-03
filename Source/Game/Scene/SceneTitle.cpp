//**********************************************************
//
//		SceneTitle�N���X
//
//**********************************************************

<<<<<<< HEAD
#include "Liblary/Systems/Blender.h"
#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/Input.h"
#include "Liblary/Systems/Logger.h"

#include "Liblary/Objects/Sprite.h"
#include "Liblary/Systems/CameraController.h"
#include "Liblary/Systems/CharacterManager.h"
#include "Liblary/Systems/RenderContext.h"
#include"Game/Characters/Player.h"
#include "Liblary/Systems/SceneManager.h"
=======
#include "MyLiblary/Systems/Blender.h"
#include "MyLiblary/Systems/Graphics.h"
#include "MyLiblary/Systems/Input.h"
#include "MyLiblary/Systems/Logger.h"

#include "MyLiblary/Objects/Sprite.h"
#include "MyLiblary/Systems/CameraController.h"
#include "MyLiblary/Systems/CharacterManager.h"
#include "MyLiblary/Systems/RenderContext.h"
#include"Game/Characters/Player.h"
#include "MyLiblary/Systems/SceneManager.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneGame.h"
#include "Game/Scene/SceneLoading.h"


void SceneTitle::Initialize()
{
	strcpy_s(title_name, "Title");
	strcpy_s(push_text, "Push ABXY");
}

void SceneTitle::Update(float elapsed_time)
{
	const GamePad& game_pad = Input::Instance().GetGamePad();

	// �Ȃɂ��{�^�����������烍�[�f�B���O�V�[���֐؂�ւ�
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::AnyBTN))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

void SceneTitle::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	//�`�揈��
	RenderContext render_context{};
	render_context.light_direction = { -0.5f, -1.0f, -0.5f, 0.0f };	// ���C�g�����i�������j

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
		const Sprite* font = graphics.GetFont();

		// �uTitle�v��`��
		const float screen_width = graphics.GetScreenWidth();
		const float screen_height =graphics.GetScreenHeight();
		const float texture_width = static_cast<float>(font->GetTextureWidth());
		const float texture_height =static_cast<float>(font->GetTextureHeight());
		const float positionX = screen_width / 3;// - textureWidth;// * (textureWidth / 2);
		const float positionY = (screen_height / 2) - texture_height;

		font->TextOutW(device_context, title_name, positionX, positionY, texture_width/2, texture_height/2);
		font->TextOutW(device_context, push_text, positionX, screen_height - texture_height, texture_width / 4, texture_height / 4);
	}

#if defined(DEBUG) | defined(_DEBUG)
	// 2D�f�o�b�OGUI�`��
	{
	}
#endif

}

bool SceneTitle::OnMessage(const Telegram& telegram)
{
	LOG(" Error :  No Function | SceneTitle::OnMessage\n")
	return  false;
}

void SceneTitle::SendMessaging(MESSAGE_TYPE msg)
{
	LOG(" Error : No Function | SceneTitle::SendMessaging\n")
}

void SceneTitle::Finalize()
{
}
