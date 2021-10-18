#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>

#include "Engine/AI/Edge.h"

const int EDGE_NUM = 8;

class Node
{
private:
public:

	Node();
	~Node();

	/// <summary>
	/// start_node_flagの設定
	/// HeuristicSearchで使用
	/// trueのnodeからHeuristicSearchを行う
	/// </summary>
	/// <param name="set"></param>
	void SetStartNodeFlag(bool set) { start_node_flag = set; }

	/// <summary>
	/// HeuristicSearchで使用
	/// trueのnodeからHeuristicSearchを行う
	/// </summary>
	/// <returns>start_node_flag</returns>
	bool GetStartNodeFlag() { return start_node_flag; }

	/// <summary>
	/// goal_node_flagの設定
	/// HeuristicSearchで使用
	/// trueのnodeを目的地としてHeuristicSearchを行う
	/// </summary>
	/// <param name="set"></param>
	void SetGoalNodeFlag(bool set) { goal_node_flag = set; }

	/// <summary>
	/// HeuristicSearchで使用
	/// trueのnodeを目的地としてHeuristicSearchを行う
	/// </summary>
	/// <returns>goal_node_flag</returns>
	bool GetGoalNodeFlag() { return goal_node_flag; }

	/// <summary>
	/// player_node_flagの設定
	/// trueならこのnodeはプレイヤーがいる
	/// </summary>
	/// <param name="set"></param>
	void SetPlayerNodeFlag(bool set) { player_node_flag = set; }

	/// <summary>
	/// trueならこのnodeはプレイヤーがいる
	/// </summary>
	/// <returns>player_node_flag</returns>
	bool GetPlayerNodeFlag() { return player_node_flag; }

	/// <summary>
	/// enemy_node_flagの設定
	/// trueならこのnodeは敵がいる
	/// </summary>
	/// <param name="set"></param>
	void SetEnemyNodeFlag(bool set) { enemy_node_flag = set; }

	/// <summary>
	/// trueならこのnodeは敵がいる
	/// </summary>
	/// <returns>enemy_node_flag</returns>
	bool GetEnemyNodeFlag() { return enemy_node_flag; }

	/// <summary>
	/// wall_node_flagの設定
	/// trueならこのnodeは壁
	/// </summary>
	/// <param name="set"></param>
	void SetWallNodeFlag(bool set) { wall_node_flag = set; }

	/// <summary>
	/// trueならこのnodeは壁
	/// </summary>
	/// <returns>wall_node_flag</returns>
	bool GetWallNodeFlag() { return wall_node_flag; }

	/// <summary>
	/// item_node_flagの設定
	/// trueならこのnodeにはアイテムが置かれている
	/// </summary>
	/// <param name="set"></param>
	void SetItemNodeFlag(bool set) { item_node_flag = set; }

	/// <summary>
	/// trueならこのnodeにはアイテムが置かれている
	/// </summary>
	/// <returns>item_node_flag</returns>
	bool GetItemNodeFlag() { return item_node_flag; }

	/// <summary>
	/// searched_node_flagの設定
	/// trueならこのnodeは探索済み
	/// </summary>
	/// <param name="set"></param>
	void SetSearchedNodeFlag(bool set) { searched_node_flag = set; }

	/// <summary>
	/// trueならこのnodeは探索済み
	/// </summary>
	/// <returns>searched_node_flag</returns>
	bool GetSearchedNodeFlag() { return searched_node_flag; }

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
	DirectX::XMFLOAT2 GetNodePosition() { return position; }

private:

	bool start_node_flag = false;//A*始める地点
	bool goal_node_flag = false;//目標地点
	bool player_node_flag = false;//Playerのいる地点
	bool enemy_node_flag = false;//Enemyのいる地点
	bool wall_node_flag = false;//壁のある地点
	bool item_node_flag = false;//アイテムのある地点
	bool searched_node_flag = false;//探索済みの地点

	DirectX::XMFLOAT2 position = { 0,0 };

public:
	int node_id = 0;
	std::vector<std::shared_ptr<Edge>> edge;//このノードが所有するエッジ情報
	float cost = 1.0f;					//ノード自体のコスト
	float cost_from_start = 100.f;//A*を始めた地点からの合計コスト
};
