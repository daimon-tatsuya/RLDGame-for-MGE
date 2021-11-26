#pragma once

//**********************************************************
//
//		PlayerState�N���X
//
//**********************************************************

#include "Engine/Systems/StateBase.h"
// ToDo PlayerState �R�����g
//�O���錾
class Player;

class PlayerState :
	public StateBase
{
private:
public:
	PlayerState(Player* player) :owner(player) {};
	virtual ~PlayerState() {};



protected:
	//�Q�Ɨp�̂��߉�����Ȃ�
	Player* owner = nullptr;// ���L��

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
