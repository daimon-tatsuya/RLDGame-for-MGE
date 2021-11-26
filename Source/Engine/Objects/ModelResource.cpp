//**********************************************************
//
//		ModelResource�N���X
//
//**********************************************************
#include <stdlib.h>
#include <fstream>
#include <functional>
#include <shlwapi.h>
#include <algorithm>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "./Engine/Systems/misc.h"
#include "./Engine/Objects/ModelResource.h"
#include"./Engine/Systems/Texture.h"
#include "Engine/Systems/Logger.h"

// CEREAL�o�[�W������`
CEREAL_CLASS_VERSION(ModelResource::Node, 1)
CEREAL_CLASS_VERSION(ModelResource::Material, 1)
CEREAL_CLASS_VERSION(ModelResource::Subset, 1)
CEREAL_CLASS_VERSION(ModelResource::Vertex, 1)
CEREAL_CLASS_VERSION(ModelResource::Mesh, 1)
CEREAL_CLASS_VERSION(ModelResource::NodeKeyData, 1)
CEREAL_CLASS_VERSION(ModelResource::Keyframe, 1)
CEREAL_CLASS_VERSION(ModelResource::Animation, 1)
CEREAL_CLASS_VERSION(ModelResource, 1)

// FbxDouble2 �� XMFLOAT2
inline DirectX::XMFLOAT2 FbxDouble2ToFloat2(const FbxDouble2& fbxValue)
{
	return DirectX::XMFLOAT2(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1])
	);
}

// FbxDouble3 �� XMFLOAT3
inline DirectX::XMFLOAT3 FbxDouble3ToFloat3(const FbxDouble3& fbxValue)
{
	return DirectX::XMFLOAT3(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1]),
		static_cast<float>(fbxValue[2])
	);
}

// FbxDouble4 �� XMFLOAT3
inline DirectX::XMFLOAT3 FbxDouble4ToFloat3(const FbxDouble4& fbxValue)
{
	return DirectX::XMFLOAT3(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1]),
		static_cast<float>(fbxValue[2])
	);
}

// FbxDouble4 �� XMFLOAT4
inline DirectX::XMFLOAT4 FbxDouble4ToFloat4(const FbxDouble4& fbxValue)
{
	return DirectX::XMFLOAT4(
		static_cast<float>(fbxValue[0]),
		static_cast<float>(fbxValue[1]),
		static_cast<float>(fbxValue[2]),
		static_cast<float>(fbxValue[3])
	);
}

// FbxDouble4 �� XMFLOAT4
inline DirectX::XMFLOAT4X4 FbxAMatrixToFloat4x4(const FbxAMatrix& fbxValue)
{
	return DirectX::XMFLOAT4X4(
		static_cast<float>(fbxValue[0][0]),
		static_cast<float>(fbxValue[0][1]),
		static_cast<float>(fbxValue[0][2]),
		static_cast<float>(fbxValue[0][3]),
		static_cast<float>(fbxValue[1][0]),
		static_cast<float>(fbxValue[1][1]),
		static_cast<float>(fbxValue[1][2]),
		static_cast<float>(fbxValue[1][3]),
		static_cast<float>(fbxValue[2][0]),
		static_cast<float>(fbxValue[2][1]),
		static_cast<float>(fbxValue[2][2]),
		static_cast<float>(fbxValue[2][3]),
		static_cast<float>(fbxValue[3][0]),
		static_cast<float>(fbxValue[3][1]),
		static_cast<float>(fbxValue[3][2]),
		static_cast<float>(fbxValue[3][3])
	);
}
template<class Archive>
void ModelResource::Node::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(parent_index),
		CEREAL_NVP(scale),
		CEREAL_NVP(rotate),
		CEREAL_NVP(translate)
	);
}

template<class Archive>
void ModelResource::Material::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(texture_filename),
		CEREAL_NVP(color)
	);
}

template<class Archive>
void ModelResource::Vertex::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(position),
		CEREAL_NVP(normal),
		CEREAL_NVP(texcoord),
		CEREAL_NVP(bone_weight),
		CEREAL_NVP(bone_index)
	);
}

template<class Archive>
void ModelResource::Subset::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(start_index),
		CEREAL_NVP(index_count),
		CEREAL_NVP(material_index)
	);
}

template<class Archive>
void ModelResource::Animation::serialize(Archive& archive, int version)
{
	archive(
		//CEREAL_NVP(name),
		CEREAL_NVP(seconds_length),
		CEREAL_NVP(keyframes)
	);
}

