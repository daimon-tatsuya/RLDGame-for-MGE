#pragma once
//**********************************************************
//
//		Player�N���X
//
//**********************************************************

#include "Engine/Systems/Character.h"
#include "Engine/Systems/StateMachine.h"

///<summary>
///�v���C���[�N���X
///</summary>
class Player :
	public Character
{
private:
	// �e�X�e�[�g
	enum class ParentState
	{
		Entry = 0,
		Reaction,
		Receive,

		StateEnd
	};

	// �q�X�e�[�g
	enum class Entry
	{
		Select = 0,
		Attack,
		WayChange,
		Move,
		// ������\��̃X�e�[�g
		//Map,
		//Ability,
		//MessageLog,
		//Inventory,
		//View,
		//Step,

		StateEnd
	};

	// �q�X�e�[�g
	enum class Reaction
	{
		ReactionSelect = 0,
		Damaged,
		Death,

		StateEnd
	};

	// �q�X�e�[�g
	enum class Receive
	{
		Wait,
	//	Call,

		StateEnd
	};

	StateMachine<ParentState, void, const float> player_state_machine;	// �v���C���[�̐e�X�e�[�g

	using PlayerEntryState = StateMachine<Entry, void, const float>; // �v���C���[�̎q�X�e�[�g
	using PlayerReactionState = StateMachine<Reaction, void, const float>; // �v���C���[�̎q�X�e�[�g
	using PlayerReceiveState = StateMachine<Receive, void, const float>; // �v���C���[�̎q�X�e�[�g
	PlayerEntryState      player_entry_state;   // �s��(����)�X�e�[�g
	PlayerReactionState  player_reaction_state;// HP�֌W�̔����X�e�[�g
	PlayerReceiveState   player_receive_state; // MetaAI����̎w���҂��X�e�[�g

	std::vector<std::vector<std::string>> player_states_string;//ImGui�p�X�e�[�g���R���e�i
	std::vector<std::string> player_entry_string;
	std::vector<std::string> player_reaction_string;
	std::vector<std::string> player_receive_string;

public:

private:

	//--------------------------
	//�e�X�e�[�g
	//--------------------------

	/// <summary>
	/// �s��(����)�X�e�[�g
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void EntryState(const float elapsed_time);

	/// <summary>
	/// HP�֌W�̔����X�e�[�g
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void ReactionState(const float elapsed_time);

	/// <summary>
	///  MetaAI����̎w���҂��X�e�[�g
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void ReceiveState(const float elapsed_time);

	//--------------------------
	//�q�X�e�[�g
	//--------------------------

	//?EntryState

	/// <summary>
	/// <para>�s���I���X�e�[�g</para>
	/// <para>���͂ɉ����đJ�ڂ���X�e�[�g��ς���</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void	SelectState(const float elapsed_time);

	/// <summary>
	/// <para>�����]���X�e�[�g</para>
	/// <para>�{�^���ƈړ��̓������͂ŕ�����ς���</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void	WayChangeState(const float elapsed_time);

	/// <summary>
	/// <para>�ړ��X�e�[�g</para>
	/// <para>�ړ��̓��͂ňړ�</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void	MoveState(const float elapsed_time);

	/// <summary>
	/// <para>�U���X�e�[�g</para>
	/// <para>�v���C���[�̊p�x����U������</para>
	/// <para> �܂��́A�v���C���[�̑O�����U������X�e�[�g </para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void	AttackState(const float elapsed_time);

	/// <summary>
	/// <para>�I�[�v�����j���[�X�e�[�g</para>
	/// <para>��������j���[�E�B���h�E�Ɉڂ�</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	//void	MenuState(const float elapsed_time);

	//?ReactionState

	/// <summary>
	/// <para>�_���[�W���󂯂����ʂ𔻕ʂ���</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void ReactionSelectState(const float elapsed_time);

	/// <summary>
	/// <para>��_���[�W�X�e�[�g</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void DamagedState(const float elapsed_time);

	/// <summary>
	/// <para>���S�X�e�[�g</para>
	/// <para>HP��0�ȉ��Ȃ�V�[���J��</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void DeathState(const float elapsed_time);

	//?ReceiveState

	/// <summary>
	/// <para>�ҋ@�X�e�[�g</para>
	/// <para>���b�Z�[�W��M�҂��̃X�e�[�g</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void WaitState(const float elapsed_time);

public:

	//�R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void Update(float elapsed_time) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dc"></param>
	/// <param name="shader">�`����@</param>
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) override;

	// �L���X�e�[�g�}�V���̏�����
	void FiniteStateMachineInitialize() override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;

	// ���S�������ɌĂ΂��
	void OnDead() override;

	/// <summary>
	/// ���b�Z�[�W��M����
	/// </summary>
	/// <param name="telegram">����</param>
	/// <returns>��M�̗L��</returns>
	bool OnMessage(const Telegram& telegram) override;

 	//���^AI�Ƀ��b�Z�[�W�𑗐M����
	void SendMessaging(MESSAGE_TYPE msg) override;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI() override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	//�L�����N�^�[�}�l�[�W���[�̃��X�g���玩�g������
	void Destroy() override;

	/// <summary>
	/// �ړ����Ă��邩���`�F�b�N
	/// </summary>
	/// <returns>�ړ����Ă�����true</returns>
	bool IsMoved();

	/// <summary>
	/// <para> ���[�h�����Ȃ�</para>
	/// <para>�Q�[���J�n���̍ŏ��̈ʒu</para>
	/// </summary>
	void SetFirstPos();


};
