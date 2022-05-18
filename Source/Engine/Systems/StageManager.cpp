//**********************************************************
//
//		StageManagerクラス
//
//**********************************************************
#include "Engine/Systems/StageManager.h"
#include "Engine/Systems/Stage.h"
#include "Engine/AI/DungeonMake.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/Logger.h"
StageManager::~StageManager()
{
	this->Clear();
}

// 更新処理
void StageManager::Update(float elapsedTime)
{
	for (const auto& stage : stages)
	{
		stage->Update(elapsedTime);
	}
}

// 描画処理
void StageManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader) const
{
	for (const auto& stage : stages)
	{
		stage->Render(context, shader);
	}
}

// ステージ登録
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}

// ステージ全削除
void StageManager::Clear()
{
	for (auto& stage : stages)
	{
		//const int b = stage.use_count();
		//LOG("before%d\n",b)
		stage.reset();
		//const int a = stage.use_count();
		//LOG("after%d\n", a)
	}
	std::vector<std::shared_ptr<Stage>>().swap(stages);
	stages.clear();
	stages.shrink_to_fit();
}

// レイキャスト
bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	bool result = false;
	hit.distance = FLT_MAX;

	for (auto& stage : stages)
	{
		HitResult tmp;
		if (stage->RayCast(start, end, tmp))
		{
			if (hit.distance > tmp.distance)
			{
				hit = tmp;
				result = true;
			}
		}
	}

	return result;
}

void StageManager::DrawDebugGUI()
{
	for (auto& stage : stages)
	{
		stage->DrawDebugGUI();
	}
}