template<class Archive>
void ModelResource::Keyframe::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(seconds),
		CEREAL_NVP(node_keys)
	);
}

template<class Archive>
void ModelResource::NodeKeyData::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(scale),
		CEREAL_NVP(rotate),
		CEREAL_NVP(translate)
	);
}

template<class Archive>
void ModelResource::Mesh::serialize(Archive& archive, int version)
{
	archive(
		CEREAL_NVP(vertices),
		CEREAL_NVP(indices),
		CEREAL_NVP(subsets),
		CEREAL_NVP(node_index),
		CEREAL_NVP(node_indices),
		CEREAL_NVP(inverse_transforms)
	);
}

// �V���A���C�Y
namespace DirectX
{
	template<class Archive>
	void serialize(Archive& archive, XMUINT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT3& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}

	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4X4& m)
	{
		archive(
			cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12), cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22), cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32), cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42), cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
		);
	}
}

ModelResource::ModelResource(ID3D11Device* device, const char* filename, const char* ignoreRootMotionNodeName)
{
	ignore_root_motion_node_name = ignoreRootMotionNodeName;
	// �}���`�o�C�g�������烏�C�h�����֕ϊ�
	size_t length;
	wchar_t wfilename[256];
	::mbstowcs_s(&length, wfilename, 256, filename, _TRUNCATE);
	//�g���q���Ƃ̓ǂݍ��݂��s��
	std::wstring extension = PathFindExtensionW(wfilename);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::towlower);

	if (L".fbx" == extension || L".FBX" == extension)
	{
		BuildModel(device, filename);
	}
	else if (L".bin" == extension)
	{
		Load(device, filename);
	}
}

void ModelResource::AddAnimation(const char* fbxFilename)
{
	FbxManager* fbxManager = FbxManager::Create();

	// FBX�ɑ΂�����o�͂��`����
	FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbxManager, IOSROOT);	// ���ʂȗ��R���Ȃ�����IOSROOT���w��
	fbxManager->SetIOSettings(fbxIOS);

	// �C���|�[�^�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	bool result = fbxImporter->Initialize(fbxFilename, -1, fbxManager->GetIOSettings());	// -1�Ńt�@�C���t�H�[�}�b�g��������
	_ASSERT_EXPR_A(result, "FbxImporter::Initialize() : Failed!!\n");

	// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();	// �V�[���𗬂����񂾂�Importer�͉������OK

	// �A�j���[�V�����\�z
	BuildAnimations(fbxScene);

	// �}�l�[�W�����
	fbxManager->Destroy();		// �֘A���邷�ׂẴI�u�W�F�N�g����������
}

// �t���p�X���擾
std::string ModelResource::GetFullPathNodeName(Node& node)
{
	std::string parentNodeName;

	if (node.parent_index >= 0)
	{
		Node& parentNode = nodes.at(node.parent_index);
		parentNodeName = GetFullPathNodeName(parentNode);
		return parentNodeName + "/" + node.name;
	}

	return node.name;
}

void ModelResource::BuildModel(ID3D11Device* device, const char* fbxFilename)
{
	FbxManager* fbxManager = FbxManager::Create();

	// FBX�ɑ΂�����o�͂��`����
	FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbxManager, IOSROOT);	// ���ʂȗ��R���Ȃ�����IOSROOT���w��
	fbxManager->SetIOSettings(fbxIOS);

	// �C���|�[�^�𐶐�
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	bool result = fbxImporter->Initialize(fbxFilename, -1, fbxManager->GetIOSettings());	// -1�Ńt�@�C���t�H�[�}�b�g��������
	_ASSERT_EXPR_A(result, "FbxImporter::Initialize() : Failed!!\n");

	// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "scene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();	// �V�[���𗬂����񂾂�Importer�͉������OK

	// �W�I���g�����O�p�`�����Ă���
	FbxGeometryConverter fbxGeometryConverter(fbxManager);
	fbxGeometryConverter.Triangulate(fbxScene, true);
	fbxGeometryConverter.RemoveBadPolygonsFromMeshes(fbxScene);

#if 0
	// DirectX���W�n�֕ϊ�
	FbxAxisSystem sceneAxisSystem = fbxScene->GetGlobalSettings().GetAxisSystem();
	if (sceneAxisSystem != FbxAxisSystem::DirectX)
	{
		FbxAxisSystem::DirectX.ConvertScene(fbxScene);
	}
