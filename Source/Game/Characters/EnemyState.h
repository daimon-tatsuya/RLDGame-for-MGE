#pragma once

//****************************************************
//
//		EnemyBaseState�N���X
//		EnemyBaseHierarchicalState�N���X
//
//****************************************************
#include "Engine/Systems/StateBase.h"
#include "Engine/Systems/EnemyBase.h"


/// <summary>
/// �G�N���X��State�̊��N���X
/// </summary>
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


/// <summary>
/// �G�N���X�̐eState�̊��N���X
/// </summary>
class EnemyBaseHierarchicalState :
	public  EnemyBaseState, public HierarchicalStateBase
{
private:

public:

	EnemyBaseHierarchicalState(EnemyBase* enemy_base) :EnemyBaseState(enemy_base) {};
	virtual ~EnemyBaseHierarchicalState() {};

protected:


};
