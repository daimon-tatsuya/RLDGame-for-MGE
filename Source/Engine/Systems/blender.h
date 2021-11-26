#pragma once
//**********************************************************
//
//		Blenderクラス
//
//**********************************************************

#include <d3d11.h>
#include <wrl.h>
#include "Engine/Systems/Misc.h"

/// <summary>
/// BlendStateを作成するクラス
/// </summary>
class Blender
{
private:

public:
	enum  BLEND_STATE : int
	{
		BS_NONE,
		BS_ALPHA,
		BS_ADD,
		BS_SUBTRACT,
		BS_REPLACE,
		BS_MULTIPLY,
		BS_LIGHTEN,
		BS_DARKEN,
		BS_SCREEN,
		BS_END
	};
	Microsoft::WRL::ComPtr<ID3D11BlendState> states[BS_END];
public:
	Blender(ID3D11Device* device);
};