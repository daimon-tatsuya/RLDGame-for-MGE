#pragma once
#pragma once

#include "Engine/Systems/Stage.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/DungeonMake.h"

// ローグライクステージ
class RogueLikeStage : public Stage
{
private:
public:
	 RogueLikeStage(RogueLikeDungeon* rogue_like_dungeon);
	 ~RogueLikeStage();

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	//メッセージ受信処理
	bool OnMessage(const Telegram& msg) override;

	// デバッグ用GUI描画
	void DrawDebugGUI()override;

	//オブジェクト配置
	void SetStageObject(std::vector<std::vector<RogueLikeMap>> map_role);

	//ステージの削除
	void Clear();

private:
protected:
	std::vector <Stage> stage_chip;
	RogueLikeDungeon*  db_rogue_like_dungeon;
public:
};
