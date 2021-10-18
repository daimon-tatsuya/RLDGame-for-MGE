#pragma once
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

	Player* player = nullptr;
	CameraController* camera_controller = nullptr;

	//float	timer = 0.0f;
	//int		counter = 0;
	//char	message[32] = { 0 };
};
