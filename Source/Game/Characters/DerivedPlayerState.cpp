#include "Game/Characters/DerivedPlayerState.h"
#include "Engine/Systems/Input.h"
//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

EntryState::~EntryState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void EntryState::Enter()
{
	// �����ɓ���T�u�X�e�[�g
	SetSubState(static_cast<int>(Player::Entry::Select));
}

void EntryState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
		sub_state->Execute(elapsedTime);

}

void EntryState::Exit()
{

}

ReactionState::~ReactionState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void ReactionState::Enter()
{
	if (owner->GetHealth() >0)
	{
		// �����ɓ���T�u�X�e�[�g
		SetSubState(static_cast<int>(Player::Reaction::Damaged));
	}
	else
	{
		SetSubState(static_cast<int>(Player::Reaction::Death));
	}
}

void ReactionState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void ReactionState::Exit()
{
}

ReceiveState::~ReceiveState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void ReceiveState::Enter()
{

}

void ReceiveState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void ReceiveState::Exit()
{
}

//-------------------------------------
//�q�X�e�[�g
//--------------------------------------


void SelectState::Enter()
{

}

void SelectState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();


	//�U��
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attack�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::Attack));
	}

	//�����ɏ������������Ȃ��̂� else if��1��true�Ȃ����Ȃ��悤�ɂ���

	//���j���[
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	{
		//Menu�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::Menu));
	}

	//�����]��
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		//WayChange�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::WayChange));
	}

	//else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	//	�ړ�
	//���X�e�B�b�N��������DPad�����͂��ꂽ�Ƃ�
	else if (game_pad.GetAxisLX()!=0 || game_pad.GetAxisLY() != 0)
	{
		//Move�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::Move));
	}
}

void SelectState::Exit()
{
}



void WayChangeState::Enter()
{
}

void WayChangeState::Execute(float elapsedTime)
{
}

void WayChangeState::Exit()
{
}



void MoveState::Enter()
{
}

void MoveState::Execute(float elapsedTime)
{
}

void MoveState::Exit()
{
	Meta meta = Meta::Instance();
}


void AttackState::Enter()
{
}

void AttackState::Execute(float elapsedTime)
{
}

void AttackState::Exit()
{
	Meta meta = Meta::Instance();
}



void MenuState::Enter()
{
}

void MenuState::Execute(float elapsedTime)
{
}

void MenuState::Exit()
{
}



void DamagedState::Enter()
{
}

void DamagedState::Execute(float elapsedTime)
{
}

void DamagedState::Exit()
{
}


void DeathState::Enter()
{
}

void DeathState::Execute(float elapsedTime)
{
}

void DeathState::Exit()
{
}

void CalledState::Enter()
{
}

void CalledState::Execute(float elapsedTime)
{
}

void CalledState::Exit()
{
}
