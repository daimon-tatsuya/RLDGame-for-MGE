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
}

void RogueLikeStage::Update(float elapsed_time)
{
	// オブジェクト行列を更新
	UpdateTransform();
	// モデル行列更新
	GetModel()->UpdateTransform(GetTransform());



}

void RogueLikeStage::Render(ID3D11DeviceContext* dc, const std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, GetModel());
}

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

