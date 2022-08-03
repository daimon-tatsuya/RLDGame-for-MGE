//**********************************************************
//
//		StageManager�N���X
//
//**********************************************************
#include "Liblary/Systems/StageManager.h"
#include "Liblary/Systems/Stage.h"
#include "Liblary/AI/DungeonMake.h"
#include "Liblary/Systems/Collision.h"
#include "Liblary/Systems/Logger.h"

StageManager::~StageManager()
{
	this->Clear();
	LOG(" Executed : StageManager's destructor\n")
}


// �X�V����
void StageManager::Update(float elapsedTime)
{
	for (auto& stage : stages)
	{
		stage->Update(elapsedTime);
	}
}

// �`�揈��
void StageManager::Render(ID3D11DeviceContext* context, std::shared_ptr<Shader> shader)
{
	for (auto& stage : stages)
	{
		stage->Render(context, shader);
	}
}

// �X�e�[�W�o�^
void StageManager::Register(Stage* stage)
{
	stages.emplace_back(stage);
}

// �X�e�[�W�S�폜
void StageManager::Clear()
{
	for (auto& stage : stages)
	{
		//delete stage;
		stage.reset();
	}

	stages.clear();
	stages.shrink_to_fit();

	LOG(" Executed : StageManager's Clear Method\n")
}

// ���C�L���X�g
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