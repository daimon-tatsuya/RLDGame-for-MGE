#pragma once
//**********************************************************
//
//		SceneTitleクラス
//
//**********************************************************

<<<<<<< HEAD
#include "Liblary/Systems/Scene.h"
#include "Liblary/Systems/CameraController.h"
=======
#include "MyLiblary/Systems/Scene.h"
#include "MyLiblary/Systems/CameraController.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3

/// <summary>
/// タイトルシーン
/// </summary>
class SceneTitle
	: public Scene
{
private:

public:

	float	timer = 0.0f;
	int	counter = 0;
	char	title_name[TEXT_BUFFER_SIZE] = { 0 };
	char	push_text[TEXT_BUFFER_SIZE] = { 0 };
private:

public:

	SceneTitle() = default;

	~SceneTitle() override = default;

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