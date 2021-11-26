#include "SceneManager.h"
//**********************************************************
//
//		SceneManagerクラス
//
//**********************************************************
void SceneManager::Update(float elapsedTime)
{
	if (current_scene != nullptr)
	{
		current_scene->Update(elapsedTime);
	}
}

void SceneManager::Render()
{
	if (current_scene != nullptr)
	{
		current_scene->Render();
	}
}

void SceneManager::Clear()
{
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
}

void SceneManager::ChangeScene(Scene* scene)
{
	// 古いシーンを終了処理
	Clear();

	// 新しいシーンを設定
	current_scene = scene;

	// シーン初期化処理
	if (!current_scene->IsReady())
	{
		current_scene->Initialize();
	}
}