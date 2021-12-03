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

public:

	std::vector<std::shared_ptr<Node>> nodes;//全ノードを保持するベクター

	std::vector<std::shared_ptr<Edge>> searched_edge;//探索済みのエッジを保存

	std::vector<int>  advance;//進行ルートのエッジをを記憶する

	static HeuristicSearch* instance;//唯一のインスタンス

    const	int AsterCostPercentage = 20;//A*のコスト計算時に割る値
private:
	/// <summary>
	/// candidateからコストを考慮して最短エッジ候補を返す
	/// </summary>
	/// <param name="candidate">探索候補の配列</param>
	/// <param name="nowEdge">現在探索中のedge</param>
	/// <returns>探索候補内の最小コストのedge候補</returns>
	std::shared_ptr<Edge> SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> nowEdge, int goal_id);

public:

	HeuristicSearch();
	~HeuristicSearch();

	//唯一のインスタンス取得
	static HeuristicSearch& Instance() { return *instance; };


	void Reset(const RogueLikeDungeon& rogue_like_dungeon);

	/// <summary>
	/// start_idからgoal_idのコストを抑えた最短経路を計算
	/// </summary>
	/// <param name="start_id">マップ情報を見た時のスタ―ト地点</param>
	/// <param name="goal_id">マップ情報を見た時のスタ―ト地点</param>
	/// /// <param name="rogue_like_dungeon">マップ情報</param>
	/// <returns>最短経路</returns>
	std::vector<int> Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon);

	/// <summary>
	/// //２つのノードの物理距離を出してその距離を返す
	/// </summary>
	/// <param name="N1">現在のノード</param>
	/// <param name="N2">探索先(ゴール)のノード</param>
	/// <returns>距離/20</returns>
	float heuristicCulc(std::shared_ptr<Node> N1, std::shared_ptr<Node> N2);
};
