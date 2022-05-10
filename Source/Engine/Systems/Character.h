#pragma once
//**********************************************************
//
//		Characterクラス
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/AI/HeuristicSearch.h"

class Character :
	public Object
{
private:
	//	名前を変えるかも health→life,HP
	int	current_health = 5;//現在の体力値
	int	max_health = 5;//最大体力値

	bool	is_decide_pos = false;//位置が決まった時にbreakするために使う

public:

	ShortestPath* shortest_path{};//最短経路

private:

	/// <summary>
/// 有限ステートマシンの初期化
/// </summary>
	virtual void FiniteStateMachineInitialize() {}

public:

	// 体力値を取得
	int GetCurrentHealth() const { return current_health; }

	// 体力値を設定
	void SetCurrentHealth(const int health) { this->current_health = health; }

	// 最大体力値を取得
	int GetMaxHealth() const { return max_health; }

	// 最大体力値を設定
	void SetMaxHealth(const int health) { this->max_health = health; }

	//最短経路を設定
	void SetShortestPath(std::vector<int> path) const { shortest_path->path_information = std::move(path); }

	bool GetIsDecidePos() const { return  is_decide_pos; }

	void SetIsDecidePos(bool set) { is_decide_pos = set; }

	virtual void Destroy()=0;
};


