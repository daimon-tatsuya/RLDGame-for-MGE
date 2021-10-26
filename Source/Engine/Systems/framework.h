#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <memory>

#include <d3d11.h>
#include <wrl.h>

#include "Engine/Systems/HighResolutionTimer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/Input.h"
class Framework
{
public:

public:

	Framework(HWND hwnd);

	~Framework() {}

	int Run();

	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:

	void Update(float elapsed_time/*Elapsed seconds from last frame*/);
	void Render(float elapsed_time/*Elapsed seconds from last frame*/);
	void CalculateFrameStats();

private:
	const HWND				hwnd;
	HighResolutionTimer	timer;
	Graphics					graphics;
	Input							input;
};
