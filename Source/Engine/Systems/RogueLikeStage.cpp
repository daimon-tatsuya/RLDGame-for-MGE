//**********************************************************
//
//		RogueLikeGameCharacterクラス
//
//**********************************************************

#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/RogueLikeStage.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/Logger.h"


RogueLikeStage::RogueLikeStage(const char* filename, DirectX::XMFLOAT3& pos, const int id):Stage(filename,  pos, id)
{}

RogueLikeStage::~RogueLikeStage()
{
	//this->Clear();
}

void RogueLikeStage::Update(float elapsed_time)
{
	//for (auto& stage : stage_chip)
	//{
	//	// オブジェクト行列を更新
	//	stage->UpdateTransform();
	//	// モデル行列更新
	//	stage->GetModel()->UpdateTransform(stage->GetTransform());
	//}

		// オブジェクト行列を更新
	UpdateTransform();
	// モデル行列更新
	GetModel()->UpdateTransform(GetTransform());



}

void RogueLikeStage::Render(ID3D11DeviceContext* dc, const std::shared_ptr<Shader> shader)
{
	//for (auto& stage : stage_chip)
	//{
	//	shader->Draw(dc, stage->GetModel());
	//}
	shader->Draw(dc, GetModel());
}

//bool RogueLikeStage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
//{
//	for (auto& stage : stage_chip)
//	{
//		//trueの時のHitResultを返す
//		if (Collision::IntersectRayToModel(start, end, stage->GetModel(), hit))
//		{
//			return true;
//		}
//	}
//	return false;
//}

bool RogueLikeStage::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	//trueの時のHitResultを返す
	if (Collision::IntersectRayToModel(start, end, GetModel(), hit))
	{
		return true;
	}
	return false;
}

bool RogueLikeStage::OnMessage(const Telegram& telegram)
{
	return false;
}

void RogueLikeStage::DrawDebugGUI()
{

}

//void RogueLikeStage::SetStageObject()
//{
//	this->Clear();
//
//	int object_num = 0;
//	bool is_once = false;//for文中、一度だけ行う作業のためのフラグ
//	//オブジェクト配置
//	for (int y = 0; y < MapSize_Y; y++)
//	{
//		for (int x = 0; x < MapSize_X; x++)
//		{
//			//階段
//			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Exit) && is_once == false)
//			{
//				float pos_x = static_cast<float>(x * CellSize);
//				float pos_z = static_cast<float> (y * CellSize);
//				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
//				std::shared_ptr<Stage> st = std::make_shared<Stage>("Assets/FBX/StageMapTip/MRTP_Obj/tento.bin", pos, object_num);
//				st.get()->SetScale(DirectX::XMFLOAT3(2.7f, 2.7f, 3));
//				//stage_chip.emplace_back(st);
//			}
//			//壁
//			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Wall))
//			{
//				float pos_x = static_cast<float>(x * CellSize);
//				float pos_z = static_cast<float> (y * CellSize);
//				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
//				std::shared_ptr<Stage> st = std::make_shared<Stage>("Assets/FBX/geometry/wall.bin", pos, object_num);
//				//stage_chip.emplace_back(st);
//			}
//			//部屋
//			else if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data >= static_cast<size_t>(Attribute::Room))
//			{
//				float pos_x = static_cast<float>(x * CellSize);
//				float pos_z = static_cast<float> (y * CellSize);
//				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
//				std::shared_ptr<Stage> st = std::make_shared<Stage>("Assets/FBX/geometry/floor.bin", pos, object_num);
//				//stage_chip.emplace_back(st);
//			}
//
//			object_num++;
//		}
//	}
//}

//void RogueLikeStage::Clear()
//{
//	for (auto& stage:stage_chip)
//	{
//			stage.reset();
//			const int after = stage.use_count();
//			if (after !=0)
//			{
//				LOG("erorr : RogueLikeStage's meomy leak")
//			}
//	}
//	std::vector<std::shared_ptr<Stage>>().swap(stage_chip);
//	stage_chip.clear();
//	stage_chip.shrink_to_fit();
//}