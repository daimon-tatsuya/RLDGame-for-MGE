//**********************************************************
//
//		PlayerのStateクラス
//
//**********************************************************

#include "Engine/Systems/Input.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Math.h"
#include "Game/Characters/DerivedPlayerState.h"


const float cos45 = cosf(Math::ConvertToRadianAngle(45.f));


//-------------------------------------
//親ステート
//--------------------------------------

PlayerEntryState::~PlayerEntryState()
{
	// 登録されたサブステートを順番に削除
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void PlayerEntryState::Enter()
{
	// 初期に入るサブステート
	SetSubState(static_cast<int>(Player::Entry::Select));
}

void PlayerEntryState::Execute(float elapsedTime)
{
	// サブステートの実行
	sub_state->Execute(elapsedTime);

}

void PlayerEntryState::Exit()
{

}

PlayerReactionState::~PlayerReactionState()
{
	// 登録されたサブステートを順番に削除
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
	owner = nullptr;
}

void PlayerReactionState::Enter()
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

void PlayerReactionState::Execute(float elapsedTime)
{
	// サブステートの実行
	sub_state->Execute(elapsedTime);
}

void PlayerReactionState::Exit()
{
}

PlayerReceiveState::~PlayerReceiveState()
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

void PlayerReceiveState::Enter()
{
	// 初期に入るサブステート
	SetSubState(static_cast<int>(Player::Receive::Wait));
}

void PlayerReceiveState::Execute(float elapsedTime)
{
	// サブステートの実行
	sub_state->Execute(elapsedTime);
}

void PlayerReceiveState::Exit()
{
}

//-------------------------------------
//子ステート
//--------------------------------------


void PlayerSelectState::Enter()
{

}

void PlayerSelectState::Execute(float elapsedTime)
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

void PlayerSelectState::Exit()
{
}



void PlayerWayChangeState::Enter()
{
}

void PlayerWayChangeState::Execute(float elapsedTime)
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
	else if (ax == -1.f && ay == 1.f)
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

void PlayerWayChangeState::Exit()
{
}



void PlayerMoveState::Enter()
{
}

void PlayerMoveState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	DirectX::XMFLOAT2 player_pos =//データ上の値にするためCell_Sizeで割る
		DirectX::XMFLOAT2(owner->GetPosition().x / Cell_Size, owner->GetPosition().z / Cell_Size);

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

	//移動処理

	//右上
	if (ax == 1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(45));
		size_t map_data_diagonal = owner->GetStageInformations()->//現在のステージの情報から一つ右上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//現在のステージの情報から一つ右の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//現在のステージの情報から一つ上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;


		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(Cell_Size);
			owner->AddPositionX(Cell_Size);
		}
	}
	//左上
	else if (ax == -1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(315));
		size_t map_data_diagonal = owner->GetStageInformations()->//現在のステージの情報から一つ左上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//現在のステージの情報から一つ左の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//現在のステージの情報から一つ上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(Cell_Size);
			owner->AddPositionX(-Cell_Size);
		}
	}
	//左下
	else if (ax == -1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(225));
		size_t map_data_diagonal = owner->GetStageInformations()->//現在のステージの情報から一つ左下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//現在のステージの情報から一つ左の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//現在のステージの情報から一つ下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(-Cell_Size);
			owner->AddPositionX(-Cell_Size);
		}
	}
	//右下
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(135));
		size_t map_data_diagonal = owner->GetStageInformations()->//現在のステージの情報から一つ右下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//現在のステージの情報から一つ右の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//現在のステージの情報から一つ下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(-Cell_Size);
			owner->AddPositionX(Cell_Size);
		}
	}

	//上
	if (ax == 0.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(0));
		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
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
		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ下の升目を見る
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
		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ右の升目を見る
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

		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ左の升目を見る
		//壁か敵でないなら
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionX(-Cell_Size);
		}
	}

	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
	//owner->GetStateMachine()->ChangeState(static_cast<int>(Player::ParentState::Receive));
}

void PlayerMoveState::Exit()
{
	//	Meta meta = Meta::Instance();
}


void PlayerAttackState::Enter()
{
}

void PlayerAttackState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void PlayerAttackState::Exit()
{
	//	Meta meta = Meta::Instance();
}



void PlayerMenuState::Enter()
{
}

void PlayerMenuState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void PlayerMenuState::Exit()
{
}



void PlayerDamagedState::Enter()
{
}

void PlayerDamagedState::Execute(float elapsedTime)
{
}

void PlayerDamagedState::Exit()
{
}


void PlayerDeathState::Enter()
{
}

void PlayerDeathState::Execute(float elapsedTime)
{
}

void PlayerDeathState::Exit()
{
}

void PlayerCalledState::Enter()
{
}

void PlayerWaitState::Enter()
{
	//Meta& meta = Meta::Instance();
	//meta.SendMessaging(owner->GetId(),
	//	static_cast<int>(Meta::Identity::Meta),
	//	MESSAGE_TYPE::MSG_END_PLAYER_TURN);
}

void PlayerWaitState::Execute(float elapsedTime)
{
}

void PlayerWaitState::Exit()
{
}


void PlayerCalledState::Execute(float elapsedTime)
{
}

void PlayerCalledState::Exit()
{
}


