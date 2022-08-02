//**********************************************************
//
//		RogueLikeGameCharacterクラス
//
//**********************************************************

#include "MyLiblary/Systems/Shader.h"
#include "MyLiblary/Objects/Model.h"
#include "MyLiblary/Systems/DebugRenderer.h"
#include "MyLiblary/Systems/ImGuiRenderer.h"
#include "Game/Stage/RogueLikeStage.h"
#include "MyLiblary/AI/DungeonMake.h"
#include "MyLiblary/Systems/Collision.h"


RogueLikeStage::RogueLikeStage()
{
	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	SetPosition(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	SetAngle(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	SetStageObject();
}

RogueLikeStage::~RogueLikeStage()
{
	this->Clear();
}

void RogueLikeStage::Update(float elapsed_time)
{

	//プレイヤーと階段の当たり判定


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
		ImGui::Text("Number of Models : %d", static_cast<int>(stage_chip.size()));
	}
	if (ImGui::CollapsingHeader("Stairs initialize Position", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//プレイヤーのマップ情報上での初期位置
		ImGui::Text("Stairs Map Position :  x : %d y : %d",
			RogueLikeDungeon::Instance().stairs_pos.x, RogueLikeDungeon::Instance().stairs_pos.y);

		//プレイヤーの初期位置
		const int stairs_pos_positionX = RogueLikeDungeon::Instance().stairs_pos.x * CELL_SIZE;
		const int stairs_pos_positionY = RogueLikeDungeon::Instance().stairs_pos.y * CELL_SIZE;

		ImGui::Text("Stairs Position : x : %d y : %d", stairs_pos_positionX, stairs_pos_positionY);
	}
	ImGui::End();
}

void RogueLikeStage::SetStageObject()
{
	this->Clear();

	int object_num = 0;
	bool is_once = false;//for文中、一度だけ行う作業のためのフラグ
	//オブジェクト配置
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			//階段
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Exit) && is_once == false)
			{
				const float pos_x = static_cast<float>(x * CELL_SIZE);
				const float pos_z = static_cast<float> (y * CELL_SIZE);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(CELL_SIZE, CELL_SIZE, CELL_SIZE);
				Stage stage("Assets/FBX/StageMapTip/MRTP_Obj/tento.bin", pos, scale, object_num);
				Register(stage);
				is_once = true;
			}
			//壁
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				const float pos_x = static_cast<float>(x * CELL_SIZE);
				const float pos_z = static_cast<float> (y * CELL_SIZE);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				Stage stage("Assets/FBX/geometry/wall.bin", pos, object_num);
				Register(stage);
			}
			//部屋
			else if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data >= static_cast<size_t>(Attribute::Room))
			{
				const float pos_x = static_cast<float>(x * CELL_SIZE);
				const float pos_z = static_cast<float> (y * CELL_SIZE);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				Stage stage("Assets/FBX/geometry/floor.bin", pos, object_num);
				Register(stage);
			}

			object_num++;
		}
	}
}

void RogueLikeStage::Clear()
{

	stage_chip.clear();
	stage_chip.shrink_to_fit();
}

void RogueLikeStage::Register(Stage stage)
{
	stage_chip.emplace_back(stage);
}