#elif 0
	// OpenGL���W�n�֕ϊ�
	FbxAxisSystem sceneAxisSystem = fbxScene->GetGlobalSettings().GetAxisSystem();
	if (sceneAxisSystem != FbxAxisSystem::OpenGL)
	{
		FbxAxisSystem::OpenGL.ConvertScene(fbxScene);
	}
#endif

	// �f�B���N�g���p�X�擾
	char dirname[256];
	::_splitpath_s(fbxFilename, nullptr, 0, dirname, 256, nullptr, 0, nullptr, 0);

	// ���f���\�z
	FbxNode* fbxRootNode = fbxScene->GetRootNode();
	BuildMaterials(device, dirname, fbxScene);
#if 1
	for (int i = 0; i < fbxRootNode->GetChildCount(); i++)
	{
		BuildNodes(fbxRootNode->GetChild(i), -1);
	}
#else
	BuildNodes(fbxRootNode, -1);
#endif
	BuildMeshes(device, fbxRootNode);

	// �������郋�[�g���[�V����������
	if (ignore_root_motion_node_name != nullptr)
	{
		root_motion_node_index = -1;
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (nodes.at(i).name == ignore_root_motion_node_name)
			{
				root_motion_node_index = static_cast<int>(i);
				break;
			}
		}
	}

	// �A�j���[�V�����\�z
	BuildAnimations(fbxScene);

	//�V���A���C�Y��

	Serialize(fbxFilename);

	// �}�l�[�W�����
	fbxManager->Destroy();		// �֘A���邷�ׂẴI�u�W�F�N�g����������
}

void ModelResource::BuildSerializedModel(ID3D11Device* device, const char* dirname)
{
	for (Material& material : materials)
	{
		// ���΃p�X�̉���
		char filename[256];
		::_makepath_s(filename, 256, nullptr, dirname, material.texture_filename.c_str(), nullptr);

		// �}���`�o�C�g�������烏�C�h�����֕ϊ�
		wchar_t wfilename[256];
		::MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, 256);

		// �e�N�X�`���ǂݍ���
		D3D11_TEXTURE2D_DESC texture2d_desc;
		LoadTextureFromFile(device, wfilename, material.shader_resource_view.GetAddressOf(), &texture2d_desc);
		if (material.shader_resource_view == nullptr)
		{
			MakeDummyTexture(device, material.shader_resource_view.GetAddressOf());
		}
	}

	for (Mesh& mesh : meshes)
	{
		// �T�u�Z�b�g
		for (Subset& subset : mesh.subsets)
		{
			subset.material = &materials.at(subset.material_index);
		}

		// ���_�o�b�t�@
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresourceData.pSysMem = mesh.vertices.data();
			subresourceData.SysMemPitch = 0;
			subresourceData.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.vertex_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
		}

		// �C���f�b�N�X�o�b�t�@
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(u_int) * mesh.indices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresourceData.pSysMem = mesh.indices.data();
			subresourceData.SysMemPitch = 0; //Not use for index buffers.
			subresourceData.SysMemSlicePitch = 0; //Not use for index buffers.
			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.index_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
		}
	}
}

// FBX�m�[�h���ċA�I�ɒH���ăf�[�^���\�z����
void ModelResource::BuildNodes(FbxNode* fbxNode, int parentNodeIndex)
{
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;

	if (fbxNodeAttribute != nullptr)
	{
		fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
	}

	switch (fbxNodeAttributeType)
	{
	case FbxNodeAttribute::eUnknown:
	case FbxNodeAttribute::eNull:
	case FbxNodeAttribute::eMarker:
	case FbxNodeAttribute::eMesh:
	case FbxNodeAttribute::eSkeleton:
		BuildNode(fbxNode, parentNodeIndex);
		break;
	}

	// �ċA�I�Ɏq�m�[�h����������
	parentNodeIndex = static_cast<int>(nodes.size() - 1);
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		BuildNodes(fbxNode->GetChild(i), parentNodeIndex);
	}
}

