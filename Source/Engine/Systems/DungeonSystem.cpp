//**********************************************************
//
//		DungeonSystem�N���X
//
//**********************************************************

#include "Engine/Systems/DungeonSystem.h"
#include <crtdbg.h>

DungeonSystem* DungeonSystem::instance = nullptr;//�B��̃|�C���^�̏�����

//�R���X�g���N�^
DungeonSystem::DungeonSystem()
{
	// �C���X�^���X�ݒ�
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;
}
