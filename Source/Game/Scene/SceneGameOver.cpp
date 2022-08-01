//**********************************************************
//
//		SceneGameOverクラス
//
//**********************************************************
#include <charconv>
#include <iostream>
#include "Game/Scene/SceneGameOver.h"
#include "Engine/Systems/Blender.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Logger.h"

#include "Engine/Objects/Sprite.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/RenderContext.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneGame.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"
#include "Engine/Systems/DungeonSystem.h"


const float TEXT_ADJUST_FROM_SCENE_OVER = 48.f;//over_textが少しずれていたので目押しで修正

// デストラクタ
SceneGameOver::~SceneGameOver()
{
	LOG(" Executed : SceneGameOver's destructor\n")
}

// 初期化
void SceneGameOver::Initialize()
{
	if (DungeonSystem::Instance().GetIsDungeonClear() == true)//ゲームをクリアしているなら
	{
		strcpy_s(over_text, "Game Clear");//ゲームクリアのテキスト
	}
	else
	{
		strcpy_s(over_text, "Game Over"); // ゲームオーバーのテキスト
	}

	//経過ターンの取得
	const int turn_number = DungeonSystem::Instance().GetElapsedTurn();

	//経過ターンをchar型に変換する
	char turn_text[TEXT_BUFFER_SIZE];
	snprintf(turn_text, TEXT_BUFFER_SIZE, "%d", turn_number);
	strcat_s(turn_text, "turn");
	strcpy_s(turn_number_text, turn_text);//ゲームを終えたときのターン数のテキスト
}

// 終了化
void SceneGameOver::Finalize()
{
	//delete[] over_text;
	//delete[] turn_number_text;
	//delete[] push_text;
}

void SceneGameOver::Update(float elapsedTime)
{
	const GamePad& game_pad = Input::Instance().GetGamePad();

	// Aボタンを押したらタイトルシーンへ切り替え
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}

void SceneGameOver::Render()
{
	//viewの取得
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// 描画処理
	RenderContext render_context{};
	render_context.light_direction = { -0.5f, -1.0f, -0.5f, 0.0f };	// ライト方向（下方向）

	// 2Dスプライト描画
	{
		// フォントを取得
		const Sprite* font = graphics.GetFont();

		// テキスト描画用の変数
		const float screen_width = graphics.GetScreenWidth();// 画面サイズ横
		const float screen_height = graphics.GetScreenHeight();// 画面サイズ縦
		const float texture_width = font->GetTextureWidth();// フォントサイズ横
		const float texture_height = font->GetTextureHeight();// フォントサイズ横
		//const float positionX = screen_width / 3;// テキストを描画するときの開始位置
		//const float positionY = (screen_height / 3) - texture_height;// テキストを描画するときの開始位置

		// ゲーム終了時のテキストの描画
		font->TextOutW(device_context, over_text, screen_width / 4 - TEXT_ADJUST_FROM_SCENE_OVER, (screen_height / 2) - texture_height, texture_width / 2, texture_height / 2);

		// ゲーム終了時の経過ターンの描画
		font->TextOutW(device_context, turn_number_text, screen_width / 2, screen_height - texture_height, texture_width / 4, texture_height / 4);
	}
}

bool SceneGameOver::OnMessage(const Telegram& telegram)
{
	return false;
}

void SceneGameOver::SendMessaging(MESSAGE_TYPE msg)
{
}
