#pragma once

<<<<<<< HEAD:Source/Liblary/Systems/RenderContext.h
#include "Liblary/Systems/Math.h"
=======
#include "MyLiblary/Systems/Math.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Systems/RenderContext.h

/// <summary>
/// ï`âÊÇÃÉäÉ\Å[ÉX
/// </summary>
struct RenderContext
{
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4			light_direction;
};