// FBX�m�[�h����m�[�h�f�[�^���\�z����
void ModelResource::BuildNode(FbxNode* fbxNode, int parentNodeIndex)
{
	FbxAMatrix& fbxLocalTransform = fbxNode->EvaluateLocalTransform();

	Node node;
	node.name = fbxNode->GetName();
	node.parent_index = parentNodeIndex;
	node.scale = FbxDouble4ToFloat3(fbxLocalTransform.GetS());
	node.rotate = FbxDouble4ToFloat4(fbxLocalTransform.GetQ());
	node.translate = FbxDouble4ToFloat3(fbxLocalTransform.GetT());

	nodes.emplace_back(node);
}

// FBX�m�[�h���ċA�I�ɒH���ă��b�V���f�[�^���\�z����
void ModelResource::BuildMeshes(ID3D11Device* device, FbxNode* fbxNode)
{
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;

	if (fbxNodeAttribute != nullptr)
	{
		fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
	}

	switch (fbxNodeAttributeType)
	{
	case FbxNodeAttribute::eMesh:
		BuildMesh(device, fbxNode, static_cast<FbxMesh*>(fbxNodeAttribute));
		break;
	}

	// �ċA�I�Ɏq�m�[�h����������
	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		BuildMeshes(device, fbxNode->GetChild(i));
	}
}

