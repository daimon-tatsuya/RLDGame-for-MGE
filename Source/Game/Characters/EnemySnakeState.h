#pragma once

#include "Game/Characters/EnemyState.h"

// ToDo EnemySnakeState コメント

class EnemySnakeState :
	public EnemyBaseState
{
private:
public:
	EnemySnakeState(EnemyBase* enemy_base) :EnemyBaseState(enemy_base) {};
	virtual ~EnemySnakeState() {};



protected:
	//参照用のため解放しない
	EnemyBase* owner = nullptr;// 所有者

};


class EnemySnakeHierarchicalState :
	public  EnemySnakeState, public  EnemyBaseHierarchicalState
{
private:
public:
	EnemySnakeHierarchicalState(EnemyBase* enemy_base) :EnemySnakeState(enemy_base) {};
	virtual ~EnemySnakeHierarchicalState() {};
protected:

};
