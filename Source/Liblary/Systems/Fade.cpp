//**********************************************************
//
//		Fader�N���X
//
//**********************************************************

#include <d3d11.h>
#include "Liblary/Systems/Fade.h"
#include "Liblary/Objects/Sprite.h"
#include "Liblary/Systems/Graphics.h"
Fader::Fader(): alpha(1.f), fade_speed(0), is_fade_in_end(false), is_fade_out_end(false)
{
	//�K�ړ����̃u���b�N�A�E�g�p�e�N�X�`��
	fade_texture = std::make_unique<Sprite>(L"Assets/Images/Black.jpg");
}

void Fader::Update(float elapsed_time)
{
	//	�X�e�[�g�}�V���̍X�V
	fade_state_machine.Update(elapsed_time);
}

void Fader::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	//�K�ړ����Ƀu���b�N�A�E�g������p�̃e�N�X�`���̕`��
	fade_texture->Render(device_context, 0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(), 0, 0,
		fade_texture->GetTextureWidth(), fade_texture->GetTextureHeight(), 0, 1.f, 1.f, 1.f, alpha);
}

//--------------------------------------------------
// �t�F�[�h�C��
//--------------------------------------------------
bool Fader::FadeIn(const float fading_speed)
{

	// ���S�ɕ\�����ꂽ��true
	if (alpha > 1.f)
	{return true;}
	// �����x�����Z
	alpha += fading_speed;

	return false;
}

//--------------------------------------------------
// �t�F�[�h�A�E�g
//--------------------------------------------------
bool Fader::FadeOut(const float fading_speed)
{

	// ���S�ɔ�\���ɂȂ�����true
	if (alpha <= 0)
	{return true;}
	// �����x�����Z
	alpha -= fading_speed;

	return false;
}
//void Fade::SetFadeIn(int fade_speed, int alpha, int r, int g, int b)
//{
//	if (fade_state != eFadeInPlay && fade_state != eFadeInEnd)
//	{
//		this->fade_speed = fade_speed;
//		this->alpha = alpha;
//		this->fade_color = GetColor(r, g, b);
//		fade_in_end = false;
//
//		// �t�F�[�h�C���J�n
//		fade_state = eFadeInPlay;
//	}
//}
//
//void Fade::SetFadeOut(int fade_speed, int alpha, int r, int g, int b)
//{
//	if (fade_state != eFadeOutPlay && fade_state != eFadeOutEnd)
//	{
//		this->fade_speed = fade_speed;
//		this->alpha = alpha;
//		this->fade_color = GetColor(r, g, b);
//		fade_out_end = false;
//
//		// �t�F�[�h�A�E�g�J�n
//		fade_state = eFadeOutPlay;
//	}
//}
//
//void Fade::Update()
//{
//	switch (fade_state)
//	{
//	case eNone:
//		break;
//	case eFadeInPlay:
//		if (alpha >= 0)
//		{
//			alpha -= fade_speed;
//		}
//		else
//		{
//			fade_state = eFadeInEnd;
//		}
//		break;
//	case eFadeInEnd:
//		fade_in_end = true;
//		break;
//	case eFadeOutPlay:
//		if (alpha <= 255)
//		{
//			alpha += fade_speed;
//		}
//		else
//		{
//			fade_state = eFadeOutEnd;
//		}
//		break;
//	case eFadeOutEnd:
//		fade_out_end = true;
//		break;
//	default:
//		break;
//	}
//}
//
//void Fade::Draw()
//{
//	// �u�����h���[�h
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
//
//	// ��ʂ𕢂��l�p�`��`��
//	DrawBox(0, 0, 640, 480, fade_color, true);
//
//	// �u�����h���[�h�I��
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//}