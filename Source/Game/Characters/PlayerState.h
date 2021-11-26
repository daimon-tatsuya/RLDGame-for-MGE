#pragma once

//**********************************************************
//
//		PlayerStateクラス
//
//**********************************************************

#include "Engine/Systems/StateBase.h"
// ToDo PlayerState コメント
//前方宣言
class Player;

class PlayerState :
	public StateBase
{
private:
public:
	PlayerState(Player* player) :owner(player) {};
	virtual ~PlayerState() {};



protected:
	//参照用のため解放しない
	Player* owner = nullptr;// 所有者

};


class PlayerHierarchicalState :
	public  PlayerState, public HierarchicalStateBase
{
private:
public:
	PlayerHierarchicalState(Player* player) :PlayerState(player) {};
	virtual ~PlayerHierarchicalState() {};
protected:

};
