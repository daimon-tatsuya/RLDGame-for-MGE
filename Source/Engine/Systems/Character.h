#pragma once
//**********************************************************
//
//		Characterクラス
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/AI/HeuristicSearch.h"

class Character:
public Object
{
private:

	int	 current_health = 5;//名前を変えるかも health→life,HP
	int	 max_health = 5;

	bool				is_decide_pos = false;//位置が決まった時にbreakするために使う

public:

	RogueLikeDungeon* stage_information = nullptr; // マップ情報
	ShortestPath* shortest_path{};//最短経路

private:

	/// <summary>
/// 有限ステートマシンの初期化
/// </summary>
	virtual void FiniteStateMachineInitialize() {}


public:
	// 健康状態を取得
	int GetCurrentHealth() const { return current_health; }

	// 健康状態を設定
	void SetCurrentHealth(const int health) { this->current_health = health; }

	// 最大健康状態を取得
	int GetMaxHealth() const { return max_health; }

	// 最大健康状態を設定
	void SetMaxHealth() { this->max_health = max_health; }

	// マップ情報を取得
	RogueLikeDungeon* GetStageInformation() const { return stage_information; }

	//最短経路を設定
	void SetShortestPath(std::vector<int> path) const { shortest_path->path_information = std::move(path); }

	bool GetIsDecidePos() const { return  is_decide_pos; }

	void SetIsDecidePos(bool set) { is_decide_pos = set; }
};
