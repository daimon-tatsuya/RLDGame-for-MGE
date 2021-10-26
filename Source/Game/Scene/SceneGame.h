#pragma once
#include <memory>
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/DungeonMake.h"

#include"Game/Characters/Player.h"

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

	//CameraController* camera_controller = nullptr;
	//Player* player = nullptr;

	//ïKóvÇ»ÇÁégÇ§ÇÃÇ≈è¡Ç≥Ç»Ç¢
	//float	timer = 0.0f;
	//int		counter = 0;
	//char	message[32] = { 0 };
};
