
//**********************************************************
//
//		EnemyManagerクラス
//
//**********************************************************

#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Shader.h"
#include "Engine/Systems/ShaderManager.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Objects/Model.h"
#include "Engine/AI/DungeonMake.h"

#include "Game/Characters/EnemySnake.h"

EnemySnake::EnemySnake(RogueLikeDungeon* rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/Rattlesnake.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 0.f;
	stage_information = rogue_like_dungeon;

	//初期ステート
	EnemySnake::FSMInitialize();

	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (stage_information->map_role[y][x].map_data == 3)
			{
				const float pos_x = static_cast<float>(x * CellSize);
				const float pos_z = static_cast<float> (y * CellSize);

				position = DirectX::XMFLOAT3(pos_x, 0, pos_z);
			}
		}
	}
}



void EnemySnake::Update(float elapsed_time)
{
	position.y = 0.f;

	//state_machine->Update(elapsedTime);

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

		//		state_machine->ChangeState(static_cast<int>(EnemySnake::ParentState::Entry));

		return true;
	default:
		;
	}
	return false;
}
