#pragma once
//**********************************************************
//
//	GamePad�N���X
//
//**********************************************************

using GamePadButton = unsigned int;

/// <summary>
/// <para>�Q�[���p�b�h�̓��͂����m����B </para>
/// <para>PC�R���g���[���[�A�L�[�{�[�h </para>
/// </summary>
class GamePad final
{
private:

	GamePadButton		button_state[2] = { 0 };
	GamePadButton		button_down = 0;
	GamePadButton		button_up = 0;

	float				axisLX = 0.0f;
	float				axisLY = 0.0f;
	float				axisRX = 0.0f;
	float				axisRY = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int				slot = 0;

public:

	const static  GamePadButton BTN_UP = (1 << 0);        // �\���L�[��
	const static  GamePadButton BTN_RIGHT = (1 << 1);  // �\���L�[�E
	const static  GamePadButton BTN_DOWN = (1 << 2);	 // �\���L�[��
	const static  GamePadButton BTN_LEFT = (1 << 3);	 // �\���L�[��
	const static  GamePadButton BTN_A = (1 << 4);//A�{�^��
	const static  GamePadButton BTN_B = (1 << 5);//B�{�^��
	const static  GamePadButton BTN_X = (1 << 6);//X�{�^��
	const static  GamePadButton BTN_Y = (1 << 7);//Y�{�^��
	const static  GamePadButton BTN_START = (1 << 8);//�E�X�^�[�g�{�^��
	const static  GamePadButton BTN_BACK = (1 << 9);  //���X�^�[�g�{�^��
	const static  GamePadButton BTN_LEFT_THUMB = (1 << 10);		// ���X�e�B�b�N��������
	const static  GamePadButton BTN_RIGHT_THUMB = (1 << 11); // �E�X�e�B�b�N��������
	const static  GamePadButton BTN_LEFT_SHOULDER = (1 << 12);		// LB�FL1
	const static  GamePadButton BTN_RIGHT_SHOULDER = (1 << 13);	// RB�FR1
	const static  GamePadButton BTN_LEFT_TRIGGER = (1 << 14);			// LT�FL2
	const static  GamePadButton BTN_RIGHT_TRIGGER = (1 << 15);		// RT�FR2

	const static  GamePadButton AnyBTN =// ABXY�{�^��
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y;

	static const GamePadButton AnyDPad =// �\���L�[
		GamePad::BTN_UP
		| GamePad::BTN_RIGHT
		| GamePad::BTN_DOWN
		| GamePad::BTN_LEFT;

private:

public:

	GamePad() = default;
	~GamePad() = default;

	// �X�V
	void Update();

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// �X���b�g�ݒ�
	void SetSlot(int slot) { this->slot = slot; }

	// �{�^�����͏�Ԃ̎擾
	GamePadButton GetButton() const { return button_state[0]; }

	// �{�^��������Ԃ̎擾
	GamePadButton GetButtonDown() const { return button_down; }

	// �{�^�������Ԃ̎擾
	GamePadButton GetButtonUp() const { return button_up; }

	// ���X�e�B�b�NX�����͏�Ԃ̎擾
	float GetAxisLX() const { return axisLX; }

	// ���X�e�B�b�NY�����͏�Ԃ̎擾
	float GetAxisLY() const { return axisLY; }

	// �E�X�e�B�b�NX�����͏�Ԃ̎擾
	float GetAxisRX() const { return axisRX; }

	// �E�X�e�B�b�NY�����͏�Ԃ̎擾
	float GetAxisRY() const { return axisRY; }

	// ���g���K�[���͏�Ԃ̎擾
	float GetTriggerL() const { return triggerL; }

	// �E�g���K�[���͏�Ԃ̎擾
	float GetTriggerR() const { return triggerR; }
};
