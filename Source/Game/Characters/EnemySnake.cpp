//**********************************************************
//
//		EnemySnake�N���X
//
//**********************************************************

#include "Engine/AI/MetaAI.h"
#include "Game/Characters/EnemySnake.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Shader.h"

#include "Engine/AI/HeuristicSearch.h"

EnemySnake::EnemySnake()
{
	SetModel("Assets/FBX/Animals/Rattlesnake.bin");
	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	SetPositionY(0.f);
	SetExist(true);

	//�����X�e�[�g
	EnemySnake::FiniteStateMachineInitialize();

	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		//	�ݒ肵���烋�[�v�𔲂���
		if (GetIsDecidePos() == true)
		{
			break;
		}
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				const float posX = static_cast<float>(x * CELL_SIZE);
				const float posZ = static_cast<float> (y * CELL_SIZE);

				SetPosition(DirectX::XMFLOAT3(posX, 0, posZ));
				SetIsDecidePos(true);
				break;
			}
		}
	}
}

EnemySnake::~EnemySnake()
{
	LOG(" Executed : EnemySnake's destructor\n")
}

void EnemySnake::Update(float elapsed_time)
{
	SetPositionY(0.f);//�W�����v�����Ȃ��̂ŌŒ�

	enemy_snake_state_machine.Update(elapsed_time);

	//��]�p�̐��K��
	NormalizeAngle();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	GetModel()->UpdateTransform(GetTransform());
}

void EnemySnake::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, GetModel());
}

void EnemySnake::FiniteStateMachineInitialize()
{
	//�e�X�e�[�g�̒ǉ�

	enemy_snake_state_machine.AddState
	(
		ParentState::Entry,
		[this](const float elapsed_time) { EntryState(elapsed_time); }
	);

	enemy_snake_state_machine.AddState
	(
		ParentState::Reaction,
		[this](const float elapsed_time) { ReactionState(elapsed_time); }
	);

	enemy_snake_state_machine.AddState
	(
		ParentState::Receive,
		[this](const float elapsed_time) { ReceiveState(elapsed_time); }
	);

	//�q�X�e�[�g�̒ǉ�

	//Entry

	enemy_snake_entry_state.AddState
	(
		Entry::Select,
		[this](const float elapsed_time) {SelectState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Approach,
		[this](const float elapsed_time) {ApproachState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Explore,
		[this](const float elapsed_time) {ExploreState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Attack,
		[this](const float elapsed_time) {AttackState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Ability,
		[this](const float elapsed_time) {AbilityState(elapsed_time); }
	);

	//ReactionState

	enemy_snake_reaction_state.AddState
	(
		Reaction::ReactionSelect,
		[this](const float elapsed_time) {ReactionSelectState(elapsed_time); }
	);

	enemy_snake_reaction_state.AddState
	(
		Reaction::Damaged,
		[this](const float elapsed_time) {DamagedState(elapsed_time); }
	);

	enemy_snake_reaction_state.AddState
	(
		Reaction::Death,
		[this](const float elapsed_time) {DeathState(elapsed_time); }
	);

	//Receive

	//enemy_snake_receive_state.AddState
	//(
	//	Receive::Wait,
	//	[this](const float elapsed_time) {WaitState(elapsed_time); }
	//);

	enemy_snake_receive_state.AddState
	(
		Receive::Called,
		[this](const float elapsed_time) {CalledState(elapsed_time); }
	);

	// �e�����X�e�[�g�̐ݒ�
	enemy_snake_state_machine.SetState(ParentState::Receive);

	enemy_snake_entry_state.SetState(Entry::Select);
	enemy_snake_reaction_state.SetState(Reaction::ReactionSelect);
	enemy_snake_receive_state.SetState(Receive::Called);
}

void EnemySnake::Destroy()
{
	//�L�����N�^�[�}�l�[�W���[�̃��X�g���玩�g������
	Character::Destroy();
}

void EnemySnake::DrawDebugGUI()
{
}

void EnemySnake::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debug_renderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

void EnemySnake::SendMessaging(MESSAGE_TYPE msg)
{
	Meta& meta = Meta::Instance();

	switch (msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		LOG(" Error : END_PLAYER_TURN is No Function | EnemySnake.cpp SendMessaging Method\n")
			break;

	case MESSAGE_TYPE::END_ENEMY_TURN:

		//���^AI�Ƀ^�[���̏I����`����
		meta.SendMessaging(GetId(),
			static_cast<int>(Identity::Meta),
			MESSAGE_TYPE::END_ENEMY_TURN);

		//�X�e�[�g�}�V���̐ݒ�
		enemy_snake_state_machine.SetState(ParentState::Receive);
		break;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		LOG(" Error : GO_NEXT_FLOOR is No Function | EnemySnake.cpp SendMessaging Method\n")
			break;

	case MESSAGE_TYPE::GO_MAX_FLOOR:

		LOG(" Error : GO_MAX_FLOOR is No Function | EnemySnake.cpp SendMessaging Method\n")
			break;

	default:
		LOG(" Error : No Message | EnemySnake.cpp SendMessaging Method\n")
			break;
	}
}

void EnemySnake::OnDamaged()
{
}

void EnemySnake::OnDead()
{
}

bool EnemySnake::OnMessage(const Telegram& telegram)
{
	//���^AI����̎�M����
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		enemy_snake_state_machine.SetState(ParentState::Entry);

		return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:

		LOG(" Error : END_ENEMY_TURN is No Function | EnemySnake.cpp OnMessage Method\n")

			return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		SetExist(false);
		//Destroy();
		return true;

	case MESSAGE_TYPE::GO_MAX_FLOOR:


		return true;

	default:

		LOG(" Error : No Message | EnemySnake.cpp OnMessage Method\n")

			return false;

	}
}

//----------------------------------------------------------------
//
// �X�e�[�g
//
//----------------------------------------------------------------

//---------------------
// �e�X�e�[�g
//---------------------

void EnemySnake::EntryState(const float elapsed_time)
{
	if (enemy_snake_state_machine.IsStateFirstTime())
	{
		// �q�X�e�[�g�̂̏�����
		if (enemy_snake_entry_state.GetState() != static_cast<int>(Entry::Select))
		{
			enemy_snake_entry_state.SetState(Entry::Select);
		}
	}
	// �T�u�X�e�[�g�̎��s
	enemy_snake_entry_state.Update(elapsed_time);
}

void EnemySnake::ReactionState(const float elapsed_time)
{
	if (enemy_snake_state_machine.IsStateFirstTime())
	{
		// �q�X�e�[�g�̂̏�����
		if (enemy_snake_reaction_state.GetState() != static_cast<int>(Reaction::ReactionSelect))
		{
			enemy_snake_reaction_state.SetState(Reaction::ReactionSelect);
		}
	}
	// �T�u�X�e�[�g�̎��s
	enemy_snake_reaction_state.Update(elapsed_time);
}

void EnemySnake::ReceiveState(const float elapsed_time)
{
	if (enemy_snake_state_machine.IsStateFirstTime())
	{
		// �q�X�e�[�g�̂̏�����
		if (enemy_snake_receive_state.GetState() != static_cast<int>(Receive::Called))
		{
			enemy_snake_receive_state.SetState(Receive::Called);
		}
	}
	// �T�u�X�e�[�g�̎��s
	enemy_snake_receive_state.Update(elapsed_time);
}

//---------------------
//�q�X�e�[�g
//---------------------

	//?EntryState

void EnemySnake::SelectState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
	enemy_snake_entry_state.SetState(Entry::Approach);
}

void EnemySnake::ApproachState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}

	SendMessaging(MESSAGE_TYPE::END_ENEMY_TURN);
}

