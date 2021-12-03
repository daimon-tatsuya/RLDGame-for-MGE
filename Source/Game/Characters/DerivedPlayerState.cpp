//**********************************************************
//
//		Player��State�N���X
//
//**********************************************************

#include "Engine/Systems/Input.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Math.h"
#include "Game/Characters/DerivedPlayerState.h"


const float cos45 = cosf(Math::ConvertToRadianAngle(45.f));


//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

PlayerEntryState::~PlayerEntryState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void PlayerEntryState::Enter()
{
	// �����ɓ���T�u�X�e�[�g
	SetSubState(static_cast<int>(Player::Entry::Select));
}

void PlayerEntryState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);

}

void PlayerEntryState::Exit()
{

}

PlayerReactionState::~PlayerReactionState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
	owner = nullptr;
}

void PlayerReactionState::Enter()
{
	if (owner->GetHealth() > 0)
	{
		// �����ɓ���T�u�X�e�[�g
		SetSubState(static_cast<int>(Player::Reaction::Damaged));
	}
	else
	{
		SetSubState(static_cast<int>(Player::Reaction::Death));
	}
}

void PlayerReactionState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void PlayerReactionState::Exit()
{
}

PlayerReceiveState::~PlayerReceiveState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
		state = nullptr;
	}
	sub_state_pool.clear();
	owner = nullptr;
}

void PlayerReceiveState::Enter()
{
	// �����ɓ���T�u�X�e�[�g
	SetSubState(static_cast<int>(Player::Receive::Wait));
}

void PlayerReceiveState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void PlayerReceiveState::Exit()
{
}

//-------------------------------------
//�q�X�e�[�g
//--------------------------------------


void PlayerSelectState::Enter()
{

}

void PlayerSelectState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();


	//�U��
	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attack�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Attack));
	}

	//�����ɏ������������Ȃ��̂� else if��1��true�Ȃ����Ȃ��悤�ɂ���

	//���j���[
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_B))
	{
		//Menu�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Menu));
	}

	//�����]��
	else if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		//WayChange�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::WayChange));
	}

	//	�ړ�
	//���X�e�B�b�N��������DPad�����͂��ꂽ�Ƃ�
	else if (game_pad.GetAxisLX() != 0 || game_pad.GetAxisLY() != 0)
	{
		//Move�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Move));
	}
}

void PlayerSelectState::Exit()
{
}



void PlayerWayChangeState::Enter()
{
}

void PlayerWayChangeState::Execute(float elapsedTime)
{

	GamePad& game_pad = Input::Instance().GetGamePad();
	//Y�{�^���𒷉������Ă����
	if (game_pad.GetButtonUp() & static_cast<GamePadButton>(GamePad::BTN_Y))
	{
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
	}
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	//�X�e�b�v�����ā@���X�e�B�b�N�̓��͂�0, 1 , -1 , 1 / ��2(��0.71)�ɂ���8������\��
	//���X�e�B�b�N��x���̃X�e�b�v
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//���X�e�B�b�N��y���̃X�e�b�v
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//�����]��
	//��
	if (ax == 0.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(0));
	}
	//��
	else if (ax == 0.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(180));
	}
	//�E
	else if (ax == 1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(90));
	}
	//��
	else if (ax == -1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(270));
	}


	//�E��
	if (ax == 1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//����
	else if (ax == -1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(315));
	}
	//����
	else if (ax == -1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(225));
	}
	//�E��
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(135));
	}

	//�U��
	else	if (game_pad.GetButtonDown() & static_cast<GamePadButton>(GamePad::BTN_A))
	{
		//Attack�X�e�[�g�ɑJ�ڂ���
		owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Attack));
	}
}

void PlayerWayChangeState::Exit()
{
}



void PlayerMoveState::Enter()
{
}

