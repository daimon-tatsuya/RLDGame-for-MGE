#pragma once

#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/EnemyBase.h"
//ToDo CharacterManager �R�����g

class EnemyManager
{
private:

	std::vector<EnemyBase*>	 enemies;// �G�l�~�[�������i�[����
	std::vector<EnemyBase*>	 removes;

	int enemy_number = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)

public:

private:

	EnemyManager() {}
	~EnemyManager();

	// �L�����N�^�[���m�̏Փˏ���
	void CollisionCharacterToCharacter();

public:

	// �B��̃C���X�^���X�擾
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

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
	void Register(Character* character, int character_type);

	// �G���폜
	void Remove(Character* character);

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
	Character* GetPlayer(int number = 0);
};
