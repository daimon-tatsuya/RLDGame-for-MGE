//**********************************************************
//
//		Objectクラス
//
//**********************************************************

#include "Engine/Systems/Object.h"
#include "Engine/Systems/Math.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Collision.h"

void Object::UpdateTransform()
{
	// スケール行列を作成
	const DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
#if 0
	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
#else

	const DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	const DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	const DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	const DirectX::XMMATRIX R = Y * X * Z;

#endif
	// 位置行列を作成
	const DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	const DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

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
	//switch (msg)
	//{
	//case MESSAGE_TYPE::END_PLAYER_TURN:

	//	break;
	//case MESSAGE_TYPE::END_ENEMY_TURN:

	//	break;
	//default:
	//	break;
	//}
}

void	Object::SetModel(const char* pass)
{
	model = std::make_shared<Model>(pass);
}

void Object::UpdateVelocity(float elapsed_time)
{
	// 経過フレーム
	const float elapsed_frame = 60.0f * elapsed_time;

	// 垂直速力更新処理
	UpdateVerticalVelocity(elapsed_frame);

	// 水平速力更新処理
	UpdateHorizontalVelocity(elapsed_frame);

	// 垂直移動更新処理
	UpdateVerticalMove(elapsed_time);

	// 水平移動更新処理
	UpdateHorizontalMove(elapsed_time);
}

void Object::UpdateInvincibleTimer(float elapsed_time)
{
	if (invincible_timer > 0.0f)
	{
		invincible_timer -= elapsed_time;
	}
}

void Object::UpdateVerticalVelocity(float elapsed_frame)
{
	// 重力処理
	gravity_cut_time -= elapsed_frame;
	if (gravity_cut_time <= 0.0f)
	{
		velocity.y += gravity * elapsed_frame;
	}
}

void Object::UpdateVerticalMove(float elapsed_time)
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
		const DirectX::XMFLOAT3 start = { position.x, position.y + step_offset, position.z };
		// レイの終点位置は移動後の位置
		const DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

		// レイキャストによる地面判定
		HitResult hit;
		if (StageManager::Instance().RayCast(start, end, hit))
		{
			// 地面に接地している
			position = hit.position;

			// 傾斜率の計算
			const float normal_lengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
			slope_rate = 1.0f - (hit.normal.y / (normal_lengthXZ + hit.normal.y));

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

void Object::UpdateHorizontalVelocity(float elapsed_frame)
{
	//! XZ平面の速力を減速する
	const float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
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

	//! XZ平面の速力を加速する
	if (length <= max_move_speed)
	{
		// 移動ベクトルがゼロベクトルでないなら加速する
		const float move_vec_length = sqrtf(move_vec_x * move_vec_x + move_vec_z * move_vec_z);
		if (move_vec_length > 0.0f)
		{
			// 加速力
			float acceleration = this->acceleration * elapsed_frame;

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
				velocity.y -= length * slope_rate * elapsed_frame;
			}
		}
	}
	// 移動ベクトルをリセット
	move_vec_x = 0.0f;
	move_vec_z = 0.0f;
}

void Object::UpdateHorizontalMove(float elapsed_time)
{
	// 水平速力量計算
	const float velocity_lengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (velocity_lengthXZ > 0.0f)
	{
		// 水平移動値
		float mx = velocity.x * elapsed_time;
		float mz = velocity.z * elapsed_time;

		//MoveCheck(mx, mz);
	}
}