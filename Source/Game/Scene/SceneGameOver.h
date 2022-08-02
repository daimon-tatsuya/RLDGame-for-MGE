#pragma once
//**********************************************************
//
//		SceneGameOverクラス
//
//**********************************************************
#include "MyLiblary/Systems/Scene.h"

//	前方宣言
class Sprite;

///<summary>
/// ゲームオーバーシーン
///</summary>
class SceneGameOver :public  Scene
{
private:

public:
	bool is_game_clear = true;
	char	over_text[TEXT_BUFFER_SIZE] = { 0 };//GameOver か GameClearのテキスト
	char	turn_number_text[TEXT_BUFFER_SIZE] = { 0 };//ゲーム終了時の経過ターンのテキスト
	char	push_text[TEXT_BUFFER_SIZE] = { 0 };
private:

public:
	//	コンストラクタ
	SceneGameOver()=default;

	//デストラクタ
	~SceneGameOver() override;

	//初期化
	void Initialize() override;

	//終了化
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
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




