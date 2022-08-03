//**********************************************************
//
//		Input�N���X
//
//**********************************************************
#include <d3d11.h>
#include <memory>
#include "Liblary/Systems/Input.h"

Input* Input::instance = nullptr;

// �R���X�g���N�^
Input::Input(HWND hWnd)
	: mouse(hWnd)
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

// �X�V����
void Input::Update()
{
	game_pad.Update();
	mouse.Update();
}