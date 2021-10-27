#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/blender.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/DungeonMake.h"

#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"

#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/RogueLikeStage.h"
#include "Game/Stages/StageMain.h"

#include "Engine/Systems/SceneManager.h"
#include "Game/Scene/SceneGame.h"
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneLoading.h"

SceneGame::SceneGame()
{
}

SceneGame::~SceneGame()
{

	// �L�����N�^�[�I����
	CharacterManager::Instance().Clear();
	// �X�e�[�W�I����
	StageManager::Instance().Clear();

}

void SceneGame::Initialize()
{
	// �J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0));

	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();


	//�_���W��������������
	RogueLikeDungeon rogue_like_dungeon;
	struct DungeonMapRole dungeon_map_role;
	rogue_like_dungeon.MapReMake(&dungeon_map_role);

	// �X�e�[�W������
	StageManager& stage_manager = StageManager::Instance();
	RogueLikeStage* rogue_like_stage = new RogueLikeStage(rogue_like_dungeon);
	stage_manager.Register(rogue_like_stage);

	// �L�����N�^�[��������
	CharacterManager& character_manager = CharacterManager::Instance();
	{
		// �v���C���[
		player = std::make_unique<Player>(rogue_like_dungeon);
		//player = new Player();
		character_manager.Register(player.get(), static_cast<int>(Meta::Identity::Player));

		//// �G�l�~�[������
		//for (int i = 0; i < 3; ++i)
		//{
		//	EnemySlime* slime = new EnemySlime();
		//	slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
		//	characterManager.Register(slime);
		//}
	}

}

void SceneGame::Finalize()
{
}

void SceneGame::Update(float elapsed_time)
{
	// �J�����R���g���[���[�X�V����
	camera_controller->Update(elapsed_time);

	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsed_time);



	// �L�����N�^�[�X�V����
	CharacterManager::Instance().Update(elapsed_time);

	GamePad& game_pad = Input::Instance().GetGamePad();

	// �Ȃɂ��{�^�����������烍�[�f�B���O�V�[���֐؂�ւ��ɂ񂵂�����
	//const GamePadButton any_button = GamePad::BTN_LEFT_SHOULDER;
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
}

void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	device_context->ClearRenderTargetView(render_target_view, color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// �`�揈��
	RenderContext render_context;
	render_context.lightDirection = { -0.5f, -1.0f, -0.5f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// 3D���f���`��
	{	// �X�e�[�W�`��
		ShaderManager* shader_manager = graphics.GetShaderManager();

		std::shared_ptr<Shader> shader = shader_manager->GetShader(ShaderManager::ShaderName::NoTexture);

		shader->Activate(device_context, render_context);

		// �X�e�[�W�`��
		StageManager::Instance().Render(device_context, shader);

		// �L�����N�^�[�`��
		CharacterManager::Instance().Render(device_context, shader);

		shader->Deactivate(device_context);
	}
	// 3D�G�t�F�N�g�`��
	{
	}
	// 2D�X�v���C�g�`��
	{
		// HUD�X�V
	}

	// 3D�f�o�b�O�`��
	{
		// �L�����N�^�[�f�o�b�O�`��
		//CharacterManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(device_context, render_context.view, render_context.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(device_context, render_context.view, render_context.projection);
	}

	// 2D�f�o�b�OGUI�`��
#if 1
	{
		CharacterManager::Instance().DrawDebugGUI();
		camera_controller->DrawDebugGUI();
		StageManager::Instance().DrawDebugGUI();
	}
#endif
}