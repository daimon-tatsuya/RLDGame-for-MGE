//**********************************************************
//
//		StateMachine�N���X
//
//**********************************************************

#include "Engine/Systems/StateMachine.h"

StateMachine::~StateMachine()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : state_pool)
	{
		delete state;
		state = nullptr;
	}
	state_pool.clear();

}

void StateMachine::Update(float elapsedTime)
{
	current_state->Execute(elapsedTime);
}

void StateMachine::SetState(int setState)
{
	current_state = state_pool.at(setState);
	current_state->Enter();
}

void StateMachine::ChangeState(int newState)
{
	current_state->Exit();
	SetState(newState);
}

//�e�X�e�[�g�̓o�^
void StateMachine::RegisterState(HierarchicalStateBase* state)
{
	state_pool.emplace_back(state);
}
//�e�X�e�[�g�̎q�X�e�[�g�̓o�^
void StateMachine::RegisterSubState(int index, StateBase* subState)
{
	state_pool.at(index)->RegisterSubState(subState);
}

int StateMachine::GetStateIndex()
{
	return 0;
}