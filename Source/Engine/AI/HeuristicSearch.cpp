//**********************************************************
//
//		HeuristicSearchクラス
//
//**********************************************************

// ヘッダー
#include "Engine/AI/HeuristicSearch.h"
#include "Engine/AI/Edge.h"
#include "Engine/AI/Node.h"
#include "Engine/AI/DungeonMake.h"

// インスタンス
HeuristicSearch* HeuristicSearch::instance = nullptr;

// コンストラクタ
HeuristicSearch::HeuristicSearch()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

// デストラクタ
HeuristicSearch::~HeuristicSearch()
{
	// コンテナがあるなら
	if (!advance.empty())
	{
		advance.clear();
	}
}

//ノードの設定
void HeuristicSearch::NodesInitialize()
{
	// nodeの初期化
	searched_edge.clear();
	int node_id = 0;
	for (int y = 0; y < MAP_SIZE_Y - 1; y++)
	{
		for (int x = 0; x < MAP_SIZE_X - 1; x++)
		{
			auto piece = std::make_shared<Node>();
			piece->node_id = node_id;
			piece->SetNodePosition(static_cast<float>(x), static_cast<float>(y));

			// マップデータからデータを取り出す
			RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();

			//マップデータの敵や壁をノードに記憶

			// 壁
			if (rogue_like_dungeon.GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				piece->SetIsWallNode(true);// 壁フラグ
			}

			// 敵(保留)
			if (rogue_like_dungeon.GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Enemy))
			{
				piece->SetIsEnemyNode(true);// 敵フラグ
			}

			nodes.emplace_back(std::move(piece));

			node_id++;
		}
	}

	// edgeの初期化
	// destination_nodeが- 1となら、edgeは無効
	int destination = 0;

	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			const int node = y * MAP_SIZE_Y + x;
			// {関数化

			// edge上方向
			std::shared_ptr<Edge> edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopCenter)];

			edge->Initialize(node, destination);
			// 一番上の段でなければ
			if (!(y <= 0))
			{
				destination = node - MAP_SIZE_X;
			}
			edge->destination_node_id = destination;

			// edge右上方向
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopRight)];
			edge->Initialize(node, destination);

			// 一番上の段で右端でなければ
			if (!(y <= 0) && !(x >= MAP_SIZE_X - 1))
			{
				destination = node - MAP_SIZE_X + 1;
			}
			edge->destination_node_id = destination;
			edge->cost = OBLIQUE_COST;

			

			// edge右方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterRight)];
			edge->Initialize(node, destination);
			// 右端でなければ
			if (!(x >= MAP_SIZE_X - 1))
			{
				destination = node + 1;
			}
			edge->destination_node_id = destination;

			// edge右下方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomRight)];
			edge->Initialize(node, destination);
			// 一番下の段で右端でなければ
			if (!(y >= MAP_SIZE_Y - 1) && !(x >= MAP_SIZE_X - 1))
			{
				destination = node + MAP_SIZE_X + 1;
			}
			edge->destination_node_id = destination;
			// edge->cost = 1.414f;

			// edge下方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomCenter)];
			edge->Initialize(node, destination);

			// 一番下の段でなければ
			if (!(y >= MAP_SIZE_Y - 1))
			{
				destination = node + MAP_SIZE_X;
			}
			edge->destination_node_id = destination;

			// edge左下方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomLeft)];
			edge->Initialize(node, destination);
			// 一番下の段で左端でなければ
			if (!(y >= MAP_SIZE_Y - 1) && !(x <= 0))
			{
				destination = node + MAP_SIZE_X - 1;
			}
			edge->destination_node_id = destination;
			// edge->cost = 1.414f;

			// edge左方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterLeft)];
			edge->Initialize(node, destination);
			// 右端でなければ
			if (!(x <= 0))
			{
				destination = node - 1;
			}
			edge->destination_node_id = destination;

			// edge左上方向
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopLeft)];
			edge->Initialize(node, destination);
			// 一番上の段で左端でなければ
			if (!(y <= 0) && !(x <= 0))
			{
				destination = node - MAP_SIZE_X - 1;
			}
			edge->destination_node_id = destination;
			// edge->cost = 1.414f;
		}
	}
}

//start_idからgoal_idのコストを抑えた最短経路を計算
bool HeuristicSearch::Search(int start_id, int goal_id)
{
	// サーチ候補の配列
	std::vector<std::shared_ptr<Edge>> candidate;

	NodesInitialize();

	// 初期化
	for (int i = 0; i < MAP_SIZE; i++)
	{
		nodes[i]->is_searched_node = false;
		nodes[i]->cost_from_start = 0.f;
	}

	// 進行ルートの初期化
	advance.clear();

	// 現在探索中のedgeを作成
	std::shared_ptr<Edge> now_edge = std::make_shared<Edge>();

	// スタート
	now_edge->destination_node_id = start_id;
	now_edge->origin_node_id = start_id;

	while (true)
	{
		float total_cost = 0;
		// 探索済みのedgeの保存
		// advance.push_back(now_edge->origin_node_id);

		advance[now_edge->destination_node_id] = now_edge->origin_node_id;

		// 次のnowEdgeのdestination_nodeが目的地なら目的地までの最短進行ルートを返す
		if (now_edge->destination_node_id == goal_id)
		{

			advance.push_back(-1);
			return true;
		}
		// now_edgeの接続先のノードを取得する。
		std::shared_ptr<Node>node = nodes[now_edge->destination_node_id];

		for (int edge_num = 0; edge_num < EDGE_NUM; edge_num++)// 8方向調べる
		{
			std::shared_ptr<Edge> edge = node->edge[edge_num];

			if (edge->destination_node_id >= 0)// このedgeが有効なら
			{
				const std::shared_ptr<Node> next_node = nodes[edge->destination_node_id];

				// スタート地点からのコストとエッジのコストをtotal_costに加算
				total_cost += (node->cost_from_start + edge->cost);

				// cost増減
				// 敵
				if (next_node->GetIsEnemyNode() == true)
				{
					total_cost += 10.0f;
				}
				// 壁
				if (next_node->GetIsWallNode() == true)
				{
					total_cost += 10.0f;
				}
				//----------

				// コストが一度も計算されてないか、新しいコストが低ければ探索候補にする
				if (next_node->cost_from_start == 0.f || next_node->cost_from_start > total_cost)
				{
					if (!next_node->GetIsWallNode())// 探索候補が壁なら候補に入れない
					{
						candidate.push_back(edge);
					}
				}
				// 最短経路を算出
				now_edge = SearchMinCost(candidate, now_edge, goal_id);

				if (now_edge == nullptr) { return false; }
			}
		}
	}
	return false;
}



