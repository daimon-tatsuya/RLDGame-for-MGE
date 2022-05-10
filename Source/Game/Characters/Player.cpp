//**********************************************************
//
//		Playerクラス
//
//**********************************************************

//#include <string>

#include  "Engine/AI/MetaAI.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Objects/Model.h"
#include "Engine/AI/DungeonMake.h"

#include "Game/Characters/Player.h"

const float cos45 = cosf(DirectX::XMConvertToRadians(45.f));

Player::Player()
{
	SetModel("Assets/FBX/Animals/BlackWidow.bin");

	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	SetPositionY(0.f);

	SetExist(true);

	// 初期ステート
	Player::FiniteStateMachineInitialize();

//オブジェクト配置
	const RogueLikeDungeon rogue_like_dungeon = RogueLikeDungeon::Instance();
	const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x)*CellSize, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CellSize);
	SetPosition(pos);
	//for (int y = 0; y < MapSize_Y; y++)
	//{
	//	if (GetIsDecidePos() == true)
	//	{
	//		break;
	//	}
	//	for (int x = 0; x < MapSize_X; x++)
	//	{
	//		if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Player))
	//		{
	//			const float pos_x = static_cast<float>(x * CellSize);
	//			const float pos_z = static_cast<float> (y * CellSize);
	//			SetPosition(DirectX::XMFLOAT3(pos_x, 0, pos_z));
	//			SetIsDecidePos(true);
	//			break;
	//		}
	//	}
	//}
}

Player::~Player()
= default;

void Player::Update(const float elapsed_time)
{
	SetPositionY(0.f);//ジャンプさせないので固定

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
	GetModel()->UpdateAnimation(elapsed_time);

	// モデル行列更新
	GetModel()->UpdateTransform(GetTransform());
}

void Player::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, GetModel());
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

	//player_receive_state.AddState
	//(
	//	Receive::Call,
	//	[this](const float elapsed_time) {CallState(elapsed_time); }
	//);

	// 各初期ステートの設定
	player_state_machine.SetState(ParentState::Entry);

	player_entry_state.SetState(Entry::Select);
	player_reaction_state.SetState(Reaction::ReactionSelect);
	player_receive_state.SetState(Receive::Wait);

	//ImGui
	player_entry_string.emplace_back("Select");
	player_entry_string.emplace_back("Attack");
	player_entry_string.emplace_back("WayChange");
	player_entry_string.emplace_back("Move");

	player_reaction_string.emplace_back("ReactionSelect");
	player_reaction_string.emplace_back("Damaged");
	player_reaction_string.emplace_back("Death");

	//	player_receive_string.emplace_back("Wait");
	player_receive_string.emplace_back("Called");

	player_states_string.emplace_back(player_entry_string);
	player_states_string.emplace_back(player_reaction_string);
	player_states_string.emplace_back(player_receive_string);
}

bool Player::OnMessage(const Telegram & telegram)
{
	// メタAIからの受信処理

	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		LOG("\n error: MESSAGE_TYPE::MSG_END_PLAYER_TURN Messages not received | MetaAI.cpp 207")
			return false;

	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		player_state_machine.SetState(ParentState::Entry);

		return true;

	default:

		LOG("\n error: No Message  | MetaAI.cpp 218")

			return false;
	}
}

