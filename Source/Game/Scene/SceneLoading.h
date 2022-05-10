#pragma once
//**********************************************************
//
//		SceneLoading�N���X
//
//**********************************************************

#include "Engine/Systems/Scene.h"

/// <summary>
/// �V�[���̓ǂݍ��݂̃V�[��
/// </summary>
class SceneLoading : public Scene
{
private:
	Scene* next_scene = nullptr;

	// Now Loading...�A�j���[�V�����p
	float		timer = 0.0f;
	int		counter = 0;
	char		message[32] = { 0 };

public:
	explicit SceneLoading(Scene* next_scene) : next_scene(next_scene) {}
	~SceneLoading() override = default;

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsed_time) override;

	// �`�揈��
	void Render() override;

	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram) override;

	//���^AI�Ƀ^�[���̏I����`����
	void SendMessaging(MESSAGE_TYPE msg)override;
private:
	// ���[�f�B���O�X���b�h
	static void LoadingThread(const SceneLoading* scene);
};