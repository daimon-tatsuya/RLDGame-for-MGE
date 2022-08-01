//**********************************************************
//
//		EnemySnakeクラス
//
//**********************************************************

#include "Engine/AI/MetaAI.h"
#include "Game/Characters/EnemySnake.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Logger.h"
#include "Engine/Systems/Shader.h"

#include "Engine/AI/HeuristicSearch.h"

EnemySnake::EnemySnake()
{
	SetModel("Assets/FBX/Animals/Rattlesnake.bin");
	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	SetPositionY(0.f);
	SetExist(true);

	//初期ステート
	EnemySnake::FiniteStateMachineInitialize();

	//オブジェクト配置
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		//	設定したらループを抜ける
		if (GetIsDecidePos() == true)
		{
			break;
		}
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				const float posX = static_cast<float>(x * CELL_SIZE);
				const float posZ = static_cast<float> (y * CELL_SIZE);

				SetPosition(DirectX::XMFLOAT3(posX, 0, posZ));
				SetIsDecidePos(true);
				break;
			}
		}
	}
}

EnemySnake::~EnemySnake()
{
	LOG(" Executed : EnemySnake's destructor\n")
}

void EnemySnake::Update(float elapsed_time)
{
	SetPositionY(0.f);//ジャンプさせないので固定

	enemy_snake_state_machine.Update(elapsed_time);

	//回転角の正規化
	NormalizeAngle();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	GetModel()->UpdateTransform(GetTransform());
}

void EnemySnake::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, GetModel());
}

