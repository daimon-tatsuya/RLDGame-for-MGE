#include "Engine/AI/HeuristicSearch.h"
#include "Engine/Systems/DungeonMake.h"

 HeuristicSearch* HeuristicSearch::instance = nullptr;

HeuristicSearch::HeuristicSearch()
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}

HeuristicSearch::~HeuristicSearch()
{
	if (advance.size())
	{
		advance.clear();
	}
}

void HeuristicSearch::Reset(const RogueLikeDungeon& rogue_like_dungeon)
{
	//nodeの初期化
	searched_edge.clear();
	int node_id = 0;
	for (int y = 0; y < static_cast<int>(MapSize_Y); y++)
	{
		for (int x = 0; x < static_cast<int>(MapSize_X); x++)
		{
			auto piece = std::make_shared<Node>();
			piece->node_id = node_id;
			piece->SetNodePosition(static_cast<float>(x * Cell_Size), static_cast<float>(y * Cell_Size));

			//マップデータからデータを取り出す

			//壁
			if (rogue_like_dungeon.map_role[y][x].map_data == 1)
			{
				piece->SetWallNodeFlag(true);
			}

			//敵(保留)
			for (int i = 1; i < Mob_Max; i++)
			{
				if (rogue_like_dungeon.map_role[y][x].map_data == 3)
				{
					piece->SetEnemyNodeFlag(true);
				}
			}

			nodes.emplace_back(std::move(piece));

			advance.emplace_back(-1);
			node_id++;
		}
	}

	//edgeの初期化
	//distnation_nodeが- 1となら、edgeは無効
	int distnation = 0;

	for (int y = 0; y < static_cast<int>(MapSize_Y); y++)
	{
		for (int x = 0; x < static_cast<int>(MapSize_X); x++)
		{
			int node = MapSize_Y * y + x;

			//edge上方向
			std::shared_ptr<Edge> edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopCenter)];
			//{関数化
			edge->Initialize(node, distnation);
			//一番上の段でなければ
			if (!(y <= 0))
			{
				distnation = node - MapSize_X;
			}
			edge->distnation_node = distnation;

			//edge右上方向
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopRight)];
			edge->Initialize(node, distnation);

			//一番上の段で右端でなければ
			if (!(y <= 0) && !(x >= MapSize_X - 1))
			{
				distnation = node - MapSize_X + 1;
			}
			edge->distnation_node = distnation;

			//edge->cost = 1.414f;

			//edge右方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterRight)];
			edge->Initialize(node, distnation);
			//右端でなければ
			if (!(x >= MapSize_X - 1))
			{
				distnation = node + 1;
			}
			edge->distnation_node = distnation;

			//edge右下方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomRight)];
			edge->Initialize(node, distnation);
			//一番下の段で右端でなければ
			if (!(y >= MapSize_Y - 1) && !(x >= MapSize_X - 1))
			{
				distnation = node + MapSize_X + 1;
			}
			edge->distnation_node = distnation;
			//edge->cost = 1.414f;

			//edge下方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomCenter)];
			edge->Initialize(node, distnation);

			//一番下の段でなければ
			if (!(y >= MapSize_Y - 1))
			{
				distnation = node + MapSize_X;
			}
			edge->distnation_node = distnation;

			//edge左下方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::BottomLeft)];
			edge->Initialize(node, distnation);
			//一番下の段で左端でなければ
			if (!(y >= MapSize_Y - 1) && !(x <= 0))
			{
				distnation = node + MapSize_X - 1;
			}
			edge->distnation_node = distnation;
			//edge->cost = 1.414f;

			//edge左方向
			edge = nodes[node]->edge[static_cast<int>(EdgeDirection::CenterLeft)];
			edge->Initialize(node, distnation);
			//右端でなければ
			if (!(x <= 0))
			{
				distnation = node - 1;
			}
			edge->distnation_node = distnation;

			//edge左上方向
			edge = nodes[node]->edge[static_cast<size_t>(EdgeDirection::TopLeft)];
			edge->Initialize(node, distnation);
			//一番上の段で左端でなければ
			if (!(y <= 0) && !(x <= 0))
			{
				distnation = node - MapSize_X - 1;
			}
			edge->distnation_node = distnation;
			//edge->cost = 1.414f;

		}
	}
}

