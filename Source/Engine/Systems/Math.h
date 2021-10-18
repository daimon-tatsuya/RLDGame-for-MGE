#pragma once
#include <DirectXMath.h>
//Todo DirectXMath.hをincludeしているファイルは全てMath.hに置き換える

// 算術class
class Math
{
public:

	/// <summary>
	/// minからmaxまでのクランプ
	/// nがmax以上ならmax,min未満ならmin,その他はn
	/// </summary>
	/// <param name="n"></param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>n,min,maxの何れか</returns>
	static float ClampFloat(float n, float min, float max);

	/// <summary>
	/// aをtの大きさでb方向に補完する(線形補完)
	/// a * (1 - t) + (b * t)
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
	static int RandomRangeInt(int min,int max);

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
};