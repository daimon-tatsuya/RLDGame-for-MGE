#pragma once
#include <DirectXMath.h>
//Todo DirectXMath.hをincludeしているファイルは全てMath.hに置き換える

// 算術class
class Math
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
	/// <param name="Any"></param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>Any,min,maxのいずれか</returns>
	static float ClampAnyFloat(float check, float Any, float min, float max);


	/// <summary>
/// <para>minからmaxまでのステップ</para>
/// <para>nがmax以上なら1,min未満なら0,その他はAny</para>
/// <para>0~1の範囲のものにつかう</para>
/// <para>主にスティック</para>
/// </summary>
/// <param name="check"></param>
/// <param name="Any"></param>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <param name="revers">trueの時-1~0の範囲にする</param>
/// <returns>Any,0,1のいずれか</returns>
	static float StepAnyFloat(float check, float Any, float min, float max, bool revers = false);

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
	/// 浮動小数点数の誤差を考慮した比較
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static bool BoolianFloat(float a, float b);
};