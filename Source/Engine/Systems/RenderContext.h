#pragma once

#include "Engine/Systems/Math.h"

/// <summary>
/// 描画のリソース
/// </summary>
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4			light_direction;
};
