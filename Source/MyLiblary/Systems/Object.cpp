//**********************************************************
//
//		Objectクラス
//
//**********************************************************

#include "Liblary/Systems/Object.h"
#include "Liblary/Systems/Math.h"
#include "Liblary/Objects/Model.h"
#include "Liblary/Systems/StageManager.h"
#include "Liblary/Systems/Collision.h"
#include "Liblary/Systems/Logger.h"

void Object::Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader)
{
	shader->Draw(device_context, this->GetModel());
}

float Object::NormalizeAnyAngle(float radian)
{
	return Math::NormalizeRadianAngle(radian);
}

void Object::NormalizeAngle()
{
	angle.x = NormalizeAnyAngle(angle.x);
	angle.y = NormalizeAnyAngle(angle.y);
	angle.z = NormalizeAnyAngle(angle.z);
}

void Object::SendMessaging(const MESSAGE_TYPE msg)
{
	switch (msg)
	{
	case MESSAGE_TYPE::END_PLAYER_TURN:
		LOG("\n")
		break;
	case MESSAGE_TYPE::END_ENEMY_TURN:
		LOG("\n")
		break;
	case MESSAGE_TYPE::GO_NEXT_FLOOR:
		LOG("\n")
		break;
	case MESSAGE_TYPE::GO_MAX_FLOOR:
		LOG("\n")
		break;
	default:
		LOG("\n")
		break;
	}
}

void	Object::SetModel(const char* pass)
{
	model = std::make_shared<Model>(pass);
}

void Object::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
#if 0
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
#endif
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

// 速力処理更新
void Object::UpdateVelocity(float elapsedTime)
{
	// 経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	// 垂直速力更新処理
	UpdateVerticalVelocity(elapsedFrame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(elapsedFrame);

	// 垂直移動更新処理
	UpdateVerticalMove(elapsedTime);

	// 水平移動更新処理
	UpdateHorizontalMove(elapsedTime);
}

// 無敵時間更新
void Object::UpdateInvincibleTimer(float elapsedTime)
{
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsedTime;
	}
}

// 垂直速力更新処理
void Object::UpdateVerticalVelocity(float elapsedFrame)
{
	// 重力処理
	gravity_cut_time -= elapsedFrame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsedFrame;
	}
}

// 垂直移動更新処理
void Object::UpdateVerticalMove(float elapsedTime)
{
	// 垂直方向の移動量
	float my = velocity.y * elapsedTime;

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
void Object::UpdateHorizontalVelocity(float elapsedFrame)
{
	// XZ平面の速力を減速する
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// 摩擦力
		float friction = this->friction * elapsedFrame;

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
		float moveVecLength = sqrtf(move_vec_x * move_vec_x + move_vec_z * move_vec_z);
		if (moveVecLength > 0.0f)
		{
			// 加速力
			float acceleration = this->acceleration * elapsedFrame;

			// 空中にいるときは加速力を減らす
			if (!is_ground) acceleration *= air_control;

			// 移動ベクトルによる加速処理
			velocity.x += move_vec_x * acceleration;
			velocity.z += move_vec_z * acceleration;

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
				velocity.y -= length * slope_rate * elapsedFrame;
			}
		}
	}
	// 移動ベクトルをリセット
	move_vec_x = 0.0f;
	move_vec_z = 0.0f;
}

// 水平移動更新処理
void Object::UpdateHorizontalMove(float elapsedTime)
{
	// 水平速力量計算
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocityLengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		// レイの開始位置と終点位置
		DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		DirectX::XMFLOAT3 end = { position.x + mx, position.y + step_offset, position.z + mz };

		// レイキャストによる壁判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// 壁までのベクトル
			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

			// 壁の法線
			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

			// 入射ベクトルを法線に射影
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

			// 補正位置の計算
			DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
			DirectX::XMFLOAT3 collectPosition;
			DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

			// 壁ずり方向へレイキャスト
			HitResult hit2;
			if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
			{
				// 壁ずり方向で壁に当たらなかったら補正位置に移動
				position.x = collectPosition.x;
				position.z = collectPosition.z;
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
}