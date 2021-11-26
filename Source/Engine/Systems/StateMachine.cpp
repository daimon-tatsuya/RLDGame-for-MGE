//**********************************************************
//
//		StateMachineクラス
//
//**********************************************************

#include "Engine/Systems/StateMachine.h"

StateMachine::~StateMachine()
{
	// 登録されたサブステートを順番に削除
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

//親ステートの登録
void StateMachine::RegisterState(HierarchicalStateBase* state)
{
	state_pool.emplace_back(state);
}
//親ステートの子ステートの登録
void StateMachine::RegisterSubState(int index, StateBase* subState)
{
	state_pool.at(index)->RegisterSubState(subState);
}

int StateMachine::GetStateIndex()
{
	return 0;
}