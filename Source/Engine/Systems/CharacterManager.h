#pragma once
//**********************************************************
//
//	   CharacterManager�N���X
//
//**********************************************************

#include <vector>
#include<memory>
#include <d3d11.h>
#include "Engine/AI/Telegram.h"

//�O���錾
class Shader;
class Character;

/// <summary>
/// Character���Ǘ�����N���X
/// </summary>
class CharacterManager final
{
private:

	std::vector<std::shared_ptr<Character>>	  characters;			// �G�����֌W�Ȃ��i�[����
	std::vector<std::shared_ptr<Character>>	  removes;				// �폜����Character���i�[���邵��,characters��index���w�肵�Ē��ڍ폜����̂����

	int enemy_number = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Enemy�����Z���ĕt�^����)
	int team_number = 0;	// �t�^����ID�̒l(���̒l��MetaAI::Identity::Team�����Z���ĕt�^����)

	//bool	is_player_turn = true;//�v���C���[�̃^�[���̂Ƃ���true�ɂȂ��āA�X�V�֐������s�����
	//bool	is_enemy_turn = false;//�G�̃^�[���̂Ƃ���true�ɂȂ��āA�X�V�֐������s�����

public:

private:
	//�R���X�g���N�^
	CharacterManager() = default;
	//�f�X�g���N�^
	~CharacterManager() = default;
public:

	// �B��̃C���X�^���X�擾
	static CharacterManager& Instance()
	{
		static CharacterManager instance;
		return instance;
	}

	// �L�����N�^�[���m�̏Փˏ���
	void CollisionCharacterToCharacter() const;

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
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) const;

	// �L�����N�^�[�̂�S�폜
	void Clear();

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() const;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI() const;

	/// <summary>
	/// �L�����N�^�[��o�^
	/// </summary>
	/// <param name="character">�o�^����L�����N�^�[</param>
	/// <param name="character_type">Identity</param>
	void Register(Character* character, int character_type);

	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram);

	/// <summary>
	/// �L�����N�^�[���폜
	/// </summary>
	/// <param name="character">�폜����L�����N�^�[</param>
	void Remove(Character* character);

	/// <summary>
   /// �G�L�����N�^�[��S�폜
   /// </summary>
	void RemoveEnemy() const;

	//------------------------------------------------
	//
	// Getter
	//
	//------------------------------------------------

	/// <summary>
	/// ID����L�����N�^�[���擾
	/// </summary>
	/// <param name="id">�擾����L�����N�^�[��ID</param>
	/// <returns></returns>
	Character* GetCharacterFromId(int id) const;

	/// <summary>
	/// �v���C���[�̎擾
	/// </summary>
	/// <returns>Player</returns>
	Character* GetPlayer() const;

	// �L�����N�^�[�𐔎擾
	int GetCharacterCount() const { return static_cast<int>(characters.size()); }

	//�@index �Ԗڂ̃L�����N�^�[���擾
	Character* GetCharacter(int index) const { return characters.at(index).get(); }

	//�L�����N�^�[�̃R���e�i���擾
	std::vector<std::shared_ptr<Character>> GetCharacters() const { return characters; }

	//�G�̐����擾
	int GetEnemyCount() const { return  enemy_number; }

	//�G�̒��� index �Ԗڂ�ID�̓G���擾
	Character* GetEnemy(int index) const;
};