// FBX���b�V�����烁�b�V���f�[�^���\�z����
void ModelResource::BuildMesh(ID3D11Device* device, FbxNode* fbxNode, FbxMesh* fbxMesh)
{
	int fbxControlPointsCount = fbxMesh->GetControlPointsCount();

	//int fbxPolygonVertexCount = fbxMesh->GetPolygonVertexCount();
	//const int* fbxPolygonVertices = fbxMesh->GetPolygonVertices();

	int fbxMaterialCount = fbxNode->GetMaterialCount();
	int fbxPolygonCount = fbxMesh->GetPolygonCount();

	meshes.emplace_back(Mesh());
	Mesh& mesh = meshes.back();
	mesh.subsets.resize(fbxMaterialCount > 0 ? fbxMaterialCount : 1);
	mesh.node_index = FindNodeIndex(fbxNode->GetName());

	// �T�u�Z�b�g�̃}�e���A���ݒ�
	for (int fbxMaterialIndex = 0; fbxMaterialIndex < fbxMaterialCount; fbxMaterialIndex++)
	{
		const FbxSurfaceMaterial* fbxSurfaceMaterial = fbxNode->GetMaterial(fbxMaterialIndex);

		Subset& subset = mesh.subsets.at(fbxMaterialIndex);
		int index = FindMaterialIndex(fbxNode->GetScene(), fbxSurfaceMaterial);
		subset.material = &materials.at(index);
		subset.material_index = index;
	}

	// �T�u�Z�b�g�̒��_�C���f�b�N�X�͈͐ݒ�
	if (fbxMaterialCount > 0)
	{
		for (int fbxPolygonIndex = 0; fbxPolygonIndex < fbxPolygonCount; fbxPolygonIndex++)
		{
			int fbxMaterialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(fbxPolygonIndex);
			mesh.subsets.at(fbxMaterialIndex).index_count += 3;
		}

		int offset = 0;
		for (Subset& subset : mesh.subsets)
		{
			subset.start_index = offset;
			offset += subset.index_count;

			subset.index_count = 0;
		}
	}

	// ���_�e���̓f�[�^
	struct BoneInfluence
	{
		int useCount = 0;
		int indices[4] = { 0, 0, 0, 0 };
		float weights[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

		void Add(int index, float weight)
		{
			if (useCount < 4)
			{
				indices[useCount] = index;
				weights[useCount] = weight;
				useCount++;
			}
		}
	};
	// ���_�e���̓f�[�^�𒊏o����
	//Todo: �e���͂̍��v��1�ȏ�ɂȂ������̏����Ɖe���͂�4�ȏ゠��Ƃ��̏���
	std::vector<BoneInfluence> boneInfluences;
	{
		boneInfluences.resize(fbxControlPointsCount);

		// �X�L�j���O�ɕK�v�ȏ����擾����
		int fbxDeformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int fbxDeformerIndex = 0; fbxDeformerIndex < fbxDeformerCount; fbxDeformerIndex++)
		{
			FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(fbxDeformerIndex, FbxDeformer::eSkin));

			int fbxClusterCount = fbxSkin->GetClusterCount();
			for (int fbxClusterIndex = 0; fbxClusterIndex < fbxClusterCount; fbxClusterIndex++)
			{
				FbxCluster* fbxCluster = fbxSkin->GetCluster(fbxClusterIndex);

				// ���_�e���̓f�[�^�𒊏o����
				{
					int fbxClusterControlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
					const int* fbxControlPointIndices = fbxCluster->GetControlPointIndices();
					const double* fbxControlPointWeights = fbxCluster->GetControlPointWeights();

					for (int i = 0; i < fbxClusterControlPointIndicesCount; i++)
					{
						BoneInfluence& boneInfluence = boneInfluences.at(fbxControlPointIndices[i]);

						boneInfluence.Add(fbxClusterIndex, static_cast<float>(fbxControlPointWeights[i]));
					}
				}

				// �{�[���ϊ��s��p�̋t�s��̌v�Z������
				{
					// ���b�V����Ԃ��烏�[���h��Ԃւ̕ϊ��s��
					FbxAMatrix fbxMeshSpaceTransform;
					fbxCluster->GetTransformMatrix(fbxMeshSpaceTransform);

					// �{�[����Ԃ��烏�[���h��Ԃւ̕ϊ��s��
					FbxAMatrix fbxBoneSpaceTransform;
					fbxCluster->GetTransformLinkMatrix(fbxBoneSpaceTransform);

					// �{�[���t�s����v�Z����
					FbxAMatrix fbxInverseTransform = fbxBoneSpaceTransform.Inverse() * fbxMeshSpaceTransform;

					DirectX::XMFLOAT4X4 inverseTransforms = FbxAMatrixToFloat4x4(fbxInverseTransform);
					mesh.inverse_transforms.emplace_back(inverseTransforms);

					int nodeIndex = FindNodeIndex(fbxCluster->GetLink()->GetName());
					mesh.node_indices.emplace_back(nodeIndex);
				}
			}
		}
	}

	// �W�I���g���s��
	FbxAMatrix fbxGeometricTransform(
		fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot),
		fbxNode->GetGeometricRotation(FbxNode::eSourcePivot),
		fbxNode->GetGeometricScaling(FbxNode::eSourcePivot)
	);
	DirectX::XMFLOAT4X4 geometricTransform = FbxAMatrixToFloat4x4(fbxGeometricTransform);
	DirectX::XMMATRIX GM = DirectX::XMLoadFloat4x4(&geometricTransform);

	// UV�Z�b�g��
	FbxStringList fbxUVSetNames;
	fbxMesh->GetUVSetNames(fbxUVSetNames);

	// ���_�f�[�^
	mesh.vertices.resize(fbxPolygonCount * 3);
	mesh.indices.resize(fbxPolygonCount * 3);

	int vertexCount = 0;
	const FbxVector4* fbxControlPoints = fbxMesh->GetControlPoints();
	for (int fbxPolygonIndex = 0; fbxPolygonIndex < fbxPolygonCount; fbxPolygonIndex++)
	{
		// �|���S���ɓK�p����Ă���}�e���A���C���f�b�N�X���擾����
		int fbxMaterialIndex = 0;
		if (fbxMaterialCount > 0)
		{
			fbxMaterialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(fbxPolygonIndex);
		}

		Subset& subset = mesh.subsets.at(fbxMaterialIndex);
		const int indexOffset = subset.start_index + subset.index_count;

		for (int fbxVertexIndex = 0; fbxVertexIndex < 3; fbxVertexIndex++)
		{
			Vertex vertex;

			int fbxControlPointIndex = fbxMesh->GetPolygonVertex(fbxPolygonIndex, fbxVertexIndex);
			// Position
			{
				DirectX::XMFLOAT3 position = FbxDouble4ToFloat3(fbxControlPoints[fbxControlPointIndex]);
				DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&position);
				V = DirectX::XMVector3TransformCoord(V, GM);
				DirectX::XMStoreFloat3(&vertex.position, V);
			}

			// Weight
			{
				BoneInfluence& boneInfluence = boneInfluences.at(fbxControlPointIndex);
				vertex.bone_index.x = boneInfluence.indices[0];
				vertex.bone_index.y = boneInfluence.indices[1];
				vertex.bone_index.z = boneInfluence.indices[2];
				vertex.bone_index.w = boneInfluence.indices[3];
				vertex.bone_weight.x = boneInfluence.weights[0];
				vertex.bone_weight.y = boneInfluence.weights[1];
				vertex.bone_weight.z = boneInfluence.weights[2];
				vertex.bone_weight.w = 1.0f - (vertex.bone_weight.x + vertex.bone_weight.y + vertex.bone_weight.z);
				//vertex.boneWeight.w = boneInfluence.weights[3];
			}

			// Normal
			if (fbxMesh->GetElementNormalCount() > 0)
			{
				FbxVector4 fbxNormal;
				fbxMesh->GetPolygonVertexNormal(fbxPolygonIndex, fbxVertexIndex, fbxNormal);

				DirectX::XMFLOAT3 normal = FbxDouble4ToFloat3(fbxNormal);
				DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&normal);
				V = DirectX::XMVector3TransformCoord(V, GM);
				V = DirectX::XMVector3Normalize(V);
				DirectX::XMStoreFloat3(&vertex.normal, V);

				vertex.normal = FbxDouble4ToFloat3(fbxNormal);
			}
			else
			{
				vertex.normal = DirectX::XMFLOAT3(0, 0, 0);
			}

			// Texcoord
			if (fbxMesh->GetElementUVCount() > 0)
			{
				bool fbxUnmappedUV;
				FbxVector2 fbxUV;
				fbxMesh->GetPolygonVertexUV(fbxPolygonIndex, fbxVertexIndex, fbxUVSetNames[0], fbxUV, fbxUnmappedUV);
				fbxUV[1] = 1.0 - fbxUV[1];
				vertex.texcoord = FbxDouble2ToFloat2(fbxUV);
			}
			else
			{
				vertex.texcoord = DirectX::XMFLOAT2(0, 0);
			}

			mesh.indices.at(indexOffset + fbxVertexIndex) = vertexCount;
			mesh.vertices.at(vertexCount) = vertex;
			vertexCount++;
		}

		subset.index_count += 3;
	}

	// ���_�o�b�t�@
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		subresourceData.pSysMem = mesh.vertices.data();
		subresourceData.SysMemPitch = 0;
		subresourceData.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}

	// �C���f�b�N�X�o�b�t�@
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(u_int) * mesh.indices.size());
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		subresourceData.pSysMem = mesh.indices.data();
		subresourceData.SysMemPitch = 0; //Not use for index buffers.
		subresourceData.SysMemSlicePitch = 0; //Not use for index buffers.
		HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_Trace(hr));
	}
}

