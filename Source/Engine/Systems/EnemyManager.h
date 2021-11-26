#pragma once

//**********************************************************
//
//		EnemyManager�N���X
//
//**********************************************************


#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/EnemyBase.h"

class EnemyManager
{
private:

	std::vector<EnemyBase*>	 enemies;// �G�l�~�[�������i�[����
	std::vector<EnemyBase*>	 removes; // �폜����EnemyBase���i�[���邵��, characteres��index���w�肵�Ē��ڍ폜����̂����

	int enemy_number = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z����Id��t�^����)

public:

private:
	EnemyManager() {}
	~EnemyManager();
public:
	//EnemyManager�̐����ƍ폜�͑S��CharacterManager����s��
	 friend class CharacterManager;

	// �L�����N�^�[���m�̏Փˏ���
	void CollisionEnemyToEnemy();

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// �G�̂�S�폜
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �G��o�^
	void Register(EnemyBase* enemy_base);

	// �G���폜
	void Remove(EnemyBase* enemy_base);

	// �G�𐔎擾
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// �G�z����擾
	std::vector<EnemyBase*> GetEnemis() { return enemies; }

	// �G���擾
	EnemyBase* GetEnemy(int index) { return enemies.at(index); }

	// ID����G���擾
	EnemyBase* GetEnemyFromId(int id);

	/// <summary>
	/// �v���C���[�̎擾
	/// </summary>
	/// <param name="number">�v���C���[�̔ԍ�:0~3</param>
	/// <returns>number�Ԗڂ�Player</returns>
	//Character* GetPlayer(int number = 0);
};
