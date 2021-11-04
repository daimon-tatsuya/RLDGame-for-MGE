#pragma once
#include <stack>
#include <queue>
#include <list>
#include "Engine/AI/Edge.h"
#include "Engine/AI/Node.h"
#include "Engine/Systems/DungeonMake.h"
class HeuristicSearch final
{
private:
	/// <summary>
	/// candidateからコストを考慮して最短エッジ候補を返す
	/// </summary>
	/// <param name="candidate">探索候補の配列</param>
	/// <param name="nowEdge">現在探索中のedge</param>
	/// <returns>探索候補内の最小コストのedge候補</returns>
	std::shared_ptr<Edge> SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> nowEdge, int goal_id);

public:

	HeuristicSearch() {};
	HeuristicSearch(const RogueLikeDungeon& rogue_like_dungeon);
	~HeuristicSearch();

	std::vector<int> Search(int start_id, int goal_id);

	static HeuristicSearch& Instance();

public:
	//全ノードを保持するベクター
	std::vector<std::shared_ptr<Node>> nodes;
	//探索済みのエッジを保存
	std::vector<std::shared_ptr<Edge>> searched_edge;
	//進行ルートのエッジをを記憶する
	std::vector<int>  advance;
};
