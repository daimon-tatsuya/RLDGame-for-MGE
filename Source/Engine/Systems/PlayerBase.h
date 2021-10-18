#pragma once
#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/Character.h"

class PlayerBase : public Character
{
private:

protected:
	// 着地した時に呼ばれる
	virtual void OnLanding() override;

	// ダメージを受けた時に呼ばれる
	virtual void OnDamaged() override;

	// 死亡した時に呼ばれる
	virtual void OnDead() override;

	// スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec(const DirectX::XMFLOAT3& cameraRight, const DirectX::XMFLOAT3& cameraFront) const;

	// 移動入力処理
	virtual bool InputMove(float elapsedTime);

	// ジャンプ入力処理
	bool InputJump(float elapsedTime);

	// 攻撃入力処理
	bool InputAttack();

public:
	PlayerBase() {};
	~PlayerBase()override {};

	// 更新処理
	virtual void Update(float elapsedTime)override = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override = 0;

	// プレイヤーとエネミーとの衝突判定
	void CollisionPlayerToEnemis();

	// デバッグ用GUI描画
	virtual void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive()override;

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) override;
private:

	float				move_speed = 5.0f;
	float				turn_speed = DirectX::XMConvertToRadians(720);
	bool				attack_flag = false;
	float				jump_speed = 20.0f;
	int					jump_count = 0;
	float				jump_wait = 10.0f;
	float				jump_wait_max = 10.0f;
	int					jump_limit = 2;

	//LockonState			lockonState = LockonState::NotLocked;
	//float							lockonTargetChangeTime = 0;
	//float							lockonTargetChangeTimeMax = 8;
	////Todo　characterだけじゃなくその他のものにもできるようにする
	//Character* lockonCharacter = nullptr;
	//DirectX::XMFLOAT3	lockDirection = { 0,0,0 };
public:
};
