//**********************************************************
//
//		Characterクラス
//
//**********************************************************
#include "Liblary/Systems/Character.h"
#include "Liblary/Systems/CharacterManager.h"
//デストラクタ
Character::~Character()
{
	//最短経路の破棄
	if (shortest_path!=nullptr)
	{
		shortest_path->Clear();
		delete shortest_path;
	}
}

void Character::Destroy()
{
	//キャラクターマネージャーの消去リストに自信を追加
	CharacterManager::Instance().Remove(this);
}

bool Character::ApplyDamage(int damage, float invincible_time)
{
	// ダメージが0の場合は健康状態を変更する必要がない
	if (damage == 0) return false;

	// 死亡している場合は健康状態を変更しない
	if (GetCurrentHealth() <= 0) return false;

	// 無敵時間中はダメージを与えない
	if (invincible_timer > 0.0f) return false;

	// 無敵時間設定
	invincible_timer = invincible_time;

	// ダメージ処理
	AddCurrentHealth(-1 * damage);

	// 死亡通知
	if (GetCurrentHealth() <= 0)
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

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	// 速力に力を加える
	AddVelocity(impulse);
}

void Character::Move(float vx, float vz, float speed)
{
	// 移動方向ベクトルを設定
	move_vec_x = vx;
	move_vec_z = vz;

	// 最大速度設定
	max_move_speed= speed;
}
// アクションゲーム用なので後回し https://gamedevelopment.tutsplus.com/tutorials/understanding-steering-behaviors-seek--gamedev-849
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
	float frontX = sinf(GetAngle().y);
	float frontZ = cosf(GetAngle().y);

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
		AddAngleY(rot);
	}
	else
	{
		AddAngleY(-1 * rot);
	}
}

void Character::Jump(float speed)
{
	SetVelocityY(speed);
}

void Character::AirDush(float vx, float vz, float gravityCutTime)
{
}
