//**********************************************************
//
//		SceneGameクラス
//
//**********************************************************

#include "Game/Scene/SceneGame.h"

#include "Engine/AI/DungeonMake.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/LineRenderer.h"
#include "Engine/Systems/RenderContext.h"
#include "Engine/Systems/RogueLikeStage.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/StageManager.h"
#include"Game/Characters/Player.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"

SceneGame::~SceneGame()
{
	// ステージ終了化
	StageManager::Instance().Clear();
	// キャラクター終了化
	CharacterManager::Instance().Clear();

}

void SceneGame::Initialize()
{
	// カメラ初期設定
	const Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0));

	//camera.SetPerspectiveFov(
	//	DirectX::XMConvertToRadians(45),
	//	graphics.GetScreenWidth() / graphics.GetScreenHeight(),
	//	0.1f,
	//	1000.0f
	//);

	//平衡投影カメラ
	camera.SetOrthoFov(
		graphics.GetScreenWidth() / 30,
		graphics.GetScreenHeight() / 30,
		0.1f,
		100.f);


	// カメラコントローラー初期化
	camera_controller = std::make_unique<CameraController>();


	//ダンジョン生成初期化
	RogueLikeDungeon rogue_like_dungeon;
	//struct DungeonMapRole dungeon_map_role;
	//rogue_like_dungeon.MapReMake(&dungeon_map_role);
	rogue_like_dungeon.MapReMake();
	storage_dungeon = rogue_like_dungeon;

	// ステージ初期化
	StageManager& stage_manager = StageManager::Instance();
	RogueLikeStage* rogue_like_stage = new RogueLikeStage(&storage_dungeon);
	stage_manager.Register(rogue_like_stage);



	// キャラクター生成処理
	{
		CharacterManager& character_manager = CharacterManager::Instance();
		//	 プレイヤー
		player = std::make_unique<Player>(&storage_dungeon);

		character_manager.Register(player.get(), static_cast<int>(Meta::Identity::Player));

		//	// エネミー初期化
		//	for (int i = 0; i < 3; ++i)
		//	{
		//		EnemySlime* slime = new EnemySlime();
		//		slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		//		characterManager.Register(slime);
		//	}
	}

	//生成されなかったオブジェクトをマップデータから消す
	storage_dungeon.UpdateMapRolePlayer();
	storage_dungeon.UpdateMapRoleEnemis();

	//視錐台カリング用のAABBの初期設定
	//axis_aligned_bounding_box_for_frustum.clear();
	//for (int array_index = 0; array_index < 16; array_index++)
	//{
	//	for (int z = 0; z < 4; z++)
	//	{
	//		for (int x = 0; x < 4; x++)
	//		{
	//			AxisAlignedBoundingBox* aabb=new AxisAlignedBoundingBox;
	//			aabb->center = DirectX::XMFLOAT3(16.f*x, 2.f, 16.f*z);
	//			aabb->radius = DirectX::XMFLOAT3(16.f*x, 2.f, 16.f*z);
	//			axis_aligned_bounding_box_for_frustum.emplace_back(aabb);
	//		}
	//	}
	//}

}

void SceneGame::Finalize()
{
}

void SceneGame::Update(const float elapsed_time)
{
	Camera& camera = Camera::Instance();

	camera.ActivateCamera();
	// カメラコントローラー更新処理
	camera_controller->FollowCameraUpdate(elapsed_time);

	// ステージ更新処理
	StageManager::Instance().Update(elapsed_time);

	// キャラクター更新処理
	CharacterManager::Instance().Update(elapsed_time);

	const GamePad& game_pad = Input::Instance().GetGamePad();

	storage_dungeon.UpdateMapRolePlayer();
	storage_dungeon.UpdateMapRoleEnemis();

	// Aボタンを押したらタイトルシーンへ切り替え
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}

}

void SceneGame::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	constexpr FLOAT clear_color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// 描画処理
	RenderContext render_context{};
	render_context.light_direction = { -0.5f, -1.0f, -0.5f, 0.0f };	// ライト方向（下方向）

	// カメラパラメータ設定
	const Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// 3Dモデル描画
	{	// ステージ描画
		ShaderManager* shader_manager = graphics.GetShaderManager();

		const std::shared_ptr<Shader> shader = shader_manager->GetShader(ShaderManager::ShaderName::NoTexture);

		shader->Activate(device_context, render_context);
		{
			// ステージ描画
			StageManager::Instance().Render(device_context, shader);

			// キャラクター描画
			CharacterManager::Instance().Render(device_context, shader);

			//for (int array_index = 0; array_index < 16; array_index++)
			//{
			//	const bool  for_debug = camera_controller->IntersectFrustum(axis_aligned_bounding_box_for_frustum[array_index]);
				//if (for_debug)
				//{
					// ステージ描画
					//StageManager::Instance().Render(device_context, shader);
					// キャラクター描画
					//CharacterManager::Instance().Render(device_context, shader);
			//	}
			//}
		}
		shader->Deactivate(device_context);
	}
	// 3Dエフェクト描画
	{

	}
	// 2Dスプライト描画
	{
		// HUD更新

	}

	// 3Dデバッグ描画
	{
		// キャラクターデバッグ描画
		//CharacterManager::Instance().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(device_context, render_context.view, render_context.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(device_context, render_context.view, render_context.projection);
	}

	// 2DデバッグGUI描画
#if 1
	{
		CharacterManager::Instance().DrawDebugGUI();
		camera_controller->DrawDebugGUI();
		StageManager::Instance().DrawDebugGUI();
	}
#endif
}