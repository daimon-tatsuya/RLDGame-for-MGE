//**********************************************************
//
//		CameraControllerクラス
//
//**********************************************************

#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"

#include "Engine/Systems/Camera.h"
#include "Engine/Systems/CameraController.h"

#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"



CameraController::CameraController()
{
	position = Camera::Instance().GetEye();
	new_position = Camera::Instance().GetEye();
}

// 更新処理
void CameraController::ActionGameCameraUpdate(float elapsed_time)
{
	//	フリーカメラ
	CharacterManager& character_manager = CharacterManager::Instance();
	Character* pl = character_manager.GetCharacterFromId(static_cast<int>(Meta::Identity::Player));
	this->new_target = pl->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// カメラの回転速度
	float speed = roll_speed * elapsed_time;


	// スティックの入力値に合わせてX軸とY軸を回転
	//angle.x += ay *speed;
	//angle.y += ax *speed;

	// X軸のカメラ回転を制限
	if (angle.x < min_angleX)
	{
		angle.x = min_angleX;
	}
	if (angle.x > max_angleX)
	{
		angle.x = max_angleX;
	}
	// Y軸の回転値を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	new_position.x = target.x - front.x * range;

	new_position.y = target.y - front.y * range + camera_heght;
	new_position.z = target.z - front.z * range;

	// 徐々に目標に近づける
	static	constexpr	float	Speed = 1.0f / 8.0f;
	position.x += (new_position.x - position.x) * Speed;
	position.y += (new_position.y - position.y) * Speed;
	position.z += (new_position.z - position.z) * Speed;
	target.x += (new_target.x - target.x) * Speed;
	target.y += (new_target.y - target.y) * Speed;
	target.z += (new_target.z - target.z) * Speed;

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::FollowCameraUpdate(float elapsed_time)
{
	//	フリーカメラ
	CharacterManager& character_manager = CharacterManager::Instance();
	Character* pl = character_manager.GetCharacterFromId(static_cast<int>(Meta::Identity::Player));
	this->new_target = pl->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// カメラの回転速度
	float speed = roll_speed * elapsed_time;

	DirectX::XMFLOAT3 player_position = pl->GetPosition();
	position.x = player_position.x;
	position.y = camera_heght;
	position.z = player_position.z-10;
	target = pl->GetPosition();

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::DrawDebugGUI()
{
	Camera& camera = Camera::Instance();
	ImGui::SetNextWindowPos(ImVec2(100, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_None))
	{
		// トランスフォーム
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
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
			//注視点
			ImGui::InputFloat3("target", &target.x);

			//正射影カメラなら行わない
			bool OrthMode = camera.GetOrthMode();
			if (OrthMode == false)
			{
				//カメラの高さ
				ImGui::DragFloat("Height", &camera_heght);
				ImGui::Checkbox("ViewMap", &ViewMap);
				if (ViewMap)
				{
					camera_heght = 400.f;
				}
				else
				{
					camera_heght = 50.f;
				}
			}
		}
	}
	ImGui::End();
}