#pragma once

#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/Character.h"

//Todo PlayerBase コメント
class PlayerBase : public Character
{
private:

public:

	PlayerBase() {};
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	~PlayerBase()override {};
	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec(const DirectX::XMFLOAT3& cameraRight, const DirectX::XMFLOAT3& cameraFront) const;

	// 移動入力処理
	virtual bool InputMove(float elapsedTime);

	// ジャンプ入力処理
	bool InputJump(float elapsedTime);

	// 攻撃入力処理
	bool InputAttack();

	// プレイヤーとエネミーとの衝突判定
	void CollisionPlayerToEnemis();

private:


public:
};