// ２つのノードの物理距離を出してその距離を返す
std::shared_ptr<Edge> HeuristicSearch::SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> now_edge, int goal_id)
{
	std::shared_ptr<Edge> shortest = nullptr;// 最短エッジ候補格納用
	int shortest_num;// 最短エッジ候補番号格納用
	float min_cost = FLT_MAX;

	for (int candidate_num = 0; candidate_num < static_cast<int>(candidate.size()); candidate_num++)
	{
		Edge* edge = candidate[candidate_num].get();
		// edgeのスタート位置からのコストを計算する。
		float total_cost = 0;
		const std::shared_ptr<Node> origin_node = nodes[edge->origin_node_id];
		const std::shared_ptr<Node> destination_node = nodes[edge->destination_node_id];
		total_cost += (origin_node->cost_from_start + edge->cost);
		// cost増減

		//----------

		// 接続先の「スタート位置からのコスト」をfront_costに取り出す(まだ登録されていないなら０となる)
		float front_cost = 0;
		front_cost = destination_node->cost_from_start;

		// front_costがまだ登録されていないか、front_costより少ないコストルートが
		// 発見されたなら、接続先のスタート位置からのコストをtotal_costに書き換え。
		if (front_cost == 0.f || front_cost >= total_cost)
		{
			const std::shared_ptr<Node> goal_node = nodes[goal_id];//ゴールのノード
			const float astar_cost = HeuristicCalculation(destination_node, goal_node);

			destination_node->cost_from_start = total_cost;
			front_cost = total_cost + astar_cost;
		}

		// front_costが、今調べているエッジの接続先のトータルコスト以上のもの中で
		// 一番小さい接続先のスタート位置からのコストを持つエッジを最短として保存。
		const std::shared_ptr<Node> now_node = nodes[now_edge->destination_node_id];
		if (min_cost > front_cost)
		{
			// candidateと現在探索中のegdeの比較
			if (front_cost >= now_node->cost_from_start)
			{
				shortest = candidate[candidate_num];
				min_cost = front_cost;
				shortest_num = candidate_num;
			}
		}
	}
	return shortest;
}

// ２つのノードの物理距離を出してその距離を返す
float  HeuristicSearch::HeuristicCalculation(const std::shared_ptr<Node> n1, const std::shared_ptr<Node> n2)
{
	const DirectX::XMFLOAT2 N1_pos = n1->GetNodePosition();
	const DirectX::XMFLOAT2 N2_pos = n2->GetNodePosition();
	const float y = N1_pos.y - N2_pos.y;
	const float x = N1_pos.x - N2_pos.x;
	return  sqrtf((y * y) + (x * x)) / 20;
}

// 一次元配列をマップ情報に変換
DirectX::XMINT2 HeuristicSearch::ConvertMapPosition(int one_dimensional)
{
	return {one_dimensional / MAP_SIZE_Y, one_dimensional - (one_dimensional / MAP_SIZE_Y)};
}

// ワールド座標(int x, int z)を1次元に変換する
int HeuristicSearch::ConvertWorldToOneDimensional(DirectX::XMFLOAT2 world)
{
	return static_cast<int>(world.y) * MAP_SIZE_Y + static_cast<int>(world.x);
}

int HeuristicSearch::ConvertWorldToOneDimensional(DirectX::XMINT2 world)
{
	return (world.y) * MAP_SIZE_Y + (world.x);
}

// マップ情報を1次元配列に変換する
void HeuristicSearch::ConvertMapInformationToOneDimensionalArray()
{
	RogueLikeDungeon& rogue_like_dungeon = RogueLikeDungeon::Instance();
	// 壁
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			//   x →
			// y		MAP_SIZE_Y*0----------------------MAP_SIZE_X*1-1
			//↓		MAP_SIZE_Y*1----------------------MAP_SIZE_X*2-1
			//			MAP_SIZE_Y*2----------------------MAP_SIZE_X*3-1
			//			MAP_SIZE_Y*3----------------------MAP_SIZE_X*4-1
			//			MAP_SIZE_Y*4----------------------MAP_SIZE_X*5-1
			//			MAP_SIZE_Y*5----------------------MAP_SIZE_X*6-1
			// ~~~~~~~~~~~~~~~~~
			//			MAP_SIZE_Y* x----------------------MAP_SIZE_X*x-1
			//		(MAP_SIZE_Y - 1)							MAP_SIZE_X - 1
			const int array_num = y * MAP_SIZE_Y + x;
			// マップ情報を一次元配列に格納
			map_info[array_num] = static_cast<int>(rogue_like_dungeon.GetMapRole()[y][x].map_data);
		}
	}
}