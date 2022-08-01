#pragma once
//**********************************************************
//
//	 HeuristicSearchクラス
//
//**********************************************************

// ヘッダー
#include <vector>
#include <memory>
#include "Engine/Systems/Math.h"

// 前方宣言
class Node;
class Edge;
class RogueLikeDungeon;

// 最短経路
struct ShortestPath
{
	DirectX::XMFLOAT3 destination_pos{};// 目的地のマップ上の位置
	std::vector<int> path_information{};// 最短経路
	unsigned int path_index{};// 最短経路用の配列index番号
	unsigned int path_size;// 最短経路のコンテナのサイズ、path_index-1と比較して同じなら

	// メンバの初期化
	void Clear()
	{
		destination_pos = DirectX::XMFLOAT3(0, 0,0);
		path_information.clear();
		path_information.shrink_to_fit();
		path_index = 0;
		path_size = 0;
	}

	// 最短経路のサイズを設定
	void SetPathSize() { path_size = static_cast<unsigned int>(path_information.size()); }

};

/// <summary>
/// A*の計算を行う
/// </summary>
class HeuristicSearch final
{
private:

public:

	const int ASTER_COST_PERCENTAGE = 20;		// A*のコスト計算時に割る値

	std::vector<std::shared_ptr<Node>> nodes;		// 全ノードを保持するベクター

	std::vector<std::shared_ptr<Edge>> searched_edge;// 探索済みのエッジを保存

	std::vector<int>  advance;				// 探索した結果最短のルート、進行ルートのエッジを記憶する。

	static HeuristicSearch* instance;	// 唯一のインスタンス

	std::vector<int>  map_info;			// マップ情報を一次元行列化したコンテナ

	//std::vector<std::vector<int>>advance_vector;

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

	//コンストラクタ
	HeuristicSearch();

	//デストラクタ
	~HeuristicSearch();

	// 唯一のインスタンス取得
	static HeuristicSearch& Instance() { return *instance; }

	/// <summary>
	/// nodesの初期化
	/// </summary>
	void NodesInitialize();

	/// <summary>
	/// start_idからgoal_idのコストを抑えた最短経路を計算
	/// </summary>
	/// <param name="start_id">マップ情報を見た時のスタ―ト地点</param>
	/// <param name="goal_id">マップ情報を見た時のスタ―ト地点</param>
	/// <returns>最短経路の位置</returns>
	bool Search(int start_id, int goal_id);

	//---------------------------
	// Convert Method
	//---------------------------

	/// <summary>
	/// 一次元配列をマップ情報に変換
	/// </summary>
	/// <para>マップ情報変換関数</para>
	/// <param name="one_dimensional">一次元配列に変換する位置</param>
	/// <returns>変換後の値</returns>
	DirectX::XMINT2 ConvertMapPosition(int one_dimensional);

	/// <summary>
	/// ワールド座標(int x,int z)を1次元に変換する
	/// </summary>
	/// <para>マップ情報変換関数</para>
	/// <param name="world">一次元配列に変換する位置</param>
	/// <returns>変換後の値</returns>
	int ConvertWorldToOneDimensional(DirectX::XMFLOAT2 world);
	int ConvertWorldToOneDimensional(DirectX::XMINT2 world);

	/// <summary>
	/// マップ情報を一次元配列に変換する
	/// </summary>
	/// <para>マップ情報変換関数</para>
	void ConvertMapInformationToOneDimensionalArray();
};
