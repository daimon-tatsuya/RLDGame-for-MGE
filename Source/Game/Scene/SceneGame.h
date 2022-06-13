#pragma once
//**********************************************************
//
//		SceneGameクラス
//
//**********************************************************
#include <memory>
#include "Engine/Systems/Scene.h"


//前方宣言
class Player;
class CameraController;

//class AxisAlignedBoundingBox;幅 高さ

/// <summary>
/// ゲームシーン
/// </summary>
class SceneGame :
	public Scene
{
private:

public:

	std::unique_ptr<CameraController> camera_controller = nullptr; //カメラ操作クラスのポインタ

	bool is_darking = true; //画面を暗くするときtrue
	bool is_next_floor = false;//次のフロアに進むならtrue
private:
	//階を移動する
	void GoingToTheNextFloor();

	//ダンジョンを削除する
	void ClearFloor();

	//次のダンジョンを生成、配置
	void CreateNextFloor();

public:

	SceneGame() = default;

	~SceneGame() override;

	void Initialize() override;

	void Finalize() override;

	void Update(float elapsed_time) override;

	void Render() override;

	/// <summary>
	/// メッセージ受信処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns>受信の有無</returns>
	bool OnMessage(const Telegram& telegram) override;

	//メタAIにターンの終了を伝える
	void SendMessaging(MESSAGE_TYPE msg)override;
};
