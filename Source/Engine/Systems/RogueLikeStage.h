#pragma once
//**********************************************************
//
//		RogueLikeStageクラス
//
//**********************************************************

#include "Engine/Systems/Stage.h"

//前方宣言


class RogueLikeStage
	: public Stage
{
private:

public:
private:
public:
	RogueLikeStage(const char* filename, DirectX::XMFLOAT3& pos, const int id);
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

};
