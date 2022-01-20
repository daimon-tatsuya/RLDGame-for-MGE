//**********************************************************
//
//		Math�N���X
//
//**********************************************************

#include <cstdlib>
#include "Engine/Systems/Math.h"
float Math::ClampFloat(const float check, const float min, const float max)
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

float Math::ClampAnyFloat(const float check, const float any, const float min, const float max)
{
	if (check < min)
	{
		return min;
	}
	else if (check > max)
	{
		return max;
	}

	return any;
}

float Math::StepAnyFloat(const float check, const float any, const float min, const float max, const bool revers)
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
		return any;
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
		return any * -1;
	}
	return any;
}
// ���`�⊮
float Math::LerpFloat(const float a, const float b, const float t)
{
	float calculated = a * (1.0f - t) + (b * t);
	return calculated;
}

float Math::RandomRangeFloat(const float min, const float max)
{
	// 0.0�`1.0�̊Ԃ܂ł̃����_���l
	float value = static_cast<float>(rand()) / RAND_MAX;

	float calculated = 0;


	// min�`max�܂ł̃����_���l�ɕϊ�
	calculated = min + (max - min) * value;
	return calculated;
}

int Math::RandomInt(const int max)
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

		//degree���}�C�i�X�̎����̗]����}�C�i�X�ɂȂ�
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

DirectX::XMFLOAT3 Math::Normalize(DirectX::XMFLOAT3 v1)
{
	DirectX::XMFLOAT3 norm = v1;
	if (const float len = sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z))
	{
		norm.x /= len;
		norm.y /= len;
		norm.z /= len;
	}
	return norm;
}

float Math::Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

DirectX::XMFLOAT3 Math::Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	DirectX::XMFLOAT3 cross = {};
	cross.x = v1.y * v2.z - v1.z * v2.y;
	cross.y = v1.z * v2.x - v1.x * v2.z;
	cross.z = v1.x * v2.y - v1.y * v2.x;
	return cross;
}
