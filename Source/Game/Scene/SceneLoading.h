#pragma once
//**********************************************************
//
//		SceneLoadingクラス
//
//**********************************************************

#include "Engine/Systems/Scene.h"

/// <summary>
/// シーンの読み込みのシーン
/// </summary>
class SceneLoading : public Scene
{
private:
	Scene* next_scene = nullptr;

	// Now Loading...アニメーション用
	float	timer = 0.0f;
	int		counter = 0;
	char	message[32] = { 0 };

public:
	explicit SceneLoading(Scene* next_scene) : next_scene(next_scene) {}
	~SceneLoading() override = default;

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsed_time) override;

	// 描画処理
	void Render() override;

private:
	// ローディングスレッド
	static void LoadingThread(const SceneLoading* scene);


};