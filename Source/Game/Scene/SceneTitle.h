#pragma once
//**********************************************************
//
//		SceneTitleクラス
//
//**********************************************************

#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CameraController.h"

/// <summary>
/// タイトルシーン
/// </summary>
class SceneTitle
	: public Scene
{
private:

public:

	SceneTitle() = default;

	~SceneTitle() override = default;

	void Initialize()override;

	void Finalize() override;

	void Update(float elapsed_time)override;

	void Render()override;

private:

public:

	float	timer = 0.0f;
	int		counter = 0;
	char	message[32] = { 0 };
};