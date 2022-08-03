//**********************************************************
//
//		Faderクラス
//
//**********************************************************

#include <d3d11.h>
#include "Liblary/Systems/Fade.h"
#include "Liblary/Objects/Sprite.h"
#include "Liblary/Systems/Graphics.h"
Fader::Fader(): alpha(1.f), fade_speed(0), is_fade_in_end(false), is_fade_out_end(false)
{
	//階移動時のブラックアウト用テクスチャ
	fade_texture = std::make_unique<Sprite>(L"Assets/Images/Black.jpg");
}

void Fader::Update(float elapsed_time)
{
	//	ステートマシンの更新
	fade_state_machine.Update(elapsed_time);
}

void Fader::Render()
{
	const Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* device_context = graphics.GetDeviceContext();
	//階移動時にブラックアウトさせる用のテクスチャの描画
	fade_texture->Render(device_context, 0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(), 0, 0,
		fade_texture->GetTextureWidth(), fade_texture->GetTextureHeight(), 0, 1.f, 1.f, 1.f, alpha);
}

//--------------------------------------------------
// フェードイン
//--------------------------------------------------
bool Fader::FadeIn(const float fading_speed)
{

	// 完全に表示されたらtrue
	if (alpha > 1.f)
	{return true;}
	// 透明度を加算
	alpha += fading_speed;

	return false;
}

//--------------------------------------------------
// フェードアウト
//--------------------------------------------------
bool Fader::FadeOut(const float fading_speed)
{

	// 完全に非表示になったらtrue
	if (alpha <= 0)
	{return true;}
	// 透明度を減算
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
//		// フェードイン開始
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
//		// フェードアウト開始
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
//	// ブレンドモード
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
//
//	// 画面を覆う四角形を描画
//	DrawBox(0, 0, 640, 480, fade_color, true);
//
//	// ブレンドモード終了
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//}