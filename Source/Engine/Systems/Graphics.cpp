//**********************************************************
//
//		Graphicsクラス
//
//**********************************************************

#include "Engine/Systems/Graphics.h"
#include "Engine//Systems/Misc.h"
#include "Engine/Objects/Sprite.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/LineRenderer.h"
#include "Engine/Systems/ShaderManager.h"
//シェーダー
#include "Engine/Systems/LambertShader.h"
#include "Engine/Systems/NoTextureShader.h"
#include "Engine/Systems/WireframeLambert.h"

Graphics* Graphics::instance = nullptr;

Graphics::Graphics(HWND hWnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	HRESULT hr = S_OK;

	// 画面のサイズを取得する。
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screen_width = rc.right - rc.left;
	UINT screen_height = rc.bottom - rc.top;

	this->screen_width = static_cast<float>(screen_width);
	this->screen_height = static_cast<float>(screen_height);
	// デバイス＆スワップチェーンの生成
	{
		UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// スワップチェーンを作成するための設定オプション
		DXGI_SWAP_CHAIN_DESC swapchain_desc;
		{
			swapchain_desc.BufferDesc.Width = screen_width;
			swapchain_desc.BufferDesc.Height = screen_height;
			swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
			swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
			swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 1ピクセルあたりの各色(RGBA)を8bit(0～255)のテクスチャ(バックバッファ)を作成する。
			swapchain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchain_desc.BufferCount = 1;			// バックバッファの数
			swapchain_desc.OutputWindow = hWnd;	// DirectXで描いた画を表示するウインドウ
			swapchain_desc.Windowed = true;			// ウインドウモードか、フルスクリーンにするか。
			swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchain_desc.Flags = 0;						// DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		}

		D3D_FEATURE_LEVEL feature_level;

		// デバイス＆スワップチェーンの生成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,											// どのビデオアダプタを使用するか？既定ならばnullptrで、IDXGIAdapterのアドレスを渡す。
			D3D_DRIVER_TYPE_HARDWARE,	// ドライバのタイプを渡す。D3D_DRIVER_TYPE_HARDWARE 以外は基本的にソフトウェア実装で、特別なことをする場合に用いる。
			nullptr,											// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定した際に、その処理を行うDLLのハンドルを渡す。それ以外を指定している際には必ずnullptrを渡す。
			createDeviceFlags,							// 何らかのフラグを指定する。詳しくはD3D11_CREATE_DEVICE列挙型で検索。
			featureLevels,									// D3D_FEATURE_LEVEL列挙型の配列を与える。nullptrにすることでも上記featureと同等の内容の配列が使用される。
			ARRAYSIZE(featureLevels),				// featureLevels配列の要素数を渡す。
			D3D11_SDK_VERSION,					// SDKのバージョン。必ずこの値。
			&swapchain_desc,							// ここで設定した構造体に設定されているパラメータでSwapChainが作成される。
			swapchain.GetAddressOf(),				// 作成が成功した場合に、SwapChainのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でSwapChainを操作する。
			device.GetAddressOf(),						// 作成が成功した場合に、Deviceのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でDeviceを操作する。
			&feature_level,								// 作成に成功したD3D_FEATURE_LEVELを格納するためのD3D_FEATURE_LEVEL列挙型変数のアドレスを設定する。
			device_context.GetAddressOf()		// 作成が成功した場合に、Contextのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でContextを操作する。
		);
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// レンダーターゲットビューの生成
	{
		// スワップチェーンからバックバッファテクスチャを取得する。
		// !※スワップチェーンに内包されているバックバッファテクスチャは'色'を書き込むテクスチャ。
		Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
		hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// バックバッファテクスチャへの書き込みの窓口となるレンダーターゲットビューを生成する。
		hr = device->CreateRenderTargetView(back_buffer.Get(), nullptr, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// 深度ステンシルビューの生成
	{
		// 深度ステンシル情報を書き込むためのテクスチャを作成する。
		D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc;
		depth_stencil_buffer_desc.Width = screen_width;
		depth_stencil_buffer_desc.Height = screen_height;
		depth_stencil_buffer_desc.MipLevels = 1;
		depth_stencil_buffer_desc.ArraySize = 1;
		depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 1ピクセルあたり、深度情報を24Bit / ステンシル情報を8bitのテクスチャを作成する。
		depth_stencil_buffer_desc.SampleDesc.Count = 1;
		depth_stencil_buffer_desc.SampleDesc.Quality = 0;
		depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// 深度ステンシル用のテクスチャを作成する。
		depth_stencil_buffer_desc.CPUAccessFlags = 0;
		depth_stencil_buffer_desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depth_stencil_buffer_desc, nullptr, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));

		// 深度ステンシルテクスチャへの書き込みに窓口になる深度ステンシルビューを作成する。
		hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, depth_stencil_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// ビューポートの設定
	{
		// 画面のどの領域にDirectXで描いた画を表示するかの設定。
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screen_width);
		viewport.Height = static_cast<float>(screen_height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		device_context->RSSetViewports(1, &viewport);
	}

	// シェーダー
	{
		shader_manager = std::make_unique<ShaderManager>();

		// Lambert
		lambert_shader = std::make_shared<LambertShader>(device.Get());
		shader_manager->AddShader(ShaderManager::ShaderName::Lambert, lambert_shader);

		// ワイヤーフレーム化したLambert
		// ラスタライザー関係をクラス化した際に削除予定
		wireframe_lambert_shader = std::make_shared<WireLambertShader>(device.Get());
		shader_manager->AddShader(ShaderManager::ShaderName::LambertWireFrame, wireframe_lambert_shader);

		// テクスチャのないモデル用
		no_texture_shader = std::make_shared<NoTextureShader>(device.Get());
		shader_manager->AddShader(ShaderManager::ShaderName::NoTexture, no_texture_shader);
	}

	// レンダラー
	{
		debug_renderer = std::make_unique<DebugRenderer>(device.Get());
		line_renderer = std::make_unique<LineRenderer>(device.Get(), 1024);
		imgui_renderer = std::make_unique<ImGuiRenderer>(hWnd, device.Get());
	}
	// フォント
	{
		font = std::make_unique<Sprite>(L"Assets/fonts/font0.png");
	}
}

// デストラクタ
Graphics::~Graphics()
{
	swapchain->SetFullscreenState(false,nullptr);
}