#pragma once

#include <set>
#include "Telegram.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/EnemyBase.h"
#include "Engine/Systems/PlayerBase.h"

//ToDo Meta���s�����Ƃ�����

class Meta final
{
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
private:
	// Player��Enemy�Ƃ̒ʐM�@�\(Messaging)
	// std::set�͏����t�����ꂽ�f�[�^�𕡐��ێ����邱�Ƃ��ł��鏇���t�W��
	std::set<Telegram> priority;

public:
	CharacterManager* character_manager = nullptr;

	enum class Identity : int
	{
		Meta = 0,
		Stage,
		Player,			// �v���C���[ID��2�`5
		Camera = 6,
		Enemy = 7		// 7�ȍ~�͑S�ēG
	};
};