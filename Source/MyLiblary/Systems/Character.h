#pragma once
//**********************************************************
//
//		Characterクラス
//
//**********************************************************

<<<<<<< HEAD:Source/Liblary/Systems/Character.h
#include "Liblary/Systems/Object.h"
#include "Liblary/AI/HeuristicSearch.h"
=======
#include "MyLiblary/Systems/Object.h"
#include "MyLiblary/AI/HeuristicSearch.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Systems/Character.h

class Character :
	public Object
{
private:
	//	名前を変えるかも health→life,HP
	int	current_health = 5;//現在の体力値
	int	max_health = 5;//最大体力値

	bool	is_decide_pos = false;//位置が決まった時にbreakするために使う




public:
	//-遅延させるために使う変数-
	float delay_time = 1.5f;
	float counted_time = 0.f;
	//-----------------------------
	ShortestPath* shortest_path{};//最短経路

private:

	/// <summary>
/// 有限ステートマシンの初期化
/// </summary>
	virtual void FiniteStateMachineInitialize() {};

public:

	Character() = default;
	~Character() override;

	//キャラクターマネージャーのリストから自身を消去
	virtual void Destroy();

	// 体力値を取得
	int GetCurrentHealth() const { return current_health; }

	// 体力値を設定
	void SetCurrentHealth(const int health) { this->current_health = health; }

	// 体力値を増加
	void AddCurrentHealth(const int health) { this->current_health += health; }

	// 最大体力値を取得
	int GetMaxHealth() const { return max_health; }

	// 最大体力値を設定
	void SetMaxHealth(const int health) { this->max_health = health; }

	// 最大体力値を増加
	void AddMaxHealth(const int health) { this->max_health += health; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincible_time);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// 移動処理
	void Move(float vx, float vz, float speed);

	// 旋回処理
	void Turn(float elapsedTime, float vx, float vz, float speed);

	// ジャンプ処理
	void Jump(float speed);

	// 空中ダッシュ処理
	void AirDush(float vx, float vz, float gravityCutTime);

	//最短経路を設定
	void SetShortestPath(std::vector<int> path) const { shortest_path->path_information = std::move(path); }

	//位置を決定しているかのフラグを取得
	bool GetIsDecidePos() const { return  is_decide_pos; }

	//位置を決定しているかのフラグの設定
	void SetIsDecidePos(const bool set) { is_decide_pos = set; }

	/// <summary>
	/// メッセージ受信処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns>受信の有無</returns>
	bool OnMessage(const Telegram& telegram) override{return false;}
};



