#pragma once
//**********************************************************
//
//	HeuristicSearchクラス
//
//**********************************************************

#include <vector>
#include <memory>
#include "Engine/Systems/Math.h"
// 前方宣言
class Node;
class Edge;
class RogueLikeDungeon;

struct ShortestPath
{
	DirectX::XMINT2 destination_pos{};//目的地のマップ上の位置
	std::vector<int> path{};//最短経路
	unsigned int path_index{};//最短経路用の配列index番号

	void Clear()
	{
		destination_pos = DirectX::XMINT2(0,0);
		path.clear();
		path_index = 0;
	}

	//メンバの初期化
};

class HeuristicSearch final
{
private:

public:
	const	int AsterCostPercentage = 20;// A*のコスト計算時に割る値
	std::vector<std::shared_ptr<Node>> nodes;// 全ノードを保持するベクター
	std::vector<std::shared_ptr<Edge>> searched_edge;// 探索済みのエッジを保存
	std::vector<int>  advance;	// 進行ルートのエッジを記憶する
	static HeuristicSearch* instance;// 唯一のインスタンス

private:

	/// <summary>
	/// 候補(candidate)からコストを考慮して最短エッジ候補を返す
	/// </summary>
	/// <param name="candidate">探索候補の配列</param>
	/// <param name="now_edge">現在探索中のedge</param>
	/// <param name="goal_id"></param>
	/// <returns>探索候補内の最小コストのedge候補</returns>
	std::shared_ptr<Edge> SearchMinCost(std::vector<std::shared_ptr<Edge>>& candidate, const std::shared_ptr<Edge> now_edge, int goal_id);

	/// <summary>
	/// ２つのノードの物理距離を出してその距離を返す
	/// </summary>
	/// <param name="n1">現在のノード</param>
	/// <param name="n2">探索先(ゴール)のノード</param>
	/// <returns>距離/20</returns>
	float HeuristicCalculation(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

public:

	HeuristicSearch();
	~HeuristicSearch();

	// 唯一のインスタンス取得
	static HeuristicSearch& Instance() { return *instance; }

	/// <summary>
	/// nodesの初期化
	/// </summary>
	/// <param name="rogue_like_dungeon">マップ情報</param>
	void Reset(const RogueLikeDungeon& rogue_like_dungeon);

	/// <summary>
	/// start_idからgoal_idのコストを抑えた最短経路を計算
	/// </summary>
	/// <param name="start_id">マップ情報を見た時のスタ―ト地点</param>
	/// <param name="goal_id">マップ情報を見た時のスタ―ト地点</param>
	/// /// <param name="rogue_like_dungeon">マップ情報</param>
	/// <returns>最短経路</returns>
	std::vector<int> Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon);


};
