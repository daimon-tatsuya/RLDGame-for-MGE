//**********************************************************
//
//		SceneGame�N���X
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
	// �X�e�[�W�I����
	StageManager::Instance().Clear();

	// �L�����N�^�[�I����
	CharacterManager::Instance().Clear();

	LOG(" Executed : SceneGame's destructor\n")
}

void SceneGame::Initialize()
{
	// �J���������ݒ�
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

	//���t���e�J����
	camera.SetOrthoFov(
		graphics.GetScreenWidth() / 30,
		graphics.GetScreenHeight() / 30,
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

	//��������Ȃ������I�u�W�F�N�g���}�b�v�f�[�^�������
	rogue_like_dungeon.UpdateMapRole();

	//������J�����O�p��AABB�̏����ݒ�
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

	//	�J�������A�N�e�B�u�ɂ���
	camera.ActivateCamera();

	// �J�����R���g���[���[�X�V����
	camera_controller->FollowCameraUpdate(elapsed_time);

	//���^AI�̍X�V
	meta.Update();

	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsed_time);

	// �L�����N�^�[�X�V����
	CharacterManager::Instance().Update(elapsed_time);

	//�}�b�v���̍X�V
	 RogueLikeDungeon::Instance().UpdateMapRole();

	const GamePad& game_pad = Input::Instance().GetGamePad();

	// A�{�^��(Z��)����������^�C�g���V�[���֐؂�ւ�
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	}
	// B�{�^��(X��)���������� �ă��[�h
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	{
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
	}
	// Start�{�^��(N��)���������� �G�̍폜
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
	// 2D�X�v���C�g�`��
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

void SceneGame::ClearFloor()
{
	StageManager::Instance().Clear();
	//CharacterManager::Instance().RemoveEnemy();
}

void SceneGame::NextFloor()
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
		DirectX::XMFLOAT3 player_pos = { static_cast<float>(rogue_like_dungeon.player_pos.x) * CellSize, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CellSize };
		CharacterManager::Instance().GetPlayer()->SetPosition(player_pos);

	}

	//��������Ȃ������I�u�W�F�N�g���}�b�v�f�[�^�������
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
