#pragma once
#pragma once
#pragma once
#include "Engine/Systems/StateBase.h"
#include "Engine/Systems/EnemyBase.h"

// ToDo EnemyBaseState �R�����g

class EnemyBaseState :
	public StateBase
{
private:

public:

	EnemyBaseState(EnemyBase* enemy_base) :owner(enemy_base) {};
	virtual ~EnemyBaseState() {};

protected:

	//�Q�Ɨp�̂��߉�����Ȃ�
	EnemyBase* owner = nullptr;// ���L��

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
