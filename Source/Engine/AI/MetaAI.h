#pragma once
//**********************************************************
//
//	Math�N���X
//
//**********************************************************

#include <set>
#include "Engine/AI/Telegram.h"


// �O���錾
class CharacterManager;
class Character;

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
		Team,			// ���Ԃ�ID��3~5
		Camera = 6,
		Enemy = 7		// 7�ȍ~�͑S�ēG
	};
private:

public:

	Meta(CharacterManager* c_manager);
	~Meta() {}

	// �B��̃C���X�^���X�擾
	static Meta& Instance();

	// �X�V����
	void Update();

	/// <summary>
	/// Character���p������N���X�̃��b�Z�[�W���V�[�o�[�̏������Ăяo���B
	/// </summary>
	/// <param name="receiver">���ߐ�</param>
	/// <param name="telegram">����</param>
	void Discharge(Character* receiver, const Telegram& telegram);

	/// <summary>
	/// MetaAI�p�̃��V�[�u�������w��
	/// </summary>
	/// <param name="telegram">����</param>
	void Discharge(const Telegram& telegram);

	/// <summary>
	///  ���b�Z�[�W��M���Ă��邩
	/// </summary>
	/// <param name="msg">����</param>
	/// <returns></returns>
	bool HandleMessage(const Telegram& msg);

	/// <summary>
	/// ���b�Z�[�W��M�����Ƃ��̏���
	/// </summary>
	/// <param name="msg">����</param>
	/// <returns></returns>
	bool OnMessage(const Telegram& msg);

	/// <summary>
	///  ���b�Z�[�W���M�֐�
	/// </summary>
	/// <param name="sender">���M��</param>
	/// <param name="receiver">��M��</param>
	/// <param name="msg">����</param>
	void SendMessaging(int sender, int receiver, MESSAGE_TYPE msg);

};