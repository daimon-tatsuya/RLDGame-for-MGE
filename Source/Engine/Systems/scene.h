#pragma once
//**********************************************************
//
//		Scenes�N���X
//
//**********************************************************

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
};
