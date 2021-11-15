#pragma once

#include "Game/Characters/EnemyState.h"

// ToDo EnemySnakeState ƒRƒƒ“ƒg

class EnemySnakeState :
	public EnemyBaseState
{
private:

public:

	EnemySnakeState(EnemyBase* enemy_base) :EnemyBaseState(enemy_base) {};
	virtual ~EnemySnakeState() {};

protected:

};


class EnemySnakeHierarchicalState :
	public  EnemySnakeState, public  EnemyBaseHierarchicalState
{
private:

public:

	EnemySnakeHierarchicalState(EnemyBase* enemy_base) :EnemySnakeState(enemy_base), EnemyBaseHierarchicalState(enemy_base) {};
	virtual ~EnemySnakeHierarchicalState() {};

protected:

};
