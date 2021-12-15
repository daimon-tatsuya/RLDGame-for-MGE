#pragma once
//**********************************************************
//
//		SceneGameクラス
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"

#include "Engine/AI/DungeonMake.h"


//前方宣言
class Player;
class CameraController;

/// <summary>
/// ゲームシーン
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	SceneGame() = default;

	~SceneGame() override;

	void Initialize()override;

	void Finalize() override;

	void Update(float elapsed_time)override;

	void Render()override;

private:

public:
	std::unique_ptr<Player>   player= nullptr ;
	std::unique_ptr<CameraController> camera_controller = nullptr;
	RogueLikeDungeon storage_dungeon;

	//必要なら使うので消さない
	//float	timer = 0.0f;
	//int		counter = 0;
	//char	message[32] = { 0 };
};
