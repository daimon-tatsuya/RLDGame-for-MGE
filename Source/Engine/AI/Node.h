#pragma once
//**********************************************************
//
//		Nodeクラス
//
//**********************************************************

#include <vector>
#include <memory>
#include "Engine/Systems/Math.h"

//前方宣言
class Edge;

class Node final
{
private:

	bool is_start_node = false;		// A*始める地点ならtrue
	bool is_goal_node = false;		// 目標地点ならtrue
	bool is_player_node = false;	// プレイヤーのいる地点ならtrue
	bool is_enemy_node = false;	// 敵のいる地点ならtrue
	bool is_wall_node = false;		// 壁のある地点ならtrue
	bool is_item_node = false;		// アイテムのある地点ならtrue

public:
	int node_id = 0;
	DirectX::XMFLOAT2 position{ 0,0 };
	std::vector<std::shared_ptr<Edge>> edge;// このノードが所有するエッジ情報
	float cost = 1.0f;		                             // ノード自体のコスト
	float cost_from_start = 100.f;                 // A*を始めた地点からの合計コスト
	bool is_searched_node = false;// 探索済みの地点ならtrue
private:

public:

	Node();

	~Node() = default;

	/// <summary>
	/// <para>start_node_flagの設定</para>
	/// <para>HeuristicSearchで使用</para>
	/// <para>trueのnodeからHeuristicSearchを行う</para>
	/// </summary>
	/// <param name="set"></param>
	void SetIsStartNode(const bool set) { is_start_node = set; }

	/// <summary>
	/// <para>HeuristicSearchで使用</para>
	/// <para>trueのnodeからHeuristicSearchを行う</para>
	/// </summary>
	/// <returns>is_start_node</returns>
	bool GetIsStartNode() const { return is_start_node; }

	/// <summary>
	/// <para>goal_node_flagの設定</para>
	/// <para>HeuristicSearchで使用</para>
	/// <para>trueのnodeを目的地としてHeuristicSearchを行う</para>
	/// </summary>
	/// <param name="set"></param>
	void SetIsGoalNode(bool set) { is_goal_node = set; }

	/// <summary>
	/// <para>HeuristicSearchで使用</para>
	/// <para>trueのnodeを目的地としてHeuristicSearchを行う</para>
	/// </summary>
	/// <returns>is_goal_node</returns>
	bool GetIsGoalNode() const { return is_goal_node; }

	/// <summary>
	/// <para>player_node_flagの設定</para>
	/// <para>trueならこのnodeはプレイヤーがいる</para>
	/// </summary>
	/// <param name="set"></param>
	void SetIsPlayerNode(bool set) { is_player_node = set; }

	/// <summary>
	/// trueならこのnodeはプレイヤーがいる
	/// </summary>
	/// <returns>is_player_node</returns>
	bool GetIsPlayerNode() const { return is_player_node; }

	/// <summary>
	/// <para>enemy_node_flagの設定</para>
	/// <para>trueならこのnodeは敵がいる</para>
	/// </summary>
	/// <param name="set"></param>
	void SetIsEnemyNode(bool set) { is_enemy_node = set; }

	/// <summary>
	/// trueならこのnodeは敵がいる
	/// </summary>
	/// <returns>is_enemy_node</returns>
	bool GetIsEnemyNode() const { return is_enemy_node; }

	/// <summary>
	/// <para>wall_node_flagの設定</para>
	/// <para>trueならこのnodeは壁</para>
	/// </summary>
	/// <param name="set"></param>
	void SetIsWallNode(bool set) { is_wall_node = set; }

	/// <summary>
	/// trueならこのnodeは壁
	/// </summary>
	/// <returns>is_wall_node</returns>
	bool GetIsWallNode() const { return is_wall_node; }

	/// <summary>
	/// <para>item_node_flagの設定</para>
	/// <para>trueならこのnodeにはアイテムが置かれている</para>
	/// </summary>
	/// <param name="set"></param>
	void SetIsItemNode(bool set) { is_item_node = set; }

	/// <summary>
	/// trueならこのnodeにはアイテムが置かれている
	/// </summary>
	/// <returns>is_item_node</returns>
	bool GetIsItemNode() const { return is_item_node; }

	/// <summary>
	/// nodeのpositionの設定
	/// </summary>
	/// <param name="pos">DirectX::XMFLOAT2</param>
	void SetNodePosition(DirectX::XMFLOAT2 pos) { position = pos; }

	/// <summary>
	/// nodeのpositionの設定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	void SetNodePosition(float x, float y) { position = { x,y }; }

	/// <summary>
	///  nodeのpositionのx座標設定
	/// </summary>
	/// <param name="x"></param>
	void SetNodePositionX(float x) { position.x = x; }

	/// <summary>
	/// nodeのpositionのy座標設定
	/// </summary>
	/// <param name="y"></param>
	void SetNodePositionY(float y) { position.y = y; }

	/// <summary>
	///  nodeのpositionの値を返す
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT2 GetNodePosition() const { return position; }
};
