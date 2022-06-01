#pragma once
//**********************************************************
//
//		StageManagerクラス
//
//**********************************************************
#include <vector>
#include <memory>
#include <d3d11.h>
#include "Engine/Systems/Math.h"

//前方宣言
class Stage;
class Shader;
struct HitResult;

/// <summary>
/// Stageを管理するクラス
/// </summary>
class StageManager final
{
private:
	std::vector<std::shared_ptr<Stage>>		stages;

private:
	StageManager() {}
	~StageManager();

public:
	// 唯一のインスタンス取得
	static StageManager& Instance()
	{
		static StageManager instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader);

	// ステージ登録
	void Register(Stage* stage);

	// ステージ全削除
	void Clear();

	// レイキャスト
	bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

	// デバッグ用GUI描画
	void DrawDebugGUI();
};
