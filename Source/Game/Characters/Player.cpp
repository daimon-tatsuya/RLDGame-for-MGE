//**********************************************************
//
//		Playerクラス
//
//**********************************************************


#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Objects/Model.h"
#include "Engine/AI/DungeonMake.h"

#include "Game/Characters/Player.h"
#include "Game/Characters/DerivedPlayerState.h"


const float cos45 = cosf(DirectX::XMConvertToRadians(45.f));

Player::Player(RogueLikeDungeon* rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	//model = std::make_shared<Model>("Assets/FBX/Animals/Rattlesnake.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 0.f;

	// 初期ステート
	Player::FiniteStateMachineInitialize();

	stage_information = rogue_like_dungeon;

	// オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_information->map_role[y][x].map_data == static_cast<size_t>(Attribute::Player))
			{
				const float pos_x = static_cast<float>(x * CellSize);
				const float pos_z = static_cast<float> (y * CellSize);

				position = DirectX::XMFLOAT3(pos_x, 0, pos_z);
			}
		}
	}
}

Player::~Player()
= default;


void Player::Update(const float elapsed_time)
{
	position.y = 0.f;

	player_state_machine.Update(elapsed_time);

	{
		//--追加---------------------------------------
		// AABB
		//
		//-----------------------------------------------
	}


	// 回転角の正規化
	NormalizeAngle();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデルアニメーション更新処理
	model->UpdateAnimation(elapsed_time);

	// モデル行列更新
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
}

void Player::FiniteStateMachineInitialize()
{

	//親ステートの追加
	player_state_machine.AddState
	(
		ParentState::Entry,
		[this](const float elapsed_time) { EntryState(elapsed_time); }
	);

	player_state_machine.AddState
	(
		ParentState::Reaction,
		[this](const float elapsed_time) { ReactionState(elapsed_time); }
	);

	player_state_machine.AddState
	(
		ParentState::Receive,
		[this](const float elapsed_time) { ReceiveState(elapsed_time); }
	);

	//子ステートの追加

	//Entry
	player_entry_state.AddState
	(
		Entry::Select,
		[this](const float elapsed_time) {SelectState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Attack,
		[this](const float elapsed_time) {AttackState(elapsed_time); }
	);

	//player_entry_state.AddState
	//(
	//	 Entry::Menu,
	//	[this](const float elapsed_time) {MenuState(elapsed_time); }
	//);

	player_entry_state.AddState
	(
		Entry::WayChange,
		[this](const float elapsed_time) {WayChangeState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Move,
		[this](const float elapsed_time) {MoveState(elapsed_time); }
	);

	//Reaction

	player_reaction_state.AddState
	(
		Reaction::ReactionSelect,
		[this](const float elapsed_time) {ReactionSelectState(elapsed_time); }
	);

	player_reaction_state.AddState
	(
		Reaction::Damaged,
		[this](const float elapsed_time) {DamagedState(elapsed_time); }
	);

	player_reaction_state.AddState
	(
		Reaction::Death,
		[this](const float elapsed_time) {DeathState(elapsed_time); }
	);


	//Recieve

	player_receive_state.AddState
	(
		Receive::Wait,
		[this](const float elapsed_time) {WaitState(elapsed_time); }
	);

	player_receive_state.AddState
	(
		Receive::Called,
		[this](const float elapsed_time) {CalledState(elapsed_time); }
	);

	// 各初期ステートの設定
	player_state_machine.SetState(ParentState::Entry);

	player_entry_state.SetState(Entry::Select);
	player_reaction_state.SetState(Reaction::ReactionSelect);
	player_receive_state.SetState(Receive::Wait);

}

bool Player::OnMessage(const Telegram & telegram)
{
	// メタAIからの受信処理

	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		player_state_machine.SetState(ParentState::Entry);

		return true;
	}
	return false;
}
void Player::OnDamaged()
{
}

void Player::OnDead()
{
}


void Player::DrawDebugGUI()
{
	// 入力された情報を取得
	GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();


	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 位置
			ImGui::InputFloat3("Position", &this->position.x);
			// 回転
			DirectX::XMFLOAT3 a{};
			a.x = DirectX::XMConvertToDegrees(this->angle.x);
			a.y = DirectX::XMConvertToDegrees(this->angle.y);
			a.z = DirectX::XMConvertToDegrees(this->angle.z);
			ImGui::InputFloat3("Radian", &this->angle.x);
			ImGui::InputFloat3("Degree", &a.x);
			this->angle.x = DirectX::XMConvertToRadians(a.x);
			this->angle.y = DirectX::XMConvertToRadians(a.y);
			this->angle.z = DirectX::XMConvertToRadians(a.z);
			// スケール
			ImGui::InputFloat3("Scale", &this->scale.x);
		}
		// ID
		ImGui::Text("ID:%d", this->id);
		// ゲームパッドのスティック入力
		ImGui::Text("GamePadAxis: x:%f y:%f", ax, ay);
		// ゲームパッドのスティック入力のステップ
		if (ax > 0.f)
		{
			ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f));
		}
		else if (ax < 0.f)
		{
			ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
		}

		if (ay > 0.f)
		{
			ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
		}
		else if (ay < 0.f)
		{
			ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
		}
		ImGui::Text("GamePadAxisOnStep: x:%f y:%f", ax, ay);
		// 周囲のマップ情報
		const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / CellSize, GetPosition().z / CellSize);//データ上の値にするためCell_Sizeで割る
		const size_t up_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
		const size_t down_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ下の升目を見る
		const size_t right_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ右の升目を見る
		const size_t left_data = GetStageInformation()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ左の升目を見る

		ImGui::Text("OmniAttribute");
		ImGui::Text("            up:%zu", up_data);
		ImGui::Text("  left:%zu          right:%zu ", left_data, right_data);
		ImGui::Text("           down:%zu", down_data);

	}
	ImGui::End();

}

