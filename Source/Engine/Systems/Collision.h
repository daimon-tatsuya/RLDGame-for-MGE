#pragma once

#include "Engine/Systems/Math.h"

#include "Engine/Objects/Model.h"


// Collision コメント

//ヒット結果
struct HitResult
{
	DirectX::XMFLOAT3		position = { 0, 0, 0 };	// レイとポリゴンの交点
	DirectX::XMFLOAT3		normal = { 0, 0, 0 };	// 衝突したポリゴンの法線ベクトル
	DirectX::XMFLOAT3		rotation = { 0, 0, 0 };	// 回転量
	float								distance = 0.0f; 		// レイの始点から交点までの距離
	int									material_index = -1; 	// 衝突したポリゴンのマテリアル番号
};

class Collision
{
public:

	/// <summary>
	/// 球と球の交差判定
	/// </summary>
	/// <param name="positionA">SphereAの位置</param>
	/// <param name="radiusA">SphereAの半径</param>
	/// <param name="positionB">SphereBの位置</param>
	/// <param name="radiusB">SphereBの半径</param>
	/// <param name="outPositionA">判定後のAの位置</param>
	/// <param name="outPositionB">判定後のBの位置</param>
	/// <returns>当たっているならtrue</returns>
	static bool IntersectSphereToSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionA,
		DirectX::XMFLOAT3& outPositionB
	);

	/// <summary>
	/// レイとモデルの交差判定
	/// </summary>
	/// <param name="start">レイの始点</param>
	/// <param name="end">レイの終点</param>
	/// <param name="model">判定するmodel</param>
	/// <param name="result">ヒット結果</param>
	/// <returns>当たっているならtrue</returns>
	static bool IntersectRayToModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

	// カプセルとカプセルの交差判定


	// カプセルと球の交差判定


	// AABB

};
