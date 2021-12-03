//**********************************************************
//
//		EnemySnake��State�̔h���N���X
//
//**********************************************************


#include "Engine/AI/MetaAI.h"
#include "Game/Characters/DerivedEnemySnakeState.h"


//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

EnemySnakeEntryState::~EnemySnakeEntryState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}


void EnemySnakeEntryState::Enter()
{
	// �����ɓ���T�u�X�e�[�g
	SetSubState(static_cast<int>(EnemySnake::Entry::Select));
}

void EnemySnakeEntryState::Execute(float elapsedTime)
{

}

void EnemySnakeEntryState::Exit()
{
}



EnemySnakeReactionState::~EnemySnakeReactionState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void EnemySnakeReactionState::Enter()
{

	if (owner->GetHealth() > 0)
	{
		// �����ɓ���T�u�X�e�[�g
		SetSubState(static_cast<int>(EnemySnake::Reaction::Damaged));
	}
	else
	{
		SetSubState(static_cast<int>(EnemySnake::Reaction::Death));
	}
}

void EnemySnakeReactionState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void EnemySnakeReactionState::Exit()
{
}



EnemySnakeReceiveState::~EnemySnakeReceiveState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();

}

void EnemySnakeReceiveState::Enter()
{
	// �����ɓ���T�u�X�e�[�g
	SetSubState(static_cast<int>(EnemySnake::Receive::Wait));
}

void EnemySnakeReceiveState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void EnemySnakeReceiveState::Exit()
{

}

//-------------------------------------
//�q�X�e�[�g
//--------------------------------------

void EnemySnakeSelectState::Enter()
{
}

void EnemySnakeSelectState::Execute(float elapsedTime)
{
}

void EnemySnakeSelectState::Exit()
{
}

void EnemySnakeApproachState::Enter()
{
}

void EnemySnakeApproachState::Execute(float elapsedTime)
{
}

void EnemySnakeApproachState::Exit()
{
}



void EnemySnakeExploreState::Enter()
{
}

void EnemySnakeExploreState::Execute(float elapsedTime)
{
}

void EnemySnakeExploreState::Exit()
{
}



void EnemySnakeAttackState::Enter()
{
}

void EnemySnakeAttackState::Execute(float elapsedTime)
{
}

void EnemySnakeAttackState::Exit()
{
}



void EnemySnakeAbilityState::Enter()
{
}

void EnemySnakeAbilityState::Execute(float elapsedTime)
{
}

void EnemySnakeAbilityState::Exit()
{
}



void EnemySnakeUseItemState::Enter()
{
}

void EnemySnakeUseItemState::Execute(float elapsedTime)
{
}

void EnemySnakeUseItemState::Exit()
{
}



void EnemySnakeDamagedState::Enter()
{
}

void EnemySnakeDamagedState::Execute(float elapsedTime)
{
}

void EnemySnakeDamagedState::Exit()
{
}



void EnemySnakeDeathState::Enter()
{
}

void EnemySnakeDeathState::Execute(float elapsedTime)
{
}

void EnemySnakeDeathState::Exit()
{
}



void EnemySnakeWaitState::Enter()
{

}

void EnemySnakeWaitState::Execute(float elapsedTime)
{

}

void EnemySnakeWaitState::Exit()
{
}



void EnemySnakeCalledState::Enter()
{
}

void EnemySnakeCalledState::Execute(float elapsedTime)
{
}

void EnemySnakeCalledState::Exit()
{
}

