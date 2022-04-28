#pragma once
//**********************************************************
//
//		Mouse�N���X
//
//**********************************************************

#include <Windows.h>

using MouseButton = unsigned int;

/// <summary>
///�}�E�X�̑�����͂����m����N���X
/// </summary>
class Mouse final
{
private:
	MouseButton		button_state[2] = { 0 };
	MouseButton		button_down = 0;
	MouseButton		button_up = 0;
	int						positionX[2]{};
	int						positionY[2]{};
	int						wheel[2]{};
	int						screen_width = 0;
	int						screen_height = 0;
	HWND				hWnd = nullptr;
public:
	static const MouseButton BTN_LEFT = (1 << 0);
	static const MouseButton BTN_MIDDLE = (1 << 1);
	static const MouseButton BTN_RIGHT = (1 << 2);

public:
	Mouse(HWND hWnd);
	~Mouse() = default;

	// �X�V
	void Update();

	// �{�^�����͏�Ԃ̎擾
	MouseButton GetButton() const { return button_state[0]; }

	// �{�^��������Ԃ̎擾
	MouseButton GetButtonDown() const { return button_down; }

	// �{�^�������Ԃ̎擾
	MouseButton GetButtonUp() const { return button_up; }

	// �z�C�[���l�̐ݒ�
	void SetWheel(int wheel) { this->wheel[0] += wheel; }

	// �z�C�[���l�̎擾
	int GetWheel() const { return wheel[1]; }

	// �}�E�X�J�[�\��X���W�擾
	int GetPositionX() const { return positionX[0]; }

	// �}�E�X�J�[�\��Y���W�擾
	int GetPositionY() const { return positionY[0]; }

	// �O��̃}�E�X�J�[�\��X���W�擾
	int GetOldPositionX() const { return positionX[1]; }

	// �O��̃}�E�X�J�[�\��Y���W�擾
	int GetOldPositionY() const { return positionY[1]; }

	// �X�N���[�����ݒ�
	void SetScreenWidth(int width) { screen_width = width; }

	// �X�N���[�������ݒ�
	void SetScreenHeight(int height) { screen_height = height; }

	// �X�N���[�����擾
	int GetScreenWidth() const { return screen_width; }

	// �X�N���[�������擾
	int GetScreenHeight() const { return screen_height; }
};
