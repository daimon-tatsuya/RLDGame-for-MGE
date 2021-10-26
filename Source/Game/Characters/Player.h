#pragma once
#include "Engine/Systems/PlayerBase.h"
#include "Engine/Systems/DungeonMake.h"
class Player :
	public PlayerBase
{
private:

public:

	Player(RogueLikeDungeon& rogue_like_dungeon);
	Player();
	~Player()override;

	// 更新処理
	void Update(float elapsedTime)override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	// デバッグ用GUI描画
	void DrawDebugGUI()override;

	// デバッグプリミティブ描画
	void DrawDebugPrimitive()override;

	//メッセージ受信処理
	bool OnMessage(const Telegram& msg) override;

	// 着地した時に呼ばれる
	void OnLanding() override;

	// ダメージを受けた時に呼ばれる
	void OnDamaged() override;

	// 死亡した時に呼ばれる
	void OnDead() override;


	//Todo 左右上下斜めの方向に一定の大きさ進む
	//bool InputMove(float elapsedTime) override;

private:

public:
};
