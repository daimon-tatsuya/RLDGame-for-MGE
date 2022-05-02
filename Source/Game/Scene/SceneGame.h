#pragma once
//**********************************************************
//
//		SceneGame�N���X
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"
#include "Engine/AI/DungeonMake.h"

//�O���錾
class Player;
class CameraController;
//class AxisAlignedBoundingBox;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	std::unique_ptr<CameraController> camera_controller = nullptr;

private:

public:

	SceneGame() = default;

	~SceneGame() override;

	void Initialize()override;

	void Finalize() override;

	void Update(float elapsed_time)override;

	void Render()override;
};
