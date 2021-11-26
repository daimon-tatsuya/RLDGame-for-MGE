#pragma once
//**********************************************************
//
//		SceneLoadingクラス
//
//**********************************************************
#include "Engine/Objects//Sprite.h"
#include "Engine/Systems/Scene.h"

/// <summary>
/// シーンの読み込みのシーン
/// </summary>
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : next_scene(nextScene) {}
	~SceneLoading() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	// ローディングスレッド
	static void LoadingThread(SceneLoading* scene);

private:
	Scene* next_scene = nullptr;
	//std::unique_ptr<Sprite> sprite = nullptr;
	// Now Loading...アニメーション用
	float	timer = 0.0f;
	int		counter = 0;
	char	message[32] = { 0 };
	//float	angle = 0.0f;
};