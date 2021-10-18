#pragma once

#include <d3d11.h>
#include <wrl.h>

class Framebuffer
{
public:
	Framebuffer(ID3D11Device* device, int width, int height, bool generateMips);
	virtual ~Framebuffer() = default;
	Framebuffer(Framebuffer&) = delete;
	Framebuffer& operator=(Framebuffer&) = delete;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> render_target_shader_resource_view;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depth_stencil_shader_resource_view;

	D3D11_VIEWPORT viewport = {};

	void Clear(ID3D11DeviceContext* immediateContext, float r = 0, float g = 0, float b = 0, float a = 1, unsigned int clearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float depth = 1, unsigned char stencil = 0)
	{
		float colour[4] = { r, g, b, a };
		if (render_target_view)
		{
			immediateContext->ClearRenderTargetView(render_target_view.Get(), colour);
		}
		if (depth_stencil_view)
		{
			immediateContext->ClearDepthStencilView(depth_stencil_view.Get(), clearFlags, depth, stencil);
		}
	}
	void Activate(ID3D11DeviceContext* immediateContext)
	{
		number_of_stored_viewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		immediateContext->RSGetViewports(&number_of_stored_viewports, default_viewports);
		immediateContext->RSSetViewports(1, &viewport);

		immediateContext->OMGetRenderTargets(1, default_render_target_view.ReleaseAndGetAddressOf(), default_depth_stencil_view.ReleaseAndGetAddressOf());
		immediateContext->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	}
	void Deactivate(ID3D11DeviceContext* immediateContext)
	{
		immediateContext->RSSetViewports(number_of_stored_viewports, default_viewports);
		immediateContext->OMSetRenderTargets(1, default_render_target_view.GetAddressOf(), default_depth_stencil_view.Get());
	}
private:
	unsigned int number_of_stored_viewports = 0;
	D3D11_VIEWPORT default_viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> default_render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> default_depth_stencil_view;
};
