//**********************************************************
//
//		DungeonSystemクラス
//
//**********************************************************

#include <crtdbg.h>
#include <Engine/Systems/DungeonSystem.h>
#include "Engine/Systems/Logger.h"

DungeonSystem::DungeonSystem()
{
	LOG(" Error : DungeonSystem's destructor\n")
}

void DungeonSystem::Initialize()
{
	//変数の初期化
	 elapsed_turn = 0;	//経過ターン 敵の行動が終わるたびに1プラスする
	 max_turn = 999;		//最大経過ターン 経過ターンがこの値を超えたら

	 current_floor = 1;//現在の階数
	 max_floor = 4;   //最大階層
	is_dungeon_clear = false;//ダンジョンをクリアをしたらtrue
}
void DungeonSystem::Initialize(const int max_floor, const int max_turn)
{
	//変数の初期化
	elapsed_turn = 0;	//経過ターン 敵の行動が終わるたびに1プラスする
	this->max_turn = max_turn;		//最大経過ターン 経過ターンがこの値を超えたら

	current_floor = 1;//現在の階数
	this->max_floor = max_floor;   //最大階層
	is_dungeon_clear = false;//ダンジョンをクリアをしたらtrue
}
bool DungeonSystem::ExceededMaxTurn() const
{
	//経過ターンが最大数を
	//超えているならtrue
	if (GetElapsedTurn() > GetMaxTurn())
	{
		return true;
	}
	return false;
}

bool DungeonSystem::BreakingThroughTopFloor() const
{
	//現在の階数が最大数を
	//超えているならtrue
	if ( GetMaxFloor() == GetCurrentFloor())
	{
		return true;
	}
	return false;
}
