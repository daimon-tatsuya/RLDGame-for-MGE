#pragma once
//**********************************************************
//
//		EnemySnake�N���X
//
//**********************************************************



#include "Engine/Systems/EnemyBase.h"

/// <summary>
/// �G�N���X(�w�r)
/// </summary>
class EnemySnake :
	public EnemyBase
{
private:

protected:

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
		UseItem,

		StateEnd
	};

	//�q�X�e�[�g
	enum class Reaction :int
	{
		Damaged = 0,
		Death,

		StateEnd
	};

	//�q�X�e�[�g
	enum class Receive
	{
		Wait,
		Called,
		StateEnd
	};


private:


protected:


public:

	EnemySnake(RogueLikeDungeon* rogue_like_dungeon);
	virtual ~EnemySnake();

	void Update(float elapsedTime)override;

	// �`�揈��
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//�L���X�e�[�g�}�V���̏�����
	void FSMInitialize() override;

	// �j��
	void Destroy();

	// �f�o�b�O�G�l�~�[���\��
	void DrawDebugGUI()override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive() override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged()override;

	// ���S�������ɌĂ΂��
	void OnDead()override;

	//���b�Z�[�W��M����
	virtual bool OnMessage(const Telegram& msg) override;

};
