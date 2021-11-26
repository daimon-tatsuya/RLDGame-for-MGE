//**********************************************************
//
//		Characterクラス
//
//**********************************************************

#include "Engine/Systems/Character.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Math.h"

// ダメージを与える
bool Character::ApplyDamage(int damage, float invincibleTime)
{
	// ダメージが0の場合は健康状態を変更する必要がない
	if (damage == 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (health <= 0) return false;

	// 無敵時間中はダメージを与えない
	if (invincible_timer > 0.0f) return false;

	// 無敵時間設定
	invincible_timer = invincibleTime;

	// ダメージ処理
	health -= damage;

	// 死亡通知
	if (health <= 0)
	{
		OnDead();
	}
	// ダメージ通知
	else
	{
		OnDamaged();
	}

	// 健康状態が変更した場合はtrueを返す
	return true;
}

// 衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// 速力に力を加える
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

// 移動処理
void Character::Move(float vx, float vz, float speed)
{
	// 移動方向ベクトルを設定
	move_vecX = vx;
	move_vecZ = vz;

	// 最大速度設定
	max_move_speed = speed;
}

void Character::MoveChack(float mx, float mz)
{
	// レイの開始位置と終点位置
	DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
	DirectX::XMFLOAT3 end = { position.x + mx, position.y + step_offset, position.z + mz };

	// レイキャストによる壁判定
	HitResult hit;
	if (StageManager::Instance().RayCast(start, end, hit))
	{
		// 壁までのベクトル
		DirectX::XMVECTOR v_start = DirectX::XMLoadFloat3(&start);
		DirectX::XMVECTOR v_end = DirectX::XMLoadFloat3(&end);
		DirectX::XMVECTOR v_vec = DirectX::XMVectorSubtract(v_end, v_start);

		// 壁の法線
		DirectX::XMVECTOR v_normal = DirectX::XMLoadFloat3(&hit.normal);

		// 入射ベクトルを法線に射影
		DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(v_vec), v_normal);

		// 補正位置の計算
		DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(v_normal, Dot, v_end);
		DirectX::XMFLOAT3 collect_position;
		DirectX::XMStoreFloat3(&collect_position, CollectPosition);

		// 壁ずり方向へレイキャスト
		HitResult hit2;
		if (!StageManager::Instance().RayCast(hit.position, collect_position, hit2))
		{
			// 壁ずり方向で壁に当たらなかったら補正位置に移動
			position.x = collect_position.x;
			position.z = collect_position.z;
		}
		else
		{
			position.x = hit2.position.x;
			position.z = hit2.position.z;
		}
	}
	else
	{
		// 移動
		position.x += mx;
		position.z += mz;
	}

}

// 旋回処理
//ToDo アクションゲーム用なので後回し https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// ゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.001f) return;

	// 単位ベクトル化
	vx /= length;
	vz /= length;

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (frontX * vx) + (frontZ * vz);

	// 内積値は-1.0～1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot;
	if (rot > speed) rot = speed;

	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (frontX * vz) - (frontZ * vx);

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える
	// 左右判定を行うことによって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

// ジャンプ処理
void Character::Jump(float speed)
{
	// 上方向の力を設定
	velocity.y = speed;
}

// 空中ダッシュ処理
void Character::AirDush(float vx, float vz, float gravityCutTime)
{
	velocity.x = vx;
	velocity.z = vz;
	velocity.y = 0;
	this->gravity_cut_time = gravityCutTime;
}

// 速力処理更新
void Character::UpdateVelocity(float elapsed_time)
{
	// 経過フレーム
	float elapsed_frame = 60.0f * elapsed_time;

	// 垂直速力更新処理
	UpdateVerticalVelocity(elapsed_frame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(elapsed_frame);

	// 垂直移動更新処理
	UpdateVerticalMove(elapsed_time);

	// 水平移動更新処理
	UpdateHorizontalMove(elapsed_time);
}

// 無敵時間更新
void Character::UpdateInvincibleTimer(float elapsed_time)
{
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

// 垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsed_frame)
{
	// 重力処理
	gravity_cut_time -= elapsed_frame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsed_frame;
	}
}

// 垂直移動更新処理
void Character::UpdateVerticalMove(float elapsed_time)
{
	// 垂直方向の移動量
	float my = velocity.y * elapsed_time;

	// キャラクターのY軸方向となる法線ベクトル
	DirectX::XMFLOAT3 normal = { 0, 1, 0 };

	slope_rate = 0.0f;

	// 落下中
	if (my < 0.0f)
	{
		// レイの開始位置は足元より少し上
		DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		// レイの終点位置は移動後の位置
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		// レイキャストによる地面判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// 地面に接地している
			position = hit.position;

			// 傾斜率の計算
			float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slope_rate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

			// 法線ベクトル取得
			normal = hit.normal;

			// 回転
			angle.y += hit.rotation.y;

			// 着地した
			if (!is_ground)
			{
				OnLanding();
			}
			is_ground = true;
			velocity.y = 0.0f;
		}
		else
		{
			// 空中に浮いている
			position.y += my;
			is_ground = false;
		}
	}
	// 上昇中
	else if (my > 0.0f)
	{
		position.y += my;
		is_ground = false;
	}

	// 地面の向きに沿うようにXZ軸回転
	{
		// Y軸が法線ベクトル方向に向くオイラー角回転を算出する
		float ax = atan2f(normal.z, normal.y);
		float az = -atan2f(normal.x, normal.y);

		// 線形補完で滑らかに回転する
		angle.x = Math::LerpFloat(angle.x, ax, 0.2f);
		angle.z = Math::LerpFloat(angle.z, az, 0.2f);
	}
}

// 水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsed_frame)
{
	// XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float friction = this->friction * elapsed_frame;

		// 空中にいるときは摩擦力を減らす
		if (!is_ground) friction *= air_control;

		// 摩擦による横方向の減速処理
		if (length > friction)
		{
			// 単位ベクトル化
			float vx = velocity.x / length;
			float vz = velocity.z / length;

			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		// 横方向の速力が摩擦力以下になったので速力を無効化
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}

	// XZ平面の速力を加速する
	if (length <= max_move_speed)
	{
		// 移動ベクトルがゼロベクトルでないなら加速する
		float move_vec_length = sqrtf(move_vecX * move_vecX + move_vecZ * move_vecZ);
		if (move_vec_length > 0.0f)
		{
			// 加速力
			float acceleration = this->acceleration * elapsed_frame;

			// 空中にいるときは加速力を減らす
			if (!is_ground) acceleration *= air_control;

			// 移動ベクトルによる加速処理
			velocity.x += move_vecX * acceleration;
			velocity.z += move_vecZ * acceleration;

			// 最大速度制限
			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > max_move_speed)
			{
				float vx = velocity.x / length;
				float vz = velocity.z / length;

				velocity.x = vx * max_move_speed;
				velocity.z = vz * max_move_speed;
			}

			// 下り坂でガタガタしないようにする
			if (is_ground && slope_rate > 0.0f)
			{
				velocity.y -= length * slope_rate * elapsed_frame;
			}
		}
	}
	// 移動ベクトルをリセット
	move_vecX = 0.0f;
	move_vecZ = 0.0f;
}

// 水平移動更新処理
void Character::UpdateHorizontalMove(float elapsed_time)
{
	// 水平速力量計算
	float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocity_lengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsed_time;
		float mz = velocity.z * elapsed_time;

		MoveChack(mx, mz);
	}
}

