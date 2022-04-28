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

EnemySnake::EnemySnake(RogueLikeDungeon* rogue_like_dungeon)
{
	SetModel("Assets/FBX/Animals/Rattlesnake.bin");
	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	SetPositionY(0.f);
	SetExist(true);
	stage_information = rogue_like_dungeon;

	//�����X�e�[�g
	EnemySnake::FiniteStateMachineInitialize();

	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		if (GetIsDecidePos() == true)
		{
			break;
		}
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_information->map_role[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				const float pos_x = static_cast<float>(x * CellSize);
				const float pos_z = static_cast<float> (y * CellSize);

				SetPosition(DirectX::XMFLOAT3(pos_x, 0, pos_z));
				SetIsDecidePos(true);
				break;
			}
		}
	}
}

EnemySnake::~EnemySnake() = default;

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

void EnemySnake::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
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
	debug_renderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

void EnemySnake::SendMessaging(MESSAGE_TYPE msg)
{
	Meta& meta = Meta::Instance();

	switch (msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		LOG("\n error: No Function")
			break;

	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		//���^AI�Ƀ^�[���̏I����`����

		meta.SendMessaging(GetId(),
			static_cast<int>(Meta::Identity::Meta),
			MESSAGE_TYPE::MSG_END_ENEMY_TURN);
		//�X�e�[�g�}�V���̐ݒ�
		enemy_snake_state_machine.SetState(ParentState::Receive);
		break;
	default:
		LOG("\n No Message")
			break;
	}
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
	enemy_snake_entry_state.SetState(Entry::Approach);
}

void EnemySnake::ApproachState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}

	SendMessaging(MESSAGE_TYPE::MSG_END_ENEMY_TURN);
}

void EnemySnake::ExploreState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{

	}



	//�Ώۂ܂ł̒T��
	if (shortest_path->path_information.empty() == true)	//�ŒZ�o�H���Ȃ�
	{
		int start_id{}, goal_id{};//Astar�̎n�߂�ʒu�ƖړI�n
		shortest_path->Clear();//������

		/*�G�̏��(position)���}�b�v���(2�����z��)map_role[enemy_posZX[0]][enemy_posZX[1]]*/
		//���ɂ͂y���̏������Ă���
		const DirectX::XMFLOAT3 enemy_posZX = { GetPosition().x / CellSize, 0,GetPosition().z / CellSize };

		/*�}�b�v���(2�����z��)��Astar(1�����z��)*/
		//Astar�̎n�߂�ʒu
		start_id = static_cast<int>(enemy_posZX.y) * MapSize_Y + static_cast<int>(enemy_posZX.x);

		//�ړI�n
		//�ł��߂��ꏊ��������@���@�ʘH�̓����
		DirectX::XMFLOAT3 shortest{};//�ʘH�̓�����̈ʒu
		float min_length = FLT_MAX;

		for (const auto& road_entrance : stage_information->roads_entrance)
		{
			DirectX::XMFLOAT2 r = road_entrance;

			//�}�b�v��񁨃��[���h���W
			r.y = r.y * CellSize;
			r.x = r.x * CellSize;

			const DirectX::XMFLOAT3 road_pos = DirectX::XMFLOAT3(r.x, 0.f, r.y);//�ʘH�̓�����̃��[���h���W

			//���g���������̒ʘH�̏�ɂ���Ƃ��X�L�b�v����
			if (Math::Comparison(GetPosition().x, road_pos.x) && Math::Comparison(GetPosition().z, road_pos.z))
			{
				continue;
			}

			const float length = Math::Length(Math::SubtractVector(GetPosition(), road_pos));//���g(�G)����ʘH�ւ̍ŒZ����

			//���݂̍ŒZ�����̓���ւ���
			if (length < min_length)
			{
				min_length = length;//���݂̍ŒZ�����̓���ւ�
				shortest = DirectX::XMFLOAT3(static_cast<float>(r.x), 0, static_cast<float>(r.y));//�ł��߂��ʘH�̃��[���h���W���i�[
			}
		}
		shortest_path->destination_pos = shortest;
		goal_id = static_cast<int>(shortest.z) * MapSize_Y + static_cast<int>(shortest.x);
		//�ŒZ�o�H�����߂�
		HeuristicSearch& Astar = HeuristicSearch::Instance();
		shortest_path->path_information = Astar.Search(start_id, goal_id, *stage_information);//���B
		shortest_path->SetPathSize();//�ŒZ�o�H�̃T�C�Y��ݒ�
	}
	else//�T���悪����
	{
		//shortest_path->path_information�͍ŒZ�o�H�̎��������̔z��
		if (true)
		{

		}
		else
		{
			LOG("error:shortest_path->path_information is nothing")//�ŒZ�o�H�̒l���Ȃ�
		}
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