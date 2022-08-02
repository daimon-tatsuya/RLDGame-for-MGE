#pragma once
//**********************************************************
//
//		EnemySnake�N���X
//
//**********************************************************

#include "MyLiblary/Systems/Character.h"
#include "MyLiblary//Systems/StateMachine.h"

/// <summary>
/// �G�N���X(�w�r)
/// </summary>
class EnemySnake :
	public Character
{
private:

public:

	//�e�X�e�[�g
	enum class ParentState :int
	{
		Entry = 0,
		Reaction,
		Receive,

		StateEnd
	};

	//�q�X�e�[�g
	enum class Entry :int
	{
		Select = 0,
		Approach,
		Explore,
		Attack,
		Ability,
		//UseItem,

		StateEnd
	};

	//�q�X�e�[�g
	enum class Reaction :int
	{
		ReactionSelect = 0,
		Damaged,
		Death,

		StateEnd
	};

	//�q�X�e�[�g
	enum class Receive
	{
		//		Wait,
		Called,
		StateEnd
	};

	StateMachine<ParentState, void, const float> enemy_snake_state_machine;	// �e�X�e�[�g

	using EnemySnakeEntryState = StateMachine<Entry, void, const float>; // �s���X�e�[�g�̐錾
	using EnemySnakeReactionState = StateMachine<Reaction, void, const float>; // �����X�e�[�g�̐錾
	using EnemySnakeReceiveState = StateMachine<Receive, void, const float>; // MetaAI����̎w���҂��X�e�[�g�̐錾
	EnemySnakeEntryState      enemy_snake_entry_state;   // �s���X�e�[�g
	EnemySnakeReactionState enemy_snake_reaction_state;//�����X�e�[�g
	EnemySnakeReceiveState   enemy_snake_receive_state; // MetaAI����̎w���҂��X�e�[�g

private:

	//�e�X�e�[�g

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

	//�q�X�e�[�g

	//? EntryState

	/// <summary>
	/// <para>�s���I���X�e�[�g</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void	SelectState(const float elapsed_time);

	/// <summary>
	/// <para>�Ώۂɋ߂Â��X�e�[�g</para>
	/// <para>�Ώۂ��߂��ɂ���Ƃ�(5*5�͈̔�)�ȒP�ȃA���S���Y���ɂ���</para>
	/// <para>���̑���A*</para>
	/// <param name="elapsed_time">�o�ߎ���</param>
	/// <summary>
	void ApproachState(const float elapsed_time);

	/// <summary>
	/// <para>�}�b�v��T������X�e�[�g</para>
	/// <para>�ʘH�͒ʘH�ɉ����āAA*</para>
	/// <para>�����͏o���Ɍ������āAA*</para>
	/// </summary>
	/// <param name="elapsed_time"></param>
	void ExploreState(const float elapsed_time);

	/// <summary>
	/// <para>�ΏۂɍU������X�e�[�g</para>
	/// </summary>
	/// <param name="elapsed_time"></param>
	void AttackState(const float elapsed_time);

	/// <summary>
	/// <para>�ΏۂɃA�r���e�B��������X�e�[�g</para>
	///<para>��{AttackState��AbilityState���I�������(8��2�̊���)</para>
	/// </summary>
	/// <param name="elapsed_time"></param>
	void AbilityState(const float elapsed_time);

	//? ReactionState

	/// <summary>
	/// <para>�_���[�W���󂯂����ʂ𔻕ʂ���</para>
	/// <para>���u��</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void ReactionSelectState(const float elapsed_time);

	/// <summary>
	/// <para>��_���[�W�X�e�[�g</para>
	/// <para></para>
	/// <para></para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void DamagedState(const float elapsed_time);

	/// <summary>
	/// <para>���S�X�e�[�g</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void DeathState(const float elapsed_time);

	//? ReceiveState

	///// <summary>
	///// <para>�ҋ@�X�e�[�g</para>
	///// </summary>
	///// <param name="elapsed_time">�o�ߎ���</param>
	void WaitState(const float elapsed_time);

	/// <summary>
	/// <para>�I�[�v�����j���[�X�e�[�g</para>
	/// <para>MetaAI���瑗���Ă��閽��(���b�Z�[�W)�ɂ���ăX�e�[�g��J�ڂ�����</para>
	/// <para>���u��</para>
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>k
	void CalledState(const float elapsed_time);

protected:

public:

	//EnemySnake(RogueLikeDungeon* rogue_like_dungeon);

	EnemySnake();

	~EnemySnake() override;

	//�X�V����
	void Update(float elapsed_time)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//�L���X�e�[�g�}�V���̏�����
	void FiniteStateMachineInitialize() override;

	//�L�����N�^�[�}�l�[�W���[�̃��X�g���玩�g������
	void Destroy() override;

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI()override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged()override;

	// ���S�������ɌĂ΂��
	void OnDead()override;

	//���b�Z�[�W��M����
	bool OnMessage(const Telegram& telegram) override;

	//���^AI���^AI�Ƀ^�[���̏I����`����
	void SendMessaging(MESSAGE_TYPE msg) override;
};
