#pragma once

//**********************************************************
//
//	CharacterManager�N���X
//
//**********************************************************

#include <vector>
#include<memory>
#include <d3d11.h>



//�O���錾
class Shader;
class Character;

/// <summary>
/// Character���Ǘ�����N���X
/// </summary>
class CharacterManager
{
private:

	std::vector<std::shared_ptr<Character>>	  characteres;			// �G�����֌W�Ȃ��i�[����
	std::vector<std::shared_ptr<Character>>	  removes;				// �폜����Character���i�[���邵��,characteres��index���w�肵�Ē��ڍ폜����̂����
	int									  enemy_number = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int									  player_number = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Player�����Z���ĕt�^����)

public:

private:
	CharacterManager() {}
	~CharacterManager(){}
public:

	// �B��̃C���X�^���X�擾
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// �L�����N�^�[���m�̏Փˏ���
	void CollisionCharacterToCharacter();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="elapsed_time"></param>
	void Update(float elapsed_time);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dc">DeviceContext</param>
	/// <param name="shader">�`��̎d��</param>
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// �L�����N�^�[�̂�S�폜
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	/// <summary>
	/// �L�����N�^�[��o�^
	/// </summary>
	/// <param name="character">�o�^����L�����N�^�[</param>
	/// <param name="character_type">Meta::Identity</param>
	void Register(Character* character, int character_type);

	/// <summary>
	/// �L�����N�^�[���폜
	/// </summary>
	/// <param name="character">�폜����L�����N�^�[</param>
	void Remove(Character* character);

	/// <summary>
	/// ID����L�����N�^�[���擾
	/// </summary>
	/// <param name="id">�擾����L�����N�^�[��ID</param>
	/// <returns></returns>
	Character* GetCharacterFromId(int id);

	/// <summary>
	/// �v���C���[�̎擾
	/// </summary>
	/// <param name="number">�v���C���[�̔ԍ�:0~3</param>
	/// <returns>number�Ԗڂ�Player</returns>
	Character* GetPlayer(int number = 0);

	//------------------------------------------------
	//
	// Getter
	//
	//------------------------------------------------

	// �L�����N�^�[�𐔎擾
	int GetCharacterCount() const { return static_cast<int>(characteres.size()); }

	//�@index �Ԗڂ̃����N�^�[���擾
	Character* GetCharacter(int index) { return characteres.at(index).get(); }

	//�G�̐����擾
	int GetEnemyCount() const { return  enemy_number; }

	// index �Ԗڂɍł��߂�ID�̓G���擾
	Character* GetEnemy(int index);

};
