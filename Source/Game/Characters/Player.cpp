//**********************************************************
//
//		Playerクラス
//
//**********************************************************

#include "Game/Characters/Player.h"

<<<<<<< HEAD
#include  "Liblary/AI/MetaAI.h"
#include "Liblary/Systems/Logger.h"
#include "Liblary/Systems/Input.h"
#include "Liblary/Systems/Graphics.h"
#include "Liblary/Systems/Shader.h"
#include "Liblary/Systems/ShaderManager.h"
#include "Liblary/Systems/DebugRenderer.h"
#include "Liblary/Systems/ImGuiRenderer.h"
#include "Liblary/Objects/Model.h"
#include "Liblary/AI/DungeonMake.h"



#include "Liblary/Systems/CharacterManager.h"
=======
#include  "MyLiblary/AI/MetaAI.h"
#include "MyLiblary/Systems/Logger.h"
#include "MyLiblary/Systems/Input.h"
#include "MyLiblary/Systems/Graphics.h"
#include "MyLiblary/Systems/Shader.h"
#include "MyLiblary/Systems/ShaderManager.h"
#include "MyLiblary/Systems/DebugRenderer.h"
#include "MyLiblary/Systems/ImGuiRenderer.h"
#include "MyLiblary/Objects/Model.h"
#include "MyLiblary/AI/DungeonMake.h"



#include "MyLiblary/Systems/CharacterManager.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3

const float COS45 = cosf(DirectX::XMConvertToRadians(45.f));

//コンストラクタ
Player::Player()
{
	SetModel("Assets/FBX/Animals/BlackWidow.bin");

	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));

	SetPositionY(0.f);//	今回のゲームはy軸に動かない

	SetExist(true);

	// ステートマシンの初期化
	//コンストラクタ内の仮想関数の呼び出しはコンパイル時に解決される
	//なのでコンストラクタ内の仮想関数の呼び出し明示的に指定する
	Player::FiniteStateMachineInitialize();

	//位置の設定
	SetFirstPos();

}

//デストラクタ
Player::~Player()
{
	LOG(" Executed : Player's destructor\n")
}

//	更新処理
void Player::Update(const float elapsed_time)
{
	SetPositionY(0.f);//ジャンプさせないので固定

	//ステートマシンの更新
	player_state_machine.Update(elapsed_time);

	// 回転角の正規化
	NormalizeAngle();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデルアニメーション更新処理
	GetModel()->UpdateAnimation(elapsed_time);

	// モデル行列更新
	GetModel()->UpdateTransform(GetTransform());

	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

	//プレイヤーと階段の位置が重なったら
	//階を進める
	if (static_cast<int>(GetPosition().x / CELL_SIZE) == rogue_like_dungeon.stairs_pos.x &&
		static_cast<int>(GetPosition().z) / CELL_SIZE == rogue_like_dungeon.stairs_pos.y)
	{
		SendMessaging(MESSAGE_TYPE::GO_NEXT_FLOOR);
	}
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, GetModel());
}

