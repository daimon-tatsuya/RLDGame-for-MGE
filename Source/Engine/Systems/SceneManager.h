#pragma once
//**********************************************************
//
//		SceneManager�N���X
//
//**********************************************************

//�O���錾
class Scene;

/// <summary>
/// �V�[�����Ǘ�����N���X
/// </summary>
class SceneManager
{
private:

	Scene* current_scene = nullptr;

public:
private:
	SceneManager() = default;

	~SceneManager();

public:
	// �B��̃C���X�^���X�擾
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsed_time) const;

	// �`�揈��
	void Render() const;

	// �V�[���N���A
	void Clear();

	// �V�[���؂�ւ�
	void ChangeScene(Scene* scene);
};