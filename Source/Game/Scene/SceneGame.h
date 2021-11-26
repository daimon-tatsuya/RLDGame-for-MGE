#pragma once
//**********************************************************
//
//		SceneGameクラス
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/DungeonMake.h"
#include "Engine/AI/HeuristicSearch.h"

#include"Game/Characters/Player.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	SceneGame();
	virtual ~SceneGame();
	void Initialize()override;
	void Finalize() override;
	void Update(float elapsedTime)override;
	void Render()override;

private:

public:
	std::unique_ptr<Player>   player = nullptr;
	std::unique_ptr<CameraController> camera_controller = nullptr;
	RogueLikeDungeon storage_dungeon;

	//必要なら使うので消さない
	//float	timer = 0.0f;
	//int		counter = 0;
	//char	message[32] = { 0 };
};
