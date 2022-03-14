//**********************************************************
//
//		EnemyManager�N���X
//
//**********************************************************

#include "Game/Characters/EnemySnake.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Shader.h"

#include "Engine/AI/HeuristicSearch.h"

EnemySnake::EnemySnake(RogueLikeDungeon* rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/Rattlesnake.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 0.f;
	exists = true;
	stage_information = rogue_like_dungeon;

	//�����X�e�[�g
	EnemySnake::FiniteStateMachineInitialize();

	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		if (is_set_pos == true)
		{
			break;
		}
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_information->map_role[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				const float pos_x = static_cast<float>(x * CellSize);
				const float pos_z = static_cast<float> (y * CellSize);

				position = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				is_set_pos = true;
				break;
			}
		}
	}
}

EnemySnake::~EnemySnake() = default;

void EnemySnake::Update(float elapsed_time)
{
	position.y = 0.f;

	enemy_snake_state_machine.Update(elapsed_time);

	//��]�p�̐��K��
	NormalizeAngle();

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void EnemySnake::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
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

//void EnemySnake::Destroy()
//{
//	CharacterManager& character_manager = CharacterManager::Instance();
//	//�L�����N�^�[�}�l�[�W���[�̃��X�g�������
//	//character_manager.Remove(this);
//}

void EnemySnake::DrawDebugGUI()
{
}

void EnemySnake::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��
	debug_renderer->DrawSphere(this->position, this->radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void EnemySnake::OnDamaged()
{
}

void EnemySnake::OnDead()
{
}

bool EnemySnake::OnMessage(const Telegram & telegram)
{
	//���^AI����̎�M����
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		enemy_snake_state_machine.SetState(ParentState::Entry);

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		LOG("\n error: MESSAGE_TYPE::MSG_END_ENEMY_TURN Messages not received")
			return false;
	default:

		LOG("\n error: No Message")

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
}

void EnemySnake::ApproachState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
	HeuristicSearch& Astar = HeuristicSearch::Instance();
	Astar.Reset(*stage_information);

	//shortest_path = Astar.Search();
}

void EnemySnake::ExploreState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}

	//�T���悪�Ȃ�
	if (shortest_path->path.empty() == true)
	{
		int start_id{}, goal_id{};//Astar�̎n�߂�ʒu�ƖړI�n

		/*�G�̏��(position)���}�b�v���(2�����z��)map_role[enemy_posZX[0]][enemy_posZX[1]]*/
		const DirectX::XMINT2 enemy_posZX = { static_cast<int>(position.z / CellSize), static_cast<int>(position.x / CellSize) };

		/*�}�b�v���(2�����z��)��Astar(1�����z��)*/

		//Astar�̎n�߂�ʒu
		start_id = enemy_posZX.y * MapSize_Y + enemy_posZX.x;

		//�ړI�n
		//�ł��߂��ꏊ��������@���@���̓����

		DirectX::XMFLOAT3 shortest{};//���̓�����̈ʒu
		float min_length = FLT_MAX;

		for (const auto& road_entrance : stage_information->roads_entrance)
		{
			DirectX::XMINT2 r = road_entrance;

			//�}�b�v��񁨃��[���h���W
			r.y = r.y * CellSize;
			r.x = r.x * CellSize;
			const DirectX::XMFLOAT3 road_pos = DirectX::XMFLOAT3(static_cast<float>(r.x), 0.f, static_cast<float>(r.y));//���̓�����̃��[���h���W

			//���g���������̓��̏�ɂ���Ƃ��X�L�b�v����
			if (position.x == road_pos.x && position.z == road_pos.z)
			{
				continue;
			}

			const float length = Math::Length(Math::SubtractVector(position, road_pos));//���g(�G)���瓹�ւ̍ŒZ����

			//���݂̍ŒZ�����̓���ւ���
			if (length < min_length)
			{
				min_length = length;//���݂̍ŒZ�����̓���ւ�
				shortest = DirectX::XMFLOAT3(static_cast<float>(r.x), 0, static_cast<float>(r.y));//�ł��߂����̃��[���h���W���i�[
			}
		}

		/*�ŒZ�o�H�����߂�*/
		HeuristicSearch& Astar = HeuristicSearch::Instance();

		shortest_path->path = Astar.Search(start_id, 0, *stage_information);//���B
	}
	else//�T���悪����
	{
	}
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

//void EnemySnake::WaitState(const float elapsed_time)
//{
//	if (enemy_snake_receive_state.IsStateFirstTime())
//	{
//
//	}
//}

void EnemySnake::CalledState(const float elapsed_time)
{
	if (enemy_snake_receive_state.IsStateFirstTime())
	{
	}
}