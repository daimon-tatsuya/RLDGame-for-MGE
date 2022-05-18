//**********************************************************
//
//		Frameworkクラス
//
//**********************************************************

#include <crtdbg.h>
#include <memory>

#include "Engine/Systems/Framework.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/Logger.h"

#include "Engine/Systems/ShaderManager.h"

#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneGame.h"

// 垂直同期間隔設定 //1の時フレームを固定
static const int syncInterval = 1;

Framework::Framework(HWND hwnd)
	: hwnd(hwnd),
	graphics(hwnd),
	input(hwnd)
{
	// シーン初期化
	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
}

void Framework::Update(float elapsed_time/*最後のフレームからの経過秒数*/)
{
	// 入力更新処理
	input.Update();

	// シーン更新処理
	SceneManager::Instance().Update(elapsed_time);
}

void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/) const
{
	// 別スレッド中にデバイスコンテキストが使われていた場合に
	// 同時アクセスしないように排他制御する
	//_Requires_lock_held_(graphics.GetMutex());
	//std::lock_guard<std::mutex> lock(graphics.GetMutex());

	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	// IMGUIフレーム開始処理
	graphics.GetImGuiRenderer()->NewFrame();

	// シーン描画処理
	SceneManager::Instance().Render();

	// IMGUI描画
	graphics.GetImGuiRenderer()->Render(device_context);

	// バックバッファに描画した画を画面に表示する。
	graphics.GetSwapChain()->Present(syncInterval, 0);
}

int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStates();

			float elapsedTime = syncInterval == 0 ? timer.TimeInterval() : syncInterval / 60.0f;

			Update(timer.TimeInterval());
			Render(timer.TimeInterval());
			//_CrtDumpMemoryLeaks();	// この時点で開放されていないメモリの情報の表示解放
		}
	}

	//_CrtDumpMemoryLeaks();	// この時点で開放されていないメモリの情報の表示

	return static_cast<int>(msg.wParam);
}

void Framework::CalculateFrameStates() const
{
	// 1 秒あたりの平均フレーム数と、1 つのフレームをレンダリングするのにかかる平均時間を計算します。
	// これらの統計情報は、ウィンドウのキャプションバーに追加されます。
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// 1秒ごとの平均値を計算する。
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		const float fps = static_cast<float>(frames); // fps = frameCnt / 1
		const float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "RougeLikeGame(NoName)" << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hwnd, outs.str().c_str());

		// 次の平均値のためにリセットします。
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

LRESULT Framework::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hwnd, msg, wparam, lparam))
	{
		return true;
	}
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVEは、ユーザーがリサイズバーを掴んだときに送られます。
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVEは、ユーザがリサイズバーを離したときに送られます。
		// ここで、新しいウィンドウの寸法に基づいてすべてをリセットします。
		timer.Start();
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}