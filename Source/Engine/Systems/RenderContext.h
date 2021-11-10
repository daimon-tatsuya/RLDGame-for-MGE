#pragma once

#include "Engine/Systems/Math.h"

//ToDo この,hが行うことを書く


// レンダーコンテキスト
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4			lightDirection;
};
