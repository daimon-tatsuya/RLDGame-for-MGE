#include <stdlib.h>
#include <memory>

#include "Engine/Systems/framework.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/blender.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneLoading.h"

// 垂直同期間隔設定
static const int syncInterval = 1;

Framework::Framework(HWND hwnd)
	: hwnd(hwnd),
	graphics(hwnd),
	input(hwnd)
{
	//// シーン初期化
	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
}

void Framework::Update(float elapsedTime/*Elapsed seconds from last frame*/)
{
	// 入力更新処理
	input.Update();

	// シーン更新処理
	SceneManager::Instance().Update(elapsedTime);
}

void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/)
{
	//Todo lockを設定する
	// 別スレッド中にデバイスコンテキストが使われていた場合に
	// 同時アクセスしないように排他制御する
	std::lock_guard<std::mutex> lock(graphics.GetMutex());

	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	// IMGUIフレーム開始処理
	graphics.GetImGuiRenderer()->NewFrame();

	//// シーン描画処理
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
			CalculateFrameStats();

			float elapsedTime = syncInterval == 0 ? timer.TimeInterval() : syncInterval / 60.0f;

			Update(timer.TimeInterval());
			Render(timer.TimeInterval());
		}
	}

	return static_cast<int>(msg.wParam);
}

void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the
	// average time it takes to render one frame.  These stats
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hwnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

LRESULT Framework::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hwnd, msg, wparam, lparam))
		return true;

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
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}