//**********************************************************
//
//		Frameworkクラス
//
//**********************************************************

#include <crtdbg.h>
#include <memory>

#include <sstream>

#include "Liblary/Systems/Framework.h"
#include "Liblary/Systems/DebugRenderer.h"
#include "Liblary/Systems/ImGuiRenderer.h"
#include "Liblary/Systems/Logger.h"

#include "Liblary/Systems/ShaderManager.h"

#include "Liblary/Systems/SceneManager.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneGame.h"

// 垂直同期間隔設定 //1の時フレームを固定
static const int SYNCHRONOUS_INTERVAL = 1;

Framework::Framework(HWND handle_window)
	: handle_window(handle_window),
	graphics(handle_window),
	input(handle_window)
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
	graphics.GetSwapChain()->Present(SYNCHRONOUS_INTERVAL, 0);
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

			float elapsed_time = SYNCHRONOUS_INTERVAL == 0 ? timer.TimeInterval() : SYNCHRONOUS_INTERVAL / 60.0f;

			Update(timer.TimeInterval());
			Render(timer.TimeInterval());
			//_CrtDumpMemoryLeaks();	// この時点で開放されていないメモリの情報の表示解放
		}
	}
	//LOG(" Execute : Detect memory leaks | Framework.cpp\n")
	//_CrtDumpMemoryLeaks();	// この時点で開放されていないメモリの情報の表示
	//LOG(" Executed:Detected memory leaks | Framework.cpp\n")
	LOG(" Warning : Memory leaks that appear after this are real | Framework.cpp\n")
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
		SetWindowTextA(handle_window, outs.str().c_str());

		// 次の平均値のためにリセットします。
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

LRESULT Framework::HandleMessage(HWND handle_window, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(handle_window, msg, wparam, lparam))
	{
		return true;
	}
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(handle_window, &ps);
		EndPaint(handle_window, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wparam == VK_ESCAPE) PostMessage(handle_window, WM_CLOSE, 0, 0);
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
		return DefWindowProc(handle_window, msg, wparam, lparam);
	}
	return 0;
}