void EnemySnake::FiniteStateMachineInitialize()
{
	//親ステートの追加

	enemy_snake_state_machine.AddState
	(
		ParentState::Entry,
		[this](const float elapsed_time) { EntryState(elapsed_time); }
	);

	enemy_snake_state_machine.AddState
	(
		ParentState::Reaction,
		[this](const float elapsed_time) { ReactionState(elapsed_time); }
	);

	enemy_snake_state_machine.AddState
	(
		ParentState::Receive,
		[this](const float elapsed_time) { ReceiveState(elapsed_time); }
	);

	//子ステートの追加

	//Entry

	enemy_snake_entry_state.AddState
	(
		Entry::Select,
		[this](const float elapsed_time) {SelectState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Approach,
		[this](const float elapsed_time) {ApproachState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Explore,
		[this](const float elapsed_time) {ExploreState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Attack,
		[this](const float elapsed_time) {AttackState(elapsed_time); }
	);

	enemy_snake_entry_state.AddState
	(
		Entry::Ability,
		[this](const float elapsed_time) {AbilityState(elapsed_time); }
	);

	//ReactionState

	enemy_snake_reaction_state.AddState
	(
		Reaction::ReactionSelect,
		[this](const float elapsed_time) {ReactionSelectState(elapsed_time); }
	);

	enemy_snake_reaction_state.AddState
	(
		Reaction::Damaged,
		[this](const float elapsed_time) {DamagedState(elapsed_time); }
	);

	enemy_snake_reaction_state.AddState
	(
		Reaction::Death,
		[this](const float elapsed_time) {DeathState(elapsed_time); }
	);

	//Receive

	//enemy_snake_receive_state.AddState
	//(
	//	Receive::Wait,
	//	[this](const float elapsed_time) {WaitState(elapsed_time); }
	//);

	enemy_snake_receive_state.AddState
	(
		Receive::Called,
		[this](const float elapsed_time) {CalledState(elapsed_time); }
	);

	// 各初期ステートの設定
	enemy_snake_state_machine.SetState(ParentState::Receive);

	enemy_snake_entry_state.SetState(Entry::Select);
	enemy_snake_reaction_state.SetState(Reaction::ReactionSelect);
	enemy_snake_receive_state.SetState(Receive::Called);
}

void EnemySnake::Destroy()
{
	//キャラクターマネージャーのリストから自身を消去
	Character::Destroy();
}

void EnemySnake::DrawDebugGUI()
{
}

void EnemySnake::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debug_renderer->DrawSphere(GetPosition(), GetRadius(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

void EnemySnake::SendMessaging(MESSAGE_TYPE msg)
{
	Meta& meta = Meta::Instance();

	switch (msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		LOG(" Error : END_PLAYER_TURN is No Function | EnemySnake.cpp SendMessaging Method\n")
			break;

	case MESSAGE_TYPE::END_ENEMY_TURN:

		//メタAIにターンの終了を伝える
		meta.SendMessaging(GetId(),
			static_cast<int>(Identity::Meta),
			MESSAGE_TYPE::END_ENEMY_TURN);

		//ステートマシンの設定
		enemy_snake_state_machine.SetState(ParentState::Receive);
		break;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		LOG(" Error : GO_NEXT_FLOOR is No Function | EnemySnake.cpp SendMessaging Method\n")
			break;

	case MESSAGE_TYPE::GO_MAX_FLOOR:

		LOG(" Error : GO_MAX_FLOOR is No Function | EnemySnake.cpp SendMessaging Method\n")
			break;

	default:
		LOG(" Error : No Message | EnemySnake.cpp SendMessaging Method\n")
			break;
	}
}

void EnemySnake::OnDamaged()
{
}

void EnemySnake::OnDead()
{
}

bool EnemySnake::OnMessage(const Telegram& telegram)
{
	//メタAIからの受信処理
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:

		enemy_snake_state_machine.SetState(ParentState::Entry);

		return true;

	case MESSAGE_TYPE::END_ENEMY_TURN:

		LOG(" Error : END_ENEMY_TURN is No Function | EnemySnake.cpp OnMessage Method\n")

			return true;

	case MESSAGE_TYPE::GO_NEXT_FLOOR:

		SetExist(false);
		//Destroy();
		return true;

	case MESSAGE_TYPE::GO_MAX_FLOOR:


		return true;

	default:

		LOG(" Error : No Message | EnemySnake.cpp OnMessage Method\n")

			return false;

	}
}

//----------------------------------------------------------------
//
// ステート
//
//----------------------------------------------------------------

//---------------------
// 親ステート
//---------------------

void EnemySnake::EntryState(const float elapsed_time)
{
	if (enemy_snake_state_machine.IsStateFirstTime())
	{
		// 子ステートのの初期化
		if (enemy_snake_entry_state.GetState() != static_cast<int>(Entry::Select))
		{
			enemy_snake_entry_state.SetState(Entry::Select);
		}
	}
	// サブステートの実行
	enemy_snake_entry_state.Update(elapsed_time);
}

void EnemySnake::ReactionState(const float elapsed_time)
{
	if (enemy_snake_state_machine.IsStateFirstTime())
	{
		// 子ステートのの初期化
		if (enemy_snake_reaction_state.GetState() != static_cast<int>(Reaction::ReactionSelect))
		{
			enemy_snake_reaction_state.SetState(Reaction::ReactionSelect);
		}
	}
	// サブステートの実行
	enemy_snake_reaction_state.Update(elapsed_time);
}

void EnemySnake::ReceiveState(const float elapsed_time)
{
	if (enemy_snake_state_machine.IsStateFirstTime())
	{
		// 子ステートのの初期化
		if (enemy_snake_receive_state.GetState() != static_cast<int>(Receive::Called))
		{
			enemy_snake_receive_state.SetState(Receive::Called);
		}
	}
	// サブステートの実行
	enemy_snake_receive_state.Update(elapsed_time);
}

//---------------------
//子ステート
//---------------------

	//?EntryState

void EnemySnake::SelectState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
	enemy_snake_entry_state.SetState(Entry::Approach);
}

void EnemySnake::ApproachState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}

	SendMessaging(MESSAGE_TYPE::END_ENEMY_TURN);
}

void EnemySnake::ExploreState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
		shortest_path->Clear();
		//探索先を決める
		RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

		DirectX::XMINT2 far_pos{};//自身より最も遠い位置を保存する

		float most_long_length{};//最も長い距離を格納する変数

		const DirectX::XMFLOAT2 enemy_map_pos =
			DirectX::XMFLOAT2(GetPosition().x / CELL_SIZE, GetPosition().z / CELL_SIZE);//マップ情報上の自身の位置

		//const DirectX::XMFLOAT2 enemy_map_pos =
		//	DirectX::XMFLOAT2(GetPosition().x / CELL_SIZE, GetPosition().z / CELL_SIZE);//マップ情報上の自身の位置

		for (const auto& room_center : rogue_like_dungeon.GetRoomsCenter())
		{
			DirectX::XMFLOAT2 room =//XMINT2がXMVECTORに出来なかったのでXMFLOAT2にキャスト
				DirectX::XMFLOAT2(static_cast<float>(room_center.x), static_cast<float>(room_center.y));

			float length = //部屋の中心と自身との二点間の距離
				DirectX::XMVectorGetX(
					DirectX::XMVector2Length(
						DirectX::XMVectorSubtract(
							DirectX::XMLoadFloat2(&room),
							DirectX::XMLoadFloat2(&enemy_map_pos)
						)
					)
				);

			if (most_long_length < length)//最も遠いなら更新する
			{
				most_long_length = length;
				far_pos = room_center;
			}
		}

		int start_id = HeuristicSearch::Instance().ConvertWorldToOneDimensional(far_pos);//Asterのスタート地点
		int goal_id = HeuristicSearch::Instance().ConvertWorldToOneDimensional(far_pos);//ASterのゴール地点

		//shortest_path->path_information = HeuristicSearch::Instance().Search(start_id, goal_id);

	}

	int destination_position =//移動先
		shortest_path->path_information[static_cast<int>(shortest_path->path_information.size() - shortest_path->path_index)];//末から引き出す

	DirectX::XMINT2 destination_map_position = HeuristicSearch::Instance().ConvertMapPosition(destination_position); //

	shortest_path->path_index++;//パスのインデックスをインクリメント
}

void EnemySnake::AttackState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
}

void EnemySnake::AbilityState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{
	}
}

//?ReactionState

void EnemySnake::ReactionSelectState(const float elapsed_time)
{
	if (enemy_snake_reaction_state.IsStateFirstTime())
	{
	}
}
void EnemySnake::DamagedState(const float elapsed_time)
{
	if (enemy_snake_reaction_state.IsStateFirstTime())
	{
	}
}

void EnemySnake::DeathState(const float elapsed_time)
{
	if (enemy_snake_reaction_state.IsStateFirstTime())
	{
	}
}

//?ReceiveState

void EnemySnake::WaitState(const float elapsed_time)
{
	if (enemy_snake_receive_state.IsStateFirstTime())
	{

	}
}

void EnemySnake::CalledState(const float elapsed_time)
{
	if (enemy_snake_receive_state.IsStateFirstTime())
	{
	}
}