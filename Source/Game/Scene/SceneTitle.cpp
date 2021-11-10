#include "Engine/Systems/Graphics.h"
#include "Game/Scene/SceneTitle.h"
#include "Engine/Systems/blender.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneGame.h"
SceneTitle::SceneTitle()
{
}
SceneTitle::~SceneTitle()
{
}
void SceneTitle::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	strcpy_s(message, "Title");
}

void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// なにかボタンを押したらローディングシーンへ切り替え
	if (gamePad.GetButtonDown() & static_cast<GamePadButton>(GamePad::AnyBTN))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
}

void SceneTitle::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// 描画処理
	RenderContext render_context;
	render_context.lightDirection = { -0.5f, -1.0f, -0.5f, 0.0f };	// ライト方向（下方向）

	//// 3Dモデル描画
	//{
	//	// ワールド行列を作成
	//	DirectX::XMMATRIX W;
	//	{
	//		static float rx = 0;
	//		static float ry = 0;
	//		//rx += DirectX::XMConvertToRadians(0.25f);	// 角度をラジアン(θ)に変換
	//		//ry += DirectX::XMConvertToRadians(0.5f);	// 角度をラジアン(θ)に変換

	//		DirectX::XMFLOAT3 scale(0.5f, 0.5f, 0.5f);
	//		DirectX::XMFLOAT3 rotate(rx, ry, 0);
	//		DirectX::XMFLOAT3 translate(0, -20, 0);

	//		DirectX::XMMATRIX S, R, T;
	//		S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//		R = DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);	// ZXY回転
	//		T = DirectX::XMMatrixTranslation(translate.x, translate.y, translate.z);

	//		W = S * R * T;
	//	}

	//	// ビュー行列を作成
	//	DirectX::XMMATRIX V;
	//	{
	//		static CameraController camera;
	//		camera.Update();

	//		// カメラの設定
	//		DirectX::XMVECTOR eye, focus, up;
	//		eye = DirectX::XMVectorSet(camera.GetEye().x, camera.GetEye().y, camera.GetEye().z, 1.0f);
	//		focus = DirectX::XMVectorSet(camera.GetFocus().x, camera.GetFocus().y, camera.GetFocus().z, 1.0f);
	//		up = DirectX::XMVectorSet(camera.GetUp().x, camera.GetUp().y, camera.GetUp().z, 1.0f);

	//		V = DirectX::XMMatrixLookAtRH(eye, focus, up);
	//	}

	//	// プロジェクション行列を作成
	//	DirectX::XMMATRIX P;
	//	{
	//		// 画面サイズ取得のためビューポートを取得
	//		D3D11_VIEWPORT viewport;
	//		UINT num_viewports = 1;
	//		device_context->RSGetViewports(&num_viewports, &viewport);

	//		// 角度をラジアン(θ)に変換
	//		float fov_y = 30 * 0.01745f;	// 画角
	//		float aspect = viewport.Width / viewport.Height;	// 画面比率
	//		float nearZ = 0.1f;	// 表示最近面までの距離
	//		float farZ = 1000.0f;	// 表示最遠面までの距離

	//		P = DirectX::XMMatrixPerspectiveFovRH(fov_y, aspect, nearZ, farZ);
	//	}

	//	// ビュー行列、プロジェクション行列を合成し行列データを取り出す。
	//	DirectX::XMFLOAT4X4 viewProjection;
	//	{
	//		DirectX::XMMATRIX VP;
	//		VP = V * P;
	//		DirectX::XMStoreFloat4x4(&viewProjection, VP);
	//		DirectX::XMStoreFloat4x4(&render_context.projection, P);
	//		DirectX::XMStoreFloat4x4(&render_context.view, V);
	//	}

	//	ShaderManager* shader_manager = graphics.GetShaderManager();
	//	std::shared_ptr<Shader> shader = shader_manager->GetShader(ShaderManager::ShaderName::Lambert);

	//	shader->Activate(device_context,
	//		render_context
	//	);

	//	shader->Deactivate(device_context);
	//}
	// 2Dスプライト描画
	{
	}

	// 3Dエフェクト描画
	{
	}

	// 3Dデバッグ描画
	{
		// キャラクターデバッグ描画
		//CharacterManager::Instance().DrawDebugPrimitive();

		//// ラインレンダラ描画実行
		//graphics.GetLineRenderer()->Render(device_context, render_context.view, render_context.projection);

		//// デバッグレンダラ描画実行
		//graphics.GetDebugRenderer()->Render(device_context, render_context.view, render_context.projection);
	}

	// 2Dスプライト描画
	{
		// HUD更新
	//	headUpDisplay->Render(dc);

		Sprite* font = graphics.GetFont();

		// 「Title」を描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = 32.f;// static_cast<float>(font->GetTextureWidth());
		float textureHeight = 32.f;// static_cast<float>(font->GetTextureHeight());
		float positionX = screenWidth / 3;// - textureWidth;// * (textureWidth / 2);
		float positionY =( screenHeight / 2 )- textureHeight;

		font->TextOutW(device_context, message, positionX, positionY, 128, 128);
	}

	// 2DデバッグGUI描画
	{
		CharacterManager::Instance().DrawDebugGUI();
	}

	//レンダーターゲットを変えて色々するやつ
	//frame_buffer[0]->Clear(immediateContext.Get());
	//frame_buffer[0]->Activate(immediateContext.Get());
	//paintar_fullsdreen->blit(immediateContext.Get(), true, true, false);
	////fbx_mesh->Render(immediateContext.Get(), world_view_projection, world, light_direction, material_color, wireframe, elapsedTime);
	//frame_buffer[0]->Deactivate(immediateContext.Get());
	//immediateContext->PSSetShaderResources(0, 1, frame_buffer[0]->render_target_shader_resource_view.GetAddressOf());
	//paintar_fullsdreen->blit(immediateContext.Get(), true, true, true);
}

void SceneTitle::Finalize()
{
}