#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/CharacterManager.h"

static Meta* instance = nullptr;

Meta& Meta::Instance()
{
	return *instance;
}

Meta::Meta(CharacterManager* c_manager)
{
	instance = this;
}
void Meta::Update()
{
	//メタAIが監視している処理
}

// このメソッドはtelegramクラスを作成して
// Characterを継承するクラスのメッセージレシーバーの処理を呼び出す。
void Meta::Discharge(Character* receiver, const Telegram& telegram)
{
	if (!receiver->OnMessage(telegram))
	{
		//受信できなかったときの処理
		_RPT0(_CRT_WARN, "\n error:Enemy Receive Failed");
	}
}

// メタAIのレシーバー処理を呼び出す
void Meta::Discharge(const Telegram& telegram)
{
	if (!HandleMessage(telegram))
	{
		//受信できなかったときの処理
		_RPT0(_CRT_WARN, "\n error:Meta Receive Failed");
	}
}
// メッセージ受信しているか
bool Meta::HandleMessage(const Telegram& msg)
{
	// メッセージを受信した時
	if (OnMessage(msg))
	{
		return true;
	}
	return false;
}
// メッセージ受信したときの処理
bool Meta::OnMessage(const Telegram& telegram)
{
	//std::vector<Enemy*> enmVec;
	//enmVec.clear();
	int enemy_count = character_manager->GetEnemyCount();
	switch (telegram.msg)
	{
	default:
		break;

		//case MESSAGE_TYPE::MSG_CALL_HELP://だれかがプレイヤーを発見したので他の敵を呼ぶ
		//	for (int i = 0; i < enemyCount; i++)//全ての敵のリスト
		//	{
		//		int enemyId = enemyManager->GetEnemy(i)->GetId();
		//		//送信者を除くエネミーにメッセージを送る
		//		if (telegram.sender != enemyId)
		//		{
		//			// 誰かがプレイヤーを発見したので送信者を除く敵にメッセージを送る
		//			// メタAIから各敵に送るメッセージも MSG_CALL_HELP です。

		//			this->SendMessaging(static_cast<int>(Meta::Identity::Meta),
		//				enemyId,
		//				MESSAGE_TYPE::MSG_CALL_HELP);
		//		}
		//	}
		//	return true;
		//	break;
		//case MESSAGE_TYPE::MSG_CHANGE_ATTACK_RIGHT:
		//	//エネミーマネージャからATACK_RANGE以内のエネミーだけの配列を作成し
		//	for (int i = 0; i < enemyCount; i++)
		//	{
		//		Enemy* enemy = enemyManager->GetEnemy(i);
		//		DirectX::XMFLOAT3 position = enemy->GetPosition();
		//		DirectX::XMFLOAT3 targetPosition = player->GetPosition();
		//		float vx = targetPosition.x - position.x;
		//		float vy = targetPosition.y - position.y;
		//		float vz = targetPosition.z - position.z;
		//		float dist = sqrtf(vx * vx + vy * vy + vz * vz);

		//		if (dist < enemy->GetAttackRange())
		//		{
		//			enmVec.push_back(enemy);
		//		}
		//	}
		//	if (enmVec.size() > 0)
		//	{
		//		// 攻撃権をenmVecに登録された敵の一人にランダムに与える
		//		// enmVecのサイズで乱数で対象を決定、決定したエネミーに攻撃権の移譲メッセージを送信。
		//		// 送信メッセージは MESSAGE_TYPE::MSG_GIVE_ATTACK_RIGHT
		//		int who_attack = rand() % enmVec.size();
		//		//Telegram tel(static_cast<int>(Meta::Identity::Meta), static_cast<int>(Meta::Identity::Enemy) + who_attack, MESSAGE_TYPE::MSG_GIVE_ATTACK_RIGHT);
		//		this->SendMessaging(static_cast<int>(Meta::Identity::Meta),
		//			static_cast<int>(Meta::Identity::Enemy) + who_attack,
		//			MESSAGE_TYPE::MSG_GIVE_ATTACK_RIGHT);
		//	}
		//	return true;
		//	break;
		//case MESSAGE_TYPE::MSG_ASK_ATTACK_RIGHT: //敵から、最初の攻撃権を要求された
		//	int count = 0;//全ての敵について、攻撃権を持っていないか調べる。
		//	for (int i = 0; i < enemyCount; i++)
		//	{
		//		Enemy* enemy = enemyManager->GetEnemy(i);
		//		if (enemy->GetAttackFlg()) count++;
		//	}
		//	//他に攻撃権持っている敵がいないなら、要求してきた敵に攻撃権を与えるメッセージを送る。
		//	//要求してきた敵のidはtelegramから取得出来ます。
		//	if (count < 1)
		//	{
		//		// 誰も攻撃権を持っていなければMetaAIから送信者に攻撃権付与のメッセージを送る
		//		// メッセージはMSG_GIVE_ATTACK_RIGHT
		//		this->SendMessaging(static_cast<int>(Meta::Identity::Meta),
		//			telegram.sender,
		//			MESSAGE_TYPE::MSG_GIVE_ATTACK_RIGHT);
		//	}
		//	return true;
		//	break;
	}
	return false;
}
void Meta::SendMessaging(int sender, int receiver, MESSAGE_TYPE msg)
{
	if (receiver == static_cast<int>(Meta::Identity::Meta))
	{// MetaAI宛の時
		//メッセージデータを作成
		Telegram telegram(sender, receiver, msg);
		//ディレイ無しメッセージ（即時配送メッセージ）
		Discharge(telegram);
	}
	else
	{// エネミーが受信者のときどのエネミーに送信するか
		// 受信者のポインタを取得
		//Enemy* receiveEnemy = enemyManager->GetEnemyFromId(receiver);
		//レシーバー居ないとき関数を終了する
		//if (receiveEnemy == nullptr) return;
		//メッセージデータを作成
		//Telegram telegram(sender, receiver, msg);
		//ディレイ無しメッセージ（即時配送メッセージ）
		//Discharge(receiveEnemy, telegram);
	}
}