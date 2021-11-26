#pragma once
//**********************************************************
//
//		SceneTitleクラス
//
//**********************************************************
#include "Engine/Systems/Scene.h"

#include "Engine/Objects/Sprite.h"

#include "Engine/Objects/Model.h"
#include "Engine/Objects/ModelResource.h"
#include "Engine/Systems/LambertShader.h"

/// <summary>
/// タイトルシーン
/// </summary>
class SceneTitle : public Scene
{
private:

public:

	SceneTitle();
	virtual ~SceneTitle();
	void Initialize()override;
	void Finalize() override;
	void Update(float elapsedTime)override;
	void Render()override;

private:

public:

	float	timer = 0.0f;
	int		counter = 0;
	char	message[32] = { 0 };
};