//ステートマシの初期化
void Player::FiniteStateMachineInitialize()
{
	//--------------------------
	//親ステートの追加
	//--------------------------
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
	//--------------------------
	//子ステートの追加
	//--------------------------

	//EntryStateの子ステート

	player_entry_state.AddState
	(
		Entry::Wait,
		[this](const float elapsed_time) {WaitState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Move,
		[this](const float elapsed_time) {MoveState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Stop,
		[this](const float elapsed_time) {StopState(elapsed_time); }
	);


	player_entry_state.AddState
	(
		Entry::Jump,
		[this](const float elapsed_time) {JumpState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Landing,
		[this](const float elapsed_time) {JumpState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Avoid,
		[this](const float elapsed_time) {AvoidState((elapsed_time)); }
	);

	player_entry_state.AddState
	(
		Entry::Attack,
		[this](const float elapsed_time) {AttackState(elapsed_time); }
	);

	player_entry_state.AddState
	(
		Entry::Counter,
		[this](const float elapsed_time) {CounterState(elapsed_time); }
	);
	//ReactionStateの子ステート

	player_reaction_state.AddState
	(
		Reaction::ReactionSelect,
		[this](const float elapsed_time) {ReactionSelectState(elapsed_time); }
	);

	player_reaction_state.AddState
	(
		Reaction::Countered,
		[this](const float elapsed_time) {CounteredState(elapsed_time); }
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

	//Receiveの子ステート

	player_receive_state.AddState
	(
		Receive::WaitReaction,
		[this](const float elapsed_time) {WaitReactionState(elapsed_time); }
	);

	// 各ステートマシンの初期ステートの設定

	player_state_machine.SetState(ParentState::Entry);

	player_entry_state.SetState(Entry::Wait);
	player_reaction_state.SetState(Reaction::ReactionSelect);
	player_receive_state.SetState(Receive::WaitReaction);


	//ImGui
	player_entry_string.emplace_back("Wait");
	player_entry_string.emplace_back("Move");
	player_entry_string.emplace_back("Stop");
	player_entry_string.emplace_back("Jump");
	player_entry_string.emplace_back("Avoid");
	player_entry_string.emplace_back("Attack");
	player_entry_string.emplace_back("Counter");

	player_reaction_string.emplace_back("ReactionSelect");
	player_reaction_string.emplace_back("Countered");
	player_reaction_string.emplace_back("Damaged");
	player_reaction_string.emplace_back("Death");

	player_receive_string.emplace_back("WaitReaction");

	player_states_string.emplace_back(player_entry_string);
	player_states_string.emplace_back(player_reaction_string);
	player_states_string.emplace_back(player_receive_string);
}

//	メタAI経由受信処理
bool Player::OnMessage(const Telegram& telegram)
{
	const RogueLikeDungeon rogue_like_dungeon = RogueLikeDungeon::Instance();
	// メタAIからの受信処理
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		LOG(" Error : END_PLAYER_TURN is No Function | Player.cpp OnMessage Method\n")
			return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:
		//プレイヤー
		player_state_machine.SetState(ParentState::Entry);

		return true;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		SetPositionY(0.f);

		const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x) * CELL_SIZE, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CELL_SIZE);

		SetPosition(pos);

		return true;

	case MESSAGE_TYPE::GO_MAX_FLOOR:

		LOG(" Error : GO_MAX_FLOOR is No Function  | Player.cpp OnMessage Method\n ")

			return true;

	default:

		LOG(" Error : No Message  | Player.cpp  OnMessage Method\n ")

			return false;

	}
}

//メタAIにメッセージを送信する
void Player::SendMessaging(MESSAGE_TYPE msg)
{
	Meta& meta = Meta::Instance();

	switch (msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		//メタAIにターンの終了を伝える
		meta.SendMessaging(GetId(),
			static_cast<int>(Identity::Meta),
			MESSAGE_TYPE::END_PLAYER_TURN);

		//ステートマシンの設定
		player_entry_state.SetState(Entry::Wait);
		player_state_machine.SetState(ParentState::Receive);
		break;
	case MESSAGE_TYPE::END_ENEMY_TURN:

		LOG(" Error : No Function | Player.cpp  MESSAGE_TYPE::END_ENEMY_TURN\n")
			break;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		meta.SendMessaging(
			GetId(),
			static_cast<int>(Identity::Meta),
			MESSAGE_TYPE::GO_NEXT_FLOOR
		);


		//ステートマシンの設定
		player_state_machine.SetState(ParentState::Receive);

		break;

	default:
		LOG(" Error : No Message | Player.cpp   SendMessaging\n ")
			break;

	}
}

//ダメージを受けたときに呼ばれる
void Player::OnDamaged()
{
}

//ダメージを受け、helathが0以下の時に呼ばれるときに呼ばれる
void Player::OnDead()
{
}

//ImGui表示用関数
void Player::DrawDebugGUI()
{
	// 入力された情報を取得
	const GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
	{
		// ID
		ImGui::Text("Character ID : %d", GetId());
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			DirectX::XMFLOAT3 pos_for_imgui = GetPosition();
			DirectX::XMFLOAT3 old_pos_for_imgui = GetOldPosition();
			DirectX::XMFLOAT3 angle_for_imgui = GetAngle();
			DirectX::XMFLOAT3 scale_for_imgui = GetScale();

			// 位置
			ImGui::InputFloat3("Position", &pos_for_imgui.x);
			// 前フレームの位置
			ImGui::InputFloat3("OldPosition", &old_pos_for_imgui.x);
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

			ImGui::Text("Parent State Name : %s", parent_state_name);
			ImGui::Text("Current State Name : %s", current_state_name);
		}
		if (ImGui::CollapsingHeader("Game Pad Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// ゲームパッドのスティック入力
			ImGui::Text("Game Pad Axis: x:%f y:%f", ax, ay);
			// ゲームパッドのスティック入力のステップ
			if (ax > 0.f)
			{
				ax = Math::StepAnyFloat(game_pad.GetAxisLX(), COS45, (COS45 / 2.f), 1.f - (COS45 / 2.f));
			}
			else if (ax < 0.f)
			{
				ax = Math::StepAnyFloat(game_pad.GetAxisLX(), COS45, -1.f + (COS45 / 2.f), (COS45 / 2.f), true);
			}

			if (ay > 0.f)
			{
				ay = Math::StepAnyFloat(game_pad.GetAxisLY(), COS45, (COS45 / 2.f), 1.f - (COS45 / 2.f)); ;
			}
			else if (ay < 0.f)
			{
				ay = Math::StepAnyFloat(game_pad.GetAxisLY(), COS45, -1.f + (COS45 / 2.f), (COS45 / 2.f), true);
			}
			ImGui::Text("Game Pad Axis On Step: x : %f y : %f", ax, ay);
		}
		if (ImGui::CollapsingHeader("Player Map Status", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// 周囲のマップ情報
			RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
			const DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(GetPosition().x / CELL_SIZE, GetPosition().z / CELL_SIZE);//データ上の値にするためCell_Sizeで割る
			const size_t up_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ上の升目を見る
			const size_t down_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//現在のステージの情報から一つ下の升目を見る
			const size_t right_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//現在のステージの情報から一つ右の升目を見る
			const size_t left_data = rogue_like_dungeon.GetMapRole()[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//現在のステージの情報から一つ左の升目を見る

			ImGui::Text("Omni Attribute");
			ImGui::Text("            up : %zu", up_data);
			ImGui::Text("  left : %zu          right : %zu ", left_data, right_data);
			ImGui::Text("           down : %zu", down_data);
			ImGui::Text("");
			if (rogue_like_dungeon.map_room_player >= rogue_like_dungeon.dungeon_map_role.map_room_count ||
				rogue_like_dungeon.map_room_player < 0)
			{
				ImGui::Text("Be RoomNumber : -1");
			}
			else
			{
				ImGui::Text("Be RoomNumber : %zu", rogue_like_dungeon.map_room_player);
			}

		}
		if (ImGui::CollapsingHeader("Player initialize Position", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//プレイヤーのマップ情報上での初期位置
			ImGui::Text("Player Map Position : x : %d  y : %d",
				RogueLikeDungeon::Instance().player_pos.x, RogueLikeDungeon::Instance().player_pos.y);

			//プレイヤーの初期位置
			const int player_positionX = RogueLikeDungeon::Instance().player_pos.x * CELL_SIZE;
			const int player_positionY = RogueLikeDungeon::Instance().player_pos.y * CELL_SIZE;

			ImGui::Text("Player Position : x : %d  y : %d", player_positionX, player_positionY);
		}
	}
	ImGui::End();
}

//
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
	return true;
}

void Player::SetFirstPos()
{
	const RogueLikeDungeon rogue_like_dungeon = RogueLikeDungeon::Instance();
	const DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(static_cast<float>(rogue_like_dungeon.player_pos.x) * CELL_SIZE, 0, static_cast<float>(rogue_like_dungeon.player_pos.y) * CELL_SIZE);
	SetPosition(pos);
}

void Player::Destroy()
{
	CharacterManager::Instance().Remove(this);
}

//	*********************************************
//
// ステート
//
//	*********************************************

//---------------------
// 親ステート
//---------------------

//----------------------------------------------------
//	EntryState
//----------------------------------------------------
void Player::EntryState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())//一度だけ実行
	{
		// 子ステートのの初期化
		if (player_entry_state.GetState() != static_cast<int>(Entry::Wait))
		{
			player_entry_state.SetState(Entry::Wait);

		}
	}
	// サブステートの実行
	player_entry_state.Update(elapsed_time);
}

//----------------------------------------------------
//	ReactionState
//----------------------------------------------------
void Player::ReactionState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())//一度だけ実行
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

//----------------------------------------------------
//	ReceiveState
//----------------------------------------------------
void Player::ReceiveState(const float elapsed_time)
{
	if (player_state_machine.IsStateFirstTime())//一度だけ実行
	{
		// 子ステートのの初期化
		if (player_receive_state.GetState() != static_cast<int>(Receive::WaitReaction))
		{
			player_receive_state.SetState(Receive::WaitReaction);
		}
	}

	// サブステートの実行
	player_receive_state.Update(elapsed_time);
}

//---------------------
//子ステート
//---------------------

//? EntryState

//----------------------------------------------------
//	WaitState
//----------------------------------------------------
void	Player::WaitState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//一度だけ実行
	{
	}
}

