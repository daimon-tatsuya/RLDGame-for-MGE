#pragma once
//**********************************************************
//
//		Faderクラス
//
//**********************************************************
#include <memory>
#include "Liblary/Systems/StateMachine.h"
class Sprite;

//フェーダーのステート
enum class FaderState:int
{
	//FadeInPlay,
	//FadeInEnd,
	//FadeOutPlay,
	//FadeOutEnd,
	FadeIn,
	FadeOut,
	StateEnd
};
class Fader
{
private:
	StateMachine<FaderState, void, const float>fade_state_machine;	// フェーダーの親ステート
	std::unique_ptr<Sprite> fade_texture;//暗転用テクスチャ
	float alpha {};//暗転用テクスチャの透明度
	int fade_speed;//フェードする速さ
	bool is_fade_in_end;
	bool is_fade_out_end;
public:

private:
public:
	Fader();
	~Fader() = default;

	void Update(float elapsed_time) ;

	void Render();
	 bool FadeIn(float fading_speed);
	 bool FadeOut(float fading_speed);



};

