#pragma once

#include "Engine/Systems/Object.h"
// キャラクター
//Todo コメント
class Character : public Object
{
private:
	// 垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);

	// 垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);

	// 水平速力更新処理
	void UpdateHorizontalVelocity(float elapsedFrame);

	// 水平移動更新処理
	void UpdateHorizontalMove(float elapsedTime);
protected:
	// 着地した時に呼ばれる
	virtual void OnLanding() {}

	// ダメージを受けた時に呼ばれる
	virtual void OnDamaged() {}

	// 死亡した時に呼ばれる
	virtual void OnDead() {}

	// 移動処理
	void Move(float vx, float vz, float speed);

	// 旋回処理
	void Turn(float elapsed_time, float vx, float vz, float speed);

	// ジャンプ処理
	void Jump(float speed);

	// 空中ダッシュ処理
	void AirDush(float vx, float vz, float gravity_cut_time);

	// 速力更新処理
	void UpdateVelocity(float elapsed_time);

	// 無敵時間更新
	void UpdateInvincibleTimer(float elapsed_time);

	// 目標地点へ移動
	//void MoveToTarget(float elapsed_time, float speed_rate);

public:
	Character() {}
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Character() {}

	// 地面に接地しているか
	bool IsGround() const { return is_ground; }

	// 健康状態を取得
	int GetHealth() const { return health; }

	// 健康状態を設定
	void SetHealth(const int health) { this->health = health; }

	// 最大健康状態を取得
	int GetMaxHealth() const { return max_health; }

	// 最大健康状態を取得
	void SetMaxHealth() { this->max_health = max_health; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincible_time);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);


private:

public:

protected:
	DirectX::XMFLOAT3	velocity = { 0, 0, 0 };

	float				gravity = -1.0f;
	float				gravity_cut_time = 0;//重力を無視するときに使う
	bool				is_ground = false;
	int					health = 5;
	int					max_health = 5;
	float				invincible_timer = 0.0f;//無敵時間
	float				friction = 0.5f;
	float				acceleration = 1.0f;
	float				max_move_speed = 5.0f;
	float				move_vecX = 0.0f;
	float				move_vecZ = 0.0f;
	float				air_control = 0.3f;
	float				step_offset = 1.0f;
	float				slope_rate = 0.0f;
};
