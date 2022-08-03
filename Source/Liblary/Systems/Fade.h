#pragma once
//**********************************************************
//
//		Fader�N���X
//
//**********************************************************
#include <memory>
#include "Liblary/Systems/StateMachine.h"
class Sprite;

//�t�F�[�_�[�̃X�e�[�g
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
	StateMachine<FaderState, void, const float>fade_state_machine;	// �t�F�[�_�[�̐e�X�e�[�g
	std::unique_ptr<Sprite> fade_texture;//�Ó]�p�e�N�X�`��
	float alpha {};//�Ó]�p�e�N�X�`���̓����x
	int fade_speed;//�t�F�[�h���鑬��
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

