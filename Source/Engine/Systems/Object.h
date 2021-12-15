#pragma once
//**********************************************************
//
//		Objectクラス
//
//**********************************************************

#include <memory>
#include "Engine/Systems/Math.h"
#include "Engine/AI/Telegram.h"

//前方宣言
class Model;
class Shader;

/// <summary>
/// ゲーム中のオブジェクトの基底クラス
/// </summary>
class Object
{
private:

protected:
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		angle = { 0, 0, 0 };// ラジアン角
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4	transform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	DirectX::XMFLOAT3		velocity = { 0, 0, 0 };
	std::shared_ptr<Model> model = nullptr;
	int					id = 0;
	float				radius = 0.5f;// 半径
	float				height = 2.0f;// 高さ

public:
	float				gravity = -1.0f;
	float				gravity_cut_time = 0;// 重力を無視するときに使う
	bool				is_ground = false;// 地面判定用
	float				invincible_timer = 0.0f;// 無敵時間
	float				friction = 0.5f;
	float				acceleration = 1.0f;
	float				max_move_speed = 5.0f;
	float				move_vecX = 0.0f;
	float				move_vecZ = 0.0f;
	float				air_control = 0.3f;
	float				step_offset = 1.0f;
	float				slope_rate = 0.0f;
private:

protected:

public:

	Object() = default;

	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Object() = default;

	// 行列更新処理
	void UpdateTransform();
	// 更新処理
	virtual void Update(float elapsed_time) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader) = 0;

	// デバッグ用GUI描画
	virtual	void DrawDebugGUI() {}

	// デバッグプリミティブ描画
	virtual	void DrawDebugPrimitive() {}

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) = 0;

	// 任意の回転角を0~360に正規化
	float NormalizeAnyAngle(float radian);

	// 回転角を0~360に正規化
	void NormalizeAngle();

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

		// 位置を取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 位置を設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	void SetPositionX(const float x) { this->position.x = x; }
	void SetPositionY(const float y) { this->position.y = y; }
	void SetPositionZ(const float z) { this->position.z = z; }

	//現在の位置から動かす
	void AddPosition(const DirectX::XMFLOAT3& position)
	{
		this->position.x += position.x;
		this->position.y += position.y;
		this->position.z += position.z;
	}
	void AddPositionX(const float x) { this->position.x += x; }
	void AddPositionY(const float y) { this->position.y += y; }
	void AddPositionZ(const float z) { this->position.z += z; }

	// 回転角度を取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// 回転角度を設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	void SetAngleX(const float x) { this->angle.x = x; }
	void SetAngleY(const float y) { this->angle.y = y; }
	void SetAngleZ(const float z) { this->angle.z = z; }

	// スケールを取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// スケールを設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	void SetScaleX(const float x) { this->scale.x = x; }
	void SetScaleY(const float y) { this->scale.y = y; }
	void SetScaleZ(const float z) { this->scale.z = z; }

	//characterごとにidをセットして管理する
	void	SetId(const int id) { this->id = id; }
	int		GetId() const { return id; }

	//モデルを取得
	void	SetModel(const char* pass);

	//モデルを設定
	Model* GetModel() const { return model.get(); }

	//姿勢行列を取得
	DirectX::XMFLOAT4X4 GetTransform() const { return transform; }

	//姿勢行列を設定
	void SetTransform(const DirectX::XMFLOAT4X4& transform) { this->transform = transform; }

	// 半径を取得
	float GetRadius() const { return radius; }

	// 半径を設定
	void SetRadius(const float radius) { this->radius = radius; }

	// 高さを取得
	float GetHeight()const { return height; }

	// 高さを設定
	void SetHeight(const float height) { this->height = height; }

};
