#pragma once
//**********************************************************
//
//		Telegramクラス
//
//**********************************************************


/// <summary>
/// 送信するメッセージの種類
/// </summary>
enum class MESSAGE_TYPE : int
{
	MSG_CALL_HELP,				// 仲間を呼ぶ(Enemy、メタAIの両方で使用)
	MSG_CHANGE_ATTACK_RIGHT,	// 攻撃権の移譲(EnemyからメタAIへのメッセージ用)
	MSG_GIVE_ATTACK_RIGHT,		// 攻撃権を与える（メタAIからEnemyへのメッセージ用）
	MSG_ASK_ATTACK_RIGHT,		// 攻撃権を要求(EnemyからメタAIへのメッセージ用)
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

	//コンストラクタ
	Telegram(int sender, int receiver, MESSAGE_TYPE msg) :sender(sender), receiver(receiver), msg(msg)
	{}
};
