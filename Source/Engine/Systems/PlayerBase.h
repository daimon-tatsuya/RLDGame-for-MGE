#pragma once


#include "Engine/Systems/Character.h"

//Todo PlayerBase コメント
class PlayerBase : public Character
{
private:
	//float				move_speed = 5.0f;
	//float				turn_speed = DirectX::XMConvertToRadians(720);
	//bool				attack_flag = false;
	//float				jump_speed = 20.0f;
	//int					jump_count = 0;
	//float				jump_wait = 10.0f;
	//float				jump_wait_max = 10.0f;
	//int					jump_limit = 2;

	//LockonState			lockonState = LockonState::NotLocked;
	//float							lockonTargetChangeTime = 0;
	//float							lockonTargetChangeTimeMax = 8;
	////Todo　characterだけじゃなくその他のものにもできるようにする
	//Character* lockonCharacter = nullptr;
	//DirectX::XMFLOAT3	lockDirection = { 0,0,0 };

public:
private:

public:

	PlayerBase() {};
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	~PlayerBase()override {};
	//// スティック入力値から移動ベクトルを取得
	//DirectX::XMFLOAT3 GetMoveVec(const DirectX::XMFLOAT3& cameraRight, const DirectX::XMFLOAT3& cameraFront) const;

	//// 移動入力処理
	//virtual bool InputMove(float elapsedTime);

	///// ジャンプ入力処理
	//bool InputJump(float elapsedTime);

	//// 攻撃入力処理
	//bool InputAttack();

	// プレイヤーとエネミーとの衝突判定
	void CollisionPlayerToEnemis();


};
