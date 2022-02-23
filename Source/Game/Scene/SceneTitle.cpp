//**********************************************************
//
//		SceneTitleクラス
//
//**********************************************************

#include "Engine/Systems/Blender.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"


#include "Engine/Objects/Sprite.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/RenderContext.h"
#include"Game/Characters/Player.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneGame.h"
#include "Game/Scene/SceneLoading.h"

void SceneTitle::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	strcpy_s(message, "Title");

}

void SceneTitle::Update(float elapsed_time)
{
	const GamePad& gamePad = Input::Instance().GetGamePad();

	// なにかボタンを押したらローディングシーンへ切り替え
	if (gamePad.GetButtonDown() & static_cast<GamePadButton>(GamePad::AnyBTN))
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
	constexpr float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	 //描画処理
	RenderContext render_context{};
	render_context.light_direction = { -0.5f, -1.0f, -0.5f, 0.0f };	// ライト方向（下方向）

	// 2Dスプライト描画
	{
	}

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
		// HUD更新
	//	headUpDisplay->Render(dc);

		const Sprite* font = graphics.GetFont();

		// 「Title」を描画
		const float screen_width = static_cast<float>(graphics.GetScreenWidth());
		const float screen_height = static_cast<float>(graphics.GetScreenHeight());
		constexpr float texture_width = 32.f;// static_cast<float>(font->GetTextureWidth());
		constexpr float texture_height = 32.f;// static_cast<float>(font->GetTextureHeight());
		const float positionX = screen_width / 3;// - textureWidth;// * (textureWidth / 2);
		const float positionY =( screen_height / 2 )- texture_height;

		font->TextOutW(device_context, message, positionX, positionY, 128, 128);
	}

	// 2DデバッグGUI描画
	{
	//	CharacterManager::Instance().DrawDebugGUI();
	}
}

void SceneTitle::Finalize()
{
}