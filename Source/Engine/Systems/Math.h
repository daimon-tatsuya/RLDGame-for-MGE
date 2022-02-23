#pragma once
//**********************************************************
//
//		Math�N���X
//
//**********************************************************
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

//! XMVECTOR�ō���������

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
	/// <returns>���K�����ꂽv1</returns>
	static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v1);

	/// <summary>
	/// ����(v1�Ev2)
	/// </summary>
	/// <param name="v1">	���ς���x�N�g��</param>
	/// <param name="v2">	���ς���x�N�g��</param>
	/// <returns>v1��v2�̓���</returns>
	static float Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	/// <summary>
	/// �O��(v1�~v2)
	/// </summary>
	/// <param name="v1">	���ς���x�N�g��</param>
	/// <param name="v2">	���ς���x�N�g��</param>
	/// <returns>v1����v2�ւ̊O��</returns>
	static   DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);


	/// <summary>
	///	�ʒu�x�N�g���̒���
	/// </summary>
	/// <returns>����</returns>
	static  float Length(DirectX::XMFLOAT3 v);

	/// <summary>
	///	�ʒu�x�N�g���̒���
	/// </summary>
	/// <returns>����</returns>
	static  float Length(DirectX::XMINT3 v);

	/// <summary>
	///	v1-v2�̃x�N�g���̌v�Z
	/// </summary>
	/// <param name="v1">(0,0,0)����̈ʒu�x�N�g��v1</param>
	/// <param name="v2">(0,0,0)����̈ʒu�x�N�g��v2</param>
	/// <returns>v1-v2�̃x�N�g��</returns>
	static  DirectX::XMFLOAT3 SubtractVector(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	/// <summary>
	///	v1+v2�̃x�N�g���̌v�Z
	/// </summary>
	///	<param name="v1">(0,0,0)����̈ʒu�x�N�g��v1</param>
	///	<param name="v2">(0,0,0)����̈ʒu�x�N�g��v2</param>
	/// <returns>v1+v2�̃x�N�g��</returns>
	static  DirectX::XMFLOAT3 AddVector(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	static  int GetOneDimensionalArrayIndex(DirectX::XMINT2);
};
//Aster�̌��ʂ�2�����z��ɕϊ�����
//void HeuristicSearch::ConvertTwoDimensionalArray()
//{
//	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
//	{
//		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
//		{
//			//   x ��
//			// y		MapSize_Y*0----------------------MapSize_X*1-1
//			//��		MapSize_Y*1----------------------MapSize_X*2-1
//			//			MapSize_Y*2----------------------MapSize_X*3-1
//			//			MapSize_Y*3----------------------MapSize_X*4-1
//			//			MapSize_Y*4----------------------MapSize_X*5-1
//			//			MapSize_Y*5----------------------MapSize_X*6-1
//			//~~~~~~~~~~~~~~~~~
//			//			MapSize_Y*  ----------------------MapSize_Y*
//			//		  (MapSize_X - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//�i�s���[�g��񎟌��z��Ɋi�[
//			advance_vector[y][x] = advance[array_num];
//		}
//	}
//}
//�}�b�v����1�����z��ɕϊ�����
//void HeuristicSearch::ConvertOneDimensionalArray(const RogueLikeDungeon& rogue_like_dungeon)
//{
//	for (int y = 0; y < static_cast<int>(MapSize_Y) - 1; y++)
//	{
//		for (int x = 0; x < static_cast<int>(MapSize_X) - 1; x++)
//		{
//			//   x ��
//			// y		MapSize_Y*0----------------------MapSize_X*1-1
//			//��		MapSize_Y*1----------------------MapSize_X*2-1
//			//			MapSize_Y*2----------------------MapSize_X*3-1
//			//			MapSize_Y*3----------------------MapSize_X*4-1
//			//			MapSize_Y*4----------------------MapSize_X*5-1
//			//			MapSize_Y*5----------------------MapSize_X*6-1
//			//~~~~~~~~~~~~~~~~~
//			//			MapSize_Y*  ----------------------MapSize_Y*
//			//		(MapSize_X - 1)							MapSize_X - 1
//			const int array_num = y*MapSize_Y + x;
//			//�}�b�v�����ꎟ���z��Ɋi�[
//			map_info[array_num] = static_cast<int>(rogue_like_dungeon.map_role[y][x].map_data);
//		}
//	}
//}