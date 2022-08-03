#pragma once
//**********************************************************
//
//		Model�N���X
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
/// ���f���̃A�j���[�V�����Ȃǂ��s���N���X
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
	// �Đ������𒲂ׂ�
	bool IsPlayAnimation() const { return current_animation >= 0; }

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	/// <param name="animation_index">�A�j���[�V������ID</param>
	/// <param name="loop">���[�v���邩</param>
	void PlayAnimation(int animation_index, bool loop = false);

	/// <summary>
	/// �A�j���[�V�����X�V����
	/// </summary>
	/// <param name="elapsed_time">�o�ߎ���</param>
	void UpdateAnimation(float elapsed_time);

	// ���[�J���ϊ��s��v�Z
	void CalculateLocalTransform();

	/// <summary>
	/// ���[���h�ϊ��s��v�Z
	/// </summary>
	/// <param name="world_transform"></param>
	void CalculateWorldTransform(const DirectX::XMMATRIX& world_transform);

	/// <summary>
	/// �s��X�V
	/// </summary>
	/// <param name="transform"></param>
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	//------------------------------------------------
	//
	// Getter Setter
	//
	//------------------------------------------------

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }

};
