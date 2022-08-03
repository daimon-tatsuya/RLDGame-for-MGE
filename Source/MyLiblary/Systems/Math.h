#pragma once
//**********************************************************
//
//		Mathクラス
//
//**********************************************************
#include <d3d11.h>
#include <DirectXMath.h>
#include <SimpleMath.h>

//! XMVECTORで高速化する

/// <summary>
/// 算術クラス
/// </summary>
class Math final
{
public:

	/// <summary>
	/// <para>minからmaxまでのクランプ</para>
	/// <para>nがmax以上ならmax,min未満ならmin,その他はn</para>
	/// </summary>
	/// <param name="check"></param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>n,min,maxのいずれか</returns>
	static float ClampFloat(float check, float min, float max);

	/// <summary>
	/// <para>minからmaxまでのクランプ</para>
	/// <para>nがmax以上ならmax,min未満ならmin,その他はAny</para>
	/// </summary>
	/// <param name="check"></param>
	/// <param name="any"></param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>Any,min,maxのいずれか</returns>
	static float ClampAnyFloat(float check, float any, float min, float max);

	/// <summary>
	/// <para>minからmaxまでのステップ</para>
	/// <para>nがmax以上なら1,min未満なら0,その他はAny</para>
	/// <para>0~1の範囲のものにつかう</para>
	/// <para>主にスティック</para>
	/// </summary>
	/// <param name="check"></param>
	/// <param name="any"></param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <param name="revers">trueの時-1~0の範囲にする</param>
	/// <returns>Any,0,1のいずれか</returns>
	static float StepAnyFloat(float check, float any, float min, float max, bool revers = false);

	/// <summary>
	/// <para>aをtの大きさでb方向に補完する(線形補完)</para>
	/// <para>a * (1 - t) + (b * t)</para>
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	static float	LerpFloat(float a, float b, float t);

	/// <summary>
	/// 指定範囲のランダム値を計算する
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns></returns>
	static float RandomRangeFloat(float min, float max);

	/// <summary>
	/// 0~maxまでのランダム値を計算する
	/// </summary>
	/// <param name="max">最大値</param>
	/// <returns></returns>
	static int RandomInt(int max);

	/// <summary>
	/// 0~maxまでのランダム値を計算する
	/// </summary>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns></returns>
	static int RandomRangeInt(int min, int max);

	/// <summary>
	/// degree角を０から360で正規化する
	/// </summary>
	/// <param name="degree">degree角</param>
	/// <returns></returns>
	static float NormalizeDegreeAngle(float degree);

	/// <summary>
	/// radian角を0から2πで正規化する
	/// </summary>
	/// <param name="radian">radian角</param>
	/// <returns></returns>
	static float NormalizeRadianAngle(float radian);

	/// <summary>
	/// degree角をradian角にコンバート
	/// </summary>
	/// <param name="degree">degree角</param>
	/// <returns></returns>
	static float ConvertToRadianAngle(float degree);

	/// <summary>
	/// radian角をdegree角にコンバート
	/// </summary>
	/// <param name="radian">radian角</param>
	/// <returns></returns>
	static float ConvertToDegreeAngle(float radian);

	/// <summary>
	///	誤差を考慮した比較
	///	xとyの差の絶対値がDBL_EPSILON以下ならほぼ同じ値
	/// </summary>
	///	<param name="x"></param>
	///	<param name="y"></param>
	/// <returns>trueならほぼ同じ</returns>
	static bool Comparison(double x, double y);

	/// <summary>
	/// 正規化
	/// </summary>
	/// <param name="v1">	正規化するベクトル</param>
	/// <returns>正規化されたv1</returns>
	static DirectX::XMFLOAT3 Normalize(DirectX::XMFLOAT3 v1);

	/// <summary>
	/// 内積(v1・v2)
	/// </summary>
	/// <param name="v1">	内積するベクトル</param>
	/// <param name="v2">	内積するベクトル</param>
	/// <returns>v1とv2の内積</returns>
	static float Dot(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	/// <summary>
	/// 外積(v1×v2)
	/// </summary>
	/// <param name="v1">	内積するベクトル</param>
	/// <param name="v2">	内積するベクトル</param>
	/// <returns>v1からv2への外積</returns>
	static   DirectX::XMFLOAT3 Cross(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	/// <summary>
	///	位置ベクトルの長さ
	/// </summary>
	/// <returns>長さ</returns>
	static  float Length(DirectX::XMFLOAT3 v);

	/// <summary>
	///	位置ベクトルの長さ
	/// </summary>
	/// <returns>長さ</returns>
	static  float Length(DirectX::XMINT3 v);

	/// <summary>
	///	v1-v2のベクトルの計算
	/// </summary>
	/// <param name="v1">(0,0,0)からの位置ベクトルv1</param>
	/// <param name="v2">(0,0,0)からの位置ベクトルv2</param>
	/// <returns>v1-v2のベクトル</returns>
	static  DirectX::XMFLOAT3 SubtractVector(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

	/// <summary>
	///	v1+v2のベクトルの計算
	/// </summary>
	///	<param name="v1">(0,0,0)からの位置ベクトルv1</param>
	///	<param name="v2">(0,0,0)からの位置ベクトルv2</param>
	/// <returns>v1+v2のベクトル</returns>
	static  DirectX::XMFLOAT3 AddVector(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);



};
