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
	int max_floor = 4;   //�ő�K�w

	bool is_next_floor = false;//���̃t���A�ɐi�ނȂ�true

	bool is_dungeon_clear = false;//�_���W�������N���A��������true
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

	//�������֐�
	void Initialize();


	void Initialize(int max_floor,int max_turn);

	//���݂̌o�߃^�[�����ő�o�߃^�[���𒴂��Ă��邩�̃`�F�b�N
	bool  ExceededMaxTurn() const;

	//���݂̊K�����ő�K���ɓ��B���Ă��邩�̃`�F�b�N
	bool  BreakingThroughTopFloor() const;

	//�^�[�����o�߂�����
	void ElapseTurns() { ++elapsed_turn; }

	//--------------------------------------------
	//	 Getter Setter
	//--------------------------------------------

	//�ő�K�w�̐ݒ�
	void SetMaxFloor(int max) { max_floor = max; }

	//�ő�K�w�̎擾
	int GetMaxFloor() const { return max_floor; }

	//���݂̊K�����擾
	int GetCurrentFloor()const { return  current_floor; }

	//�K��i�܂���
	void ElapseCurrentFloor() { ++current_floor; }

	//�o�߂��Ă���^�[�����擾
	int GetElapsedTurn() const { return elapsed_turn; }

	//�ő�o�߃^�[���̐ݒ�
	void SetMaxTurn(int max) { max_turn = max; }

	//�ő�o�߃^�[���̎擾
	int GetMaxTurn() const { return max_turn; }

	//�_���W�����N���A�̃t���O�̐ݒ�
	void SetIsDungeonClear(bool set_flag) { is_dungeon_clear = set_flag; }

	//�_���W�����N���A�̃t���O�̎擾
	bool GetIsDungeonClear() const { return is_dungeon_clear; }

	//���̊K�ɐi�ނ̃t���O�̐ݒ�
	void SetIsNextFloor(bool set_flag) { is_next_floor = set_flag; }

	//���̊K�ɐi�ނ̃t���O�̎擾
	bool GetIsNextFloor() const { return is_next_floor; }

};


