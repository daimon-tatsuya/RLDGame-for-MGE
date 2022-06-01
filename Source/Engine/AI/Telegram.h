#pragma once
//**********************************************************
//
//	Telegram�N���X
//
//**********************************************************

/// <summary>
/// ���M���郁�b�Z�[�W�̎��
/// </summary>
enum class MESSAGE_TYPE : int
{
	END_PLAYER_TURN,
	END_ENEMY_TURN,
	GO_NEXT_FLOOR,
};

/// <summary>
/// ���b�Z�[�W��ݒ肷��N���X
/// </summary>
class Telegram final
{
public:
	int sender;			// ������ID
	int receiver;		// �󂯂Ă�ID
	MESSAGE_TYPE msg;	// ���b�Z�[�WNumber

	Telegram(int sender, int receiver, const MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
