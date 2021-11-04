#include "Engine/Systems/Input.h"
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Math.h"
#include "Game/Characters/DerivedPlayerState.h"


const float cos45 = cosf(Math::ConvertToRadianAngle(45.f));
//const float
//-------------------------------------
//�e�X�e�[�g
//--------------------------------------

EntryState::~EntryState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
}

void EntryState::Enter()
{
	// �����ɓ���T�u�X�e�[�g
	SetSubState(static_cast<int>(Player::Entry::Select));
}

void EntryState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);

}

void EntryState::Exit()
{

}

ReactionState::~ReactionState()
{
	// �o�^���ꂽ�T�u�X�e�[�g�����Ԃɍ폜
	for (auto& state : sub_state_pool)
	{
		delete state;
	}
	sub_state_pool.clear();
	owner = nullptr;
}

void ReactionState::Enter()
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

void ReactionState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void ReactionState::Exit()
{
}

ReceiveState::~ReceiveState()
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

void ReceiveState::Enter()
{

}

void ReceiveState::Execute(float elapsedTime)
{
	// �T�u�X�e�[�g�̎��s
	sub_state->Execute(elapsedTime);
}

void ReceiveState::Exit()
{
}

//-------------------------------------
//�q�X�e�[�g
//--------------------------------------


void SelectState::Enter()
{

}

void SelectState::Execute(float elapsedTime)
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

void SelectState::Exit()
{
}



void WayChangeState::Enter()
{
}

void WayChangeState::Execute(float elapsedTime)
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
	else if (ax == 1.f && ay == -1.f)
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

void WayChangeState::Exit()
{
}



void MoveState::Enter()
{
}

void MoveState::Execute(float elapsedTime)
{
	GamePad& game_pad = Input::Instance().GetGamePad();

	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	DirectX::XMFLOAT2 player_pos = DirectX::XMFLOAT2(owner->GetPosition().x / Cell_Size, owner->GetPosition().z / Cell_Size);//�f�[�^��̒l�ɂ��邽��Cell_Size�Ŋ���

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


	//��
	if (ax == 0.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(0));
		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y) + 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
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
		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y) - 1][static_cast<size_t>(player_pos.x)].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
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
		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) + 1].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
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

		size_t map_data = owner->GetStageInformations()->map_role[static_cast<size_t>(player_pos.y)][static_cast<size_t>(player_pos.x) - 1].map_data;//���݂̃X�e�[�W�̏�񂩂���̏��ڂ�����
		//�ǂ��G�łȂ��Ȃ�
		if (map_data == 1 || map_data == 4 || map_data == 5)
		{
			owner->AddPositionX(-Cell_Size);
		}
	}


	//�E��
	if (ax == 1.f && ay == 1.f)
	{
		owner->SetAngleY(Math::ConvertToRadianAngle(45));
	}
	//����
	else if (ax == 1.f && ay == -1.f)
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

	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void MoveState::Exit()
{
	//	Meta meta = Meta::Instance();
}


void AttackState::Enter()
{
}

void AttackState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void AttackState::Exit()
{
	//	Meta meta = Meta::Instance();
}



void MenuState::Enter()
{
}

void MenuState::Execute(float elapsedTime)
{
	owner->GetStateMachine()->GetState()->ChangeSubState(static_cast<int>(Player::Entry::Select));
}

void MenuState::Exit()
{
}



void DamagedState::Enter()
{
}

void DamagedState::Execute(float elapsedTime)
{
}

void DamagedState::Exit()
{
}


void DeathState::Enter()
{
}

void DeathState::Execute(float elapsedTime)
{
}

void DeathState::Exit()
{
}

void CalledState::Enter()
{
}

void CalledState::Execute(float elapsedTime)
{
}

void CalledState::Exit()
{
}
