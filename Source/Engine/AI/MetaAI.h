#pragma once
//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include <set>
#include "Engine/AI/Telegram.h"
#include "Engine/Systems/EnemyBase.h"
#include "Game/Characters/Player.h"

//�O���錾
class CharacterManager;

/// <summary>
/// ���^AI�N���X
/// </summary>
class Meta final
{
private:
	// Player��Enemy�Ƃ̒ʐM�@�\(Messaging)
	// std::set�͏����t�����ꂽ�f�[�^�𕡐��ێ����邱�Ƃ��ł��鏇���t�W��
	std::set<Telegram> priority;

	static Meta* instance;

public:
	CharacterManager* character_manager = nullptr;

	enum class Identity : int
	{
		Meta = 0,
		Stage,
		Player,			// �v���C���[ID��2
		Team,			//���Ԃ�ID��3~5
		Camera = 6,
		Enemy = 7		// 7�ȍ~�͑S�ēG
	};
private:

public:

	// �R���X�g���N�^
	Meta(CharacterManager* c_manager);
	~Meta() {}

	// �B��̃C���X�^���X�擾
	static Meta& Instance();

	// MetaAI�̋@�\��ǉ����Ă���
	// �X�V����
	void Update();

	// ���̃��\�b�h��telegram�N���X���쐬����
	// Character���p������N���X�̃��b�Z�[�W���V�[�o�[�̏������Ăяo���B
	void Discharge(Character* receiver, const Telegram& telegram);

	// MetaAI�p�̃��V�[�u�������w��
	void Discharge(const Telegram& telegram);

	// ���b�Z�[�W��M���Ă��邩
	bool HandleMessage(const Telegram& msg);
	// ���b�Z�[�W��M�����Ƃ��̏���

	bool OnMessage(const Telegram& msg);
	// ���b�Z�[�W���M�֐�
	void SendMessaging(int sender, int receiver, MESSAGE_TYPE msg);

};