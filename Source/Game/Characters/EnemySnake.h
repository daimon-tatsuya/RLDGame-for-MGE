#pragma once

#include "Engine/Systems/EnemyBase.h"
//Todo EnemyBase コメント
class EnemySnake :
	public EnemyBase
{
private:


protected:


public:
	//親ステート
	//子ステート
private:


protected:


public:

	EnemySnake(RogueLikeDungeon* rogue_like_dungeon);
	EnemySnake();
	virtual ~EnemySnake();

	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	// 破棄
	void Destroy();

	// デバッグエネミー情報表示
	void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;



	// ダメージを受けた時に呼ばれる
	void OnDamaged()override;

	// 死亡した時に呼ばれる
	void OnDead()override;

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) override;

};
