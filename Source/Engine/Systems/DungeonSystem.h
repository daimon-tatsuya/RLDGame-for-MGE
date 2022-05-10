#pragma once
//**********************************************************
//
//		DungeonSystem�N���X
//
//**********************************************************

///<summary>
/// �_���W�����̃V�X�e���Ǘ��N���X
///</summary>
class DungeonSystem final
{
private:
	int elapsed_turn = 0;	//�o�߃^�[�� �G�̍s�����I��邽�т�1�v���X����
	int max_turn = 999;		//�ő�o�߃^�[�� �o�߃^�[�������̒l�𒴂�����

	int current_floor = 1;//���݂̊K��
	int max_floor = 99;   //�ő�K�w


public:

	//	�R���X�g���N�^
	DungeonSystem();

	//�f�X�g���N�^
	~DungeonSystem() = default;

		// �B��̃C���X�^���X�擾
	static DungeonSystem& Instance()
	{
		static DungeonSystem instance;
		return instance;
	}

	//�^�[�����o�߂�����
	void ElapseTurns() { ++elapsed_turn; }

	//�ő�K�w�̐ݒ�
	void SetMaxFloor(int max) { max_floor = max; }

	//�ő�K�w�̎擾
	int GetMaxFloor() const { return max_floor; }

	//���i�܂���
	void ElapseCurrentFloor() { ++current_floor; }

	//�o�߂��Ă���^�[�����擾
	int GetElapsedTurn() const { return elapsed_turn; }

	//�ő�o�߃^�[���̐ݒ�
	void SetMaxTurn(int max) { max_turn = max; }

	//�ő�o�߃^�[���̎擾
	int GetMaxTurn() const { return max_turn; }
};


