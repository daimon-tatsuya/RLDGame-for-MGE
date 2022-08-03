#pragma once
//**********************************************************
//
//		Modelクラス
//
//**********************************************************
#include <memory>
#include <vector>

<<<<<<< HEAD:Source/Liblary/Objects/Model.h
#include "Liblary/Systems/Math.h"
=======
#include "MyLiblary/Systems/Math.h"
>>>>>>> 391d7ac7691752defb0d457a531fb1c9072387b3:Source/MyLiblary/Objects/Model.h

class ModelResource;

/// <summary>
/// モデルのアニメーションなどを行うクラス
/// </summary>
class Model final
{

public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char* name;
		Node* parent;
		DirectX::XMFLOAT3		scale;
		DirectX::XMFLOAT4		rotate;
		DirectX::XMFLOAT3		translate;
		DirectX::XMFLOAT4X4	local_transform;
		DirectX::XMFLOAT4X4	world_transform;

		std::vector<Node*>			children;
	};
private:
	std::shared_ptr<ModelResource> resource;
	std::vector<Node>							nodes;
	int													current_animation = -1;
	float													current_seconds = 0.0f;
	bool													loop_animation = false;
	bool													end_animation = false;

private:

public:
	// 再生中かを調べる
	bool IsPlayAnimation() const { return current_animation >= 0; }

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	/// <param name="animation_index">アニメーションのID</param>
	/// <param name="loop">ループするか</param>
	void PlayAnimation(int animation_index, bool loop = false);

	/// <summary>
	/// アニメーション更新処理
	/// </summary>
	/// <param name="elapsed_time">経過時間</param>
	void UpdateAnimation(float elapsed_time);

	// ローカル変換行列計算
	void CalculateLocalTransform();

	/// <summary>
	/// ワールド変換行列計算
	/// </summary>
	/// <param name="world_transform"></param>
	void CalculateWorldTransform(const DirectX::XMMATRIX& world_transform);

	/// <summary>
	/// 行列更新
	/// </summary>
	/// <param name="transform"></param>
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

};
