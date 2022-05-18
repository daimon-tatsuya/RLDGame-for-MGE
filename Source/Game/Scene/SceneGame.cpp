//**********************************************************
//
//		SceneGame�N���X
//
//**********************************************************

#include "Game/Scene/SceneGame.h"
#include "Engine/Systems/Logger.h"

#include "Engine/AI/DungeonMake.h"
#include "Engine/AI/MetaAI.h"

#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/CharacterManager.h"
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
#include "Game/Characters/EnemySnake.h"

#include "Game/Scene/SceneLoading.h"
#include "Game/Scene/SceneTitle.h"

SceneGame::~SceneGame()
{

	// �X�e�[�W�I����
	StageManager::Instance().Clear();
	// �L�����N�^�[�I����
	CharacterManager::Instance().Clear();
	LOG("success: SceneGame's destructor\n")

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
		graphics.GetScreenWidth(),
		graphics.GetScreenHeight(),
		0.1f,
		1000.f);

	// �J�����R���g���[���[������
	camera_controller = std::make_unique<CameraController>();

	//�_���W��������������
	RogueLikeDungeon::Instance().MakeDungeon();

	// �X�e�[�W������
	StageManager& stage_manager = StageManager::Instance();
//	stage_manager.Register(new RogueLikeStage());

	int object_num = 0;
	bool is_once = false;//for�����A��x�����s����Ƃ̂��߂̃t���O
	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//�K�i
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Exit) && is_once == false)
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				RogueLikeStage* st =new RogueLikeStage("Assets/FBX/StageMapTip/MRTP_Obj/tento.bin", pos, object_num);
				st->SetScale(DirectX::XMFLOAT3(2.7f, 2.7f, 3));
				stage_manager.Register(st);
				is_once = true;
			}
			//��
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				RogueLikeStage* st = new RogueLikeStage("Assets/FBX/geometry/wall.bin", pos, object_num);
				stage_manager.Register(st);
			}
			//����
			else if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data >= static_cast<size_t>(Attribute::Room))
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				RogueLikeStage* st = new RogueLikeStage("Assets/FBX/geometry/floor.bin", pos, object_num);
				stage_manager.Register(st);
			}

			object_num++;
		}
	}

	// �L�����N�^�[��������

	//	 �v���C���[
	CharacterManager::Instance().Register(new Player(), static_cast<int>(Identity::Player));
	// �G
	CharacterManager::Instance().Register(new EnemySnake(), static_cast<int>(Identity::Enemy));
	Meta::Instance().SendMessaging(static_cast<int>(Identity::Meta), static_cast<int>(Identity::CharacterManager), MESSAGE_TYPE::END_ENEMY_TURN);

	//��������Ȃ������I�u�W�F�N�g���}�b�v�f�[�^�������
	RogueLikeDungeon::Instance().UpdateMapRole();

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

	Camera::Instance().ActivateCamera();
	// �J�����R���g���[���[�X�V����
	camera_controller->FollowCameraUpdate(elapsed_time);

	Meta::Instance().Update();
	// �X�e�[�W�X�V����
	StageManager::Instance().Update(elapsed_time);

	// �L�����N�^�[�X�V����
	CharacterManager::Instance().Update(elapsed_time);

	const GamePad& game_pad = Input::Instance().GetGamePad();

	RogueLikeDungeon::Instance().UpdateMapRole();


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
		RogueLikeDungeon::Instance().DrawDebugGUI();
	}
#endif
}