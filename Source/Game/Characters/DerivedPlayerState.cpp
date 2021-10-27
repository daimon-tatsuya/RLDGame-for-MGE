#include "Game/Characters/DerivedPlayerState.h"
#include "Engine/Systems/Input.h"
//-------------------------------------
//親ステート
//--------------------------------------

EntryState::~EntryState()
{
	// 登録されたサブステートを順番に削除
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void EntryState::Enter()
{
	// 初期に入るサブステート
	SetSubState(static_cast<int>(Player::Entry::Select));
}

void EntryState::Execute(float elapsedTime)
{
	// サブステートの実行
		sub_state->Execute(elapsedTime);

}

void EntryState::Exit()
{

}

ReactionState::~ReactionState()
{
	// 登録されたサブステートを順番に削除
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
		// 初期に入るサブステート
		SetSubState(static_cast<int>(Player::Reaction::Damaged));
	}
	else
	{
		SetSubState(static_cast<int>(Player::Reaction::Death));
	}
}

void ReactionState::Execute(float elapsedTime)
{
	// サブステートの実行
	sub_state->Execute(elapsedTime);
}

void ReactionState::Exit()
{
}

ReceiveState::~ReceiveState()
{
	// 登録されたサブステートを順番に削除
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
	// サブステートの実行
	sub_state->Execute(elapsedTime);
}

void ReceiveState::Exit()
{
}

//-------------------------------------
//子ステート
//--------------------------------------


void SelectState::Enter()
{

}

void SelectState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();


	//攻撃
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attackステートに遷移する
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::Attack));
	}

	//同時に処理をしたくないので else ifで1つtrueなら入らないようにする

	//メニュー
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	{
		//Menuステートに遷移する
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::Menu));
	}

	//方向転換
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		//WayChangeステートに遷移する
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::Entry::WayChange));
	}

	//else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	//	移動
	//左スティックもしくはDPadが入力されたとき
	else if (game_pad.GetAxisLX()!=0 || game_pad.GetAxisLY() != 0)
	{
		//Moveステートに遷移する
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
