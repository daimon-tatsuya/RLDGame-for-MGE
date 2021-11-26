#pragma once
//**********************************************************
//
//		Scenesクラス
//
//**********************************************************
#include <windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <memory>

#include "Engine/Systems/Math.h"

/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
public:

	Scene() {}
	virtual ~Scene() {}
	// 初期化
	virtual void Initialize() = 0;
	// 終了化
	virtual void Finalize() = 0;
	// 更新処理
	virtual void Update(float elapsedTime) = 0;
	// 描画処理
	virtual void Render() = 0;
	// 準備完了しているか
	bool IsReady() const { return ready; }
	// 準備完了設定
	void SetReady() { ready = true; }
private:

public:

private:
	bool	ready = false;
};
