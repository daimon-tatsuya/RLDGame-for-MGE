#include "Engine/Systems/Input.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Math.h"
#include "Game/Characters/DerivedPlayerState.h"


const float cos45 = cosf(Math::ConvertToRadianAngle(45.f));
//const float
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
	owner = nullptr;
}

void ReactionState::Enter()
{
	if (owner->GetHealth() > 0)
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
		state = nullptr;
	}
	sub_state_pool.clear();
	owner = nullptr;
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
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Attack));
	}

	//同時に処理をしたくないので else ifで1つtrueなら入らないようにする

	//メニュー
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	{
		//Menuステートに遷移する
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Menu));
	}

	//方向転換
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		//WayChangeステートに遷移する
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::WayChange));
	}

	//	移動
	//左スティックもしくはDPadが入力されたとき
	else if (game_pad.GetAxisLX() != 0 || game_pad.GetAxisLY() != 0)
	{
		//Moveステートに遷移する
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Move));
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

	GamePad& game_pad = Input::Instance().GetGamePad();
	//Yボタンを長押ししている間
	if (game_pad.GetButtonUp() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
	}
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	//ステップをして　左スティックの入力を0, 1 , -1 , 1 / √2(約0.71)にして8方向を表現
	//左スティックのx軸のステップ
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//左スティックのy軸のステップ
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//方向転換
	//上
	if (ax == 0.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(0));
	}
	//下
	else if (ax == 0.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(180));
	}
	//右
	else if (ax == 1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(90));
	}
	//左
	else if (ax == -1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(270));
	}


	//右上
	if (ax == 1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//左上
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(315));
	}
	//左下
	else if (ax == -1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(225));
	}
	//右下
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(135));
	}

	//攻撃
	else	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attackステートに遷移する
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Attack));
	}
}

void WayChangeState::Exit()
{
}



void MoveState::Enter()
{
}

void MoveState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(owner->GetPosition().x / Cell_Size, owner->GetPosition().z / Cell_Size);//データ上の値にするためCell_Sizeで割る

		//ステップをして　左スティックの入力を0, 1 , -1 , 1 / √2(約0.71)にする
		//左スティックのx軸のステップ
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//左スティックのy軸のステップ
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}


	//上
	if (ax == 0.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(0));
		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
		//壁か敵でないなら
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionZ(Cell_Size);
		}
	}
	//下
	else if (ax == 0.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(180));
		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
		//壁か敵でないなら
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionZ(-Cell_Size);
		}
	}
	//右
	else if (ax == 1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(90));
		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ上の升目を見る
	//壁か敵でないなら
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionX(Cell_Size);
		}
	}
	//左
	else if (ax == -1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(270));

		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ上の升目を見る
		//壁か敵でないなら
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionX(-Cell_Size);
		}
	}


	//右上
	if (ax == 1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//左上
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(315));
	}
	//左下
	else if (ax == -1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(225));
	}
	//右下
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(135));
	}

	//攻撃
	else	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attackステートに遷移する
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Attack));
	}

	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void MoveState::Exit()
{
	//	Meta meta = Meta::Instance();
}


void AttackState::Enter()
{
}

void AttackState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void AttackState::Exit()
{
	//	Meta meta = Meta::Instance();
}



void MenuState::Enter()
{
}

void MenuState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
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
