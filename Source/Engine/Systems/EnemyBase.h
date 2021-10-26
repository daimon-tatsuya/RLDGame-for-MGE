#pragma once
#include "Engine/Systems/Character.h"
//Todo　コメント
//敵クラスの基底クラス
class EnemyBase :public Character
{
private:

public:

	EnemyBase() {};
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	~EnemyBase()override {};

	// 更新処理
	virtual void Update(float elapsedTime) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) = 0;

	// 破棄
	void Destroy();

	// デバッグプリミティブ描画
	//virtual void DrawDebugPrimitive() override;

	// 攻撃フラグ取得
	bool	GetAttackFlg() { return attack_flg; }

	// 攻撃フラグセット
	void	SetAttackFlg(bool flg) { attack_flg = flg; };

	//敵同士の交差判定
	void CollisionEnemyToEnemy();


private:

protected:
	int	 state = 0;
	bool attack_flg = false;
	int id = 0;
	float search_range = 0.0f;
	float attack_range = 0.0f;
public:
};
