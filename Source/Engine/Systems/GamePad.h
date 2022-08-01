#pragma once
//**********************************************************
//
//	GamePadクラス
//
//**********************************************************

using GamePadButton = unsigned int;

/// <summary>
/// <para>ゲームパッドの入力を検知する。 </para>
/// <para>PCコントローラー、キーボード </para>
/// </summary>
class GamePad final
{
private:

	GamePadButton		button_state[2] = { 0 };
	GamePadButton		button_down = 0;
	GamePadButton		button_up = 0;

	float				axisLX = 0.0f;
	float				axisLY = 0.0f;
	float				axisRX = 0.0f;
	float				axisRY = 0.0f;
	float				triggerL = 0.0f;
	float				triggerR = 0.0f;
	int				slot = 0;

public:

	const static  GamePadButton BTN_UP = (1 << 0);        // 十字キー上
	const static  GamePadButton BTN_RIGHT = (1 << 1);  // 十字キー右
	const static  GamePadButton BTN_DOWN = (1 << 2);	 // 十字キー下
	const static  GamePadButton BTN_LEFT = (1 << 3);	 // 十字キー左
	const static  GamePadButton BTN_A = (1 << 4);//Aボタン
	const static  GamePadButton BTN_B = (1 << 5);//Bボタン
	const static  GamePadButton BTN_X = (1 << 6);//Xボタン
	const static  GamePadButton BTN_Y = (1 << 7);//Yボタン
	const static  GamePadButton BTN_START = (1 << 8);//右スタートボタン
	const static  GamePadButton BTN_BACK = (1 << 9);  //左スタートボタン
	const static  GamePadButton BTN_LEFT_THUMB = (1 << 10);		// 左スティック押し込み
	const static  GamePadButton BTN_RIGHT_THUMB = (1 << 11); // 右スティック押し込み
	const static  GamePadButton BTN_LEFT_SHOULDER = (1 << 12);		// LB：L1
	const static  GamePadButton BTN_RIGHT_SHOULDER = (1 << 13);	// RB：R1
	const static  GamePadButton BTN_LEFT_TRIGGER = (1 << 14);			// LT：L2
	const static  GamePadButton BTN_RIGHT_TRIGGER = (1 << 15);		// RT：R2

	const static  GamePadButton AnyBTN =// ABXYボタン
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y;

	static const GamePadButton AnyDPad =// 十字キー
		GamePad::BTN_UP
		| GamePad::BTN_RIGHT
		| GamePad::BTN_DOWN
		| GamePad::BTN_LEFT;

private:

public:

	GamePad() = default;
	~GamePad() = default;

	// 更新
	void Update();

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// スロット設定
	void SetSlot(int slot) { this->slot = slot; }

	// ボタン入力状態の取得
	GamePadButton GetButton() const { return button_state[0]; }

	// ボタン押下状態の取得
	GamePadButton GetButtonDown() const { return button_down; }

	// ボタン押上状態の取得
	GamePadButton GetButtonUp() const { return button_up; }

	// 左スティックX軸入力状態の取得
	float GetAxisLX() const { return axisLX; }

	// 左スティックY軸入力状態の取得
	float GetAxisLY() const { return axisLY; }

	// 右スティックX軸入力状態の取得
	float GetAxisRX() const { return axisRX; }

	// 右スティックY軸入力状態の取得
	float GetAxisRY() const { return axisRY; }

	// 左トリガー入力状態の取得
	float GetTriggerL() const { return triggerL; }

	// 右トリガー入力状態の取得
	float GetTriggerR() const { return triggerR; }
};
