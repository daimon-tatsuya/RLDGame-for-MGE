#pragma once
#include <DirectXMath.h>
//Todo DirectXMath.h��include���Ă���t�@�C���͑S��Math.h�ɒu��������

// �Z�pclass
class Math
{
public:

	/// <summary>
	/// min����max�܂ł̃N�����v
	/// n��max�ȏ�Ȃ�max,min�����Ȃ�min,���̑���n
	/// </summary>
	/// <param name="n"></param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>n,min,max�̉��ꂩ</returns>
	static float ClampFloat(float n, float min, float max);

	/// <summary>
	/// a��t�̑傫����b�����ɕ⊮����(���`�⊮)
	/// a * (1 - t) + (b * t)
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static float	LerpFloat(float a, float b, float t);

	/// <summary>
	/// �w��͈͂̃����_���l���v�Z����
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns></returns>
	static float RandomRangeFloat(float min, float max);


	/// <summary>
	/// 0~max�܂ł̃����_���l���v�Z����
	/// </summary>
	/// <param name="max">�ő�l</param>
	/// <returns></returns>
	static int RandomInt(int max);

	/// <summary>
	/// 0~max�܂ł̃����_���l���v�Z����
	/// </summary>
	/// <param name="max">�ő�l</param>
	/// <returns></returns>
	static int RandomRangeInt(int min,int max);

	/// <summary>
	/// degree�p���O����360�Ő��K������
	/// </summary>
	/// <param name="degree">degree�p</param>
	/// <returns></returns>
	static float NormalizeDegreeAngle(float degree);

	/// <summary>
	/// radian�p��0����2�΂Ő��K������
	/// </summary>
	/// <param name="radian">radian�p</param>
	/// <returns></returns>
	static float NormalizeRadianAngle(float radian);
};