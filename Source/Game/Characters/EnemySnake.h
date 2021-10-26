#pragma once
#include "Engine/Systems/EnemyBase.h"
//Todo　コメント
class EnemySnake :
	public EnemyBase
{
private:
protected:
public:
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	// 破棄
	void Destroy();

	// デバッグプリミティブ描画
	void DrawDebugPrimitive() override;

	// デバッグエネミー情報表示
	void DrawDebugGUI()override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged()override;

	// 死亡した時に呼ばれる
	void OnDead()override;

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) override;
private:
protected:
public:
};
