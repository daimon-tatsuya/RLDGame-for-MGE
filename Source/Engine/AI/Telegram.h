#pragma once
//**********************************************************
//
//	Telegramクラス
//
//**********************************************************


/// <summary>
/// 送信するメッセージの種類
/// </summary>
enum class MESSAGE_TYPE : int
{
	MSG_END_PLAYER_TURN,
	MSG_END_ENEMY_TURN,
};

/// <summary>
/// メッセージを設定するクラス
/// </summary>
class Telegram
{
public:
	int sender;			// 送り手のID
	int receiver;		// 受けてのID
	MESSAGE_TYPE msg;	// メッセージNumber

	Telegram(int sender, int receiver, const MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
