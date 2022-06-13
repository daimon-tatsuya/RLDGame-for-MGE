#pragma once
//**********************************************************
//
//		SceneTitle�N���X
//
//**********************************************************

#include "Engine/Systems/Scene.h"
#include "Engine/Systems/CameraController.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class SceneTitle
	: public Scene
{
private:

public:

	float	timer = 0.0f;
	int	counter = 0;
	char	title_name[TEXT_BUFFER_SIZE] = { 0 };
	char	push_text[TEXT_BUFFER_SIZE] = { 0 };
private:

public:

	SceneTitle() = default;

	~SceneTitle() override = default;

	void Initialize()override;

	void Finalize() override;

	void Update(float elapsed_time)override;

	void Render()override;
	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram) override;

	//���^AI�Ƀ^�[���̏I����`����
	void SendMessaging(MESSAGE_TYPE msg)override;


};