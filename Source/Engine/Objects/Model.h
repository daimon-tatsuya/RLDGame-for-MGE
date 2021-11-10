#pragma once

#include <memory>
#include <vector>
#include <d3d11.h>
#include "Engine/Objects/ModelResource.h"
#include "Engine/Systems/Math.h"

//ToDo Model �R�����g

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

	// �A�j���[�V����
	//�Đ������𒲂ׂ�
	bool IsPlayAnimation() const { return current_animation >= 0; }
	//�A�j���[�V�������Đ�
	void PlayAnimation(int animationIndex, bool loop = false);
	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsed_time);

	// �s��v�Z
	void CalculateLocalTransform();
	void CalculateWorldTransform(const DirectX::XMMATRIX& world_transform);
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);


	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }
	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>						nodes;
	int												current_animation = -1;
	float											current_seconds = 0.0f;
	bool											loop_animation = false;
	bool											end_animation = false;
};
