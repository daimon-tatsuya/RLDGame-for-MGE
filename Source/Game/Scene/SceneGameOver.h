#pragma once
//**********************************************************
//
//		SceneGameOver�N���X
//
//**********************************************************
<<<<<<< HEAD
#include "Liblary/Systems/Scene.h"
=======
#include "MyLiblary/Systems/Scene.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3

//	�O���錾
class Sprite;

///<summary>
/// �Q�[���I�[�o�[�V�[��
///</summary>
class SceneGameOver :public  Scene
{
private:

public:
	bool is_game_clear = true;
	char	over_text[TEXT_BUFFER_SIZE] = { 0 };//GameOver �� GameClear�̃e�L�X�g
	char	turn_number_text[TEXT_BUFFER_SIZE] = { 0 };//�Q�[���I�����̌o�߃^�[���̃e�L�X�g
	char	push_text[TEXT_BUFFER_SIZE] = { 0 };
private:

public:
	//	�R���X�g���N�^
	SceneGameOver()=default;

	//�f�X�g���N�^
	~SceneGameOver() override;

	//������
	void Initialize() override;

	//�I����
	void Finalize() override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render() override;

	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram) override;

	//���^AI�Ƀ^�[���̏I����`����
	void SendMessaging(MESSAGE_TYPE msg)override;
};




