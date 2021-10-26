#include "Game/Stages/StageMain.h"
#include "Engine/Systems/Graphics.h"
StageMain::StageMain()
{
	// �X�e�[�W���f����ǂݍ���
	model = std::make_shared<Model>("Assets/FBX/geometry/floor.bin");
	scale.x = scale.y = scale.z = 1.f;

}

StageMain::~StageMain()
{
//	delete model;
}

void StageMain::Update(float elapsedTime)
{
	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���f���s��X�V
	model->UpdateTransform(transform);

}

void StageMain::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
	return Collision::IntersectRayToModel(start, end, model.get(), hit);
}

bool StageMain::OnMessage(const Telegram& msg)
{
	return false;
}

void StageMain::DrawDebugGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Stage", nullptr, ImGuiWindowFlags_None))
	{
		// �g�����X�t�H�[��
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::InputFloat3("Position", &this->position.x);
			// ��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(this->angle.x);
			a.y = DirectX::XMConvertToDegrees(this->angle.y);
			a.z = DirectX::XMConvertToDegrees(this->angle.z);
			ImGui::InputFloat3("Radian", &this->angle.x);
			ImGui::InputFloat3("Degree", &a.x);
			// �X�P�[��
			ImGui::InputFloat3("Scale", &this->scale.x);
		}
	}
	ImGui::End();
}