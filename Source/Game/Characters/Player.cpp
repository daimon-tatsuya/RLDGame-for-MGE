#include "Game/Characters/Player.h"
#include "Engine/Systems/Input.h"
#include "Engine/Systems/Graphics.h"
#include "Engine/Systems/Camera.h"
#include "Engine/Systems/Collision.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/AI/MetaAI.h"

Player::Player(RogueLikeDungeon& rogue_like_dungeon)
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	scale.x = scale.y = scale.z = 1.f;
	position.y = 15.f;
	//初期ステート

	//position.x = mob_role.position.x;// *Cell_Size;
	//position.z = mob_role.position.y;// *Cell_Size;

	//オブジェクト配置
	for (int y = 0; y < MapSize_Y; y++)
	{
		for (int x = 0; x < MapSize_X; x++)
		{
			if (rogue_like_dungeon.map_role[y][x].map_data == 2)
			{
				position = DirectX::XMFLOAT3(x * Cell_Size, 0, y * Cell_Size);
			}
		}
	}
}

Player::Player()
{
	model = std::make_shared<Model>("Assets/FBX/Animals/BlackWidow.bin");
	scale.x = scale.y = scale.z = 1.f;
	position = DirectX::XMFLOAT3(0, 3, 0);

}

Player::~Player()
{
	//delete model;
}

void Player::Update(float elapsedTime)
{
	position.y = 0.f;

	InputMove(elapsedTime);

	{
		//--追加---------------------------------------
		// AABB
		//
		//-----------------------------------------------
	}

	//--消す予定----------------------------------
	// 速力更新処理
	UpdateVelocity(elapsedTime);

	// 無敵時間更新
	UpdateInvincibleTimer(elapsedTime);
	//------------------------------------------------

	// オブジェクト行列を更新
	UpdateTransform();

	// モデルアニメーション更新処理
	model->UpdateAnimation(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);
}

void Player::Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader)
{
	shader->Draw(dc, model.get());
}

void Player::DrawDebugGUI()
{
	//入力された情報を取得
	GamePad& game_pad = Input::Instance().GetGamePad();
	float ax = game_pad.GetAxisLX();
	float ay = game_pad.GetAxisLY();

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
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

			// スケール
			ImGui::InputFloat3("Scale", &this->scale.x);
			//ID
			ImGui::Text(u8"ID:%d", this->id);
			//ゲームパッドのスティック入力
			ImGui::Text(u8"GamePadAxis:%f %f", ax, ay);
		}
	}
	ImGui::End();
}

void Player::DrawDebugPrimitive()
{
	DebugRenderer* debug_renderer = Graphics::Instance().GetDebugRenderer();

	// 衝突判定用のデバッグ球を描画
	debug_renderer->DrawSphere(this->position, this->radius, DirectX::XMFLOAT4(0, 0, 0, 1));
}

bool Player::OnMessage(const Telegram& msg)
{
	//Todo　メタAIからの受信処理

	return false;
}

void Player::OnLanding()
{
}

void Player::OnDamaged()
{
}

void Player::OnDead()
{
}

//bool Player::InputMove(float elapsedTime)
//{
//	// カメラ方向とスティックの入力値によって進行方向を計算する
//	Camera& camera = Camera::Instance();
//	DirectX::XMFLOAT3 camera_right = camera.GetRight();
//	DirectX::XMFLOAT3 camera_front = camera.GetFront();
//
//	// 進行ベクトル取得
//	DirectX::XMFLOAT3 move_vec = GetMoveVec(camera_right, camera_front);
//
//	// 移動処理
//	Move(move_vec.x, move_vec.z, 1.0f);
//	// 進行ベクトルがゼロベクトルでない場合は入力された
//	return move_vec.x != 0.0f || move_vec.y != 0.0f || move_vec.z != 0.0f;
//}