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

bool DungeonSystem::ExceededMaxFloor() const
{
	//現在の階数が最大数を
	//超えているならtrue
	if (GetCurrentFloor() > GetMaxFloor())
	{
		return true;
	}
	return false;
}