void Player::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debug_renderer->DrawSphere(this->position, this->radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

//----------------------------------------------------------------
//
// ステート
//
//----------------------------------------------------------------

//---------------------
// 親ステート
//---------------------

void Player::EntryState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())
	{
		// 子ステートのの初期化
		if (player_entry_state.GetState() != static_cast<int>(Entry::Select))
		{
			player_entry_state.SetState(Entry::Select);
		}
	}
	// サブステートの実行
	player_entry_state.Update(elapsed_time);

}

void Player::ReactionState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())
	{
		// 子ステートのの初期化
		if (player_reaction_state.GetState() != static_cast<int>(Reaction::ReactionSelect))
		{
			player_reaction_state.SetState(Reaction::ReactionSelect);
		}

	}

	// サブステートの実行
	player_reaction_state.Update(elapsed_time);


}

void Player::ReceiveState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())
	{
		// 子ステートのの初期化
		if (player_receive_state.GetState() != static_cast<int>(Receive::Wait))
		{
			player_receive_state.SetState(Receive::Wait);
		}
	}

	// サブステートの実行
	player_receive_state.Update(elapsed_time);

}

//---------------------
//子ステート
//---------------------

// EntryState

void	Player::SelectState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}

	GamePad& game_pad = Input::Instance().GetGamePad();


	//攻撃
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attackステートに遷移する
		player_entry_state.SetState(Entry::Attack);
	}

	//同時に処理をしたくないので else ifで1つtrueなら入らないようにする

	//メニュー
	//else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	//{
	//	//Menuステートに遷移する
	//	player_entry_state.SetState( Entry::Menu);
	//}

	//方向転換
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		//WayChangeステートに遷移する
		player_entry_state.SetState(Entry::WayChange);
	}

	//	移動
	//左スティックもしくはDPadが入力されたとき
	else if (game_pad.GetAxisLX() != 0 || game_pad.GetAxisLY() != 0)
	{
		//Moveステートに遷移する
		player_entry_state.SetState(Entry::Move);
	}

}

void	Player::AttackState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}
	//仮
	player_entry_state.SetState(Entry::Select);
	//player_state.SetState(ParentState::Receive);
}

//void	Player::MenuState(const float elapsed_time)
//{
//	if (player_entry_state.IsStateFirstTime())
//	{
//
//	}
//
//	// ステートの終了
//	if (false)
//	{
//
//	}
//}

void	Player::WayChangeState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}

	GamePad& game_pad = Input::Instance().GetGamePad();
	//Yボタンを長押ししている間
	if (game_pad.GetButtonUp() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		player_entry_state.SetState(Entry::Select);
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
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
	}

	//左スティックのy軸のステップ
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true);
	}

	//方向転換
	//上
	if (ax == 0.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(0));
	}
	//下
	else if (ax == 0.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(180));
	}
	//右
	else if (ax == 1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(90));
	}
	//左
	else if (ax == -1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(270));
	}

	//右上
	if (ax == 1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//左上
	else if (ax == -1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(315));
	}
	//左下
	else if (ax == -1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(225));
	}
	//右下
	else if (ax == 1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(135));
	}

	//攻撃
	else	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attackステートに遷移する
		player_entry_state.SetState(Entry::Attack);
	}

	// ステートの終了
	if (false)
	{

	}
}

