#pragma once

//**********************************************************
//
//		EnemyManager�N���X
//
//**********************************************************


#include <vector>

//�O���錾
class Character;

class EnemyManager
{
private:

	std::vector<Character*>	 enemies;// �G�l�~�[�������i�[����
	std::vector<Character*>	 removes; // �폜����Enemy(Character)���i�[���邵��, characteres��index���w�肵�Ē��ڍ폜����̂����
public:

private:

	EnemyManager() {}
	~EnemyManager();

public:

	//EnemyManager�̐����ƍ폜�͑S��CharacterManager����s��
	 friend class CharacterManager;

	// �G�̂�S�폜
	void Clear();

	/// <summary>
	/// �G��o�^
	/// </summary>
	/// <param name="character">�o�^����L����</param>
	void Register(Character* character);

	/// <summary>
	/// �G���폜
	/// </summary>
	/// <param name="character">�폜����L����</param>
	void Remove(Character* character);

//------------------------------------------------
//
// Getter
//
//------------------------------------------------

	// �G�𐔎擾
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// �G�z����擾
	std::vector<Character*> GetEnemis() { return enemies; }

	// �G���擾
	Character* GetEnemy(int index) { return enemies.at(index); }

	// ID����G���擾
	Character* GetEnemyFromId(int id);

};
