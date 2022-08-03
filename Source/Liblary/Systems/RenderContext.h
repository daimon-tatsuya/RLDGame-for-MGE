#pragma once

#include "Liblary/Systems/Math.h"

/// <summary>
/// �`��̃��\�[�X
/// </summary>
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4			light_direction;
};
