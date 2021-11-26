#pragma once
//**********************************************************
//
//		Player�N���X
//
//**********************************************************

#include "Engine/Systems/Character.h"
#include "Engine/Systems/DungeonMake.h"

class Player :
	public Character
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
		WayChange,
		Move,
		Attack,
		Menu,
		//������\��̃X�e�[�g
		//Map,
		//Ability,
		//MessageLog,
		//Inventry,
		//View,
		//Step,

		StateEnd
	};

	//�q�X�e�[�g
	enum class Reaction :int
	{
		Damaged=0,
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

	Player(RogueLikeDungeon* rogue_like_dungeon);
	~Player()override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="elapsedTime">�o�ߎ���</param>
	void Update(float elapsedTime)override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dc"></param>
	/// <param name="shader">�`����@</param>
	void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)override;

	//�L���X�e�[�g�}�V���̏�����
	void FSMInitialize() override;

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI()override;

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive()override;

	//���b�Z�[�W��M����
	bool OnMessage(const Telegram& msg) override;

	// �_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;

	// ���S�������ɌĂ΂��
	void OnDead() override;

};
