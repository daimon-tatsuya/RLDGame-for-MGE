#pragma once
//**********************************************************
//
//		Math�N���X
//
//**********************************************************
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

//#include <SimpleMath.h>
//#include<SimpleMath.inl>

/// <summary>
/// �Z�p�N���X
/// </summary>
class Math
{
public:

	/// <summary>
	/// <para>min����max�܂ł̃N�����v</para>
	/// <para>n��max�ȏ�Ȃ�max,min�����Ȃ�min,���̑���n</para>
	/// </summary>
	/// <param name="check"></param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>n,min,max�̂����ꂩ</returns>
	static float ClampFloat(float check, float min, float max);

	/// <summary>
	/// <para>min����max�܂ł̃N�����v</para>
	/// <para>n��max�ȏ�Ȃ�max,min�����Ȃ�min,���̑���Any</para>
	/// </summary>
	/// <param name="check"></param>
	/// <param name="any"></param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>Any,min,max�̂����ꂩ</returns>
	static float ClampAnyFloat(float check, float any, float min, float max);


	/// <summary>
	/// <para>min����max�܂ł̃X�e�b�v</para>
	/// <para>n��max�ȏ�Ȃ�1,min�����Ȃ�0,���̑���Any</para>
	/// <para>0~1�͈̔͂̂��̂ɂ���</para>
	/// <para>��ɃX�e�B�b�N</para>
	/// </summary>
	/// <param name="check"></param>
	/// <param name="any"></param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <param name="revers">true�̎�-1~0�͈̔͂ɂ���</param>
	/// <returns>Any,0,1�̂����ꂩ</returns>
	static float StepAnyFloat(float check, float any, float min, float max, bool revers = false);

	/// <summary>
	/// <para>a��t�̑傫����b�����ɕ⊮����(���`�⊮)</para>
	/// <para>a * (1 - t) + (b * t)</para>
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
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns></returns>
	static int RandomRangeInt(int min, int max);

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

	/// <summary>
	/// degree�p��radian�p�ɃR���o�[�g
	/// </summary>
	/// <param name="degree">degree�p</param>
	/// <returns></returns>
	static float ConvertToRadianAngle(float degree);

	/// <summary>
	/// radian�p��degree�p�ɃR���o�[�g
	/// </summary>
	/// <param name="radian">radian�p</param>
	/// <returns></returns>
	static float ConvertToDegreeAngle(float radian);


	/// <summary>
	/// ���������_���̌덷���l��������r
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns>a��b</returns>
	static bool BoolianFloat(float a, float b);

	/// <summary>
	/// ���K��
	/// </summary>
	/// <param name="v1">	���K������x�N�g��</param>
	/// <returns>a��b</returns>
	static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v1);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="v1">	���ς���x�N�g��</param>
	/// <param name="v2">	���ς���x�N�g��</param>
	static float Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	/// <summary>
	/// �O��
	/// </summary>
	/// <param name="v1">	���ς���x�N�g��</param>
	/// <param name="v2">	���ς���x�N�g��</param>
	static   DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
};