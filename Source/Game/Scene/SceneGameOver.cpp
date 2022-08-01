//**********************************************************
//
//		SceneGameOver�N���X
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


const float TEXT_ADJUST_FROM_SCENE_OVER = 48.f;//over_text����������Ă����̂Ŗډ����ŏC��

// �f�X�g���N�^
SceneGameOver::~SceneGameOver()
{
	LOG(" Executed : SceneGameOver's destructor\n")
}

// ������
void SceneGameOver::Initialize()
{
	if (DungeonSystem::Instance().GetIsDungeonClear() == true)//�Q�[�����N���A���Ă���Ȃ�
	{
		strcpy_s(over_text, "Game Clear");//�Q�[���N���A�̃e�L�X�g
	}
	else
	{
		strcpy_s(over_text, "Game Over"); // �Q�[���I�[�o�[�̃e�L�X�g
	}

	//�o�߃^�[���̎擾
	const int turn_number = DungeonSystem::Instance().GetElapsedTurn();

	//�o�߃^�[����char�^�ɕϊ�����
	char turn_text[TEXT_BUFFER_SIZE];
	snprintf(turn_text, TEXT_BUFFER_SIZE, "%d", turn_number);
	strcat_s(turn_text, "turn");
	strcpy_s(turn_number_text, turn_text);//�Q�[�����I�����Ƃ��̃^�[�����̃e�L�X�g
}

// �I����
void SceneGameOver::Finalize()
{
	//delete[] over_text;
	//delete[] turn_number_text;
	//delete[] push_text;
}

void SceneGameOver::Update(float elapsedTime)
{
	const GamePad& game_pad = Input::Instance().GetGamePad();

	// A�{�^������������^�C�g���V�[���֐؂�ւ�
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}

void SceneGameOver::Render()
{
	//view�̎擾
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	const float clear_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0�`1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// �`�揈��
	RenderContext render_context{};
	render_context.light_direction = { -0.5f, -1.0f, -0.5f, 0.0f };	// ���C�g�����i�������j

	// 2D�X�v���C�g�`��
	{
		// �t�H���g���擾
		const Sprite* font = graphics.GetFont();

		// �e�L�X�g�`��p�̕ϐ�
		const float screen_width = graphics.GetScreenWidth();// ��ʃT�C�Y��
		const float screen_height = graphics.GetScreenHeight();// ��ʃT�C�Y�c
		const float texture_width = font->GetTextureWidth();// �t�H���g�T�C�Y��
		const float texture_height = font->GetTextureHeight();// �t�H���g�T�C�Y��
		//const float positionX = screen_width / 3;// �e�L�X�g��`�悷��Ƃ��̊J�n�ʒu
		//const float positionY = (screen_height / 3) - texture_height;// �e�L�X�g��`�悷��Ƃ��̊J�n�ʒu

		// �Q�[���I�����̃e�L�X�g�̕`��
		font->TextOutW(device_context, over_text, screen_width / 4 - TEXT_ADJUST_FROM_SCENE_OVER, (screen_height / 2) - texture_height, texture_width / 2, texture_height / 2);

		// �Q�[���I�����̌o�߃^�[���̕`��
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
