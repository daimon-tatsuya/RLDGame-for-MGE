#pragma once

//ToDo Telegram���s�����Ƃ�����

// ���M���郁�b�Z�[�W�̎��
enum class MESSAGE_TYPE : int
{
	MSG_CALL_HELP,				// ���Ԃ��Ă�(Enemy�A���^AI�̗����Ŏg�p)
	MSG_CHANGE_ATTACK_RIGHT,	// �U�����̈ڏ�(Enemy���烁�^AI�ւ̃��b�Z�[�W�p)
	MSG_GIVE_ATTACK_RIGHT,		// �U������^����i���^AI����Enemy�ւ̃��b�Z�[�W�p�j
	MSG_ASK_ATTACK_RIGHT,		// �U������v��(Enemy���烁�^AI�ւ̃��b�Z�[�W�p)
};

class Telegram
{
public:
	int sender;			// ������ID
	int receiver;		// �󂯂Ă�ID
	MESSAGE_TYPE msg;	// ���b�Z�[�WNumber

	//�R���X�g���N�^
	Telegram(int sender, int receiver, MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
