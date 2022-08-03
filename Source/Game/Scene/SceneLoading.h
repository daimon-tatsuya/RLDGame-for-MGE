#pragma once
//**********************************************************
//
//		SceneLoadingクラス
//
//**********************************************************

<<<<<<< HEAD
#include "Liblary/Systems/Scene.h"
=======
#include "MyLiblary/Systems/Scene.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3

/// <summary>
/// シーンの読み込みのシーン
/// </summary>
class SceneLoading : public Scene
{
private:
	Scene* next_scene = nullptr;

	// Now Loading...アニメーション用
	float		timer = 0.0f;
	int		counter = 0;
	char		message[32] = { 0 };

private:
	// ローディングスレッド
	static void LoadingThread(const SceneLoading* scene);
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

	bool OnMessage(const Telegram& telegram) override;

	void SendMessaging(MESSAGE_TYPE msg) override;
};