std::vector<int> HeuristicSearch::Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon)
{
	//サーチ候補の配列
	std::vector<std::shared_ptr<Edge>> candidate;

	Reset(rogue_like_dungeon);

	//初期化
	for (int i = 0; i < MapSize; i++)
	{
		nodes[i]->SetSearchedNodeFlag(false);
		nodes[i]->cost_from_start = 0.f;
		advance[i] = -1;
	}
	std::shared_ptr<Edge> now_edge = std::make_shared<Edge>();
	//ダミー
	now_edge->distnation_node = start_id;
	now_edge->origin_node = start_id;
	while (true)
	{
		float total_cost = 0;
		//探索済みのedgeの保存
		advance[now_edge->distnation_node] = now_edge->origin_node;

		//次のnowEdgeのdistnation_nodeが目的地なら目的地までの最短進行ルートを返す
		if (now_edge->distnation_node == goal_id)
		{
			return advance;
		}
		//nowEdgeの接続先のノードを取得する。
		std::shared_ptr<Node>node = nodes[now_edge->distnation_node];

		for (int edge_num = 0; edge_num < EDGE_NUM; edge_num++)
		{
			std::shared_ptr<Edge> edge = node->edge[edge_num];
			if (edge->distnation_node >= 0)//このedgeが有効なら
			{
				std::shared_ptr<Node> next_node = nodes[edge->distnation_node];

				// スタート地点からのコストとエッジのコストをtotal_costに加算
				total_cost += (node->cost_from_start + edge->cost);

				//cost増減//

				//----------

				//コストが一度も計算されてないか、新しいコストが低ければ探索候補にする
				if (next_node->cost_from_start == 0.f || next_node->cost_from_start > total_cost)
				{
					if (!next_node->GetWallNodeFlag())//探索候補が壁なら候補に入れない
					{
						candidate.emplace_back(edge);
					}
				}

				now_edge = SearchMinCost(candidate, now_edge, goal_id);
			}
		}
	}
	return advance;
}

std::shared_ptr<Edge> HeuristicSearch::SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> nowEdge, int goal_id)
{
	std::shared_ptr<Edge> shortest = nullptr;//最短エッジ候補格納用
	int shortest_num = 0;//最短エッジ候補番号格納用
	float min_cost = FLT_MAX;

	for (int candidate_num = 0; candidate_num < static_cast<int>(candidate.size()); candidate_num++)
	{
		Edge* edge = candidate[candidate_num].get();
		//elemetに入っているedgeのスタート位置からのコストを計算する。
		float total_cost = 0;
		std::shared_ptr<Node> origin_node = nodes[edge->origin_node];
		std::shared_ptr<Node> distnation_node = nodes[edge->distnation_node];
		total_cost += (origin_node->cost_from_start + edge->cost);
		//cost増減//

		//----------

		//接続先の「スタート位置からのコスト」をfront_costに取り出す(まだ登録されていないなら０となる)
		float front_cost = 0;
		front_cost = distnation_node->cost_from_start;

		//front_costがまだ登録されていないか、front_costより少ないコストルートが
		//発見されたなら、接続先のスタート位置からのコストをtotal_costに書き換え。
		if (front_cost == 0.f || front_cost >= total_cost)
		{
			std::shared_ptr<Node> goalNode = nodes[goal_id];//ゴールのノード
			float AstarCost = heuristicCulc(distnation_node, goalNode);

			distnation_node->cost_from_start = total_cost;
			front_cost = total_cost+AstarCost;
		}

		//front_costが、今調べているエッジの接続先のトータルコスト以上のもの中で
		//一番小さい接続先のスタート位置からのコストを持つエッジを最短として保存。
		std::shared_ptr<Node> now_node = nodes[nowEdge->distnation_node];
		if (min_cost > front_cost)
		{
			//candidateと現在探索中のegdeの比較
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

//２つのノードの物理距離を出してその距離を返す
float  HeuristicSearch::heuristicCulc(std::shared_ptr<Node> N1, std::shared_ptr<Node> N2)
{
	DirectX::XMFLOAT2 N1_pos = N1->GetNodePosition();
	DirectX::XMFLOAT2 N2_pos = N2->GetNodePosition();
	float y = N1_pos.y - N2_pos.y;
	float x = N1_pos.x - N2_pos.x;
	return  sqrtf((y * y) + (x * x)) / 20;
}