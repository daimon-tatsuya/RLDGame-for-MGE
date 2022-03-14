//**********************************************************
//
//		RogueLikeStageクラス
//
//**********************************************************

#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/RogueLikeStage.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/Collision.h"

RogueLikeStage::RogueLikeStage(RogueLikeDungeon* rogue_like_dungeon) : rogue_like_dungeon_imgui(rogue_like_dungeon)
{
	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	angle = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	SetStageObject(rogue_like_dungeon->map_role);
}

RogueLikeStage::~RogueLikeStage()
{
	this->Clear();
}

void RogueLikeStage::Update(float elapsed_time)
{

	for (auto& stage : stage_chip)
	{
		// オブジェクト行列を更新
		stage.UpdateTransform();
		// モデル行列更新
		stage.GetModel()->UpdateTransform(stage.GetTransform());
	}
}

void RogueLikeStage::Render(ID3D11DeviceContext* dc, const std::shared_ptr<Shader> shader)
{
	for (auto& stage : stage_chip)
	{
		shader->Draw(dc, stage.GetModel());
	}
}

bool RogueLikeStage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	for (auto& stage : stage_chip)
	{
		//trueの時のHitResultを返す
		if (Collision::IntersectRayToModel(start, end, stage.GetModel(), hit))
		{
			return true;
		}
	}
	return false;
}

bool RogueLikeStage::OnMessage(const Telegram& telegram)
{
	return false;
}

void RogueLikeStage::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("RogueLikeStage", nullptr, ImGuiWindowFlags_None))
	{
		//モデル数
		ImGui::Text("Number of Models:%d", static_cast<int>(stage_chip.size()));

		//プレイヤーのマップ情報上での初期位置
		ImGui::Text("PlayerMapPosition	:%f %f",
			rogue_like_dungeon_imgui->mobs[0].position.x, rogue_like_dungeon_imgui->mobs[0].position.y);

		//プレイヤーの初期位置
		const float player_positionX = rogue_like_dungeon_imgui->mobs[0].position.x * 2.f;
		const float player_positionY = rogue_like_dungeon_imgui->mobs[0].position.y * 2.f;

		ImGui::Text("PlayerPosition:%f %f", player_positionX, player_positionY);
	}
	ImGui::End();
}

void RogueLikeStage::SetStageObject(std::vector<std::vector<RogueLikeMap>> map_role)
{
	this->Clear();

	int object_num = 0;
	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//壁
			if (map_role[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				Stage st("Assets/FBX/geometry/wall.bin", pos, object_num);
				stage_chip.emplace_back(st);
			}
			//部屋
			else if (map_role[y][x].map_data >= static_cast<size_t>(Attribute::Room))
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				Stage st("Assets/FBX/geometry/floor.bin", pos, object_num);
				stage_chip.emplace_back(st);
			}

			object_num++;
		}
	}
}

void RogueLikeStage::Clear()
{
	stage_chip.clear();
}