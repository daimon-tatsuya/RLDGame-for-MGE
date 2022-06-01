//**********************************************************
//
//		SceneLoading�N���X
//
//**********************************************************

#include <thread>

#include "Engine/Systems/Graphics.h"
#include "Engine/Objects//Sprite.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneLoading.h"
#include "Engine/Systems/Logger.h"


// ������
void SceneLoading::Initialize()
{
	// �X���b�h�J�n
	std::thread thread(LoadingThread, this);

	// �X���b�h�̊Ǘ������
	thread.detach();
}

// �I����
void SceneLoading::Finalize()
{
}

// �X�V����
void SceneLoading::Update(float elapsed_time)
{
	// ���[�f�B���O���o�����B
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

	// ���̃V�[���̏���������������V�[����؂�ւ���
	if (next_scene->IsReady())
	{
		SceneManager::Instance().ChangeScene(next_scene);
		next_scene = nullptr;
	}
}

// �`�揈��
void SceneLoading::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	const FLOAT clear_color[] = { 0.0f, 0.0f, 0.f, 1.0f };	// RGBA(0.0�`1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// 2D�X�v���C�g�`��
	{
		// Now Loading...�`��B
		const Sprite* font = graphics.GetFont();

		// ��ʉE���Ƀ��[�f�B���O�A�C�R����`��
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

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(const SceneLoading* scene)
{
	//�V���A���C�Y���Ă��ă��[�h����������̂Ń~���b�P�ʂŎ~�߂�
	Sleep(3000);
	// COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������

	/*HRSULT hr =*/ CoInitialize(nullptr);

	// ���̃V�[���̏��������s��
	scene->next_scene->Initialize();

	// �X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	// ���̃V�[���̏��������ݒ�
	scene->next_scene->SetReady();
}