void Player::SendMessaging(MESSAGE_TYPE msg)
{
	Meta& meta = Meta::Instance();

	switch (msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		//メタAIにターンの終了を伝える
		meta.SendMessaging(GetId(),
			static_cast<int>(Meta::Identity::Meta),
			MESSAGE_TYPE::MSG_END_PLAYER_TURN);

		//ステートマシンの設定
		player_entry_state.SetState(Entry::Select);
		player_state_machine.SetState(ParentState::Receive);
		break;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		LOG("\n error: No Function | MetaAI.cpp 243")
			break;

	default:
		LOG("\n No Message | MetaAI.cpp 247 ")
			break;
	}
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
	const GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// ID
		ImGui::Text("Character ID:%d", GetId());
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			DirectX::XMFLOAT3 pos_for_imgui = GetPosition();
			DirectX::XMFLOAT3 angle_for_imgui = GetAngle();
			DirectX::XMFLOAT3 scale_for_imgui = GetScale();
			// 位置
			ImGui::InputFloat3("Position", &pos_for_imgui.x);
			// 回転
			DirectX::XMFLOAT3 a{};
			a.x = DirectX::XMConvertToDegrees(angle_for_imgui.x);
			a.y = DirectX::XMConvertToDegrees(angle_for_imgui.y);
			a.z = DirectX::XMConvertToDegrees(angle_for_imgui.z);
			ImGui::InputFloat3("Radian", &angle_for_imgui.x);
			ImGui::InputFloat3("Degree", &a.x);
			//SetAngle(a);
			// スケール
			ImGui::InputFloat3("Scale", &scale_for_imgui.x);

		}

		//現在のステートを表示
		if (ImGui::CollapsingHeader("Current State", ImGuiTreeNodeFlags_DefaultOpen))
		{
			char parent_state_name[64] = "empty";
			char current_state_name[64] = "empty";

			//	player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at
			//	(static_cast<u_int>(player_entry_state.GetState()));

			//!関数化

			if (player_state_machine.GetState() == static_cast<int>(ParentState::Entry))
			{
				//"empty"の文字列を削除
				memset(current_state_name, '\0', sizeof(current_state_name));
				memset(parent_state_name, '\0', sizeof(parent_state_name));

				//ステート名を入れる
				strcat_s(parent_state_name, sizeof(parent_state_name), "Entry");
				const char* state_name = player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at(static_cast<u_int>(player_entry_state.GetState())).c_str();
				strcat_s(current_state_name, sizeof(current_state_name), state_name);
			}

			if (player_state_machine.GetState() == static_cast<int>(ParentState::Reaction))
			{
				//"empty"の文字列を削除
				memset(current_state_name, '\0', sizeof(current_state_name));
				memset(parent_state_name, '\0', sizeof(parent_state_name));

				//ステート名を入れる
				strcat_s(parent_state_name, sizeof(parent_state_name), "Reaction");
				const char* state_name = player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at(static_cast<u_int>(player_reaction_state.GetState())).c_str();
				strcat_s(current_state_name, sizeof(current_state_name), state_name);
			}

			if (player_state_machine.GetState() == static_cast<int>(ParentState::Receive))
			{
				//"empty"の文字列を削除
				memset(current_state_name, '\0', sizeof(current_state_name));
				memset(parent_state_name, '\0', sizeof(parent_state_name));

				//ステート名を入れる
				strcat_s(parent_state_name, sizeof(parent_state_name), "Receive");
				const char* state_name = player_states_string.at(static_cast<u_int>(player_state_machine.GetState())).at(static_cast<u_int>(player_receive_state.GetState())).c_str();
				strcat_s(current_state_name, sizeof(current_state_name), state_name);
			}

			ImGui::Text("Parent State Name:%s", parent_state_name);
			ImGui::Text("Current State Name:%s", current_state_name);
		}
		if (ImGui::CollapsingHeader("Game Pad Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// ゲームパッドのスティック入力
			ImGui::Text("Game Pad Axis: x:%f y:%f", ax, ay);
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
			ImGui::Text("Game Pad Axis On Step: x:%f y:%f", ax, ay);
		}
		if (ImGui::CollapsingHeader("Player Omni Attribute", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 周囲のマップ情報
			RogueLikeDungeon& rogue_like_dungeon =RogueLikeDungeon::Instance();
			const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / CellSize, GetPosition().z / CellSize);//データ上の値にするためCell_Sizeで割る
			const size_t up_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
			const size_t down_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ下の升目を見る
			const size_t right_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ右の升目を見る
			const size_t left_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ左の升目を見る

			ImGui::Text("Omni Attribute");
			ImGui::Text("            up:%zu", up_data);
			ImGui::Text("  left:%zu          right:%zu ", left_data, right_data);
			ImGui::Text("           down:%zu", down_data);
		}
		if (ImGui::CollapsingHeader("Player initialize Position", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//プレイヤーのマップ情報上での初期位置
			ImGui::Text("Player Map Position: 	%d %d",
				RogueLikeDungeon::Instance().player_pos.x, RogueLikeDungeon::Instance().player_pos.y);

			//プレイヤーの初期位置
			const int player_positionX = RogueLikeDungeon::Instance().player_pos.x * CellSize;
			const int player_positionY = RogueLikeDungeon::Instance().player_pos.y * CellSize;

			ImGui::Text("Player Position:	 %d %d", player_positionX, player_positionY);
		}
	}
	ImGui::End();
}

void Player::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debug_renderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

bool Player::IsMoved()
{
	if (Math::Comparison(GetPosition().x, GetOldPosition().x) || Math::Comparison(GetPosition().z, GetOldPosition().z))//trueなら動いていない
	{
		return false;
	}
	//今回は動かす予定がないので削除
	//if (Math::Comparison(position.y, old_position.y))
	//{
	//	return true;
	//}
	return true;
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

}

