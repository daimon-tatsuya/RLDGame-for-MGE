//**********************************************************
//
//		SceneGame�N���X
//
//**********************************************************

#include "Game/Scene/SceneGame.h"

#include "Engine/AI/DungeonMake.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Objects/Sprite.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/LineRenderer.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/RenderContext.h"
#include "Engine/Systems/SceneManager.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/StageManager.h"
#include "Game/Characters/EnemySnake.h"
#include"Game/Characters/Player.h"
#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"
#include "Game/Scene/SceneGameOver.h"
#include "Game/Stage/RogueLikeStage.h"
#include "Engine/Systems/DungeonSystem.h"

//�f�X�g���N�^
SceneGame::~SceneGame()
{
	LOG(" Executed : SceneGame's destructor\n")
}

//������
void SceneGame::Initialize()
{
	// �J���������ݒ�
	const Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0));

	//�������e
	//camera.SetPerspectiveFov(
	//	DirectX::XMConvertToRadians(45),
	//	graphics.GetScreenWidth() / graphics.GetScreenHeight(),
	//	0.1f,
	//	1000.0f
	//);

	//���s���e�J����
	camera.SetOrthoFov(
		graphics.GetScreenWidth(),
		graphics.GetScreenHeight(),
		0.1f,
		100.f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();

	//�_���W��������������
	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	rogue_like_dungeon.MakeDungeon();

	// �X�e�[�W������
	StageManager& stage_manager = StageManager::Instance();
	RogueLikeStage* rogue_like_stage = new RogueLikeStage();
	stage_manager.Register(rogue_like_stage);


	{
		//	 �v���C���[
		CharacterManager::Instance().Register(new Player(), static_cast<int>(Identity::Player));
		// �G
		CharacterManager::Instance().Register(new EnemySnake(), static_cast<int>(Identity::Enemy));
		Meta& meta = Meta::Instance();

		meta.SendMessaging(static_cast<int>(Identity::Meta), static_cast<int>(Identity::CharacterManager), MESSAGE_TYPE::END_ENEMY_TURN);
	}

	//��������Ȃ������I�u�W�F�N�g�̃}�b�v�f�[�^���㏑��
	rogue_like_dungeon.UpdateMapRole();

	//�_���W�����̃V�X�e���̏�����
	DungeonSystem::Instance().Initialize();


}

void SceneGame::Finalize()
{
	// �X�e�[�W�I����
	StageManager::Instance().Clear();

	// �L�����N�^�[�I����
	CharacterManager::Instance().Clear();
}

void SceneGame::Update(const float elapsed_time)
{
	Camera& camera = Camera::Instance();
	Meta& meta = Meta::Instance();

	//	�J�������A�N�e�B�u�ɂ���
	camera.ActivateCamera();

	// �J�����R���g���[���[�X�V����
	camera_controller->FollowCameraUpdate(elapsed_time);

	//���^AI�̍X�V
	meta.Update();

	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsed_time);

	//���̊K�ɐi�ނȂ�X�V���Ȃ�
	if (DungeonSystem::Instance().GetIsNextFloor() != true)
	{
		// �L�����N�^�[�X�V����
		CharacterManager::Instance().Update(elapsed_time);
	}


	//�}�b�v���̍X�V
	RogueLikeDungeon::Instance().UpdateMapRole();

	//�K�ړ�����
	//GoingToTheNextFloor();//GO_NEXT_FLOOR�̃��b�Z�[�W���󂯎��܂œ����Ȃ�



	const GamePad& game_pad = Input::Instance().GetGamePad();

	// A�{�^��(Z��)����������^�C�g���V�[���֐؂�ւ�
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
	// �_���W�������N���A���Ă���Q�[���I�[�o�[�V�[���ɑJ��
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_X) ||
		DungeonSystem::Instance().GetIsDungeonClear()==true )
	{

		//LOG("No Func\n");
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
	}
	//
	//if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_X))
	//{
	//	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
	//}
}

void SceneGame::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	ID3D11RenderTargetView* render_target_view = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* depth_stencil_view = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	const FLOAT clear_color[] = { 0.0f, 0.5f, 0.5f, 0.5f };	// RGBA(0.0�`1.0)
	device_context->ClearRenderTargetView(render_target_view, clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// �`�揈��
	RenderContext render_context{};
	render_context.light_direction = { 0.0f, 1.0f, 0.f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	const Camera& camera = Camera::Instance();
	render_context.view = camera.GetView();
	render_context.projection = camera.GetProjection();

	// 3D���f���`��
	{	// �X�e�[�W�`��
		ShaderManager* shader_manager = graphics.GetShaderManager();

		std::shared_ptr<Shader> shader = shader_manager->GetShader(ShaderManager::ShaderName::Lambert);

		shader->Activate(device_context, render_context);
		{
			// �X�e�[�W�`��
			StageManager::Instance().Render(device_context, shader);
			// �L�����N�^�[�`��
			CharacterManager::Instance().Render(device_context, shader);
		}
		shader->Deactivate(device_context);

	}
	// 3D�G�t�F�N�g�`��
	{

	}
#if defined(DEBUG) | defined(_DEBUG)
	// 3D�f�o�b�O�`��
	{
		// �L�����N�^�[�f�o�b�O�`��
		//CharacterManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(device_context, render_context.view, render_context.projection);

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(device_context, render_context.view, render_context.projection);
	}

	// 2D�X�v���C�g�`��
	{

	}
	// 2D�f�o�b�OGUI�`��

	{

		CharacterManager::Instance().DrawDebugGUI();
		camera_controller->DrawDebugGUI();
		StageManager::Instance().DrawDebugGUI();
		RogueLikeDungeon::Instance().DrawDebugGUI();
		Meta::Instance().DrawDebugGUI();
	}
#endif
}

void SceneGame::GoingToTheNextFloor()
{



}


//�_���W�������폜����
void SceneGame::ClearFloor()
{
	StageManager::Instance().Clear();
	//CharacterManager::Instance().RemoveEnemy();
}
//���̃_���W�����𐶐��A�z�u
void SceneGame::CreateNextFloor()
{
	//�_���W��������������
	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	rogue_like_dungeon.MakeDungeon();

	// �X�e�[�W������
	StageManager& stage_manager = StageManager::Instance();
	RogueLikeStage* rogue_like_stage = new RogueLikeStage();
	stage_manager.Register(rogue_like_stage);

	// �L�����N�^�[��������
	{
		const DirectX::XMFLOAT3 player_pos = { static_cast<float>(rogue_like_dungeon.player_pos.x) * CELL_SIZE, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CELL_SIZE };
		CharacterManager::Instance().GetPlayer()->SetPosition(player_pos);

	}

	//��������Ȃ������I�u�W�F�N�g�̃}�b�v�f�[�^���㏑��
	rogue_like_dungeon.UpdateMapRole();

	//is_darking = false;//���]�J�n
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

	//	�ŏ�K�ɓ��B���b�Z�[�W
	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		//	�Ó]����
		//is_darking = true;

		ClearFloor();
		CreateNextFloor();
		return true;
		//	�ŏ�K�ɓ��B���b�Z�[�W
	case MESSAGE_TYPE::GO_MAX_FLOOR:

		////�_���W�����N���A�̃t���O��true
		DungeonSystem::Instance().SetIsDungeonClear(true);
		return true;

	default:
		;
	}

	return false;
}

void SceneGame::SendMessaging(MESSAGE_TYPE msg)
{
	LOG(" Error :  No Function | SceneGame::SendMessaging\n")
}