// FBX�V�[������FBX�}�e���A������}�e���A���f�[�^���\�z����
void ModelResource::BuildMaterials(ID3D11Device* device, const char* dirname, FbxScene* fbxScene)
{
	int fbxMaterialCount = fbxScene->GetMaterialCount();

	if (fbxMaterialCount > 0)
	{
		for (int fbxMaterialIndex = 0; fbxMaterialIndex < fbxMaterialCount; fbxMaterialIndex++)
		{
			FbxSurfaceMaterial* fbxSurfaceMaterial = fbxScene->GetMaterial(fbxMaterialIndex);

			BuildMaterial(device, dirname, fbxSurfaceMaterial);
		}
	}
	else
	{
		Material material;
		material.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materials.emplace_back(material);
	}
}

// FBX�}�e���A������}�e���A���f�[�^���\�z����
void ModelResource::BuildMaterial(ID3D11Device* device, const char* dirname, FbxSurfaceMaterial* fbxSurfaceMaterial)
{
	bool ret = false;

	Material material;

	// �f�B�t���[�Y�J���[
	FbxProperty fbxDiffuseProperty = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxProperty fbxDiffuseFactorProperty = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	if (fbxDiffuseProperty.IsValid() && fbxDiffuseFactorProperty.IsValid())
	{
		FbxDouble fbxFactor = fbxDiffuseFactorProperty.Get<FbxDouble>();
		FbxDouble3 fbxColor = fbxDiffuseProperty.Get<FbxDouble3>();

		material.color.x = static_cast<float>(fbxColor[0] * fbxFactor);
		material.color.y = static_cast<float>(fbxColor[1] * fbxFactor);
		material.color.z = static_cast<float>(fbxColor[2] * fbxFactor);
		material.color.w = 1.0f;
	}
#if 0
	FbxProperty fbx_transparency_factor_property = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (fbx_transparency_factor_property.IsValid())
	{
		FbxDouble fbxFactor = fbx_transparency_factor_property.Get<FbxDouble>();

		material.color.w = static_cast<float>(fbxFactor);
	}
#endif
	// �f�B�t���[�Y�e�N�X�`��
	if (fbxDiffuseProperty.IsValid())
	{
		int fbxTextureCount = fbxDiffuseProperty.GetSrcObjectCount<FbxFileTexture>();
		if (fbxTextureCount > 0)
		{
			FbxFileTexture* fbxTexture = fbxDiffuseProperty.GetSrcObject<FbxFileTexture>();

			// ���΃p�X�̉���
			char filename[256];
			::_makepath_s(filename, 256, nullptr, dirname, fbxTexture->GetRelativeFileName(), nullptr);

			// �}���`�o�C�g�������烏�C�h�����֕ϊ�
			size_t length;
			wchar_t wfilename[256];
			::mbstowcs_s(&length, wfilename, 256, filename, _TRUNCATE);

			// �e�N�X�`���ǂݍ���
			D3D11_TEXTURE2D_DESC texture2d_desc;
			LoadTextureFromFile(device, wfilename, material.shader_resource_view.GetAddressOf(), &texture2d_desc);
			if (material.shader_resource_view == nullptr)
			{
				MakeDummyTexture(device, material.shader_resource_view.GetAddressOf());
			}
			else
			{
				material.texture_filename = fbxTexture->GetRelativeFileName();
			}
		}
	}
	//

	materials.emplace_back(material);
}

