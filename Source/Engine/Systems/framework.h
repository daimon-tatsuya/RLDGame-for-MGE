#pragma once

//**********************************************************
//
//		Framework�N���X
//
//**********************************************************

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <memory>

#include <d3d11.h>
#include <wrl.h>

#include "Engine/Systems/HighResolutionTimer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"



class Framework
{

private:

	const HWND				hwnd;
	HighResolutionTimer	timer;//�t���[���Ǘ��N���X
	Graphics					graphics;//�`�����Ǘ��N���X
	Input							input;//���͊Ǘ��N���X

public:


private:
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void Update(float elapsed_time/*�Ō�̃t���[������̌o�ߕb��*/);

	/// <summary>
	///  �`�揈��
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void Render(float elapsed_time/*�Ō�̃t���[������̌o�ߕb��*/);

	//1 �b������̕��σt���[�����ƁA1 �̃t���[����`�悷��̂ɂ����镽�ώ��Ԃ̌v�Z
	void CalculateFrameStats();

public:

	Framework(HWND hwnd);

	~Framework() {}

	// WIN32���b�Z�[�W�n���h���[
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	int Run();

};
