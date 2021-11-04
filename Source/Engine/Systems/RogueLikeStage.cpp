#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/RogueLikeStage.h"

RogueLikeStage::RogueLikeStage(RogueLikeDungeon* rogue_like_dungeon)
{

	scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	angle = DirectX::XMFLOAT3(0.f, 0.f, 0.f);;
	db_rogue_like_dungeon = rogue_like_dungeon;
	//SetStageObject(rogue_like_dungeon.map_role);
	SetStageObject(rogue_like_dungeon->map_role);
}

RogueLikeStage::~RogueLikeStage()
{
	this->Clear();
}

void RogueLikeStage::Update(float elapsedTime)
{
	// オブジェクト行列を更新
	//UpdateTransform();
	// モデル行列更新
	//	model->UpdateTransform(transform);

	for (auto& stage : stage_chip)
	{
		// オブジェクト行列を更新
		stage.UpdateTransform();
		// モデル行列更新
		stage.GetModel()->UpdateTransform(stage.GetTransform());
	}
}

void RogueLikeStage::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
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

bool RogueLikeStage::OnMessage(const Telegram& msg)
{
	return false;
}

void RogueLikeStage::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("RogueLikeStage", nullptr, ImGuiWindowFlags_None))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_OpenOnArrow))
		{
			// 位置
			ImGui::InputFloat3("Position", &this->position.x);
			// 回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(this->angle.x);
			a.y = DirectX::XMConvertToDegrees(this->angle.y);
			a.z = DirectX::XMConvertToDegrees(this->angle.z);
			ImGui::InputFloat3("Radian", &this->angle.x);
			ImGui::InputFloat3("Degree", &a.x);
			// スケール
			ImGui::InputFloat3("Scale", &this->scale.x);
		}
		//モデル数
			ImGui::Text(u8"モデル数:%d", static_cast<int>(stage_chip.size()));
			//プレイヤー初期位置
			//ImGui::Text(u8"SetPlayerMapPosition	:%f %f", db_rogue_like_dungeon.mobs[0].position.x, db_rogue_like_dungeon.mobs[0].position.y);
			//float PlayerFirstPosition_x = db_rogue_like_dungeon.mobs[0].position.x * 2.f;
			//float PlayerFirstPosition_y = db_rogue_like_dungeon.mobs[0].position.y * 2.f;
			//ImGui::Text(u8"PlayerFirstPosition:%f %f", PlayerFirstPosition_x, PlayerFirstPosition_y);
			//ImGui::Text(u8"PlayerPointAttribute:%d", db_rogue_like_dungeon.map_role[db_rogue_like_dungeon.mobs[0].position.y][db_rogue_like_dungeon.mobs[0].position.x]);

			ImGui::Text(u8"SetPlayerMapPosition	:%f %f", db_rogue_like_dungeon->mobs[0].position.x, db_rogue_like_dungeon->mobs[0].position.y);
			float PlayerFirstPosition_x = db_rogue_like_dungeon->mobs[0].position.x * 2.f;
			float PlayerFirstPosition_y = db_rogue_like_dungeon->mobs[0].position.y * 2.f;
			ImGui::Text(u8"PlayerFirstPosition:%f %f", PlayerFirstPosition_x, PlayerFirstPosition_y);
			ImGui::Text(u8"PlayerPointAttribute:%d", db_rogue_like_dungeon->map_role[db_rogue_like_dungeon->mobs[0].position.y][db_rogue_like_dungeon->mobs[0].position.x]);
		
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
			////map_dataが0の時、床を配置
			//if (map_role[y][x].map_data==0)
			//{
			//	st[object_num] = new Stage("Assets/FBX/geometry/wall.bin", DirectX::XMFLOAT3(x * Cell_Size, 0, y * Cell_Size), object_num);
			//	stage_chip.push_back(st[object_num]);
			//}

			//else if (map_role[y][x].map_data > 0)
			//{
			//	st[object_num] = new Stage("Assets/FBX/geometry/floor.bin", DirectX::XMFLOAT3(x * Cell_Size, 0, y * Cell_Size), object_num);
			//	stage_chip.push_back(st[object_num]);
			//}
			//object_num++;
			//map_dataが0の時、壁を配置
			if (map_role[y][x].map_data == 0)
			{
				Stage st("Assets/FBX/geometry/wall.bin", DirectX::XMFLOAT3(x * Cell_Size, 0, y * Cell_Size), object_num);
				stage_chip.push_back(st);
			}

			else if (map_role[y][x].map_data > 0)
			{
				Stage st("Assets/FBX/geometry/floor.bin", DirectX::XMFLOAT3(x * Cell_Size, 0, y * Cell_Size), object_num);
				stage_chip.push_back(st);
			}
			object_num++;
		}
	}
}

void RogueLikeStage::Clear()
{
	stage_chip.clear();
}
