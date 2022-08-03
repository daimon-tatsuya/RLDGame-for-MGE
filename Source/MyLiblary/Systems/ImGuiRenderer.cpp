//**********************************************************
//
//	ImGuiRenderer�N���X
//
//**********************************************************

#include <cstdio>
#include <memory>

#include "Liblary/Systems/Misc.h"
#include "Liblary/Systems/ImGuiRenderer.h"

ImGuiRenderer::ImGuiRenderer(HWND hWnd, ID3D11Device* device)
	:hWnd(hWnd)
{
	// ImGui�R���e�L�X�g�̐ݒ�
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("Assets\\fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//io.Fonts->AddFontDefault();
	//io.Fonts->Build();

	// ImGui�X�^�C���̐ݒ�
	ImGui::StyleColorsDark();
	// QueryPerformanceCounter(���g��)��b�ɕϊ�����B
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&ticks_per_second));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));

	// �o�b�N�G���h�@�\�̃t���O�ݒ�
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
																							  // �o�b�N�G���h��OS�̃J�[�\���`���ύX���邽�߂�GetMouseCursor()�̒l�𑸏d���邱�Ƃ��ł���?�i�C��)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
																							  // �o�b�N�G���h�́AOS�̃}�E�X�ʒu���Ĕz�u���邽�߂�io.WantSetMousePos���N�G�X�g�𑸏d���邱�Ƃ��ł��܂�?ConfigFlags::NavEnableSetMousePos���ݒ肳��Ă���ꍇ�̂ݎg�p����܂��B�i�C�ӁA�قƂ�ǎg�p���Ȃ�)
	io.BackendPlatformName = "ImGuiRenderer";
	io.ImeWindowHandle = hWnd;

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array that we will update during the application lifetime.
	// �L�[�{�[�h�̃}�b�s���O.ImGui�͂����̃C���f�b�N�X���g���āA�A�v���P�[�V�����̃��C�t�^�C�����ɍX�V�����io.KeysDown[]�z���`���܂��B
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Space] = VK_SPACE;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	// ���_�V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\ImGui_vs.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// ���_�V�F�[�_�[����
		HRESULT hr = device->CreateVertexShader(cso_data.get(), cso_size, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{"POSITION",	0,	DXGI_FORMAT_R32G32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{"TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
			{"COLOR",		0,	DXGI_FORMAT_R8G8B8A8_UNORM,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
		};
		hr = device->CreateInputLayout(input_element_desc, ARRAYSIZE(input_element_desc), cso_data.get(), cso_size, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	//�s�N�Z���V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shaders\\ImGui_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long cso_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> cso_data = std::make_unique<u_char[]>(cso_size);
		fread(cso_data.get(), cso_size, 1, fp);
		fclose(fp);

		// �s�N�Z���V�F�[�_�[����
		HRESULT hr = device->CreatePixelShader(cso_data.get(), cso_size, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �萔�o�b�t�@
	{
		D3D11_BUFFER_DESC desc = {};
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(ConstantBuffer);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �[�x�X�e���V���X�e�[�g
	{
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_tate.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ���X�^���C�U�[�X�e�[�g
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �T���v���X�e�[�g
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = device->CreateSamplerState(&desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// �t�H���g�e�N�X�`��
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// �e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
		{
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA subresource_data;
			subresource_data.pSysMem = pixels;
			subresource_data.SysMemPitch = desc.Width * 4;
			subresource_data.SysMemSlicePitch = 0;
			HRESULT hr = device->CreateTexture2D(&desc, &subresource_data, texture.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
		}

		// �V�F�[�_�[���\�[�X�r���[
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;
			HRESULT hr = device->CreateShaderResourceView(texture.Get(), &desc, shader_resource_view.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
		}

		// �e�N�X�`����n��
		io.Fonts->TexID = static_cast<ImTextureID>(shader_resource_view.Get());
	}
}

ImGuiRenderer::~ImGuiRenderer()
{
	ImGui::DestroyContext();
}

void ImGuiRenderer::NewFrame()
{
	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

	// �\���T�C�Y�̐ݒ�i�E�B���h�E�̃T�C�Y�ύX�ɍ��킹��1�t���[�����Ƃɐݒ�)
	RECT rect;
	::GetClientRect(hWnd, &rect);
	io.DisplaySize = ImVec2(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));

	// ���Ԃ̐ݒ�
	INT64 current_time;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&current_time));
	io.DeltaTime = static_cast<float>(current_time - time) / ticks_per_second;
	time = current_time;

	//�L�[�{�[�h�ҏW�̓��͂�ǂݎ��
	io.KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;

	// OS�̃}�E�X�|�W�V�����̍X�V
	UpdateMousePos();

	// OS�̃}�E�X�J�[�\����imgui���v������J�[�\���ɍX�V����
	ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
	if (last_mouse_cursor != mouse_cursor)
	{
		last_mouse_cursor = mouse_cursor;
		UpdateMouseCursor();
	}

	ImGui::NewFrame();
}

void ImGuiRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	ImGui::Render();

	ImDrawData* draw_data = ImGui::GetDrawData();

	// ���_�o�b�t�@�\�z
	if (vertex_buffer == nullptr || vertex_count < draw_data->TotalVtxCount)
	{
		vertex_buffer.Reset();
		vertex_count = draw_data->TotalVtxCount + 5000;

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(ImDrawVert) * vertex_count;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(ImDrawVert);

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		deviceContext->GetDevice(device.GetAddressOf());
		HRESULT hr = device->CreateBuffer(&desc, nullptr, vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	//�C���f�b�N�X�o�b�t�@
	if (index_buffer == nullptr || index_count < draw_data->TotalIdxCount)
	{
		index_buffer.Reset();
		index_count = draw_data->TotalIdxCount + 10000;

		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = sizeof(ImDrawIdx) * index_count;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(ImDrawIdx);

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		deviceContext->GetDevice(device.GetAddressOf());
		HRESULT hr = device->CreateBuffer(&desc, nullptr, index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
	// �萔�o�b�t�@�X�V
	{
		ConstantBuffer data{};

		float left = draw_data->DisplayPos.x;
		float right = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
		float top = draw_data->DisplayPos.y;
		float bottom = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
		data.wvp._11 = 2.0f / (right - left);
		data.wvp._12 = 0.0f;
		data.wvp._13 = 0.0f;
		data.wvp._14 = 0.0f;
		data.wvp._21 = 0.0f;
		data.wvp._22 = 2.0f / (top - bottom);
		data.wvp._23 = 0.0f;
		data.wvp._24 = 0.0f;
		data.wvp._31 = 0.0f;
		data.wvp._32 = 0.0f;
		data.wvp._33 = 0.5f;
		data.wvp._34 = 0.0f;
		data.wvp._41 = (right + left) / (left - right);
		data.wvp._42 = (top + bottom) / (bottom - top);
		data.wvp._43 = 0.5f;
		data.wvp._44 = 1.0f;

		deviceContext->UpdateSubresource(constant_buffer.Get(), 0, nullptr, &data, 0, 0);
	}

	// �`��X�e�[�g�ݒ�
	{
		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT viewPort = {};
		viewPort.Width = draw_data->DisplaySize.x;
		viewPort.Height = draw_data->DisplaySize.y;
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = viewPort.TopLeftY = 0;
		deviceContext->RSSetViewports(1, &viewPort);

		// �V�F�[�_�[
		deviceContext->VSSetShader(vertex_shader.Get(), nullptr, 0);
		deviceContext->PSSetShader(pixel_shader.Get(), nullptr, 0);
		deviceContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

		// ���_�o�b�t�@
		UINT stride = sizeof(ImDrawVert);
		UINT offset = 0;
		deviceContext->IASetInputLayout(input_layout.Get());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(index_buffer.Get(), sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);

		// �X�e�[�g
		const float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetBlendState(blend_state.Get(), blend_factor, 0xFFFFFFFF);
		deviceContext->OMSetDepthStencilState(depth_stencil_tate.Get(), 0);
		deviceContext->RSSetState(rasterizer_state.Get());
		deviceContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
	}

	// ���_�f�[�^�ςݍ���
	{
		D3D11_MAPPED_SUBRESOURCE mapped_VB, mapped_IB;
		HRESULT hr = deviceContext->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_VB);
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		hr = deviceContext->Map(index_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_IB);
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		ImDrawVert* dst_vertex = static_cast<ImDrawVert*>(mapped_VB.pData);
		ImDrawIdx* dst_index = static_cast<ImDrawIdx*>(mapped_IB.pData);
		for (int i = 0; i < draw_data->CmdListsCount; i++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[i];
			::memcpy(dst_vertex, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
			::memcpy(dst_index, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

			dst_vertex += cmd_list->VtxBuffer.Size;
			dst_index += cmd_list->IdxBuffer.Size;
		}

		deviceContext->Unmap(vertex_buffer.Get(), 0);
		deviceContext->Unmap(index_buffer.Get(), 0);
	}

	// �`��
	{
		int global_idx_offset = 0;
		int global_vtx_offset = 0;
		ImVec2 clip_off = draw_data->DisplayPos;
		for (int i = 0; i < draw_data->CmdListsCount; i++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[i];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback != NULL)
				{
					//User callback, registered via ImDrawList::AddCallback()
					//(ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
					//(ImDrawCallback_ResetRenderState�́A���[�U�[�������_���[�Ƀ����_�����O��Ԃ̃��Z�b�g��v�����邽�߂Ɏg�p������ʂȃR�[���o�b�N�l�ł�)�B
					if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
					{
						//SetRenderState(context, drawData);
					}
					else
					{
						pcmd->UserCallback(cmd_list, pcmd);
					}
				}
				else
				{
					// Apply scissor/clipping rectangle
					// �V�U�[/�N���b�s���O�E���N�^���O���̓K�p
					const D3D11_RECT r = { static_cast<LONG>(pcmd->ClipRect.x - clip_off.x), static_cast<LONG>(pcmd->ClipRect.y - clip_off.y), static_cast<LONG>(pcmd->ClipRect.z - clip_off.x), static_cast<LONG>(pcmd->ClipRect.w - clip_off.y) };
					deviceContext->RSSetScissorRects(1, &r);

					// Bind texture, Draw
					// �e�N�X�`���[�̃o�C���h�A�h���[
					ID3D11ShaderResourceView* srv = static_cast<ID3D11ShaderResourceView*>(pcmd->TextureId);
					deviceContext->PSSetShaderResources(0, 1, &srv);
					deviceContext->DrawIndexed(pcmd->ElemCount, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset);
				}
			}
			global_idx_offset += cmd_list->IdxBuffer.Size;
			global_vtx_offset += cmd_list->VtxBuffer.Size;
		}
		ID3D11ShaderResourceView* nullSRV = nullptr;
		deviceContext->PSSetShaderResources(0, 1, &nullSRV);
	}
}
// WIN32���b�Z�[�W�n���h���[
LRESULT ImGuiRenderer::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() == nullptr)
	{
		return 0;
	}

	ImGuiIO& io = ImGui::GetIO();
	switch (msg)
	{
	case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
	case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
	{
		int button = 0;
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
		{
			button = 0;
		}
		if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
		{
			button = 1;
		}
		if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK)
		{
			button = 2;
		}
		if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK)
		{
			button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
		}

		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == nullptr)
		{
			::SetCapture(hWnd);
		}
		io.MouseDown[button] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		int button = 0;
		if (msg == WM_LBUTTONUP)
		{
			button = 0;
		}
		if (msg == WM_RBUTTONUP)
		{
			button = 1;
		}
		if (msg == WM_MBUTTONUP)
		{
			button = 2;
		}
		if (msg == WM_XBUTTONUP)
		{
			button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
		}
		io.MouseDown[button] = false;
		if (!ImGui::IsAnyMouseDown() && ::GetCapture() == hWnd)
		{
			::ReleaseCapture();
		}
		return 0;
	}
	case WM_MOUSEWHEEL:
		io.MouseWheel += static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
		return 0;
	case WM_MOUSEHWHEEL:
		io.MouseWheelH += static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
		return 0;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return 0;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return 0;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		// ToAscii()+GetKeyboardState()���g���ĕ��������o�����Ƃ��ł��܂��B
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacterUTF16(static_cast<unsigned short>(wParam));
		return 0;
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT && UpdateMouseCursor())
			return 1;
		return 0;
	}
	return 0;
}

bool ImGuiRenderer::UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		return false;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
	{
		//Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		// imgui ���}�E�X�J�[�\����`�悵�Ă���ꍇ��A�J�[�\�����Ȃ����Ƃ�]��ł���ꍇ�� OS �̃}�E�X�J�[�\�����B��
		::SetCursor(nullptr);
	}
	else
	{
		//Show OS mouse cursor
		// OS�̃}�E�X�J�[�\����\������
		LPTSTR win32_cursor = IDC_ARROW;
		switch (imgui_cursor)
		{
		case ImGuiMouseCursor_Arrow:
			win32_cursor = IDC_ARROW;
			break;

		case ImGuiMouseCursor_TextInput:
			win32_cursor = IDC_IBEAM;
			break;

		case ImGuiMouseCursor_ResizeAll:
			win32_cursor = IDC_SIZEALL;
			break;

		case ImGuiMouseCursor_ResizeEW:
			win32_cursor = IDC_SIZEWE;
			break;

		case ImGuiMouseCursor_ResizeNS:
			win32_cursor = IDC_SIZENS;
			break;

		case ImGuiMouseCursor_ResizeNESW:
			win32_cursor = IDC_SIZENESW;
			break;

		case ImGuiMouseCursor_ResizeNWSE:
			win32_cursor = IDC_SIZENWSE;
			break;

		case ImGuiMouseCursor_Hand:
			win32_cursor = IDC_HAND;
			break;

		case ImGuiMouseCursor_NotAllowed:
			win32_cursor = IDC_NO;
			break;
		}

		::SetCursor(::LoadCursor(nullptr, win32_cursor));
	}
	return true;
}

void ImGuiRenderer::UpdateMousePos()
{
	ImGuiIO& io = ImGui::GetIO();

	// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
	//  �v���ɉ����� OS �̃}�E�X�̈ʒu��ݒ肷��i�قƂ�ǎg�p����܂���B���[�U�[�ɂ���� ImGuiConfigFlags_NavEnableSetMousePos ���L���ɂȂ��Ă���ꍇ�̂݁j�B
	if (io.WantSetMousePos)
	{
		POINT pos = { static_cast<int>(io.MousePos.x), static_cast<int>(io.MousePos.y) };
		::ClientToScreen(hWnd, &pos);
		::SetCursorPos(pos.x, pos.y);
	}

	// Set mouse position
	//  �}�E�X�|�W�V�����̐ݒ�
	io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	POINT pos;
	if (HWND active_window = ::GetForegroundWindow())
		if (active_window == hWnd || ::IsChild(active_window, hWnd))
			if (::GetCursorPos(&pos) && ::ScreenToClient(hWnd, &pos))
				io.MousePos = ImVec2(static_cast<float>(pos.x), static_cast<float>(pos.y));
}