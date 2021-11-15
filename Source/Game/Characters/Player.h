#pragma once

#include "Engine/Systems/PlayerBase.h"
#include "Engine/Systems/DungeonMake.h"

//ToDo Player���s�����Ƃ�����


class Player :
	public PlayerBase
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

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
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
