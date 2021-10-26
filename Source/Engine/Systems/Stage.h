#pragma once

#include "Engine/Systems/Object.h"

#include "Engine/Systems/Collision.h"

// ステージ
class Stage : public Object
{
public:
	Stage() {}
	Stage(const char* filename, DirectX::XMFLOAT3& pos,int id)
	{
		SetModel(filename);
		SetPosition(pos);
		SetAngle({ 0,0,0 });
		SetScale({ 1,1,1 });
		SetId(id);
		//this->model = stModel(filename);
		//this->position = pos;
		//this->angle = { 0,0,0 };
		//this->scale = { 1,1,1 };
		//this->id = id;
	}
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Stage() {}

	// 更新処理
	virtual void Update(float elapsedTime) {};

	// 描画処理
	virtual void Render(ID3D11DeviceContext* dc, std::shared_ptr<Shader> shader){};

	// レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) { return false; };

	//メッセージ受信処理
	virtual bool OnMessage(const Telegram& msg) override { return false; };

	// デバッグ用GUI描画
	virtual void DrawDebugGUI()override {};
};
