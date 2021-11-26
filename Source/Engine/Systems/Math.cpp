#include <stdlib.h>
#include "Engine/Systems/Math.h"
//**********************************************************
//
//		Mathクラス
//
//**********************************************************

float Math::ClampFloat(float check, float min, float max)
{
	if (check < min)
	{
		return min;
	}

	if (check > max)
	{
		return max;
	}

	return check;
}

float Math::ClampAnyFloat(float check, float Any, float min, float max)
{
	if (check < min)
	{
		return min;
	}
	else if (check > max)
	{
		return max;
	}

	return Any;
}

float Math::StepAnyFloat(float check, float Any, float min, float max, bool revers)
{
	if (revers == false)
	{
		if (check < min)
		{
			return 0.f;
		}
		else if (check > max)
		{
			return 1.f;
		}
		return Any;
	}
	else
	{
		if (check < min)
		{
			return -1.f;
		}
		else if (check > max)
		{
			return 0.f;
		}
		return Any * -1;
	}
	return Any;
}
// 線形補完
float Math::LerpFloat(float a, float b, float t)
{
	float calculated = a * (1.0f - t) + (b * t);
	return calculated;
}

float Math::RandomRangeFloat(float min, float max)
{
	// 0.0～1.0の間までのランダム値
	float value = static_cast<float>(rand()) / RAND_MAX;

	float calculated = 0;


	// min～maxまでのランダム値に変換
	calculated = min + (max - min) * value;
	return calculated;
}

int Math::RandomInt(int max)
{

	int calculated = 0;
	if (max == 0)
	{
		return calculated;
	}

	calculated = rand() % max;
	return calculated;
}

int Math::RandomRangeInt(int min, int max)
{
	int calculated = 0;
	if (max == 0)
	{
		return calculated;
	}

	calculated = min + rand() % (max - min);
	return calculated;
}

float Math::NormalizeDegreeAngle(float degree)
{
	int deg = 0;
	if (degree < 0.0f || degree >= 360.f)
	{
		int deg = static_cast<int>(degree) % 360;

		//degreeがマイナスの時その余りもマイナスになる
		if (deg < 0)
		{
			deg += 360;
		}
	}
	return static_cast<float>(deg);
}

float Math::NormalizeRadianAngle(float radian)
{
	if (radian < 0.0f || radian >= DirectX::XM_2PI)
	{
		int div = static_cast<int>(radian / DirectX::XM_2PI);
		radian -= (div * DirectX::XM_2PI);

		if (radian < 0.0f)
			radian = DirectX::XM_2PI + (radian);
	}

	return radian;
}

float Math::ConvertToRadianAngle(float degree)
{
	return DirectX::XMConvertToRadians(degree);
}

float Math::ConvertToDegreeAngle(float radian)
{
	return  DirectX::XMConvertToDegrees(radian);
}

bool Math::BoolianFloat(float a, float b)
{
	if (fabsf(a - b) <= FLT_EPSILON * fmaxf(1.f, fmaxf(fabsf(a), fabsf(b))))
	{
		return true;
	}
	else
	{
		return false;
	}
}
