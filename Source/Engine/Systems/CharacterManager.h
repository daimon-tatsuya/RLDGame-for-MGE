#pragma once

#include <vector>
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/EnemyBase.h"
// �L�����N�^�[�}�l�[�W���[
class CharacterManager
{
private:
	CharacterManager() {}
	~CharacterManager();

	// �L�����N�^�[���m�̏Փˏ���
	void CollisionCharacterToCharacter();

public:
	// �B��̃C���X�^���X�擾
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time);

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �L�����N�^�[��o�^
	void Register(Character* character, int character_type);

	// �L�����N�^�[�̂�S�폜
	void Clear();

	// �L�����N�^�[���폜
	void Remove(Character* character);

	// �L�����N�^�[�𐔎擾
	int GetCharacterCount() const { return static_cast<int>(characteres.size()); }

	// �L�����N�^�[���擾
	Character* GetCharacter(int index) { return characteres.at(index); }

	// �L�����N�^�[�𐔎擾
	int GetEnemyCount() { return static_cast<int>(enemies.size()); }

	// �G�l�~�[���擾
	EnemyBase* GetEnemy(int index) { return enemies.at(index); }

	//ID����L�����N�^�[���擾
	Character* GetCharacterFromId(int id);

private:
	std::vector<Character*>	 characteres;
	std::vector<Character*>	 removes;
	std::vector<EnemyBase*>  enemies;// �G�l�~�[�������i�[����
	int enemy_identity = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int player_identity = 0; // �t�^����ID�̒l(���̒l��MetaAI::Identity::Player�����Z���ĕt�^����)
public:
};
