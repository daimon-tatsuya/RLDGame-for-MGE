//**********************************************************
//
//		Blender.cpp
//
//**********************************************************

#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/Blender.h"
#include "Liblary/Systems/Misc.h"
#include "Liblary/Systems/Logger.h"
#include <assert.h>

Blender::Blender(ID3D11Device* device)
{
	D3D11_BLEND_DESC blend_desc{};

	for (int state = 0; state < static_cast<u_int>(BlendState::End); state++)
	{
		switch (state)
		{
		case static_cast<u_int>(BlendState::None):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = FALSE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//-- Settings for Alpha
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created None blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Alpha):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Alpha blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Add):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Add blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Subtract):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Subtract blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Replace):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Replace blend state | Blender.cpp constructor\n")
			break
				;
		case static_cast<u_int>(BlendState::Multiply):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Multiply blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Lighten):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Lighten blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Darken):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Darken blend state | Blender.cpp constructor\n")
			break;

		case static_cast<u_int>(BlendState::Screen):
			blend_desc = {};
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.AlphaToCoverageEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
			//-- Settings for RGB --//
			blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			//-- Settings for Alpha --//
			blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			LOG("Success :  Created Screen blend state | Blender.cpp constructor\n")
			break;

		}
		HRESULT hr = device->CreateBlendState(&blend_desc, blend_states[state].GetAddressOf());
		if (FAILED(hr))
			assert(!"CreateBlendState error");
		else
		{
			LOG("Success : Created  blend states | Blender.cpp constructor\n")
		}
	}
}


void Blender::Activate(ID3D11DeviceContext* device_context, BlendState state)
{
	if (current_state == state)//同じならスキップ
	{
		LOG("Executed : This state is the same as the current state | Blender.cpp Activate Method\n")
		return;
	}

	current_state = state;
	device_context->OMSetBlendState(blend_states[static_cast<u_int>(current_state)].Get(), nullptr, 0xffffffff);
	LOG("Success :  Activated a blend state | Blender.cpp Activate Method\n")
}
