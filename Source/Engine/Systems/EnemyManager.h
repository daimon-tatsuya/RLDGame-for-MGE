#pragma once

//**********************************************************
//
//		EnemyManager�N���X
//
//**********************************************************


#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"


class EnemyManager
{
private:

	std::vector<Character*>	 enemies;// �G�l�~�[�������i�[����
	std::vector<Character*>	 removes; // �폜����EnemyBase���i�[���邵��, characteres��index���w�肵�Ē��ڍ폜����̂����
public:

private:

	EnemyManager() {}
	~EnemyManager();

public:

	//EnemyManager�̐����ƍ폜�͑S��CharacterManager����s��
	 friend class CharacterManager;

	// �L�����N�^�[���m�̏Փˏ���
	//void CollisionEnemyToEnemy();

	//// �X�V����
	//void Update(float elapsed_time);

	//// �`�揈��
	//void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// �G�̂�S�폜
	void Clear();

	//// �f�o�b�O�v���~�e�B�u�`��
	//void DrawDebugPrimitive();

	//// �f�o�b�O�pGUI�`��
	//void DrawDebugGUI();

	// �G��o�^
	void Register(Character* enemy_base);

	// �G���폜
	void Remove(Character* enemy_base);

	// �G�𐔎擾
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// �G�z����擾
	std::vector<Character*> GetEnemis() { return enemies; }

	// �G���擾
	Character* GetEnemy(int index) { return enemies.at(index); }

	// ID����G���擾
	Character* GetEnemyFromId(int id);

};
