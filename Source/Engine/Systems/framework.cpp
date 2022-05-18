//**********************************************************
//
//		Framework�N���X
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

// ���������Ԋu�ݒ� //1�̎��t���[�����Œ�
static const int syncInterval = 1;

Framework::Framework(HWND hwnd)
	: hwnd(hwnd),
	graphics(hwnd),
	input(hwnd)
{
	// �V�[��������
	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
}

void Framework::Update(float elapsed_time/*�Ō�̃t���[������̌o�ߕb��*/)
{
	// ���͍X�V����
	input.Update();

	// �V�[���X�V����
	SceneManager::Instance().Update(elapsed_time);
}

void Framework::Render(float elapsedTime/*Elapsed seconds from last frame*/) const
{
	// �ʃX���b�h���Ƀf�o�C�X�R���e�L�X�g���g���Ă����ꍇ��
	// �����A�N�Z�X���Ȃ��悤�ɔr�����䂷��
	//_Requires_lock_held_(graphics.GetMutex());
	//std::lock_guard<std::mutex> lock(graphics.GetMutex());

	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	// IMGUI�t���[���J�n����
	graphics.GetImGuiRenderer()->NewFrame();

	// �V�[���`�揈��
	SceneManager::Instance().Render();

	// IMGUI�`��
	graphics.GetImGuiRenderer()->Render(device_context);

	// �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
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
			//_CrtDumpMemoryLeaks();	// ���̎��_�ŊJ������Ă��Ȃ��������̏��̕\�����
		}
	}

	//_CrtDumpMemoryLeaks();	// ���̎��_�ŊJ������Ă��Ȃ��������̏��̕\��

	return static_cast<int>(msg.wParam);
}

void Framework::CalculateFrameStates() const
{
	// 1 �b������̕��σt���[�����ƁA1 �̃t���[���������_�����O����̂ɂ����镽�ώ��Ԃ��v�Z���܂��B
	// �����̓��v���́A�E�B���h�E�̃L���v�V�����o�[�ɒǉ�����܂��B
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// 1�b���Ƃ̕��ϒl���v�Z����B
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		const float fps = static_cast<float>(frames); // fps = frameCnt / 1
		const float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "RougeLikeGame(NoName)" << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hwnd, outs.str().c_str());

		// ���̕��ϒl�̂��߂Ƀ��Z�b�g���܂��B
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
		// WM_EXITSIZEMOVE�́A���[�U�[�����T�C�Y�o�[��͂񂾂Ƃ��ɑ����܂��B
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE�́A���[�U�����T�C�Y�o�[�𗣂����Ƃ��ɑ����܂��B
		// �����ŁA�V�����E�B���h�E�̐��@�Ɋ�Â��Ă��ׂĂ����Z�b�g���܂��B
		timer.Start();
		break;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}