//----------------------------------------------------
//	MoveState
//----------------------------------------------------
void	Player::MoveState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//一度だけ実行
	{
	}
}

//----------------------------------------------------
//	StopState
//----------------------------------------------------
void Player::StopState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//一度だけ実行
	{

	}
}

//----------------------------------------------------
//	JumpState
//----------------------------------------------------
void	Player::JumpState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//一度だけ実行
	{

	}
}

void Player::LandingState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
	{

	}
}

void Player::AvoidState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
	{

	}
}

//----------------------------------------------------
//	AttackState
//----------------------------------------------------
void	Player::AttackState(const float elapsed_time)
{
	if (player_entry_state.IsStateFirstTime())//一度だけ実行
	{

	}
}


void Player::CounterState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
	{

	}
}

//? ReactionState

//----------------------------------------------------
//	ReactionSelectState
//----------------------------------------------------
void Player::ReactionSelectState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
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



void Player::CounteredState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
	{

	}
}

//----------------------------------------------------
//	DamagedState
//----------------------------------------------------
void Player::DamagedState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
	{

	}
	OnDamaged();
}

//----------------------------------------------------
//	DeathState
//----------------------------------------------------
void Player::DeathState(const float elapsed_time)
{
	if (player_reaction_state.IsStateFirstTime())//一度だけ実行
	{
	}
	OnDead();
}

//? ReceiveState

//----------------------------------------------------
//	WaitReactionState
//----------------------------------------------------
void Player::WaitReactionState(const float elapsed_time)
{
	if (player_receive_state.IsStateFirstTime())//一度だけ実行
	{

	}
	//何もしない
}

