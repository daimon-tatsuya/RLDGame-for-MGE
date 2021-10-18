#pragma once

#include "Engine/Systems/GamePad.h"
#include "Engine/Systems/Mouse.h"

// �C���v�b�g
class Input
{
public:
	Input(HWND hWnd);
	~Input() {}

public:
	// �C���X�^���X�擾
	static Input& Instance() { return *instance; }

	// �X�V����
	void Update();

	// �Q�[���p�b�h�擾
	GamePad& GetGamePad() { return game_pad; }

	// �}�E�X�擾
	Mouse& GetMouse() { return mouse; }

private:
	static Input* instance;
	GamePad				game_pad;
	Mouse					mouse;
};