void EnemySnake::ExploreState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
		shortest_path->Clear();
		//�T��������߂�
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		DirectX::XMINT2 far_pos{};//���g���ł������ʒu��ۑ�����

		float most_long_length{};//�ł������������i�[����ϐ�

		const DirectX::XMFLOAT2 enemy_map_pos =
			DirectX::XMFLOAT2(GetPosition().x / CELL_SIZE, GetPosition().z / CELL_SIZE);//�}�b�v����̎��g�̈ʒu

		//const DirectX::XMFLOAT2 enemy_map_pos =
		//	DirectX::XMFLOAT2(GetPosition().x / CELL_SIZE, GetPosition().z / CELL_SIZE);//�}�b�v����̎��g�̈ʒu

		for (const auto& room_center : rogue_like_dungeon.GetRoomsCenter())
		{
			DirectX::XMFLOAT2 room =//XMINT2��XMVECTOR�ɏo���Ȃ������̂�XMFLOAT2�ɃL���X�g
				DirectX::XMFLOAT2(static_cast<float>(room_center.x), static_cast<float>(room_center.y));

			float length = //�����̒��S�Ǝ��g�Ƃ̓�_�Ԃ̋���
				DirectX::XMVectorGetX(
					DirectX::XMVector2Length(
						DirectX::XMVectorSubtract(
							DirectX::XMLoadFloat2(&room),
							DirectX::XMLoadFloat2(&enemy_map_pos)
						)
					)
				);

			if (most_long_length < length)//�ł������Ȃ�X�V����
			{
				most_long_length = length;
				far_pos = room_center;
			}
		}

		int start_id = HeuristicSearch::Instance().ConvertWorldToOneDimensional(far_pos);//Aster�̃X�^�[�g�n�_
		int goal_id = HeuristicSearch::Instance().ConvertWorldToOneDimensional(far_pos);//ASter�̃S�[���n�_

		//shortest_path->path_information = HeuristicSearch::Instance().Search(start_id, goal_id);

	}

	int destination_position =//�ړ���
		shortest_path->path_information[static_cast<int>(shortest_path->path_information.size() - shortest_path->path_index)];//����������o��

	DirectX::XMINT2 destination_map_position = HeuristicSearch::Instance().ConvertMapPosition(destination_position); //

	shortest_path->path_index++;//�p�X�̃C���f�b�N�X���C���N�������g
}

void EnemySnake::AttackState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
}

void EnemySnake::AbilityState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
}

//?ReactionState

void EnemySnake::ReactionSelectState(const float elapsed_time)
{
	if (enemy_snake_reaction_state.IsStateFirstTime())
	{
	}
}
void EnemySnake::DamagedState(const float elapsed_time)
{
	if (enemy_snake_reaction_state.IsStateFirstTime())
	{
	}
}

void EnemySnake::DeathState(const float elapsed_time)
{
	if (enemy_snake_reaction_state.IsStateFirstTime())
	{
	}
}

//?ReceiveState

void EnemySnake::WaitState(const float elapsed_time)
{
	if (enemy_snake_receive_state.IsStateFirstTime())
	{

	}
}

void EnemySnake::CalledState(const float elapsed_time)
{
	if (enemy_snake_receive_state.IsStateFirstTime())
	{
	}
}