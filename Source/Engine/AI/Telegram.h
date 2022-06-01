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
	END_PLAYER_TURN,
	END_ENEMY_TURN,
	GO_NEXT_FLOOR,
};

/// <summary>
/// メッセージを設定するクラス
/// </summary>
class Telegram final
{
public:
	int sender;			// 送り手のID
	int receiver;		// 受けてのID
	MESSAGE_TYPE msg;	// メッセージNumber

	Telegram(int sender, int receiver, const MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
