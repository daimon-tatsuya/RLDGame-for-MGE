#pragma once
//**********************************************************
//
//		Scenes�N���X
//
//**********************************************************
#include "Engine/AI/Telegram.h"

const int TEXT_BUFFER_SIZE = 32;

/// <summary>
/// �V�[���̊��N���X
/// </summary>
class Scene
{
private:

	bool	ready = false;

public:

private:

public:

	Scene() = default;
	// �Ⴆ�f�X�g���N�^����ł�
	// virtual �ȃf�X�g���N�^�͖����I�ɒ�`����
	virtual ~Scene() = default;

	// ������
	virtual void Initialize() = 0;
	// �I����
	virtual void Finalize() = 0;
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// �`�揈��
	virtual void Render() = 0;
	// �����������Ă��邩
	bool IsReady() const { return ready; }
	// ���������ݒ�
	void SetReady() { ready = true; }

	/// <summary>
	/// ���^AI�o�R��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	virtual	bool OnMessage(const Telegram& telegram) = 0;
	//���^AI�Ƀ^�[���̏I����`����
	virtual	void SendMessaging(MESSAGE_TYPE msg) = 0;
};
