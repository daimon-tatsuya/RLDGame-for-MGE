//**********************************************************
//
//	EnemyManager�N���X
//
//**********************************************************

#include "Liblary/Systems/EnemyManager.h"
#include "Liblary/Systems/Character.h"


EnemyManager::~EnemyManager()
{
	this->Clear();
}

// �L�����N�^�[�̓o�^
void EnemyManager::Register(Character* character)
{
		// �o�^
		enemies.emplace_back(character);
}

// �L�����N�^�[�̑S�폜
void EnemyManager::Clear()
{
	for (const auto& enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}

// �L�����N�^�[�̍폜
void EnemyManager::Remove(Character* character)
{
	// �j�����X�g�ɂ��łɂ���Βe��
	for (const auto& it : removes)
	{
		if (it == character)
			break;
	}
	// �j�����X�g�ɒǉ�
	removes.emplace_back(character);
}

// ID����L�����N�^�[���擾����
Character* EnemyManager::GetEnemyFromId(int id)
{
	for (const auto& enemy : enemies)
	{
		if (enemy->GetId() == id)
			return enemy;
	}
	return nullptr;
}