// �A�j���[�V�����f�[�^���\�z
void ModelResource::BuildAnimations(FbxScene* fbxScene)
{
	// ���ׂẴA�j���[�V���������擾
	FbxArray<FbxString*> fbxAnimStackNames;
	fbxScene->FillAnimStackNameArray(fbxAnimStackNames);

	int fbxAnimationCount = fbxAnimStackNames.Size();
	for (int fbxAnimationIndex = 0; fbxAnimationIndex < fbxAnimationCount; fbxAnimationIndex++)
	{
		animations.emplace_back(Animation());
		Animation& animation = animations.back();

		// �A�j���[�V�����f�[�^�̃T���v�����O�ݒ�
		FbxTime::EMode fbxTimeMode = fbxScene->GetGlobalSettings().GetTimeMode();
		FbxTime fbxFrameTime;
		fbxFrameTime.SetTime(0, 0, 0, 1, 0, fbxTimeMode);

		float samplingRate = static_cast<float>(fbxFrameTime.GetFrameRate(fbxTimeMode));
		float samplingTime = 1.0f / samplingRate;

		FbxString* fbxAnimStackName = fbxAnimStackNames.GetAt(fbxAnimationIndex);
		FbxAnimStack* fbxAnimStack = fbxScene->FindMember<FbxAnimStack>(fbxAnimStackName->Buffer());

		// �Đ�����A�j���[�V�������w�肷��B
		fbxScene->SetCurrentAnimationStack(fbxAnimStack);

		// �A�j���[�V�����̍Đ��J�n���ԂƍĐ��I�����Ԃ��擾����
		FbxTakeInfo* fbxTakeInfo = fbxScene->GetTakeInfo(fbxAnimStackName->Buffer());
		FbxTime fbxStartTime = fbxTakeInfo->mLocalTimeSpan.GetStart();
		FbxTime fbxEndTime = fbxTakeInfo->mLocalTimeSpan.GetStop();

		// ���o����f�[�^��60�t���[����ŃT���v�����O����
		FbxTime fbxSamplingStep;
		fbxSamplingStep.SetTime(0, 0, 1, 0, 0, fbxTimeMode);
		fbxSamplingStep = static_cast<FbxLongLong>(fbxSamplingStep.Get() * samplingTime);

		int startFrame = static_cast<int>(fbxStartTime.Get() / fbxSamplingStep.Get());
		int endFrame = static_cast<int>(fbxEndTime.Get() / fbxSamplingStep.Get());
		int frameCount = static_cast<int>((fbxEndTime.Get() - fbxStartTime.Get()) / fbxSamplingStep.Get());

		// �A�j���[�V�����̑ΏۂƂȂ�m�[�h��񋓂���
		std::vector<FbxNode*> fbxNodes;
		FbxNode* fbxRootNode = fbxScene->GetRootNode();
		for (Node& node : nodes)
		{
			FbxNode* fbxNode = fbxRootNode->FindChild(node.name.c_str());
			fbxNodes.emplace_back(fbxNode);
		}

		// �A�j���[�V�����f�[�^�𒊏o����
		animation.seconds_length = frameCount * samplingTime;
		animation.keyframes.resize(frameCount + 1);

		float seconds = 0.0f;
		Keyframe* keyframe = animation.keyframes.data();
		size_t fbxNodeCount = fbxNodes.size();
		FbxTime fbxCurrentTime = fbxStartTime;
		for (FbxTime fbxCurrentTime = fbxStartTime; fbxCurrentTime < fbxEndTime; fbxCurrentTime += fbxSamplingStep, keyframe++)
		{
			// �L�[�t���[�����̎p���f�[�^�����o���B
			keyframe->seconds = seconds;
			keyframe->node_keys.resize(fbxNodeCount);
			for (size_t fbxNodeIndex = 0; fbxNodeIndex < fbxNodeCount; fbxNodeIndex++)
			{
				NodeKeyData& keyData = keyframe->node_keys.at(fbxNodeIndex);
				FbxNode* fbxNode = fbxNodes.at(fbxNodeIndex);
				if (fbxNode == nullptr)
				{
					// �A�j���[�V�����Ώۂ̃m�[�h���Ȃ������̂Ń_�~�[�f�[�^��ݒ�
					Node& node = nodes.at(fbxNodeIndex);
					keyData.scale = node.scale;
					keyData.rotate = node.rotate;
					keyData.translate = node.translate;
				}
				else if (fbxNodeIndex == root_motion_node_index)
				{
					// ���[�g���[�V�����͖�������
					Node& node = nodes.at(fbxNodeIndex);
					keyData.scale = DirectX::XMFLOAT3(1, 1, 1);
					keyData.rotate = DirectX::XMFLOAT4(0, 0, 0, 1);
					keyData.translate = DirectX::XMFLOAT3(0, 0, 0);
				}
				else
				{
					// �w�莞�Ԃ̃��[�J���s�񂩂�X�P�[���l�A��]�l�A�ړ��l�����o���B
					const FbxAMatrix& fbxLocalTransform = fbxNode->EvaluateLocalTransform(fbxCurrentTime);

					keyData.scale = FbxDouble4ToFloat3(fbxLocalTransform.GetS());
					keyData.rotate = FbxDouble4ToFloat4(fbxLocalTransform.GetQ());
					keyData.translate = FbxDouble4ToFloat3(fbxLocalTransform.GetT());
				}
			}
			seconds += samplingTime;
		}
	}

	// ��n��
	for (int i = 0; i < fbxAnimationCount; i++)
	{
		delete fbxAnimStackNames[i];
	}
}

