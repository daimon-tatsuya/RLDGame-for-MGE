#pragma once

//****************************************************
//
//		EnemyBaseStateクラス
//		EnemyBaseHierarchicalStateクラス
//
//****************************************************
#include "Engine/Systems/StateBase.h"
#include "Engine/Systems/EnemyBase.h"


/// <summary>
/// 敵クラスのStateの基底クラス
/// </summary>
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


/// <summary>
/// 敵クラスの親Stateの基底クラス
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
