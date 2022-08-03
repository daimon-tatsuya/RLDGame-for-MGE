//**********************************************************
//
//		SceneTitleクラス
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

	// なにかボタンを押したらローディングシーンへ切り替え
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

	// 画面クリア＆レンダーターゲット設定
	const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	//描画処理
	RenderContext render_context{};
	render_context.light_direction = { -0.5f, -1.0f, -0.5f, 0.0f };	// ライト方向（下方向）

	// 3Dエフェクト描画
	{
	}

	// 3Dデバッグ描画
	{
		// キャラクターデバッグ描画
		//CharacterManager::Instance().DrawDebugPrimitive();

		//// ラインレンダラ描画実行
		//graphics.GetLineRenderer()->Render(device_context, render_context.view, render_context.projection);

		//// デバッグレンダラ描画実行
		//graphics.GetDebugRenderer()->Render(device_context, render_context.view, render_context.projection);
	}

	// 2Dスプライト描画
	{
		const Sprite* font = graphics.GetFont();

		// 「Title」を描画
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
	// 2DデバッグGUI描画
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
