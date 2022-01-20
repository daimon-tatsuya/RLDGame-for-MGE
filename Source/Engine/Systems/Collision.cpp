//**********************************************************
//
//	Collision�N���X
//
//**********************************************************
#include "Engine/Systems/Collision.h"
#include "Engine/Objects/ModelResource.h"
#include "Engine/Objects/Model.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(const DirectX::XMFLOAT3 min_value, const DirectX::XMFLOAT3 max_value,DirectX::XMFLOAT3 object_pos)
:position_min(min_value), position_max(max_value), object_old_position(object_pos)
{
	center = {(position_max.x - position_min.x)/2,(position_max.y - position_min.y)/2, (position_max.z - position_min.z)/2};
}

bool Collision::IntersectSphereToSphere(
	const DirectX::XMFLOAT3& positionA,
	float radiusA,
	const DirectX::XMFLOAT3& positionB,
	float radiusB,
	DirectX::XMFLOAT3& out_positionA,
	DirectX::XMFLOAT3& out_positionB)
{
	//B->A�̒P�ʃx�N�g��
	DirectX::XMVECTOR v_positionA = DirectX::XMLoadFloat3(&positionA);
	DirectX::XMVECTOR v_positionB = DirectX::XMLoadFloat3(&positionB);
	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(v_positionB, v_positionA);
	DirectX::XMVECTOR v_length = DirectX::XMVector3LengthSq(vec);
	float length;
	DirectX::XMStoreFloat(&length, v_length);

	// ��������
	float range = radiusA + radiusB;
	if (length > range)
	{
		return false;
	}

	// A��B�������o��
	vec = DirectX::XMVector3Normalize(vec);
	// �����蔻��Ɏ��ʓ���t�^���ďd�ݔ�����s���Əd����\�����₷��
	vec = DirectX::XMVectorScale(vec, (range - length) * 0.5f);

	v_positionA = DirectX::XMVectorAdd(v_positionA, vec);
	v_positionB = DirectX::XMVectorAdd(v_positionB, vec);

	DirectX::XMStoreFloat3(&out_positionA, v_positionA);
	DirectX::XMStoreFloat3(&out_positionB, v_positionB);

	return true;
}

bool Collision::IntersectRayToModel(
	const DirectX::XMFLOAT3& start,
	const DirectX::XMFLOAT3& end,
	const Model* model,
	HitResult& result)
{
	//���[���h���
	DirectX::XMVECTOR world_start = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR world_end = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR world_ray_vec = DirectX::XMVectorSubtract(world_end, world_start);
	DirectX::XMVECTOR world_ray_length = DirectX::XMVector3Length(world_ray_vec);

	// ���[���h��Ԃ̃��C�̒���
	DirectX::XMStoreFloat(&result.distance, world_ray_length);

	bool hit = false;
	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// ���b�V���m�[�h�擾
		const Model::Node node = model->GetNodes().at(mesh.node_index);

		// ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
		DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&node.world_transform);
		DirectX::XMMATRIX inverse_world_transform = DirectX::XMMatrixInverse(nullptr, world_transform);

		DirectX::XMVECTOR start = DirectX::XMVector3TransformCoord(world_start, inverse_world_transform);
		DirectX::XMVECTOR end = DirectX::XMVector3TransformCoord(world_end, inverse_world_transform);
		DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
		DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(vec);
		DirectX::XMVECTOR length = DirectX::XMVector3Length(vec);

		// ���C�̒���
		float neart;
		DirectX::XMStoreFloat(&neart, length);

		// �O�p�`(��)�Ƃ̌�������
		const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;

		int material_index = -1;
		DirectX::XMVECTOR hit_position;
		DirectX::XMVECTOR hit_normal;
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.index_count; i += 3)
			{
				UINT index = subset.start_index + i;
				// �O�p�`�̒��_�𒊏o
				const ModelResource::Vertex& a = vertices.at(indices.at(index));
				const ModelResource::Vertex& b = vertices.at(indices.at((static_cast<size_t>(index + 1))));
				const ModelResource::Vertex& c = vertices.at(indices.at((static_cast<size_t>(index + 2))));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				// �O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//�O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR normal = DirectX::XMVector3Cross(AB, BC);

				// ���ς̌��ʂ��v���X�Ȃ�Η�����
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(dir, normal);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot >= 0)continue;

				// ���C�Ƃ̕��ʂ̌�_���Z�o
				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(normal, V), Dot);
				float t;
				DirectX::XMStoreFloat(&t, T);
				// ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫�����̓X�L�b�v
				if (t<0.0f || t>neart)continue;

				DirectX::XMVECTOR position = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(dir, T), start);

				// ��_���O�p�`�̓����ɂ��邩����
				// �P��
				DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(A, position);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(V1, AB);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, normal);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f)continue;

				// �Q��
				DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(B, position);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(V2, BC);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, normal);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f)continue;

				// �R��
				DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(C, position);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(V3, CA);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, normal);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f)continue;

				// �ŋߋ������X�V
				neart = t;

				// ��_�Ɩ@�����X�V
				hit_position = position;
				hit_normal = normal;
				material_index = subset.material_index;
			}
		}
		if (material_index >= 0)
		{
			// ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
			DirectX::XMVECTOR world_position = DirectX::XMVector3TransformCoord(hit_position, world_transform);
			DirectX::XMVECTOR world_cross_vec = DirectX::XMVectorSubtract(world_position, world_start);
			DirectX::XMVECTOR world_cross_length = DirectX::XMVector3Length(world_cross_vec);
			float distance;
			DirectX::XMStoreFloat(&distance, world_cross_length);

			// �q�b�g���ۑ�
			if (result.distance > distance)
			{
				DirectX::XMVECTOR world_normal = DirectX::XMVector3Transform(hit_normal, world_transform);
				result.distance = distance;
				result.material_index = material_index;
				DirectX::XMStoreFloat3(&result.position, world_position);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(world_normal));
				hit = true;
			}
		}
	}
	return hit;
}

bool Collision::IntersectAxisAlignedBoundingBox(const AxisAlignedBoundingBox& box1, const AxisAlignedBoundingBox& box2)
{
	if (box1.position_min.x > box2.position_max.x)
	{
		return false;
	}

	if (box1.position_max.x < box2.position_min.x)
	{
		return false;
	}

	if (box1.position_min.y > box2.position_max.y)
	{
		return false;
	}

	if (box1.position_max.y < box2.position_min.y)
	{
		return false;
	}

	if (box1.position_min.z > box2.position_max.z)
	{
		return false;
	}

	if (box1.position_max.z < box2.position_min.z)
	{
		return false;
	}

	return true; // �������Ă���
}
