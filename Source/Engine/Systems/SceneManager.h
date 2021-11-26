#pragma once
//**********************************************************
//
//		SceneManager�N���X
//
//**********************************************************
#include	 "Engine/Systems/scene.h"

/// <summary>
/// �V�[�����Ǘ�����N���X
/// </summary>
class SceneManager
{
private:
	SceneManager() {}
	~SceneManager()
	{
		delete current_scene;
	}

public:
	// �B��̃C���X�^���X�擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �`�揈��
	void Render();

	// �V�[���N���A
	void Clear();

	// �V�[���؂�ւ�
	void ChangeScene(Scene* scene);

private:
	Scene* current_scene = nullptr;
};