#pragma once

#include "Game/Characters/EnemyState.h"

// ToDo EnemySnakeState �R�����g

class EnemySnakeState :
	public EnemyBaseState
{
private:
public:
	EnemySnakeState(EnemyBase* enemy_base) :EnemyBaseState(enemy_base) {};
	virtual ~EnemySnakeState() {};



protected:
	//�Q�Ɨp�̂��߉�����Ȃ�
	EnemyBase* owner = nullptr;// ���L��

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
