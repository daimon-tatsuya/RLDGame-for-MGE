//**********************************************************
//
//	GamePad�N���X
//
//**********************************************************

#include <Windows.h>
#include <cmath>
#include <Xinput.h>

#include "Liblary/Systems/GamePad.h"

void GamePad::Update()
{
	axisLX = axisLY = 0.0f;
	axisRX = axisRY = 0.0f;
	triggerL = triggerR = 0.0f;

	GamePadButton new_button_state = 0;

	// �{�^�����擾
	XINPUT_STATE xinput_state{};
	if (XInputGetState(slot, &xinput_state) == ERROR_SUCCESS)
	{
		//! XINPUT_CAPABILITIES caps;
		//! XInputGetCapabilities(m_slot, XINPUT_FLAG_GAMEPAD, &caps);
		XINPUT_GAMEPAD& pad = xinput_state.Gamepad;

		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_UP)							    new_button_state |= BTN_UP;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)						new_button_state |= BTN_RIGHT;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)						new_button_state |= BTN_DOWN;
		if (pad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)							new_button_state |= BTN_LEFT;
		if (pad.wButtons & XINPUT_GAMEPAD_A)											new_button_state |= BTN_A;
		if (pad.wButtons & XINPUT_GAMEPAD_B)											new_button_state |= BTN_B;
		if (pad.wButtons & XINPUT_GAMEPAD_X)											new_button_state |= BTN_X;
		if (pad.wButtons & XINPUT_GAMEPAD_Y)											new_button_state |= BTN_Y;
		if (pad.wButtons & XINPUT_GAMEPAD_START)									new_button_state |= BTN_START;
		if (pad.wButtons & XINPUT_GAMEPAD_BACK)										new_button_state |= BTN_BACK;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)						new_button_state |= BTN_LEFT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)					new_button_state |= BTN_RIGHT_THUMB;
		if (pad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)				new_button_state |= BTN_LEFT_SHOULDER;
		if (pad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)				new_button_state |= BTN_RIGHT_SHOULDER;
		if (pad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	new_button_state |= BTN_LEFT_TRIGGER;
		if (pad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)	new_button_state |= BTN_RIGHT_TRIGGER;

		if ((pad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(pad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && pad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			pad.sThumbLX = 0;
			pad.sThumbLY = 0;
		}

		if ((pad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(pad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			pad.sThumbRX = 0;
			pad.sThumbRY = 0;
		}

		triggerL = static_cast<float>(pad.bLeftTrigger) / 255.0f;
		triggerR = static_cast<float>(pad.bRightTrigger) / 255.0f;
		axisLX = static_cast<float>(pad.sThumbLX) / static_cast<float>(0x8000);
		axisLY = static_cast<float>(pad.sThumbLY) / static_cast<float>(0x8000);
		axisRX = static_cast<float>(pad.sThumbRX) / static_cast<float>(0x8000);
		axisRY = static_cast<float>(pad.sThumbRY) / static_cast<float>(0x8000);
	}
	else
	{
#if 0
		//! XInput�œ��͏�񂪎擾�o���Ȃ������ꍇ��WindowsAPI�Ŏ擾����
		JOYINFOEX joyInfo;
		joyInfo.dwSize = sizeof(JOYINFOEX);
		joyInfo.dwFlags = JOY_RETURNALL;	// �S�Ă̏����擾

		if (joyGetPosEx(slot, &joyInfo) == JOYERR_NOERROR)
		{
			// ���iID���`�F�b�N����PS4�R���g���[���[�����Ή�����
			const static  WORD PS4_PID = 1476;

			JOYCAPS joy_caps;
			if (joyGetDevCaps(slot, &joy_caps, sizeof(JOYCAPS)) == JOYERR_NOERROR)
			{
				// �\���L�[
				if (joyInfo.dwPOV != 0xFFFF)
				{
					const static  int pov_bit[8] =
					{
						BTN_UP,								// ��
						BTN_RIGHT | BTN_UP,		// �E��
						BTN_RIGHT,						// �E
						BTN_RIGHT | BTN_DOWN,	// �E��
						BTN_DOWN,						// ��
						BTN_LEFT | BTN_DOWN,	// ����
						BTN_LEFT,							// ��
						BTN_LEFT | BTN_UP			// ����
					};
					int angle = joyInfo.dwPOV / 4500;
					new_button_state |= pov_bit[angle];
				}
				if (joy_caps.wPid == PS4_PID)
				{
					// �{�^�����
					if (joyInfo.dwButtons & JOY_BUTTON1)  new_button_state |= BTN_Y;
					if (joyInfo.dwButtons & JOY_BUTTON2)  new_button_state |= BTN_B;
					if (joyInfo.dwButtons & JOY_BUTTON3)  new_button_state |= BTN_A;
					if (joyInfo.dwButtons & JOY_BUTTON4)  new_button_state |= BTN_X;
					if (joyInfo.dwButtons & JOY_BUTTON5)  new_button_state|= BTN_LEFT_SHOULDER;
					if (joyInfo.dwButtons & JOY_BUTTON6)  new_button_state|= BTN_RIGHT_SHOULDER;
					if (joyInfo.dwButtons & JOY_BUTTON7)  new_button_state|= BTN_LEFT_TRIGGER;
					if (joyInfo.dwButtons & JOY_BUTTON8)  new_button_state|= BTN_RIGHT_TRIGGER;
					if (joyInfo.dwButtons & JOY_BUTTON9)  new_button_state|= BTN_BACK;
					if (joyInfo.dwButtons & JOY_BUTTON10)new_button_state |= BTN_START;
					if (joyInfo.dwButtons & JOY_BUTTON11)new_button_state |= BTN_LEFT_THUMB;
					if (joyInfo.dwButtons & JOY_BUTTON12)new_button_state |= BTN_RIGHT_THUMB;
					//if (joyInfo.dwButtons & JOY_BUTTON13) newButtonState |= BTN_?;	// PS
					//if (joyInfo.dwButtons & JOY_BUTTON14) newButtonState |= BTN_?;	// Touch

					// ���X�e�B�b�N
					axisLX = static_cast<int>(joyInfo.dwXpos - 0x7FFF) / static_cast<float>(0x8000);
					axisLY = -static_cast<int>(joyInfo.dwYpos - 0x7FFF) / static_cast<float>(0x8000);

					// �E�X�e�B�b�N
					axisRX = static_cast<int>(joyInfo.dwZpos - 0x7FFF) / static_cast<float>(0x8000);
					axisRY = -static_cast<int>(joyInfo.dwRpos - 0x7FFF) / static_cast<float>(0x8000);

					// LR�g���K�[
					triggerL = static_cast<float>(joyInfo.dwVpos) / static_cast<float>(0xFFFF);
					triggerR = static_cast<float>(joyInfo.dwUpos) / static_cast<float>(0xFFFF);

					if (axisLX > -0.25f && axisLX< 0.25f) axisLX = 0.0f;
					if (axisRX > -0.25f && axisRX < 0.25f) axisRX = 0.0f;
				}
			}
		}
#endif
	}

	// �L�[�{�[�h�ŃG�~�����[�V����
	{
		float lx = 0.0f;
		float ly = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		if (GetAsyncKeyState('W') & 0x8000) ly = 1.0f;
		if (GetAsyncKeyState('A') & 0x8000) lx = -1.0f;
		if (GetAsyncKeyState('S') & 0x8000) ly = -1.0f;
		if (GetAsyncKeyState('D') & 0x8000) lx = 1.0f;
		if (GetAsyncKeyState('I') & 0x8000) ry = 1.0f;
		if (GetAsyncKeyState('J') & 0x8000) rx = -1.0f;
		if (GetAsyncKeyState('K') & 0x8000) ry = -1.0f;
		if (GetAsyncKeyState('L') & 0x8000) rx = 1.0f;
		if (GetAsyncKeyState('Z') & 0x8000) new_button_state |= BTN_A;
		if (GetAsyncKeyState('X') & 0x8000) new_button_state |= BTN_B;
		if (GetAsyncKeyState('C') & 0x8000) new_button_state |= BTN_X;
		if (GetAsyncKeyState('V') & 0x8000) new_button_state |= BTN_Y;
		if (GetAsyncKeyState(VK_UP) & 0x8000)			new_button_state |= BTN_UP;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	new_button_state |= BTN_RIGHT;
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)	new_button_state |= BTN_DOWN;
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)		new_button_state |= BTN_LEFT;

		// �e�L�g�[�ɐݒ�
		if (GetAsyncKeyState('N') & 0x8000) new_button_state |= BTN_START;
		if (GetAsyncKeyState('M') & 0x8000) new_button_state |= BTN_BACK;

		if (GetAsyncKeyState('Q') & 0x8000) new_button_state |= BTN_LEFT_THUMB;
		if (GetAsyncKeyState('E') & 0x8000) new_button_state |= BTN_LEFT_SHOULDER;
		if (GetAsyncKeyState('R') & 0x8000) new_button_state |= BTN_LEFT_TRIGGER;
		if (GetAsyncKeyState('U') & 0x8000) new_button_state |= BTN_RIGHT_THUMB;
		if (GetAsyncKeyState('O') & 0x8000) new_button_state |= BTN_RIGHT_SHOULDER;
		if (GetAsyncKeyState('P') & 0x8000) new_button_state |= BTN_RIGHT_TRIGGER;

#if 1
		if (new_button_state & BTN_UP)    ly = 1.0f;
		if (new_button_state & BTN_RIGHT) lx = 1.0f;
		if (new_button_state & BTN_DOWN)  ly = -1.0f;
		if (new_button_state & BTN_LEFT)  lx = -1.0f;
#endif

		if (lx >= 1.0f || lx <= -1.0f || ly >= 1.0f || ly <= -1.0)
		{
			const float power = ::sqrtf(lx * lx + ly * ly);
			axisLX = lx / power;
			axisLY = ly / power;
		}

		if (rx >= 1.0f || rx <= -1.0f || ry >= 1.0f || ry <= -1.0)
		{
			float power = ::sqrtf(rx * rx + ry * ry);
			axisRX = rx / power;
			axisRY = ry / power;
		}
	}

	// �{�^�����̍X�V
	{
		button_state[1] = button_state[0];	// �X�C�b�`����
		button_state[0] = new_button_state;

		button_down = ~button_state[1] & new_button_state;	// �������u��
		button_up = ~new_button_state & button_state[1];			// �������u��
	}
}