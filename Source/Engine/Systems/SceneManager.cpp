//**********************************************************
//
//		SceneManager�N���X
//
//**********************************************************
#include "Engine/Systems/Scene.h"
#include "Engine/Systems/SceneManager.h"

SceneManager::~SceneManager()
{
	delete current_scene;
}

void SceneManager::Update(float elapsed_time) const
{
	if (current_scene != nullptr)
	{
		current_scene->Update(elapsed_time);
	}
}

void SceneManager::Render() const
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
	// �Â��V�[�����I������
	Clear();

	// �V�����V�[����ݒ�
	current_scene = scene;

	// �V�[������������
	if (!current_scene->IsReady())
	{
		current_scene->Initialize();
	}
}