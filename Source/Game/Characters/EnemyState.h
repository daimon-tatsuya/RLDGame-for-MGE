#pragma once
#pragma once
#pragma once
#include "Engine/Systems/StateBase.h"
#include "Engine/Systems/EnemyBase.h"

// ToDo EnemyBaseState コメント

class EnemyBaseState :
	public StateBase
{
private:

public:

	EnemyBaseState(EnemyBase* enemy_base) :owner(enemy_base) {};
	virtual ~EnemyBaseState() {};

protected:

	//参照用のため解放しない
	EnemyBase* owner = nullptr;// 所有者

};


class EnemyBaseHierarchicalState :
	public  EnemyBaseState, public HierarchicalStateBase
{
private:

public:

	EnemyBaseHierarchicalState(EnemyBase* enemy_base) :EnemyBaseState(enemy_base) {};
	virtual ~EnemyBaseHierarchicalState() {};

protected:


};
