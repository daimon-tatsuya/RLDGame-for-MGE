#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Engine/Systems/Shader.h"
#include "Engine/AI/Telegram.h"
#include "Engine/Objects/Model.h"
//Todo コメント
class Object
{
private:

protected:

public:

	Object() {}
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Object() {}

	// 行列更新処理
	void UpdateTransform();
	// 更新処理
	virtual void Update(float elapsed_time) = 0;

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader) = 0;

	// デバッグ用GUI描画
	virtual	void DrawDebugGUI() {}

	// デバッグプリミティブ描画
	virtual	void DrawDebugPrimitive() {}

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) = 0;

	// 位置を取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 位置を設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 回転角度を取得
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	// 回転角度を設定
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	// スケールを取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// スケールを設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	//characterごとにidをセットして管理する
	void	SetId(int id) { this->id = id; }
	int		GetId() { return id; }

	//モデルを取得
	void	SetModel(const char* pass) { model = std::make_shared<Model>(pass); }

	//モデルを設定
	Model* GetModel() { return model.get(); }

	//姿勢行列を取得
	DirectX::XMFLOAT4X4 GetTransform() { return transform; }

	//姿勢行列を設定
	void SetTransform(DirectX::XMFLOAT4X4& transform) { this->transform = transform; }

	// 半径を取得
	float GetRadius() const { return radius; }

	// 半径を設定
	void SetRadius(const float radius) { this->radius = radius; }

	// 高さを取得
	float GetHeight()const { return height; };

	// 高さを設定
	void SetHeight(float height) { this->height = height; };
private:

protected:
	DirectX::XMFLOAT3		position = { 0, 0, 0 };
	DirectX::XMFLOAT3		angle = { 0, 0, 0 };
	DirectX::XMFLOAT3		scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4	transform =
	{ 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	int					id = 0;
	float				radius = 0.5f;
	float				height = 2.0f;
	std::shared_ptr<Model> model = nullptr;
//	Model*			model = nullptr;
public:
};
