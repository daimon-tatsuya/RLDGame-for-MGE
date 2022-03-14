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
	// �v���C���[��G�ȂǂƂ̒ʐM�@�\(Messaging)
	// std::set�͏����t�����ꂽ�f�[�^�𕡐��ێ����邱�Ƃ��ł��鏇���t�W��
	std::set<Telegram> priority;

	static Meta* instance;

public:
	//CharacterManager* character_manager = nullptr;

	enum class Identity : int
	{
		Meta = 0,
		Stage,
		Camera,
		CharacterManager,
		Player,				// �v���C���[ID��4
		Team,				// ���Ԃ�ID��5~7
		Enemy				// �ȍ~�̒l�͑S�ēG
	};
private:

public:

	Meta();
	~Meta() = default;

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
	/// Identity�������̃N���X�̃��b�Z�[�W���V�[�o�[�̏������Ăяo���B
	/// </summary>
	/// <param name="identity"></param>
	/// <param name="telegram">����</param>
	void Discharge(int identity, const Telegram& telegram);

	/// <summary>
	/// MetaAI�p�̃��V�[�u�������w��
	/// </summary>
	/// <param name="telegram">����</param>
	void Discharge(const Telegram& telegram);

	/// <summary>
	///  ���b�Z�[�W��M���Ă��邩
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns></returns>
	bool HandleMessage(const Telegram& telegram);

	/// <summary>
	/// ���b�Z�[�W��M�����Ƃ��̏���
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns></returns>
	bool OnMessage(const Telegram& telegram);

	/// <summary>
	///  ���b�Z�[�W���M�֐�
	/// </summary>
	/// <param name="sender">���M��</param>
	/// <param name="receiver">��M��</param>
	/// <param name="msg">����</param>
	void SendMessaging(int sender, int receiver, MESSAGE_TYPE msg);
};