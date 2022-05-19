//**********************************************************
//
//		RogueLikeGameCharacter�N���X
//
//**********************************************************

#include "Engine/Systems/Shader.h"
#include "Engine/Objects/Model.h"
#include "Engine/Systems/DebugRenderer.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Game/Stage/RogueLikeStage.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/Collision.h"


RogueLikeStages::RogueLikeStages()
{
	SetScale(DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	SetPosition(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	SetAngle(DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	SetStageObject();
}

RogueLikeStages::~RogueLikeStages()
{
	this->Clear();
}

void RogueLikeStages::Update(float elapsed_time)
{

	//�v���C���[�ƊK�i�̓����蔻��


	for (auto& stage : stage_chip)
	{
		// �I�u�W�F�N�g�s����X�V
		stage->UpdateTransform();
		// ���f���s��X�V
		stage->GetModel()->UpdateTransform(stage->GetTransform());
	}


}

void RogueLikeStages::Render(ID3D11DeviceContext* dc, const std::shared_ptr<Shader> shader)
{
	for (auto& stage : stage_chip)
	{
		shader->Draw(dc, stage->GetModel());
	}
}

bool RogueLikeStages::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	for (auto& stage : stage_chip)
	{
		//true�̎���HitResult��Ԃ�
		if (Collision::IntersectRayToModel(start, end, stage->GetModel(), hit))
		{
			return true;
		}
	}
	return false;
}

bool RogueLikeStages::OnMessage(const Telegram& telegram)
{
	return false;
}

void RogueLikeStages::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("RogueLikeStages", nullptr, ImGuiWindowFlags_None))
	{
		//���f����
		ImGui::Text("Number of Models:%d", static_cast<int>(stage_chip.size()));
	}
	if (ImGui::CollapsingHeader("Stairs initialize Position", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//�v���C���[�̃}�b�v����ł̏����ʒu
		ImGui::Text("Stairs Map Position: 	%d %d",
			RogueLikeDungeon::Instance().stairs_pos.x, RogueLikeDungeon::Instance().stairs_pos.y);

		//�v���C���[�̏����ʒu
		const int stairs_pos_positionX = RogueLikeDungeon::Instance().stairs_pos.x * CellSize;
		const int stairs_pos_positionY = RogueLikeDungeon::Instance().stairs_pos.y * CellSize;

		ImGui::Text("Stairs Position:	 %d %d", stairs_pos_positionX, stairs_pos_positionY);
	}
	ImGui::End();
}

void RogueLikeStages::SetStageObject()
{
	this->Clear();

	int object_num = 0;
	bool is_once = false;//for�����A��x�����s����Ƃ̂��߂̃t���O
	//�I�u�W�F�N�g�z�u
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			//�K�i
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Exit) && is_once == false)
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				DirectX::XMFLOAT3 scale=DirectX::XMFLOAT3(CellSize, CellSize, CellSize);
				stage_chip.emplace_back(new Stage("Assets/FBX/StageMapTip/MRTP_Obj/tento.bin", pos, scale, object_num));
			}
			//��
			if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data == static_cast<size_t>(Attribute::Wall))
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				stage_chip.emplace_back(new Stage("Assets/FBX/geometry/wall.bin", pos, object_num));
			}
			//����
			else if (RogueLikeDungeon::Instance().GetMapRole()[y][x].map_data >= static_cast<size_t>(Attribute::Room))
			{
				float pos_x = static_cast<float>(x * CellSize);
				float pos_z = static_cast<float> (y * CellSize);
				DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(pos_x, 0, pos_z);
				stage_chip.emplace_back(new Stage("Assets/FBX/geometry/floor.bin", pos, object_num));
			}

			object_num++;
		}
	}
}

void RogueLikeStages::Clear()
{
	for (auto& stage:stage_chip)
	{
		 stage.reset();
	}
	stage_chip.clear();
	stage_chip.shrink_to_fit();
}