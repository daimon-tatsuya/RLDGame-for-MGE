//**********************************************************
//
//		CameraControllerクラス
//
//**********************************************************

#include "Engine/Systems/Input.h"

#include "Engine/Systems/CameraController.h"
#include "Engine/Systems/Camera.h"
#include  "Engine/Systems/Graphics.h"
#include "Engine/Systems/ImGuiRenderer.h"
#include "Engine/Systems/Math.h"
#include "Engine/AI/MetaAI.h"
#include "Engine/Systems/Character.h"
#include "Engine/Systems/CharacterManager.h"
#include "Engine/Systems/Collision.h"

CameraController::CameraController()
	: position(Camera::Instance().GetEye()),
	  new_position(Camera::Instance().GetEye()), frustum()
{
}

// 更新処理
void CameraController::ActionGameCameraUpdate(const float elapsed_time)
{
	// フリーカメラ
	const CharacterManager& character_manager = CharacterManager::Instance();
	const Character* player = character_manager.GetCharacterFromId(static_cast<int>(Identity::Player));
	this->new_target = player->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// カメラの回転速度
	float speed = roll_speed * elapsed_time;

	// スティックの入力値に合わせてX軸とY軸を回転
	//angle.x += ay *speed;
	//angle.y += ax *speed;

	//! X軸のカメラ回転を制限
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
	DirectX::XMFLOAT3 front{};
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	new_position.x = target.x - front.x * range;

	new_position.y = target.y - front.y * range + camera_height;
	new_position.z = target.z - front.z * range;

	// 徐々に目標に近づける

	position.x += (new_position.x - position.x) * CAMERA_MOVE_SPEED;
	position.y += (new_position.y - position.y) * CAMERA_MOVE_SPEED;
	position.z += (new_position.z - position.z) * CAMERA_MOVE_SPEED;
	target.x += (new_target.x - target.x) * CAMERA_MOVE_SPEED;
	target.y += (new_target.y - target.y) * CAMERA_MOVE_SPEED;
	target.z += (new_target.z - target.z) * CAMERA_MOVE_SPEED;

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::FollowCameraUpdate(float elapsed_time)
{
	// フリーカメラ
	CharacterManager& character_manager = CharacterManager::Instance();
	const Character* player = character_manager.GetCharacterFromId(static_cast<int>(Identity::Player));
	this->new_target = player->GetPosition();
	GamePad& gamePad = Input::Instance().GetGamePad();
	// カメラの回転速度
	float speed = roll_speed * elapsed_time;

	const DirectX::XMFLOAT3 player_position = player->GetPosition();
	position.x = player_position.x;
	position.y = camera_height;
	position.z = player_position.z - 10;
	target = player->GetPosition();

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::CalculateFrustum()
{
	const Camera& camera = Camera::Instance();

	// ビュープロジェクション行列を取得する
	DirectX::XMMATRIX matrix = {};
	const DirectX::XMMATRIX view_matrix = DirectX::XMLoadFloat4x4(&camera.GetView());
	const DirectX::XMMATRIX projction_matrix = DirectX::XMLoadFloat4x4(&camera.GetProjection());
	matrix = view_matrix * projction_matrix;

	// ビュープロジェクション行列の逆行列
	DirectX::XMMATRIX inverse_matrix = DirectX::XMMatrixInverse(nullptr, matrix);

	// ビュープロジェクション内の頂点算出用位置ベクトル
	DirectX::XMVECTOR vertexs[8] =
	{
		// near plane corners
		{ -1, -1, 0 },	// [0]:左下
		{  1, -1, 0 },	// [1]:右下
		{  1,  1, 0 },	// [2]:右上
		{ -1,  1 ,0 },	// [3]:左上

		// far plane corners.
		{ -1, -1, 1 },	// [4]:左下
		{  1, -1, 1 },	// [5]:右下
		{  1,  1, 1 },	// [6]:右上
		{ -1,  1, 1 } 	// [7]:左上
	};

	// ビュープロジェクション行列の逆行列を用いて、各頂点を算出する
	for (int i = 0; i < 8; i++)
	{
		vertexs[i] = DirectX::XMVector3TransformCoord(vertexs[i], inverse_matrix);
		if (i < 4)
		{
			DirectX::XMStoreFloat3(&frustum.near_vertices_position[i], vertexs[i]);
		}
		else
		{
			DirectX::XMStoreFloat3(&frustum.far_vertices_position[i - 4], vertexs[i]);
		}
	}

	DirectX::XMFLOAT4X4 matrix4X4 = {};
	DirectX::XMStoreFloat4x4(&matrix4X4, matrix);

	// 視錐台（フラスタム）を構成する６平面を算出する
	// 0:左側面, 1:右側面, 2:下側面, 3:上側面, 4:奥側面, 5:手前側面
	// 全ての面の法線は内側を向くように設定すること

	// 左側面
	frustum.normal[0].x = matrix4X4._14 + matrix4X4._11;
	frustum.normal[0].y = matrix4X4._24 + matrix4X4._21;
	frustum.normal[0].z = matrix4X4._34 + matrix4X4._31;
	frustum.normal[0] = Math::Normalize(frustum.normal[0]);
	frustum.shortest_direction[0] = Math::Dot(frustum.normal[0], frustum.near_vertices_position[0]);

	// 右側面
	frustum.normal[1].x = matrix4X4._14 - matrix4X4._11;
	frustum.normal[1].y = matrix4X4._24 - matrix4X4._21;
	frustum.normal[1].z = matrix4X4._34 - matrix4X4._31;
	frustum.normal[1] = Math::Normalize(frustum.normal[1]);
	frustum.shortest_direction[1] = Math::Dot(frustum.normal[1], frustum.near_vertices_position[1]);

	// 下側面
	frustum.normal[2].x = matrix4X4._14 + matrix4X4._12;
	frustum.normal[2].y = matrix4X4._24 + matrix4X4._22;
	frustum.normal[2].z = matrix4X4._34 + matrix4X4._32;
	frustum.normal[2] = Math::Normalize(frustum.normal[2]);
	frustum.shortest_direction[2] = Math::Dot(frustum.normal[2], frustum.near_vertices_position[0]);

	// 上側面
	frustum.normal[3].x = matrix4X4._14 - matrix4X4._12;
	frustum.normal[3].y = matrix4X4._24 - matrix4X4._22;
	frustum.normal[3].z = matrix4X4._34 - matrix4X4._32;
	frustum.normal[3] = Math::Normalize(frustum.normal[3]);
	frustum.shortest_direction[3] = Math::Dot(frustum.normal[3], frustum.near_vertices_position[2]);

	// 奥側面
	frustum.normal[4].x = -matrix4X4._14 - matrix4X4._13;
	frustum.normal[4].y = -matrix4X4._24 - matrix4X4._23;
	frustum.normal[4].z = -matrix4X4._34 - matrix4X4._33;
	frustum.normal[4] = Math::Normalize(frustum.normal[4]);
	frustum.shortest_direction[4] = Math::Dot(frustum.normal[4], frustum.far_vertices_position[0]);

	// 手前側面
	frustum.normal[5].x = -matrix4X4._14 + matrix4X4._13;
	frustum.normal[5].y = -matrix4X4._24 + matrix4X4._23;
	frustum.normal[5].z = -matrix4X4._34 + matrix4X4._33;
	frustum.normal[5] = Math::Normalize(frustum.normal[5]);
	frustum.shortest_direction[5] = Math::Dot(frustum.normal[5], frustum.near_vertices_position[0]);

	//各境界線outLineNormを面の法線の外積から求めて正規化する
	// 左下境界線 ( frustum[0]×frustum[2] )
	frustum.out_line_norm[0] = Math::Normalize(Math::Cross(frustum.normal[0], frustum.normal[2]));
	// 右下境界線 ( frustum[2]×frustum[1] )
	frustum.out_line_norm[1] = Math::Normalize(Math::Cross(frustum.normal[2], frustum.normal[1]));
	// 右上境界線 ( frustum[1]×frustum[3] )
	frustum.out_line_norm[2] = Math::Normalize(Math::Cross(frustum.normal[1], frustum.normal[3]));
	// 左上境界線 ( frustum[3]×frustum[0] )
	frustum.out_line_norm[3] = Math::Normalize(Math::Cross(frustum.normal[3], frustum.normal[0]));
}

bool CameraController::IntersectFrustum(AxisAlignedBoundingBox* aabb)
{
	// 視錐台（フラスタム）との交差・内外判定
	// AABBが視錐台の内部にある			：collision_state = 0
	// AABBが視錐台の境界と交わっている：collision_state = 1
	// AABBが視錐台の外部にある			：collision_state = 2

	// いったん内部にあるとしておく
	frustum.collision_state = true;

	for (int i = 0; i < 6; i++)//視錐台の6面
	{
		//④各平面の法線の成分を用いてAABBの８頂点の中から最近点と最遠点を求める
		DirectX::XMFLOAT3 nega_pos = aabb->center;	// 最近点
		DirectX::XMFLOAT3 posi_pos = aabb->center;	// 最遠点
		if (frustum.normal[i].x > 0.0f)
		{
			nega_pos.x -= aabb->radius.x;
			posi_pos.x += aabb->radius.x;
		}
		else
		{
			nega_pos.x += aabb->radius.x;
			posi_pos.x -= aabb->radius.x;
		}
		if (frustum.normal[i].y > 0.0f)
		{
			nega_pos.y -= aabb->radius.y;
			posi_pos.y += aabb->radius.y;
		}
		else
		{
			nega_pos.y += aabb->radius.y;
			posi_pos.y -= aabb->radius.y;
		}
		if (frustum.normal[i].z > 0.0f)
		{
			nega_pos.z -= aabb->radius.z;
			posi_pos.z += aabb->radius.z;
		}
		else
		{
			nega_pos.z += aabb->radius.z;
			posi_pos.z -= aabb->radius.z;
		}

		//  各平面との内積を計算し、交差・内外判定を行う
		//  外部と分かれば処理をbreakし確定させる
		//  交差状態であれば、ステータスを変更してから次の平面とのチェックに続ける
		//  内部であれば、そのまま次の平面とのチェックに続ける
		const float dot_negative = Math::Dot(frustum.normal[i], nega_pos);
		const float dot_positive = Math::Dot(frustum.normal[i], posi_pos);

		if (dot_negative < frustum.shortest_direction[i] && dot_positive < frustum.shortest_direction[i])
		{
			frustum.collision_state = false;
			break;
		}
		else if ((dot_negative - frustum.shortest_direction[i]) * (dot_positive - frustum.shortest_direction[i]) < 0.0f)
		{
			frustum.collision_state = true;
		}
	}

	// 交差状態の詳細判定
	// AABBの面の中心点と法線を作り、中心点から視錐台の８頂点に対して表裏判定を行う。
	// ８頂点全てが表側にあれば、交差していなかったことが確定するのでcollisionStateを更新してreturnする

	if (frustum.collision_state == 1)	// AABBが視錐台の境界と交わっている場合、詳細判定を行う
	{
		DirectX::XMFLOAT3 aabb_face_center = {};	// 面の中心点
		DirectX::XMFLOAT3 normal = {};			// 面の法線（単位化）

		for (int i = 0; i < 6; i++)	// AABBの６面分ループ
		{
			switch (i)
			{
			case 0:
				normal = { 1,0,0 };
				aabb_face_center = { aabb->center.x + aabb->radius.x, aabb->center.y, aabb->center.z };
				break;
			case 1:
				normal = { 0,1,0 };
				aabb_face_center = { aabb->center.x, aabb->center.y + aabb->radius.y, aabb->center.z };
				break;
			case 2:
				normal = { 0,0,1 };
				aabb_face_center = { aabb->center.x, aabb->center.y, aabb->center.z + aabb->radius.z };
				break;
			case 3:
				normal = { -1,0,0 };
				aabb_face_center = { aabb->center.x - aabb->radius.x, aabb->center.y, aabb->center.z };
				break;
			case 4:
				normal = { 0,-1,0 };
				aabb_face_center = { aabb->center.x, aabb->center.y - aabb->radius.y, aabb->center.z };
				break;
			case 5:
				normal = { 0,0,-1 };
				aabb_face_center = { aabb->center.x, aabb->center.y, aabb->center.z - aabb->radius.z };
				break;
			default:
				break;
			}
			for (int j = 0; j < 8; j++)	// 視錐台の８頂点分ループ
			{
				DirectX::XMFLOAT3 vec_near = {};
				if (j < 4)
				{
					vec_near = { frustum.near_vertices_position[j].x - aabb_face_center.x, frustum.near_vertices_position[j].y - aabb_face_center.y, frustum.near_vertices_position[j].z - aabb_face_center.z };
				}
				else
				{
					vec_near = { frustum.far_vertices_position[j - 4].x - aabb_face_center.x, frustum.far_vertices_position[j - 4].y - aabb_face_center.y, frustum.far_vertices_position[j - 4].z - aabb_face_center.z };
				}

				// 表裏チェック。一つでも裏があれば、この面では交差している位置関係にある
				if (Math::Dot(vec_near, normal) < 0)
				{
					break;
				}

				// ８頂点が全て表側にあれば、交差していないことが確定するためcollisionStateを変更してreturn
				if (j == 7)
				{
					frustum.collision_state = false;
					return frustum.collision_state;
				}
			}
		}
	}
	return frustum.collision_state;
}

void CameraController::DrawDebugGUI()
{
	Camera& camera = Camera::Instance();
	const Graphics& graphics = Graphics::Instance();
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
			DirectX::XMFLOAT3 a{};
			a.x = DirectX::XMConvertToDegrees(this->angle.x);
			a.y = DirectX::XMConvertToDegrees(this->angle.y);
			a.z = DirectX::XMConvertToDegrees(this->angle.z);
			ImGui::InputFloat3("Radian", &this->angle.x);
			ImGui::InputFloat3("Degree", &a.x);
			// 注視点
			ImGui::InputFloat3("target", &target.x);
		}
		DirectX::XMFLOAT4X4 view{};			// ビュー行列
		DirectX::XMFLOAT4X4 projection{};	// 投影行列

		DirectX::XMFLOAT3	eye{ 0,1,10 };		// カメラの位置
		DirectX::XMFLOAT3	focus{ 0,0,0 };	// カメラの方向
		DirectX::XMFLOAT3	up{ 0,1,0 };		// カメラの上方向、通常は<0.0f、1.0f、0.0f>
		DirectX::XMFLOAT3	front{};		// カメラの前方方向
		DirectX::XMFLOAT3	right{};		// カメラの右方向

		ImGui::Text("width:%f", camera.GetWidth());
		ImGui::Text("height:%f", camera.GetHeight());
		ImGui::Text("aspect:%f", camera.GetAspect());
		ImGui::Text("fovY:%f", camera.GetFov());
		ImGui::Text("nearZ:%f", camera.GetNear());
		ImGui::Text("farZ:%f", camera.GetFar());

		// 平行投影カメラ
		if (camera.GetOrthMode() == true)
		{
			ImGui::Checkbox("ViewMap", &view_map);
			if (view_map)
			{
				camera.SetWidth(graphics.GetScreenWidth() / 5);
				camera.SetHeight(graphics.GetScreenHeight() / 5);
			}
			else
			{
				camera.SetWidth(graphics.GetScreenWidth() / 30);
				camera.SetHeight(graphics.GetScreenHeight() / 30);
			}
		}
		else	//透視投影カメラ
		{
			// カメラの高さ
			ImGui::DragFloat("Height", &camera_height);
			ImGui::Checkbox("ViewMap", &view_map);
			if (view_map)
			{
				camera_height = 400.f;
			}
			else
			{
				camera_height = 50.f;
			}
		}
	}
	ImGui::End();
}