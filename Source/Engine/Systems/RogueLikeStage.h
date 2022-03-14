#pragma once
//**********************************************************
//
//		RogueLikeStageクラス
//
//**********************************************************

#include "Engine/Systems/Stage.h"

//前方宣言
class RogueLikeDungeon;
struct RogueLikeMap;

class RogueLikeStage
	: public Stage
{
private:
public:
	RogueLikeStage(RogueLikeDungeon* rogue_like_dungeon);
	~RogueLikeStage() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) override;

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

	//メッセージ受信処理
	bool OnMessage(const Telegram& telegram) override;

	// デバッグ用GUI描画
	void DrawDebugGUI()override;

	//オブジェクト配置
	void SetStageObject(std::vector<std::vector<RogueLikeMap>> map_role);

	//ステージの削除
	void Clear();

private:
protected:
	std::vector <Stage> stage_chip;
	//imgui
	RogueLikeDungeon* rogue_like_dungeon_imgui;
public:
};
