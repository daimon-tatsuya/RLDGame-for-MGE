#pragma once
//**********************************************************
//
//		Stageクラス
//
//**********************************************************
#include "Liblary/Systems/Object.h"

struct HitResult;

/// <summary>
/// ステージの基底クラス
/// </summary>
class Stage
	: public Object
{
private:
	int Identity = 0;
public:
private:
public:
	Stage() = default;

	Stage(const char* filename, DirectX::XMFLOAT3& pos, const int id)
	{
		SetModel(filename);
		SetPosition(pos);
		SetAngle({ 0,0,0 });
		SetScale({ 1,1,1 });
		SetId(id);
	}
	Stage(const char* filename, DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& scale, const int id)
	{
		SetModel(filename);
		SetPosition(pos);
		SetAngle({ 0,0,0 });
		SetScale(scale);
		SetId(id);
	}

	~Stage() override = default;

	// 更新処理
	void Update(float elapsed_time) override {}

	// 描画処理
	void Render(ID3D11DeviceContext* device_context, std::shared_ptr<Shader> shader) override
	{
		shader->Draw(device_context, this->GetModel());
	}

	// レイキャスト
	virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit) { return false; }

	//メッセージ受信処理
	bool OnMessage(const Telegram& telegram) override { return false; }

	// デバッグ用GUI描画
	void DrawDebugGUI()override {}
};
