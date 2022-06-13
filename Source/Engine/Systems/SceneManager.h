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
class SceneManager final
{
private:

	Scene* current_scene = nullptr;	//���݂̃V�[��
	bool is_game_clear = false;//�Q�[�����N���A�����Ȃ�true
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

	//���݂̃V�[�����擾
	Scene* GetCurrentScene() const { return current_scene; }

	//�Q�[�����N���A���Ă��邩�̃t���O���擾
	bool GetIsGameClear()const { return  is_game_clear; }

	//�Q�[�����N���A���Ă��邩�̃t���O���擾
	void SetIsGameClear(bool flag) {   is_game_clear=flag; }

};