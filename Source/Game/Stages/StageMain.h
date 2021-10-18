#pragma once
#include "Engine/Systems/Stage.h"

class StageMain :
	public Stage
{
private:

public:

	StageMain();
	~StageMain() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) override;

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) override;

	//メッセージ受信処理
	bool OnMessage(const Telegram& msg) override;

	// デバッグ用GUI描画
	void DrawDebugGUI()override;

private:


public:
};
