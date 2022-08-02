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

void DungeonSystem::Initialize()
{
	//�ϐ��̏�����
	 elapsed_turn = 0;	//�o�߃^�[�� �G�̍s�����I��邽�т�1�v���X����
	 max_turn = 999;		//�ő�o�߃^�[�� �o�߃^�[�������̒l�𒴂�����

	 current_floor = 1;//���݂̊K��
	 max_floor = 4;   //�ő�K�w
	is_dungeon_clear = false;//�_���W�������N���A��������true
}
void DungeonSystem::Initialize(const int max_floor, const int max_turn)
{
	//�ϐ��̏�����
	elapsed_turn = 0;	//�o�߃^�[�� �G�̍s�����I��邽�т�1�v���X����
	this->max_turn = max_turn;		//�ő�o�߃^�[�� �o�߃^�[�������̒l�𒴂�����

	current_floor = 1;//���݂̊K��
	this->max_floor = max_floor;   //�ő�K�w
	is_dungeon_clear = false;//�_���W�������N���A��������true
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

bool DungeonSystem::BreakingThroughTopFloor() const
{
	//���݂̊K�����ő吔��
	//�����Ă���Ȃ�true
	if ( GetMaxFloor() == GetCurrentFloor())
	{
		return true;
	}
	return false;
}
