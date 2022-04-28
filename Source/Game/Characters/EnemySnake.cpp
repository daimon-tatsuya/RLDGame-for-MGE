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

EnemySnake::EnemySnake(RogueLikeDungeon* rogue_like_dungeon)
{
	SetModel("Assets/FBX/Animals/Rattlesnake.bin");
	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	SetPositionY(0.f);
	SetExist(true);
	stage_information = rogue_like_dungeon;

	//初期ステート
	EnemySnake::FiniteStateMachineInitialize();

	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		if (GetIsDecidePos() == true)
		{
			break;
		}
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_information->map_role[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				const float pos_x = static_cast<float>(x * CellSize);
				const float pos_z = static_cast<float> (y * CellSize);

				SetPosition(DirectX::XMFLOAT3(pos_x, 0, pos_z));
				SetIsDecidePos(true);
				break;
			}
		}
	}
}

EnemySnake::~EnemySnake() = default;

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

void EnemySnake::Render(ID3D11DeviceContext * dc, std::shared_ptr<Shader> shader)
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

//void EnemySnake::Destroy()
//{
//	CharacterManager& character_manager = CharacterManager::Instance();
//	//キャラクターマネージャーのリストから消去
//	//character_manager.Remove(this);
//}

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
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		LOG("\n error: No Function")
			break;

	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		//メタAIにターンの終了を伝える

		meta.SendMessaging(GetId(),
			static_cast<int>(Meta::Identity::Meta),
			MESSAGE_TYPE::MSG_END_ENEMY_TURN);
		//ステートマシンの設定
		enemy_snake_state_machine.SetState(ParentState::Receive);
		break;
	default:
		LOG("\n No Message")
			break;
	}
}

void EnemySnake::OnDamaged()
{
}

void EnemySnake::OnDead()
{
}

bool EnemySnake::OnMessage(const Telegram & telegram)
{
	//メタAIからの受信処理
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

		enemy_snake_state_machine.SetState(ParentState::Entry);

		return true;
	case MESSAGE_TYPE::MSG_END_ENEMY_TURN:

		LOG("\n error: MESSAGE_TYPE::MSG_END_ENEMY_TURN Messages not received")
			return false;
	default:

		LOG("\n error: No Message")

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

	SendMessaging(MESSAGE_TYPE::MSG_END_ENEMY_TURN);
}

void EnemySnake::ExploreState(const float elapsed_time)
{
	if (enemy_snake_entry_state.IsStateFirstTime())
	{

	}



	//対象までの探索
	if (shortest_path->path_information.empty() == true)	//最短経路がない
	{
		int start_id{}, goal_id{};//Astarの始める位置と目的地
		shortest_path->Clear();//初期化

		/*敵の情報(position)→マップ情報(2次元配列)map_role[enemy_posZX[0]][enemy_posZX[1]]*/
		//ｙにはＺ軸の情報を入れておく
		const DirectX::XMFLOAT3 enemy_posZX = { GetPosition().x / CellSize, 0,GetPosition().z / CellSize };

		/*マップ情報(2次元配列)→Astar(1次元配列)*/
		//Astarの始める位置
		start_id = static_cast<int>(enemy_posZX.y) * MapSize_Y + static_cast<int>(enemy_posZX.x);

		//目的地
		//最も近い場所を見つける　→　通路の入り口
		DirectX::XMFLOAT3 shortest{};//通路の入り口の位置
		float min_length = FLT_MAX;

		for (const auto& road_entrance : stage_information->roads_entrance)
		{
			DirectX::XMFLOAT2 r = road_entrance;

			//マップ情報→ワールド座標
			r.y = r.y * CellSize;
			r.x = r.x * CellSize;

			const DirectX::XMFLOAT3 road_pos = DirectX::XMFLOAT3(r.x, 0.f, r.y);//通路の入り口のワールド座標

			//自身が検索中の通路の上にいるときスキップする
			if (Math::Comparison(GetPosition().x, road_pos.x) && Math::Comparison(GetPosition().z, road_pos.z))
			{
				continue;
			}

			const float length = Math::Length(Math::SubtractVector(GetPosition(), road_pos));//自身(敵)から通路への最短距離

			//現在の最短距離の入れ替える
			if (length < min_length)
			{
				min_length = length;//現在の最短距離の入れ替え
				shortest = DirectX::XMFLOAT3(static_cast<float>(r.x), 0, static_cast<float>(r.y));//最も近い通路のワールド座標を格納
			}
		}
		shortest_path->destination_pos = shortest;
		goal_id = static_cast<int>(shortest.z) * MapSize_Y + static_cast<int>(shortest.x);
		//最短経路を求める
		HeuristicSearch& Astar = HeuristicSearch::Instance();
		shortest_path->path_information = Astar.Search(start_id, goal_id, *stage_information);//到達
		shortest_path->SetPathSize();//最短経路のサイズを設定
	}
	else//探索先がある
	{
		//shortest_path->path_informationは最短経路の示す数字の配列
		if (true)
		{

		}
		else
		{
			LOG("error:shortest_path->path_information is nothing")//最短経路の値がない
		}
	}

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