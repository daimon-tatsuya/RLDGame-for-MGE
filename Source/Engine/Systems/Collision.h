#pragma once

#include "Engine/Systems/Math.h"

#include "Engine/Objects/Model.h"


//ToDo Collision コメント

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
	// 球と球の交差判定
	static bool IntersectSphereToSphere(
		const DirectX::XMFLOAT3& positionA,
		float radiusA,
		const DirectX::XMFLOAT3& positionB,
		float radiusB,
		DirectX::XMFLOAT3& outPositionA,
		DirectX::XMFLOAT3& outPositionB
	);

	// レイとモデルの交差判定
	static bool IntersectRayToModel(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const Model* model,
		HitResult& result
	);

	//Todo カプセルとカプセルの交差判定


	//Todo カプセルと球の交差判定


	//Todo AABB

};
