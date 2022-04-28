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
	DirectX::XMFLOAT3 destination_pos{};//目的地のマップ上の位置
	std::vector<int> path_information{};//最短経路
	unsigned int path_index{};//最短経路用の配列index番号
	unsigned int path_size;//最短経路のコンテナのサイズ、path_index-1と比較して同じなら
	//メンバの初期化
	void Clear()
	{
		destination_pos = DirectX::XMFLOAT3(0, 0,0);
		path_information.clear();
		path_index = 0;
		path_size = 0;
	}

	//最短経路のサイズを設定
	void SetPathSize() { path_size = static_cast<unsigned int>(path_information.size()); }

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
	static float HeuristicCalculation(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2);

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
	/// <returns>最短経路の位置</returns>
	std::vector<int> Search(int start_id, int goal_id, const RogueLikeDungeon& rogue_like_dungeon);

	///<summary>
	///2次元配列に変換する
	///</summary>


};
//Asterの結果を2次元配列に変換する
//void HeuristicSearch::ConvertTwoDimensionalArray()
//{
//	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
//	{
//		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
//		{
//			//   x →
//			// y		MapSize_Y*0----------------------MapSize_X*1-1
//			//↓		MapSize_Y*1----------------------MapSize_X*2-1
//			//			MapSize_Y*2----------------------MapSize_X*3-1
//			//			MapSize_Y*3----------------------MapSize_X*4-1
//			//			MapSize_Y*4----------------------MapSize_X*5-1
//			//			MapSize_Y*5----------------------MapSize_X*6-1
//			//~~~~~~~~~~~~~~~~~
//			//			MapSize_Y*  ----------------------MapSize_X*x-1
//			//		  (MapSize_Y - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//進行ルートを二次元配列に格納
//			advance_vector[y][x] = advance[array_num];
//		}
//	}
//}
//マップ情報を1次元配列に変換する
//void HeuristicSearch::ConvertOneDimensionalArray(const RogueLikeDungeon& rogue_like_dungeon)
//{
//	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
//	{
//		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
//		{
//			//   x →
//			// y		MapSize_Y*0----------------------MapSize_X*1-1
//			//↓		MapSize_Y*1----------------------MapSize_X*2-1
//			//			MapSize_Y*2----------------------MapSize_X*3-1
//			//			MapSize_Y*3----------------------MapSize_X*4-1
//			//			MapSize_Y*4----------------------MapSize_X*5-1
//			//			MapSize_Y*5----------------------MapSize_X*6-1
//			//~~~~~~~~~~~~~~~~~
//			//			MapSize_Y* x----------------------MapSize_X*x-1
//			//		(MapSize_Y - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//マップ情報を一次元配列に格納
//			map_info[array_num] = static_cast<int>(rogue_like_dungeon.map_role[y][x].map_data);
//		}
//	}
//}