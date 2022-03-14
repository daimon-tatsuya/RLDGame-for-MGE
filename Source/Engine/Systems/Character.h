#pragma once
//**********************************************************
//
//		Character.h
//
//**********************************************************

#include <utility>

#include "Engine/AI/HeuristicSearch.h"
#include "Engine/Systems/Object.h"

// ゲームに共通するものだけ残し、あとはこのクラスを継承して、アクションゲームのキャラクターベースと分ける仕様にする予定
// 使わない関数があるが、あくまでアクションゲームベースなので消さないでおく
/*
 使っていない関数
 UpdateVerticalVelocity
 UpdateVerticalMove
 UpdateHorizontalVelocity
 UpdateHorizontalMove
 UpdateVelocity
 UpdateInvincibleTimer
 AddImpulse
 AirDash
 ApplyDamage
 Jump
 Move
 Turn
 OnLanding
 OnDamaged
 OnDead
 IsGround
 MoveCheck
*/
//前方宣言
class RogueLikeDungeon;

struct  ShortestPath;

/// <summary>
/// キャラクター(敵、プレイヤー)などの基底クラス
/// </summary>
class Character : public Object
{
private:

public:
	int	 current_health = 5;//名前を変えるかも health→life,HP
	int	 max_health = 5;

	RogueLikeDungeon* stage_information = nullptr; // マップ情報　(値をコピーした実体にするか検討中)

	ShortestPath* shortest_path{};//最短経路

private:

	/// <summary>
	/// 有限ステートマシンの初期化
	/// </summary>
	virtual void FiniteStateMachineInitialize() {}

	/// <summary>
	/// 垂直速力更新処理
	/// </summary>
	/// <param name="elapsed_frame">経過時間</param>
	virtual void UpdateVerticalVelocity(float elapsed_frame);

	/// <summary>
	///  垂直移動更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	virtual  void UpdateVerticalMove(float elapsed_time);

	/// <summary>
	///  水平速力更新処理
	/// </summary>
	/// <param name="elapsed_frame">経過時間</param>
	virtual  void UpdateHorizontalVelocity(float elapsed_frame);

	/// <summary>
	///  水平移動更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	virtual  void UpdateHorizontalMove(float elapsed_time);

	/// <summary>
	///  速力更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void UpdateVelocity(float elapsed_time);

	/// <summary>
	/// 無敵時間更新
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void UpdateInvincibleTimer(float elapsed_time);

public:

	Character() = default;

	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	~Character() override = default;

	/// <summary>
	/// 衝撃を与える
	/// </summary>
	 /// <param name="impulse">速度に加算する衝撃</param>
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	/// <summary>
	/// 空中ダッシュ処理
	/// </summary>
	/// <param name="vx">x軸に加速する大きさ</param>
	/// <param name="vz">z軸に加速する大きさ</param>
	/// <param name="gravity_cut_time">重力を無視する時間</param>
	virtual  void AirDash(float vx, float vz, float gravity_cut_time);

	/// <summary>
	/// ダメージを与える
	/// </summary>
	/// <param name="damage">hpへ減算する量</param>
	/// <param name="invincible_time">無敵時間</param>
	bool ApplyDamage(int damage, float invincible_time);

	/// <summary>
	/// 破棄
	/// </summary>
	//virtual  void Destroy();

	/// <summary>
	/// ジャンプ処理
	/// </summary>
	/// <param name="speed">y軸に加速する大きさ</param>
	virtual void Jump(float speed);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="vx">x軸に加速する大きさ</param>
	/// <param name="vz">z軸に加速する大きさ</param>
	/// <param name="speed">最大速度</param>
	virtual  void Move(float vx, float vz, float speed);

	/// <summary>
	/// 旋回処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	/// <param name="vx">x軸に加速する大きさ</param>
	/// <param name="vz">z軸に加速する大きさ</param>
	/// <param name="speed">最大速度</param>
	virtual  void Turn(float elapsed_time, float vx, float vz, float speed);

	/// <summary>
	/// 着地した時に呼ばれる
	/// </summary>
	virtual void OnLanding() {}

	/// <summary>
	/// ダメージを受けた時に呼ばれる
	/// </summary>
	virtual void OnDamaged() {}

	/// <summary>
	/// 死亡した時に呼ばれる
	/// </summary>
	virtual void OnDead() {}

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// 健康状態を取得
	int GetHealth() const { return current_health; }

	// 健康状態を設定
	void SetHealth(const int health) { this->current_health = health; }

	// 地面に接地しているか
	bool IsGround() const { return is_ground; }

	//プレイヤーが移動できるかチェックする
	void MoveCheck(float mx, float mz);

	// 最大健康状態を取得
	int GetMaxHealth() const { return max_health; }

	// 最大健康状態を設定
	void SetMaxHealth() { this->max_health = max_health; }

	// マップ情報を取得
	RogueLikeDungeon* GetStageInformation() const { return stage_information; }

	//最短経路を設定
	void SetShortestPath(std::vector<int> path) const { shortest_path->path = std::move(path); }
};
