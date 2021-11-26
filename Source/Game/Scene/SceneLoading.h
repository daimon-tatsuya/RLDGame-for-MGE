#pragma once
//**********************************************************
//
//		SceneLoading�N���X
//
//**********************************************************
#include "Engine/Objects//Sprite.h"
#include "Engine/Systems/Scene.h"

/// <summary>
/// �V�[���̓ǂݍ��݂̃V�[��
/// </summary>
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : next_scene(nextScene) {}
	~SceneLoading() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;

private:
	// ���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);

private:
	Scene* next_scene = nullptr;
	//std::unique_ptr<Sprite> sprite = nullptr;
	// Now Loading...�A�j���[�V�����p
	float	timer = 0.0f;
	int		counter = 0;
	char	message[32] = { 0 };
	//float	angle = 0.0f;
};