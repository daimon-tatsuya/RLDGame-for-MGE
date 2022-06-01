//**********************************************************
//
//		SceneLoadingクラス
//
//**********************************************************

#include <thread>

#include "Engine/Systems/Graphics.h"
#include "Engine/Objects//Sprite.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneLoading.h"
#include "Engine/Systems/Logger.h"


// 初期化
void SceneLoading::Initialize()
{
	// スレッド開始
	std::thread thread(LoadingThread, this);

	// スレッドの管理を放棄
	thread.detach();
}

// 終了化
void SceneLoading::Finalize()
{
}

// 更新処理
void SceneLoading::Update(float elapsed_time)
{
	// ローディング演出処理。
	timer -= elapsed_time;
	if (timer < 0)
	{
		timer = 0.1f;

		strcpy_s(message, "Now Loading");
		counter = (counter + 1) % 6;
		for (int i = 0; i < counter; i++)
		{
			strcat_s(message, ".");
		}
	}

	// 次のシーンの準備が完了したらシーンを切り替える
	if (next_scene->IsReady())
	{
		SceneManager::Instance().ChangeScene(next_scene);
		next_scene = nullptr;
	}
}

// 描画処理
void SceneLoading::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	const FLOAT clear_color[] = { 0.0f, 0.0f, 0.f, 1.0f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// 2Dスプライト描画
	{
		// Now Loading...描画。
		const Sprite* font = graphics.GetFont();

		// 画面右下にローディングアイコンを描画
		const float screen_width = static_cast<float>(graphics.GetScreenWidth());
		const float screen_height = static_cast<float>(graphics.GetScreenHeight());
		const float texture_width = 32.f;// static_cast<float>(font->GetTextureWidth());
		const float texture_height = 32.f;// static_cast<float>(font->GetTextureHeight());
		const float positionX = screen_width - texture_width * (texture_width / 2);
		const float positionY = screen_height - texture_height;

		font->TextOutW(device_context, message, positionX, positionY, 32, 32);
	}
}

bool SceneLoading::OnMessage(const Telegram& telegram)
{
	LOG(" Error : SceneLoading::OnMessage is No Function\n")
	return false;
}

void SceneLoading::SendMessaging(MESSAGE_TYPE msg)
{
	LOG(" Error : SceneLoading::SendMessaging is No Function\n")
}

// ローディングスレッド
void SceneLoading::LoadingThread(const SceneLoading* scene)
{
	//シリアライズしていてロードが速すぎるのでミリ秒単位で止める
	Sleep(3000);
	// COM関連の初期化でスレッド毎に呼ぶ必要がある

	/*HRSULT hr =*/ CoInitialize(nullptr);

	// 次のシーンの初期化を行う
	scene->next_scene->Initialize();

	// スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	// 次のシーンの準備完了設定
	scene->next_scene->SetReady();
}