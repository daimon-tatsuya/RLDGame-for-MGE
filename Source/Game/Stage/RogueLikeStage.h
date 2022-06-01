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
	RogueLikeStage();
	RogueLikeStage(const char* filename, DirectX::XMFLOAT3& pos, const int id):Stage(filename,pos,id){}
	RogueLikeStage(const char* filename, DirectX::XMFLOAT3& pos,DirectX::XMFLOAT3 scale, const int id) :Stage(filename, pos, scale,id) {}
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
	void SetStageObject();

	//ステージの削除
	void Clear();

	// ステージ登録
	void Register(Stage stage);

private:
protected:
	std::vector <Stage> stage_chip;
	//imgui
public:
};