void	Player::MoveState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{

	}

	GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	DirectX::XMFLOAT2 player_pos =//データ上の値にするためCell_Sizeで割る
		DirectX::XMFLOAT2(GetPosition().x / CellSize, GetPosition().z / CellSize);

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
		SetAngleY(Math::ConvertToRadianAngle(45));
		size_t map_data_diagonal = GetStageInformation()->//現在のステージの情報から一つ右上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//現在のステージの情報から一つ右の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//現在のステージの情報から一つ上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;


		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(CellSize);
			AddPositionX(CellSize);
		}
	}
	//左上
	else if (ax == -1.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(315));
		size_t map_data_diagonal = GetStageInformation()->//現在のステージの情報から一つ左上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//現在のステージの情報から一つ左の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//現在のステージの情報から一つ上の升目を見る
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(CellSize);
			AddPositionX(-CellSize);
		}
	}
	//左下
	else if (ax == -1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(225));
		size_t map_data_diagonal = GetStageInformation()->//現在のステージの情報から一つ左下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//現在のステージの情報から一つ左の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//現在のステージの情報から一つ下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(-CellSize);
			AddPositionX(-CellSize);
		}
	}
	//右下
	else if (ax == 1.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(135));
		size_t map_data_diagonal = GetStageInformation()->//現在のステージの情報から一つ右下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = GetStageInformation()->//現在のステージの情報から一つ右の升目を見る
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = GetStageInformation()->//現在のステージの情報から一つ下の升目を見る
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal == static_cast<size_t>(Attribute::Road) || map_data_diagonal == static_cast<size_t>(Attribute::Room)) &&
			(map_data_horizon == static_cast<size_t>(Attribute::Road) || map_data_horizon == static_cast<size_t>(Attribute::Room)) &&
			(map_data_vertical == static_cast<size_t>(Attribute::Road) || map_data_vertical == static_cast<size_t>(Attribute::Room))
			)
		{
			AddPositionZ(-CellSize);
			AddPositionX(CellSize);
		}
	}

	//上
	if (ax == 0.f && ay == 1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(0));
		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
		//壁か敵でないなら
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionZ(CellSize);
		}
	}
	//下
	else if (ax == 0.f && ay == -1.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(180));
		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ下の升目を見る
		//壁か敵でないなら
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionZ(-CellSize);
		}
	}
	//右
	else if (ax == 1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(90));
		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ右の升目を見る
	//壁か敵でないなら
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionX(CellSize);
		}
	}
	//左
	else if (ax == -1.f && ay == 0.f)
	{
		SetAngleY(Math::ConvertToRadianAngle(270));

		size_t map_data = GetStageInformation()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ左の升目を見る
		//壁か敵でないなら
		if (map_data == static_cast<size_t>(Attribute::Road) || map_data == static_cast<size_t>(Attribute::Room))
		{
			AddPositionX(-CellSize);
		}
	}
	//仮
	player_entry_state.SetState(Entry::Select);
	//player_state.SetState(ParentState::Receive);

}

// ReactionState

void Player::ReactionSelectState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
		if (current_health > 0)
		{
			player_reaction_state.SetState(Reaction::Damaged);
		}
		else
		{
			player_reaction_state.SetState(Reaction::Death);
		}
	}

}

void Player::DamagedState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
		//Meta& meta = Meta::Instance();
		//meta.SendMessaging(GetId(),
		//static_cast<int>(Meta::Identity::Meta),
		//MESSAGE_TYPE::MSG_END_PLAYER_TURN);
	}
	//player_state.SetState(ParentState::Entry);

}

void Player::DeathState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{

	}

	// ステートの終了
	if (false)
	{

	}
}

// ReceiveState

void Player::WaitState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())
	{
		//メタAIにターンの終了を伝える
		//Meta& meta = Meta::Instance();
		//meta.SendMessaging(owner->GetId(),
		//	static_cast<int>(Meta::Identity::Meta),
		//	MESSAGE_TYPE::MSG_END_PLAYER_TURN);
	}

	// ステートの終了
	if (false)
	{

	}
}

void Player::CalledState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())
	{

	}

}
