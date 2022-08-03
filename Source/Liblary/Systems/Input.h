#pragma once
//**********************************************************
//
//		Input�N���X
//
//**********************************************************

#include "Liblary/Systems/GamePad.h"
#include "Liblary/Systems/Mouse.h"

/// <summary>
/// ���͊Ǘ��N���X
/// </summary>
class Input final
{
private:
	static Input* instance;
	GamePad	   game_pad;
	Mouse		   mouse;

public:

private:

public:
	Input(HWND hWnd);
	~Input() = default;

	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>Input�N���X�̗B��̃C���X�^���X</returns>
	static Input& Instance() { return *instance; }

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �Q�[���p�b�h�擾
	/// </summary>
	/// <returns>�Q�[���p�b�h�̓��͂����N���X�̃C���X�^���X</returns>
	GamePad& GetGamePad() { return game_pad; }

	/// <summary>
	///  �}�E�X�擾
	/// </summary>
	/// <returns>�}�E�X�̓��͂����N���X�̃C���X�^���X</returns>
	Mouse& GetMouse() { return mouse; }
};