void PlayerMoveState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	DirectX::XMFLOAT2 player_pos =//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���
		DirectX::XMFLOAT2(owner->GetPosition().x / Cell_Size, owner->GetPosition().z / Cell_Size);

	//�X�e�b�v�����ā@���X�e�B�b�N�̓��͂�0, 1 , -1 , 1 / ��2(��0.71)�ɂ���
	//���X�e�B�b�N��x���̃X�e�b�v
	if (ax > 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ax < 0.f)
	{
		ax = Math::StepAnyFloat(game_pad.GetAxisLX(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//���X�e�B�b�N��y���̃X�e�b�v
	if (ay > 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, (cos45 / 2.f), 1.f - (cos45 / 2.f)); ;
	}
	else if (ay < 0.f)
	{
		ay = Math::StepAnyFloat(game_pad.GetAxisLY(), cos45, -1.f + (cos45 / 2.f), (cos45 / 2.f), true); ;
	}

	//�ړ�����

	//�E��
	if (ax == 1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(45));
		size_t map_data_diagonal = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂��E��̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;


		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(Cell_Size);
			owner->AddPositionX(Cell_Size);
		}
	}
	//����
	else if (ax == -1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(315));
		size_t map_data_diagonal = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂�����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(Cell_Size);
			owner->AddPositionX(-Cell_Size);
		}
	}
	//����
	else if (ax == -1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(225));
		size_t map_data_diagonal = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂������̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(-Cell_Size);
			owner->AddPositionX(-Cell_Size);
		}
	}
	//�E��
	else if (ax == 1.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(135));
		size_t map_data_diagonal = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂��E���̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_horizon = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;

		size_t map_data_vertical = owner->GetStageInformations()->//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;

		//�ǂ��G�łȂ��Ȃ�
		//�ȂȂ߂̈ړ��Ȃ̂ŏ㉺���E�̈ʒu���m�F����
		if (
			(map_data_diagonal == 1 || map_data_diagonal == 4 || map_data_diagonal == 5) &&
			(map_data_horizon == 1 || map_data_horizon == 4 || map_data_horizon == 5) &&
			(map_data_vertical == 1 || map_data_vertical == 4 || map_data_vertical == 5)
			)
		{
			owner->AddPositionZ(-Cell_Size);
			owner->AddPositionX(Cell_Size);
		}
	}

	//��
	if (ax == 0.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(0));
		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionZ(Cell_Size);
		}
	}
	//��
	else if (ax == 0.f && ay == -1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(180));
		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionZ(-Cell_Size);
		}
	}
	//�E
	else if (ax == 1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(90));
		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂��E�̏��ڂ�����
	//�ǂ��G�łȂ��Ȃ�
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionX(Cell_Size);
		}
	}
	//��
	else if (ax == -1.f && ay == 0.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(270));

		size_t map_data = owner->GetStageInformations()->
			map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂����̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionX(-Cell_Size);
		}
	}

	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
	//owner->GetStateMachine()->ChangeState(static_cast<int>(Player::ParentState::Receive));
}

void PlayerMoveState::Exit()
{
	//	Meta meta = Meta::Instance();
}


void PlayerAttackState::Enter()
{
}

void PlayerAttackState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void PlayerAttackState::Exit()
{
	//	Meta meta = Meta::Instance();
}



void PlayerMenuState::Enter()
{
}

void PlayerMenuState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void PlayerMenuState::Exit()
{
}



void PlayerDamagedState::Enter()
{
}

void PlayerDamagedState::Execute(float elapsedTime)
{
}

void PlayerDamagedState::Exit()
{
}


void PlayerDeathState::Enter()
{
}

void PlayerDeathState::Execute(float elapsedTime)
{
}

void PlayerDeathState::Exit()
{
}

void PlayerCalledState::Enter()
{
}

void PlayerWaitState::Enter()
{
	//Meta& meta = Meta::Instance();
	//meta.SendMessaging(owner->GetId(),
	//	static_cast<int>(Meta::Identity::Meta),
	//	MESSAGE_TYPE::MSG_END_PLAYER_TURN);
}

void PlayerWaitState::Execute(float elapsedTime)
{
}

void PlayerWaitState::Exit()
{
}


void PlayerCalledState::Execute(float elapsedTime)
{
}

void PlayerCalledState::Exit()
{
}


