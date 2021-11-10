#pragma once

#include "Engine/Systems/scene.h"

#include "Engine/Objects/sprite.h"

#include "Engine/Objects/GeometricPrimitive.h"

#include "Engine/Objects/StaticMesh.h"

#include "Engine/Objects/SkinnedMesh.h"

#include "Engine/Systems/rasterizer.h"

#include "Engine/Systems/framebuffer.h"
#include "Engine/Objects/Model.h"
#include "Engine/Objects/ModelResource.h"
#include "Engine/Systems/LambertShader.h"

//Todo SceneTitle ÉRÉÅÉìÉg

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