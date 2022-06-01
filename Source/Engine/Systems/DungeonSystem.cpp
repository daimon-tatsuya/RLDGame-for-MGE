//**********************************************************
//
//		DungeonSystem�N���X
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
	//�o�߃^�[�����ő吔��
	//�����Ă���Ȃ�true
	if (GetElapsedTurn() > GetMaxTurn())
	{
		return true;
	}
	return false;
}

bool DungeonSystem::ExceededMaxFloor() const
{
	//���݂̊K�����ő吔��
	//�����Ă���Ȃ�true
	if (GetCurrentFloor() > GetMaxFloor())
	{
		return true;
	}
	return false;
}
