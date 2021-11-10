#pragma once

#include <memory>
#include <vector>
#include <d3d11.h>
#include "Engine/Objects/ModelResource.h"
#include "Engine/Systems/Math.h"

//ToDo Model コメント

class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*						name;
		Node*							parent;
		DirectX::XMFLOAT3		scale;
		DirectX::XMFLOAT4		rotate;
		DirectX::XMFLOAT3		translate;
		DirectX::XMFLOAT4X4	local_transform;
		DirectX::XMFLOAT4X4	world_transform;

		std::vector<Node*>			children;
	};

	// アニメーション
	//再生中かを調べる
	bool IsPlayAnimation() const { return current_animation >= 0; }
	//アニメーションを再生
	void PlayAnimation(int animationIndex, bool loop = false);
	// アニメーション更新処理
	void UpdateAnimation(float elapsed_time);

	// 行列計算
	void CalculateLocalTransform();
	void CalculateWorldTransform(const DirectX::XMMATRIX& world_transform);
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);


	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>						nodes;
	int												current_animation = -1;
	float											current_seconds = 0.0f;
	bool											loop_animation = false;
	bool											end_animation = false;
};