void	Player::WayChangeState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())
	{
	}

	const GamePad& game_pad = Input::Instance().GetGamePad();
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

}


void	Player::MoveState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//一度だけ実行
	{
	}
	//SetOldPosition();//前回の位置を保存

	const GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	const DirectX::XMFLOAT2 player_pos =//データ上の値にするためCell_Sizeで割る
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
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(45));

		const size_t map_data_diagonal = rogue_like_dungeon.//現在のステージの情報から一つ右上の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//現在のステージの情報から一つ右の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//現在のステージの情報から一つ上の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//壁か敵でないなら
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//右上に移動
			AddPositionZ(CellSize);
			AddPositionX(CellSize);

			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//左上
	else if (ax == -1.f && ay == 1.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(315));

		const size_t map_data_diagonal = rogue_like_dungeon.//現在のステージの情報から一つ左上の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//現在のステージの情報から一つ左の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//現在のステージの情報から一つ上の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//進行方向が移動可能な属性かをチェック
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//左上に移動
			AddPositionZ(CellSize);
			AddPositionX(-CellSize);
			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//左下
	else if (ax == -1.f && ay == -1.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(225));

		const size_t map_data_diagonal = rogue_like_dungeon.//現在のステージの情報から一つ左下の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//現在のステージの情報から一つ左の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//現在のステージの情報から一つ下の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//進行方向が移動可能な属性かをチェック
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//左下に移動
			AddPositionZ(-CellSize);
			AddPositionX(-CellSize);

			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//右下
	else if (ax == 1.f && ay == -1.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(135));

		const size_t map_data_diagonal = rogue_like_dungeon.//現在のステージの情報から一つ右下の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_horizon = rogue_like_dungeon.//現在のステージの情報から一つ右の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		const size_t map_data_vertical = rogue_like_dungeon.//現在のステージの情報から一つ下の升目を見る
			GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//進行方向が移動可能な属性かをチェック
		//ななめの移動なので上下左右の位置も確認する
		if (
			(map_data_diagonal <= static_cast<size_t>(Attribute::Road) && map_data_diagonal > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_horizon <= static_cast<size_t>(Attribute::Road) && map_data_horizon > static_cast<size_t>(Attribute::Wall)) &&
			(map_data_vertical <= static_cast<size_t>(Attribute::Road) && map_data_vertical > static_cast<size_t>(Attribute::Wall))
			)
		{
			//右下に移動
			AddPositionZ(-CellSize);
			AddPositionX(CellSize);

			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}

	//上
	if (ax == 0.f && ay == 1.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(0));

		const size_t map_data = rogue_like_dungeon.
		GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る

		//進行方向が移動可能な属性かをチェック
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//上に移動
			AddPositionZ(CellSize);

			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//下
	else if (ax == 0.f && ay == -1.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();


		SetAngleY(Math::ConvertToRadianAngle(180));
		const size_t map_data =rogue_like_dungeon.
		GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ下の升目を見る

		//進行方向が移動可能な属性かをチェック
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//下に移動
			AddPositionZ(-CellSize);
			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//右
	else if (ax == 1.f && ay == 0.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(90));

		const size_t map_data = rogue_like_dungeon.
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ右の升目を見る

		//進行方向が移動可能な属性かをチェック
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//右に移動
			AddPositionX(CellSize);
			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}
	}
	//左
	else if (ax == -1.f && ay == 0.f)
	{
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		SetAngleY(Math::ConvertToRadianAngle(270));

		const size_t map_data = rogue_like_dungeon.
			GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ左の升目を見る

		//進行方向が移動可能な属性かをチェック
		if (map_data <= static_cast<size_t>(Attribute::Road) && map_data > static_cast<size_t>(Attribute::Wall))
		{
			//左に移動
			AddPositionX(-CellSize);
			//プレイヤーの行動を終了する
			SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
			player_state_machine.SetState(ParentState::Receive);
		}

	}
	player_entry_state.SetState(Entry::Select);
}


// ReactionState

void Player::ReactionSelectState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
		if (GetCurrentHealth() > 0)
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

	}

}

void Player::DeathState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())
	{
	}

}

// ReceiveState

void Player::WaitState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())
	{

	}

}



//void Player::CallState(const float elapsed_time)
//{
//	if (player_receive_state.IsStateFirstTime())
//	{
//		//プレイヤーが行動したことを伝える
//		SendMessaging(MESSAGE_TYPE::MSG_END_PLAYER_TURN);
//		//行動ステートを選択するステートに戻す
//		player_entry_state.SetState(Entry::Select);
//	}
//	//待機ステートに移行
//	player_receive_state.SetState(Receive::Wait);
//}