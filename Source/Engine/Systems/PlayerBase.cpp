//**********************************************************
//
//		PlayerBaseクラス
//
//**********************************************************
#include "Engine/Systems/PlayerBase.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"

void PlayerBase::CollisionPlayerToEnemis()
{
	//CharacterManager& character_manager = CharacterManager::Instance();

	//// 全ての敵と総当たりで衝突処理
	//int enemy_count = character_manager.GetEnemyCount();
	//for (int i = 0; i < enemy_count; ++i)
	//{
	//	Character* enemy = character_manager.GetEnemy(i);

	//	// 衝突処理
	//	DirectX::XMFLOAT3 out_enemy_position, out_player_position;
	//	if (Collision::IntersectSphereToSphere(
	//		this->position,
	//		this->radius,
	//		enemy->GetPosition(),
	//		enemy->GetRadius(),
	//		out_player_position,
	//		out_enemy_position)
	//		)
	//	{
	//		// 押し出し後の位置設定
	//		enemy->SetPosition(out_enemy_position);
	//		this->SetPosition(out_player_position);
	//	}
	//}
}

//DirectX::XMFLOAT3 PlayerBase::GetMoveVec(const DirectX::XMFLOAT3& camera_right, const DirectX::XMFLOAT3& camera_front) const
//{
//	//入力された情報を取得
//	GamePad& game_pad = Input::Instance().GetGamePad();
//	float ax = game_pad.GetAxisLX();
//	float ay = game_pad.GetAxisLY();
//
//	// 移動ベクトルはXZ平面に水平なベクトルなるようにする
//
//	// カメラ右方向ベクトルをXZ単位ベクトルに変換
//	float camera_rightX = camera_right.x;
//	float camera_rightZ = camera_right.z;
//	float camera_right_length = sqrtf(camera_rightX * camera_rightX + camera_rightZ * camera_rightZ);
//	if (camera_right_length > 0.0f)
//	{
//		// 単位ベクトル化
//		camera_rightX /= camera_right_length;
//		camera_rightZ /= camera_right_length;
//	}
//
//	// カメラ前方向ベクトルをXZ単位ベクトルに変換
//	float camera_frontX = camera_front.x;
//	float camera_frontZ = camera_front.z;
//	float camera_front_length = sqrtf(camera_frontX * camera_frontX + camera_frontZ * camera_frontZ);
//	if (camera_front_length > 0.0f)
//	{
//		// 単位ベクトル化
//		camera_frontX /= camera_front_length;
//		camera_frontZ /= camera_front_length;
//	}
//
//	// スティックの水平入力値をカメラ右方向に反映し、
//	// スティックの垂直入力値をカメラ前方向に反映し、
//	// 進行ベクトルを計算する
//	DirectX::XMFLOAT3 vec;
//	vec.x = (camera_rightX * ax) + (camera_frontX * ay);
//	vec.z = (camera_rightZ * ax) + (camera_frontZ * ay);
//	// Y軸方向には移動しない
//	vec.y = 0.0f;
//
//	return vec;
//}
//
//bool PlayerBase::InputMove(float elapsedTime)
//{
//	// カメラ方向とスティックの入力値によって進行方向を計算する
//	Camera& camera = Camera::Instance();
//	DirectX::XMFLOAT3 camera_right = camera.GetRight();
//	DirectX::XMFLOAT3 camera_front = camera.GetFront();
//	//if (lockonState != LockonState::NotLocked)
//	//{
//	//	//	ロックオン中は敵への向きで考える
//	//	cameraFront = lockDirection;
//	//	DirectX::XMVECTOR	z = DirectX::XMLoadFloat3(&lockDirection);
//	//	DirectX::XMVECTOR	y = DirectX::XMVectorSet(0, 1, 0, 0);
//	//	DirectX::XMVECTOR	x = DirectX::XMVector3Cross(y, z);
//	//	DirectX::XMStoreFloat3(&cameraRight, x);
//	//}
//	// 進行ベクトル取得
//	DirectX::XMFLOAT3 move_vec = GetMoveVec(camera_right, camera_front);
//
//	// 移動処理
//	Move(move_vec.x, move_vec.z, this->move_speed);
//
//	// 旋回処理
//	//if (jumpCount == 0)
//	//{
//		//if (lockonState != LockonState::NotLocked)
//		//{
//		//	//	ロックオン処理中はロックオン対象に向ける
//		//	Turn(elapsedTime, cameraFront.x, cameraFront.z, turnSpeed);
//		//}
//		//else
//		//{
//	Turn(elapsedTime, move_vec.x, move_vec.z, this->turn_speed);
//	//}
////}
//
//// 進行ベクトルがゼロベクトルでない場合は入力された
//	return move_vec.x != 0.0f || move_vec.y != 0.0f || move_vec.z != 0.0f;
//}
//
//// ジャンプ入力処理
//bool PlayerBase::InputJump(float elapsedTime)
//{
//	// 経過フレーム
//	float elapsedFrame = 60.0f * elapsedTime;
//	jump_wait -= elapsedFrame;
//	if (jump_wait > 0)
//		return false;
//
//	// ジャンプ回数制限
//	if (jump_count < jump_limit)
//	{
//		GamePad& gamePad = Input::Instance().GetGamePad();
//		if (gamePad.GetButtonDown() & GamePad::BTN_A)
//		{
//			float ax = gamePad.GetAxisLX();
//			float ay = gamePad.GetAxisLY();
//			if (ax * ax + ay * ay < 0.3f * 0.3f || jump_count == 0)
//			{
//				// ジャンプ
//				Jump(jump_speed);
//			}
//			else
//			{
//				// 空中ダッシュ
//				// 自身の回転値から前方向を求めてその方向へ進ませる
//				float frontX = sinf(angle.y) * 12;
//				float frontZ = cosf(angle.y) * 12;
//				AirDush(frontX, frontZ, 15.0f);
//			}
//			jump_count++;
//			jump_wait = jump_wait_max;
//			// ジャンプ入力した
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool PlayerBase::InputAttack()
//{
//	return false;
//}
