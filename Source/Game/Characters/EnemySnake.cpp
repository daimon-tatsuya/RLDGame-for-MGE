
//**********************************************************
//
//		EnemyManagerクラス
//
//**********************************************************

#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"

#include "Game/Characters/EnemySnake.h"

#include "Game/Characters/DerivedEnemySnakeState.h"

EnemySnake::EnemySnake(RogueLikeDungeon* rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/Rattlesnake.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 0.f;
	stage_informations = rogue_like_dungeon;

	//初期ステート
	FSMInitialize();

	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_informations->map_role[y][x].map_data == 3)
			{
				float pos_x = static_cast<float>(x * Cell_Size);
				float pos_z = static_cast<float> (y * Cell_Size);

				position = DirectX::XMFLOAT3(pos_x, 0, pos_z);
			}
		}
	}
}

EnemySnake::~EnemySnake()
{
}

void EnemySnake::Update(float elapsedTime)
{
	position.y = 0.f;

	state_machine->Update(elapsedTime);

	//回転角の正規化
	NormalizeAngle();

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);
}

void EnemySnake::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
}

void EnemySnake::FSMInitialize()
{
	state_machine = std::make_unique<StateMachine>(this);

	//親ステート
	state_machine->RegisterState(new EnemySnakeEntryState(this));
	state_machine->RegisterState(new EnemySnakeReactionState(this));
	state_machine->RegisterState(new  EnemySnakeReceiveState(this));
	//子ステート
	//Select
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Entry), new EnemySnakeSelectState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Entry), new EnemySnakeApproachState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Entry), new EnemySnakeExploreState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Entry), new EnemySnakeAttackState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Entry), new EnemySnakeAbilityState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Entry), new EnemySnakeUseItemState(this));

	//Reaction
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Reaction), new EnemySnakeDamagedState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Reaction), new EnemySnakeDeathState(this));

	//Receive
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Receive), new EnemySnakeWaitState(this));
	state_machine->RegisterSubState(static_cast<int>(EnemySnake::ParentState::Receive), new EnemySnakeCalledState(this));

	state_machine->SetState(static_cast<int>(EnemySnake::ParentState::Receive));

}

void EnemySnake::Destroy()
{

}

void EnemySnake::DrawDebugGUI()
{
}

void EnemySnake::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debug_renderer->DrawSphere(this->position, this->radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

void EnemySnake::OnDamaged()
{
}

void EnemySnake::OnDead()
{
}

bool EnemySnake::OnMessage(const Telegram& telegram)
{
	//メタAIからの受信処理
	switch (telegram.msg)
	{
	case MESSAGE_TYPE::MSG_END_PLAYER_TURN:

			state_machine->ChangeState(static_cast<int>(EnemySnake::ParentState::Entry));

		return true;

	}
	return false;
}