// �m�[�h�C���f�b�N�X���擾����
int ModelResource::FindNodeIndex(const char* name)
{
	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].name == name)
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}

// �}�e���A���C���f�b�N�X���擾����
int ModelResource::FindMaterialIndex(FbxScene* fbxScene, const FbxSurfaceMaterial* fbxSurfaceMaterial)
{
	int fbxMaterialCount = fbxScene->GetMaterialCount();

	for (int i = 0; i < fbxMaterialCount; i++)
	{
		if (fbxScene->GetMaterial(i) == fbxSurfaceMaterial)
		{
			return i;
		}
	}
	return -1;
}

void ModelResource::Serialize(const char* filename)
{
	char result[MAX_PATH];
	strncpy_s(result, filename, MAX_PATH - 1);
	PathRenameExtensionA(result, ".bin");

	//	 �V���A���C�Y
	std::ofstream ostream(result, std::ios::binary);
	if (ostream.is_open())
	{
		cereal::BinaryOutputArchive archive(ostream);

		try
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes),
				CEREAL_NVP(animations)
			);
		}
		catch (...)
		{
			LOG("model deserialize failed.\n%s\n", result);
			return;
		}
	}
}

// �f�V���A���C�Y
void ModelResource::Deserialize(const char* filename)
{
	std::ifstream istream(filename, std::ios::binary);
	if (istream.is_open())
	{
		cereal::BinaryInputArchive archive(istream);

		try
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes),
				CEREAL_NVP(animations)
			);
		}
		catch (...)
		{
			LOG("model deserialize failed.\n%s\n", filename);
			return;
		}
	}
}

void ModelResource::Load(ID3D11Device* device, const char* filename)
{
	// �f�B���N�g���p�X�擾
	char drive[32], dir[256], dirname[256];
	::_splitpath_s(filename, drive, sizeof(drive), dir, sizeof(dir), nullptr, 0, nullptr, 0);
	::_makepath_s(dirname, sizeof(dirname), drive, dir, nullptr, nullptr);

	// �f�V���A���C�Y
	Deserialize(filename);

	BuildSerializedModel(device, dirname);
}