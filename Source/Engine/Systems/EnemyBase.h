#pragma once
#include "Engine/Systems/Character.h"

//敵クラスの基底クラス
class EnemyBase :public Character
{
private:

protected:

	// 着地した時に呼ばれる
	virtual void OnLanding() {}

	// ダメージを受けた時に呼ばれる
	virtual void OnDamaged() {}

	// 死亡した時に呼ばれる
	virtual void OnDead() {}

public:

	EnemyBase() {};

	~EnemyBase()override {};

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) = 0;

	// 破棄
	void Destroy();

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive() override;

	// デバッグエネミー情報表示
	virtual void DrawDebugGUI()override;

	// 攻撃フラグ取得
	bool	GetAttackFlg() { return attack_flg; }

	// 攻撃フラグセット
	void	SetAttackFlg(bool flg) { attack_flg = flg; };

	//敵同士の交差判定
	void CollisionEnemyToEnemy();

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) override;

private:

protected:
	int	 state = 0;
	bool attack_flg = false;
	int id = 0;
	float search_range = 0.0f;
	float attack_range = 0.0f;
public:
};
