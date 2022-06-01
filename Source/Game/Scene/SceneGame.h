#pragma once
//**********************************************************
//
//		SceneGameクラス
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"
#include "Engine/AI/DungeonMake.h"

//前方宣言
class Player;
class CameraController;
//class AxisAlignedBoundingBox;

/// <summary>
/// ゲームシーン
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	std::unique_ptr<CameraController> camera_controller = nullptr;

private:
	void ClearFloor();

	void NextFloor();
public:

	SceneGame() = default;

	~SceneGame() override;

	void Initialize()override;

	void Finalize() override;

	void Update(float elapsed_time)override;

	void Render()override;

	/// <summary>
	/// メッセージ受信処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns>受信の有無</returns>
	bool OnMessage(const Telegram& telegram) override;

	//メタAIにターンの終了を伝える
	void SendMessaging(MESSAGE_TYPE msg)override;
};
