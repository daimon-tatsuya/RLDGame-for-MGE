#include "Engine/Systems/misc.h"
#include "Engine/Objects/Model.h"
#include  "Engine/Systems/ResourceManager.h"
Model::Model(const char* filename)
{
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// �m�[�h
	const std::vector<ModelResource::Node>& res_nodes = resource->GetNodes();

	nodes.resize(res_nodes.size());
	for (size_t node_index = 0; node_index < nodes.size(); node_index++)
	{
		auto&& src = res_nodes.at(node_index);
		auto&& dst = nodes.at(node_index);

		dst.name = src.name.c_str();
		dst.parent = src.parent_index >= 0 ? &nodes.at(src.parent_index) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	//const DirectX::XMFLOAT4X4 transform =
	//{ 1, 0, 0, 0,
	//  0, 1, 0, 0,
	//  0, 0, 1, 0,
	//  0, 0, 0, 1 };
	//UpdateTransform(transform);
}

// �A�j���[�V�����Đ�
void Model::PlayAnimation(int animationIndex, bool loop)
{
	current_animation = animationIndex;
	loop_animation = loop;
	end_animation = false;
	current_seconds = 0.0f;
}

// �A�j���[�V�����v�Z
void Model::UpdateAnimation(float elapsed_time)
{
	if (current_animation < 0)
	{
		return;
	}

	if (resource->GetAnimations().empty())
	{
		return;
	}

	const ModelResource::Animation& animation = resource->GetAnimations().at(current_animation);

	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int key_count = static_cast<int>(keyframes.size());
	for (int key_index = 0; key_index < key_count - 1; key_index++)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		//Todo
		const ModelResource::Keyframe& keyframe0 = keyframes.at(key_index);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(key_index + 1);
		if (current_seconds >= keyframe0.seconds && current_seconds < keyframe1.seconds)
		{
			float rate = (current_seconds - keyframe0.seconds / keyframe1.seconds - keyframe0.seconds);

			assert(nodes.size() == keyframe0.node_keys.size());
			assert(nodes.size() == keyframe1.node_keys.size());
			int node_count = static_cast<int>(nodes.size());
			for (int node_index = 0; node_index < node_count; node_index++)
			{
				// �Q�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.node_keys.at(node_index);
				const ModelResource::NodeKeyData& key1 = keyframe1.node_keys.at(node_index);

				Node& node = nodes[node_index];

				//Todo�@�u�����h�⊮(���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮)

				// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
				// �v�Z���ʂ��{�[���Ɋi�[
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);
			}
			break;
		}
	}

	// �ŏI�t���[������
	if (end_animation)
	{
		end_animation = false;
		current_animation = -1;
		return;
	}

	// ���Ԍo��
	current_seconds += elapsed_time;

	if (current_seconds >= animation.seconds_length)
	{
		if (loop_animation)
		{
			current_seconds -= animation.seconds_length;
		}
		else
		{
			current_seconds = animation.seconds_length;
			end_animation = true;
		}
	}
}

void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX m_transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX local_transform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX parent_transform;
		if (node.parent != nullptr)
		{
			parent_transform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
		}
		else
		{
			parent_transform = m_transform;
		}
		DirectX::XMMATRIX world_transform = local_transform * parent_transform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.local_transform, local_transform);
		DirectX::XMStoreFloat4x4(&node.world_transform, world_transform);
	}
}

// ���[�J���ϊ��s��v�Z
void Model::CalculateLocalTransform()
{
	for (Node& node : nodes)
	{
		DirectX::XMMATRIX scale, rotate, translate;
		scale = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		rotate = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(node.rotate.x, node.rotate.y, node.rotate.z, node.rotate.w));
		translate = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);

		DirectX::XMStoreFloat4x4(&node.local_transform, scale * rotate * translate);
	}
}

// ���[���h�ϊ��s��v�Z
void Model::CalculateWorldTransform(const DirectX::XMMATRIX& world_transform)
{
	for (Node& node : nodes)
	{
		DirectX::XMMATRIX local_transform = DirectX::XMLoadFloat4x4(&node.local_transform);
		if (node.parent != nullptr)
		{
			DirectX::XMMATRIX parent_transform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
			DirectX::XMStoreFloat4x4(&node.world_transform, local_transform * parent_transform);
		}
		else
		{
			DirectX::XMStoreFloat4x4(&node.world_transform, local_transform * world_transform);
		}
	}
}