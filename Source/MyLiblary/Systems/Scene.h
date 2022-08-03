#pragma once
//**********************************************************
//
//		Scenesクラス
//
//**********************************************************
<<<<<<< HEAD:Source/Liblary/Systems/Scene.h
#include "Liblary/AI/Telegram.h"
=======
#include "MyLiblary/AI/Telegram.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Systems/Scene.h

const int TEXT_BUFFER_SIZE = 128;

/// <summary>
/// シーンの基底クラス
/// </summary>
class Scene
{
private:

	bool	ready = false;

public:

private:

public:

	Scene() = default;
	// 例えデストラクタが空でも
	// virtual なデストラクタは明示的に定義する
	virtual ~Scene() = default;

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

	/// <summary>
	/// メタAI経由受信処理
	/// </summary>
	/// <param name="telegram">命令</param>
	/// <returns>受信の有無</returns>
	virtual	bool OnMessage(const Telegram& telegram) = 0;
	//メタAIにターンの終了を伝える
	virtual	void SendMessaging(MESSAGE_TYPE msg) = 0;
};
