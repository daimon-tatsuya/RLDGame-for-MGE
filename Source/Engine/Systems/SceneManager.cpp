#include "SceneManager.h"
//**********************************************************
//
//		SceneManager�N���X
//
//**********************************************************
void SceneManager::Update(float elapsedTime)
{
	if (current_scene != nullptr)
	{
		current_scene->Update(elapsedTime);
	}
}

void SceneManager::Render()
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