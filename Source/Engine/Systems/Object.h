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
	//ToDo protectedである必要がないので変更する,あとカプセル化する(全てのクラス)
	DirectX::XMFLOAT3		position{};
	DirectX::XMFLOAT3		old_position{};
	DirectX::XMFLOAT3		angle{};// ラジアン角
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4	transform =
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	DirectX::XMFLOAT3		velocity{};

	std::shared_ptr<Model> model = nullptr;

	int				id = 0;
	float				radius = 0.5f;// 半径
	float				height = 2.0f;// 高さ

	bool				exists = false;//存在フラグ

public:

	float				invincible_timer = 0.0f;// 無敵時間

	float				gravity = -1.0f;
	float				gravity_cut_time = 0;// 重力を無視するときに使う
	float				air_control = 0.3f;
	bool				is_ground = false;// 地面判定用

	float				friction = 0.5f;
	float				acceleration = 1.0f;
	float				max_move_speed = 5.0f;
	float				move_vec_x = 0.0f;
	float				move_vec_z = 0.0f;

	float				step_offset = 1.0f;
	float				slope_rate = 0.0f;


private:
	/// <summary>
	/// 垂直速力更新処理
	/// </summary>
	/// <param name="elapsed_frame">経過時間</param>
	virtual void UpdateVerticalVelocity(float elapsed_frame);

	/// <summary>
	///  垂直移動更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	virtual  void UpdateVerticalMove(float elapsed_time);

	/// <summary>
	///  水平速力更新処理
	/// </summary>
	/// <param name="elapsed_frame">経過時間</param>
	virtual  void UpdateHorizontalVelocity(float elapsed_frame);

	/// <summary>
	///  水平移動更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	virtual  void UpdateHorizontalMove(float elapsed_time);

	/// <summary>
	///  速力更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void UpdateVelocity(float elapsed_time);

	/// <summary>
	/// 無敵時間更新
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void UpdateInvincibleTimer(float elapsed_time);


protected:
	/// <summary>
	/// 着地した時に呼ばれる
	/// </summary>
	virtual void OnLanding() {}

	/// <summary>
	/// ダメージを受けた時に呼ばれる
	/// </summary>
	virtual void OnDamaged() {}

	/// <summary>
	/// 死亡した時に呼ばれる
	/// </summary>
	virtual void OnDead() {}
public:

	Object() = default;

	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Object() = default;

	// 行列更新処理
	void UpdateTransform();
	// 更新処理
	virtual void Update(float elapsed_time) {}

	// 描画処理
	virtual void Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader) {}

	// デバッグ用GUI描画
	virtual	void DrawDebugGUI() {}

	// デバッグプリミティブ描画
	virtual	void DrawDebugPrimitive() {}

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& telegram) = 0;

	// 任意の回転角を0~360に正規化
	float NormalizeAnyAngle(float radian);

	// 回転角を0~360に正規化
	void NormalizeAngle();

	//メタAIメタAIにターンの終了を伝える
	virtual  void SendMessaging(MESSAGE_TYPE msg);
	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// 位置を取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 位置を設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 位置xを設定
	void SetPositionX(const float x) { this->position.x = x; }

	// 位置yを設定
	void SetPositionY(const float y) { this->position.y = y; }

	// 位置zを設定
	void SetPositionZ(const float z) { this->position.z = z; }

	//現在の位置から動かす
	void AddPosition(const DirectX::XMFLOAT3& position)
	{
		this->position.x += position.x;
		this->position.y += position.y;
		this->position.z += position.z;
	}
	//現在の位置からx方向動かす
	void AddPositionX(const float x)
	{
		this->position.x += x;
	}
	//現在の位置からy方向動かす
	void AddPositionY(const float y)
	{
		this->position.y += y;
	}
	//現在の位置からz方向動かす
	void AddPositionZ(const float z)
	{
		this->position.z += z;
	}

	// 位置を取得
	const DirectX::XMFLOAT3& GetOldPosition() const { return old_position; }

	// 位置を設定
	void SetOldPosition() { old_position = position; }
	void SetOldPositionX() { old_position.x = position.x; }
	void SetOldPositionY() { old_position.y = position.y; }
	void SetOldPositionZ() { old_position.z = position.z; }

	// 位置を取得
	const DirectX::XMFLOAT3& GetVelocity() const { return velocity; }

	// 位置を設定
	void SetVelocity(const DirectX::XMFLOAT3& velocity) { this->velocity = velocity; }

	// 位置xを設定
	void SetVelocityX(const float x) { this->velocity.x = x; }

	// 位置yを設定
	void SetVelocityY(const float y) { this->velocity.y = y; }

	// 位置zを設定
	void SetVelocityZ(const float z) { this->velocity.z = z; }

	//現在の位置から動かす
	void AddVelocity(const DirectX::XMFLOAT3& velocity)
	{
		this->velocity.x += velocity.x;
		this->velocity.y += velocity.y;
		this->velocity.z += velocity.z;
	}
	//現在の位置からx方向動かす
	void AddVelocityX(const float x)
	{
		this->velocity.x += x;
	}
	//現在の位置からy方向動かす
	void AddVelocityY(const float y)
	{
		this->velocity.y += y;
	}
	//現在の位置からz方向動かす
	void AddVelocityZ(const float z)
	{
		this->velocity.z += z;
	}

	// 回転角度を取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// 回転角度を設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	void SetAngleX(const float x) { this->angle.x = x; }
	void SetAngleY(const float y) { this->angle.y = y; }
	void SetAngleZ(const float z) { this->angle.z = z; }

	//現在の角度を回転する
	void AddAngle(const DirectX::XMFLOAT3& angle)
	{
		this->angle.x += angle.x;
		this->angle.y += angle.y;
		this->angle.z += angle.z;
	}
	//現在の角度をx軸回転する
	void AddAngleX(const float x)
	{
		this->angle.x += x;
	}

	//現在の角度をy軸回転する
	void AddAngleY(const float y)
	{
		this->angle.y += y;
	}

	//現在の角度をz軸回転する
	void AddAngleZ(const float z)
	{
		this->angle.z += z;
	}

	// スケールを取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// スケールを設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
	void SetScaleX(const float x) { this->scale.x = x; }
	void SetScaleY(const float y) { this->scale.y = y; }
	void SetScaleZ(const float z) { this->scale.z = z; }

	//現在の角度を回転する
	void AddScale(const DirectX::XMFLOAT3& scale)
	{
		this->scale.x += scale.x;
		this->scale.y += scale.y;
		this->scale.z += scale.z;
	}

	//characterごとにidをセットして管理する
	void	SetId(const int id) { this->id = id; }
	int	GetId() const { return id; }

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

	//存在フラグの取得
	bool GetExist()const { return  exists; }

	//存在フラグの設定
	void SetExist(bool set) { exists = set; }

};
