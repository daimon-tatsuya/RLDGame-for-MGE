//**********************************************************
//
//		SceneManager�N���X
//
//**********************************************************
#include "Liblary/Systems/Scene.h"
#include "Liblary/Systems/SceneManager.h"
#include "Liblary/Systems/Logger.h"
#include "Liblary/AI/MetaAI.h"
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
