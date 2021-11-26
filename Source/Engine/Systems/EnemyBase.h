#pragma once

//**********************************************************
//
//		EnemyBaseクラス
//
//**********************************************************

#include "Engine/Systems/Character.h"

//敵クラスの基底クラス
class EnemyBase :public Character
{
private:

protected:

public:


private:

public:

	EnemyBase() {};
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~EnemyBase()override {};

	// 破棄
	void Destroy();

	//敵同士の交差判定
	void CollisionEnemyToEnemy();

};
