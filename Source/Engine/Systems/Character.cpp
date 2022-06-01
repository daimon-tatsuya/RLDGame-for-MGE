//**********************************************************
//
//		Character�N���X
//
//**********************************************************
#include "Engine/Systems/Character.h"
#include "Engine/Systems/CharacterManager.h"
//�f�X�g���N�^
Character::~Character()
{
	//�ŒZ�o�H�̔j��
	if (shortest_path!=nullptr)
	{
		shortest_path->Clear();
		delete shortest_path;
	}
}


void Character::Destroy()
{
	//�L�����N�^�[�}�l�[�W���[�̏������X�g�Ɏ��M��ǉ�
	CharacterManager::Instance().Remove(this);
}
