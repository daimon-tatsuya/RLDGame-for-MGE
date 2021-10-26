#include "Engine/Systems/StateBase.h"

void HierarchicalStateBase::ChangeSubState(int newState)
{
	sub_state->Exit();
	SetSubState(newState);
}

void HierarchicalStateBase::SetSubState(int setState)
{
	sub_state = sub_state_pool.at(setState);
	sub_state->Enter();
}
void HierarchicalStateBase::RegisterSubState(StateBase* state)
{
	sub_state_pool.emplace_back(state);
}

int HierarchicalStateBase::GetSubStateIndex()
{
	int i = 0;
	for (auto& state : sub_state_pool)
	{
		if (state == sub_state)
		{
			return i;
		}
		i++;
	}
	return -1;
}