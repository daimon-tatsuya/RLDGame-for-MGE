#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <memory>

#include <d3d11.h>
#include <wrl.h>

#include "Engine/Systems/HighResolutionTimer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"


//ToDo Framework �R�����g
class Framework
{

private:

	const HWND				hwnd;
	HighResolutionTimer	timer;
	Graphics					graphics;
	Input							input;

public:


private:

	void Update(float elapsed_time/*�Ō�̃t���[������̌o�ߕb��*/);

	void Render(float elapsed_time/*�Ō�̃t���[������̌o�ߕb��*/);

	void CalculateFrameStats();

public:

	Framework(HWND hwnd);

	~Framework() {}

	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	int Run();

};
