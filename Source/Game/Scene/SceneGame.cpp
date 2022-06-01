//**********************************************************
//
//		SceneGameクラス
//
//**********************************************************

#include "Game/Scene/SceneGame.h"
#include "Engine/Systems/Logger.h"

#include "Engine/AI/DungeonMake.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Logger.h"


#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/LineRenderer.h"
#include "Engine/Systems/RenderContext.h"
#include "Game/Stage/RogueLikeStage.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/StageManager.h"

#include"Game/Characters/Player.h"
#include "Game/Characters/EnemySnake.h"

#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"

SceneGame::~SceneGame()
{
	// ステージ終了化
	StageManager::Instance().Clear();

	// キャラクター終了化
	CharacterManager::Instance().Clear();

	LOG(" Executed : SceneGame's destructor\n")
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
	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	rogue_like_dungeon.MakeDungeon();

	// ステージ初期化
	StageManager& stage_manager = StageManager::Instance();
	RogueLikeStage* rogue_like_stage = new RogueLikeStage();
	stage_manager.Register(rogue_like_stage);

	
	{
		//	 プレイヤー
		CharacterManager::Instance().Register(new Player(), static_cast<int>(Identity::Player));
		// 敵
		CharacterManager::Instance().Register(new EnemySnake(), static_cast<int>(Identity::Enemy));
		Meta& meta = Meta::Instance();

		meta.SendMessaging(static_cast<int>(Identity::Meta), static_cast<int>(Identity::CharacterManager), MESSAGE_TYPE::END_ENEMY_TURN);
	}

	//生成されなかったオブジェクトをマップデータから消す
	rogue_like_dungeon.UpdateMapRole();

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
	Meta& meta = Meta::Instance();

	//	カメラをアクティブにする
	camera.ActivateCamera();

	// カメラコントローラー更新処理
	camera_controller->FollowCameraUpdate(elapsed_time);

	//メタAIの更新
	meta.Update();

	// ステージ更新処理
	StageManager::Instance().Update(elapsed_time);

	// キャラクター更新処理
	CharacterManager::Instance().Update(elapsed_time);

	//マップ情報の更新
	 RogueLikeDungeon::Instance().UpdateMapRole();

	const GamePad& game_pad = Input::Instance().GetGamePad();

	// Aボタン(Zｷｰ)を押したらタイトルシーンへ切り替え
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
	// Bボタン(Xｷｰ)を押したら 再ロード
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
	// Startボタン(Nｷｰ)を押したら 敵の削除
	//if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_START))
	//{
	////	CharacterManager::Instance().Remove(CharacterManager::Instance().GetCharacterFromId(static_cast<int>(Identity::Enemy)));
	//}
}

void SceneGame::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	const FLOAT clear_color[] = { 0.0f, 0.5f, 0.5f, 0.5f };	// RGBA(0.0～1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// 描画処理
	RenderContext render_context{};
	render_context.light_direction = { 0.0f, 1.0f, 0.f, 0.0f };	// ライト方向（下方向）

	// カメラパラメータ設定
	const Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// 3Dモデル描画
	{	// ステージ描画
		ShaderManager* shader_manager = graphics.GetShaderManager();

		 std::shared_ptr<Shader> shader = shader_manager->GetShader(ShaderManager::ShaderName::Lambert);

		shader->Activate(device_context, render_context);
		{
			// ステージ描画
			StageManager::Instance().Render(device_context, shader);
			// キャラクター描画
			CharacterManager::Instance().Render(device_context, shader);
		}
		shader->Deactivate(device_context);

	}
	// 3Dエフェクト描画
	{

	}
	// 2Dスプライト描画
	{

	}
#if defined(DEBUG) | defined(_DEBUG)
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

	{

		CharacterManager::Instance().DrawDebugGUI();
		camera_controller->DrawDebugGUI();
		StageManager::Instance().DrawDebugGUI();
		RogueLikeDungeon::Instance().DrawDebugGUI();
		Meta::Instance().DrawDebugGUI();
	}
#endif
}

void SceneGame::ClearFloor()
{
	StageManager::Instance().Clear();
	//CharacterManager::Instance().RemoveEnemy();
}

void SceneGame::NextFloor()
{
	//ダンジョン生成初期化
	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	rogue_like_dungeon.MakeDungeon();

	// ステージ初期化
	StageManager& stage_manager = StageManager::Instance();
	RogueLikeStage* rogue_like_stage = new RogueLikeStage();
	stage_manager.Register(rogue_like_stage);

	// キャラクター生成処理
	{
		DirectX::XMFLOAT3 player_pos = { static_cast<float>(rogue_like_dungeon.player_pos.x) * CellSize, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CellSize };
		CharacterManager::Instance().GetPlayer()->SetPosition(player_pos);

	}

	//生成されなかったオブジェクトをマップデータから消す
	rogue_like_dungeon.UpdateMapRole();

}
bool SceneGame::OnMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		LOG(" Error : No Function | SceneGame.cpp   MESSAGE_TYPE::END_PLAYER_TURN\n")

		return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:

		LOG(" Error : No Function | SceneGame.cpp  MESSAGE_TYPE::END_ENEMY_TURN\n")

		return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		ClearFloor();
		NextFloor();

		return true;

	default: ;
	}

	return false;
}

void SceneGame::SendMessaging(MESSAGE_TYPE msg)
{
	LOG(" Error :  No Function | SceneGame::SendMessaging\n")
}
