//**********************************************************
//
//		ModelResourceクラス
//
//**********************************************************
#include <algorithm>
#include <fstream>
#include <functional>
#include <Shlwapi.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "Liblary/Objects/ModelResource.h"
#include "Liblary/Systems/Logger.h"
#include "Liblary/Systems/Misc.h"
#include"Liblary/Systems/Texture.h"

// CEREALバージョン定義
CEREAL_CLASS_VERSION(ModelResource::Node, 1)
CEREAL_CLASS_VERSION(ModelResource::Material, 1)
CEREAL_CLASS_VERSION(ModelResource::Subset, 1)
CEREAL_CLASS_VERSION(ModelResource::Vertex, 1)
CEREAL_CLASS_VERSION(ModelResource::Mesh, 1)
CEREAL_CLASS_VERSION(ModelResource::NodeKeyData, 1)
CEREAL_CLASS_VERSION(ModelResource::Keyframe, 1)
CEREAL_CLASS_VERSION(ModelResource::Animation, 1)
CEREAL_CLASS_VERSION(ModelResource, 1)

// FbxDouble2 → XMFLOAT2
inline DirectX::XMFLOAT2 FbxDouble2ToFloat2(const FbxDouble2& fbx_value)
{
	return DirectX::XMFLOAT2(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1])
	);
}

// FbxDouble3 → XMFLOAT3
inline DirectX::XMFLOAT3 FbxDouble3ToFloat3(const FbxDouble3& fbx_value)
{
	return DirectX::XMFLOAT3(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1]),
		static_cast<float>(fbx_value[2])
	);
}

// FbxDouble4 → XMFLOAT3
inline DirectX::XMFLOAT3 FbxDouble4ToFloat3(const FbxDouble4& fbx_value)
{
	return DirectX::XMFLOAT3(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1]),
		static_cast<float>(fbx_value[2])
	);
}

// FbxDouble4 → XMFLOAT4
inline DirectX::XMFLOAT4 FbxDouble4ToFloat4(const FbxDouble4& fbx_value)
{
	return DirectX::XMFLOAT4(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1]),
		static_cast<float>(fbx_value[2]),
		static_cast<float>(fbx_value[3])
	);
}

// FbxDouble4 → XMFLOAT4
inline DirectX::XMFLOAT4X4 FbxAMatrixToFloat4x4(const FbxAMatrix& fbx_value)
{
	return DirectX::XMFLOAT4X4(
		static_cast<float>(fbx_value[0][0]),
		static_cast<float>(fbx_value[0][1]),
		static_cast<float>(fbx_value[0][2]),
		static_cast<float>(fbx_value[0][3]),
		static_cast<float>(fbx_value[1][0]),
		static_cast<float>(fbx_value[1][1]),
		static_cast<float>(fbx_value[1][2]),
		static_cast<float>(fbx_value[1][3]),
		static_cast<float>(fbx_value[2][0]),
		static_cast<float>(fbx_value[2][1]),
		static_cast<float>(fbx_value[2][2]),
		static_cast<float>(fbx_value[2][3]),
		static_cast<float>(fbx_value[3][0]),
		static_cast<float>(fbx_value[3][1]),
		static_cast<float>(fbx_value[3][2]),
		static_cast<float>(fbx_value[3][3])
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
		CEREAL_NVP(inverse_transforms),
		CEREAL_NVP(position_max_value),
		CEREAL_NVP(position_min_value)
	);
}

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

ModelResource::ModelResource(ID3D11Device* device, const char* filename, const char* ignore_root_motion_node_name)
{
	this->ignore_root_motion_node_name = ignore_root_motion_node_name;
	// マルチバイト文字からワイド文字へ変換
	size_t length;
	wchar_t wfilename[256];
	::mbstowcs_s(&length, wfilename, 256, filename, _TRUNCATE);
	// 拡張子ごとの読み込みを行う
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

void ModelResource::AddAnimation(const char* fbx_filename)
{
	FbxManager* fbx_manager = FbxManager::Create();

	// FBXに対する入出力を定義する
	FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbx_manager, IOSROOT);	// 特別な理由がない限りIOSROOTを指定
	fbx_manager->SetIOSettings(fbxIOS);

	// インポータを生成
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	bool result = fbx_importer->Initialize(fbx_filename, -1, fbx_manager->GetIOSettings());	// -1でファイルフォーマット自動判定
	_ASSERT_EXPR_A(result, "FbxImporter::Initialize() : Failed!!\n");

	// SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "scene");
	fbx_importer->Import(fbx_scene);
	fbx_importer->Destroy();	// シーンを流し込んだらImporterは解放してOK

	// アニメーション構築
	BuildAnimations(fbx_scene);

	// マネージャ解放
	fbx_manager->Destroy();// 関連するすべてのオブジェクトが解放される
}

// フルパス名取得
std::string ModelResource::GetFullPathNodeName(Node& node)
{
	std::string parent_node_name;

	if (node.parent_index >= 0)
	{
		Node& parent_node = nodes.at(node.parent_index);
		parent_node_name = GetFullPathNodeName(parent_node);
		return parent_node_name + "/" + node.name;
	}

	return node.name;
}

void ModelResource::BuildModel(ID3D11Device* device, const char* fbx_filename)
{
	FbxManager* fbx_manager = FbxManager::Create();

	// FBXに対する入出力を定義する
	FbxIOSettings* fbxIOS = FbxIOSettings::Create(fbx_manager, IOSROOT);	// 特別な理由がない限りIOSROOTを指定
	fbx_manager->SetIOSettings(fbxIOS);

	// インポータを生成
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	bool result = fbx_importer->Initialize(fbx_filename, -1, fbx_manager->GetIOSettings());	// -1でファイルフォーマット自動判定
	_ASSERT_EXPR_A(result, "FbxImporter::Initialize() : Failed!!\n");

	// SceneオブジェクトにFBXファイル内の情報を流し込む
	FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "scene");
	fbx_importer->Import(fbx_scene);
	fbx_importer->Destroy();	// シーンを流し込んだらImporterは解放してOK

	// ジオメトリを三角形化しておく
	FbxGeometryConverter fbx_geometry_converter(fbx_manager);
	fbx_geometry_converter.Triangulate(fbx_scene, true);
	fbx_geometry_converter.RemoveBadPolygonsFromMeshes(fbx_scene);

#if 0
	// DirectX座標系へ変換
	FbxAxisSystem scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
	if (scene_axis_system != FbxAxisSystem::DirectX)
	{
		FbxAxisSystem::DirectX.ConvertScene(fbx_scene);
	}
#elif 0
	// OpenGL座標系へ変換
	FbxAxisSystem scene_axis_system = fbx_scene->GetGlobalSettings().GetAxisSystem();
	if (scene_axis_system != FbxAxisSystem::OpenGL)
	{
		FbxAxisSystem::OpenGL.ConvertScene(fbx_scene);
	}
#endif

	// ディレクトリパス取得
	char dirname[256];
	::_splitpath_s(fbx_filename, nullptr, 0, dirname, 256, nullptr, 0, nullptr, 0);

	// モデル構築
	FbxNode* fbx_root_node = fbx_scene->GetRootNode();
	BuildMaterials(device, dirname, fbx_scene);
#if 1
	for (int i = 0; i < fbx_root_node->GetChildCount(); i++)
	{
		BuildNodes(fbx_root_node->GetChild(i), -1);
	}
#else
	BuildNodes(fbxRootNode, -1);
#endif
	BuildMeshes(device, fbx_root_node);

	// 無視するルートモーションを検索
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

	// アニメーション構築
	BuildAnimations(fbx_scene);

	// シリアライズ化

	Serialize(fbx_filename);

	// マネージャ解放
	fbx_manager->Destroy();		// 関連するすべてのオブジェクトが解放される
}

void ModelResource::BuildSerializedModel(ID3D11Device* device, const char* dirname)
{
	for (Material& material : materials)
	{
		// 相対パスの解決
		char filename[256];
		::_makepath_s(filename, 256, nullptr, dirname, material.texture_filename.c_str(), nullptr);

		// マルチバイト文字からワイド文字へ変換
		wchar_t wfilename[256];
		::MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, 256);

		// テクスチャ読み込み
		D3D11_TEXTURE2D_DESC texture2d_desc;
		LoadTextureFromFile(device, wfilename, material.shader_resource_view.GetAddressOf(), &texture2d_desc);
		if (material.shader_resource_view == nullptr)
		{
			MakeDummyTexture(device, material.shader_resource_view.GetAddressOf());
		}
	}

	for (Mesh& mesh : meshes)
	{
		// サブセット
		for (Subset& subset : mesh.subsets)
		{
			subset.material = &materials.at(subset.material_index);
		}

		// 頂点バッファ
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = mesh.vertices.data();
			subresource_data.SysMemPitch = 0;
			subresource_data.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.vertex_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
		}

		// インデックスバッファ
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(u_int) * mesh.indices.size());
			//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = mesh.indices.data();
			subresource_data.SysMemPitch = 0; // インデックスバッファには使用しません。
			subresource_data.SysMemSlicePitch = 0; // インデックスバッファには使用しません。
			HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.index_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
		}
	}
}

// FBXノードを再帰的に辿ってデータを構築する
void ModelResource::BuildNodes(FbxNode* fbx_node, int parent_node_index)
{
	FbxNodeAttribute* fbx_node_attribute = fbx_node->GetNodeAttribute();
	FbxNodeAttribute::EType fbx_node_attribute_type = FbxNodeAttribute::EType::eUnknown;

	if (fbx_node_attribute != nullptr)
	{
		fbx_node_attribute_type = fbx_node_attribute->GetAttributeType();
	}

	switch (fbx_node_attribute_type)
	{
	case FbxNodeAttribute::eUnknown:
	case FbxNodeAttribute::eNull:
	case FbxNodeAttribute::eMarker:
	case FbxNodeAttribute::eMesh:
	case FbxNodeAttribute::eSkeleton:
		BuildNode(fbx_node, parent_node_index);
		break;
	}

	// 再帰的に子ノードを処理する
	parent_node_index = static_cast<int>(nodes.size() - 1);
	for (int i = 0; i < fbx_node->GetChildCount(); i++)
	{
		BuildNodes(fbx_node->GetChild(i), parent_node_index);
	}
}

// FBXノードからノードデータを構築する
void ModelResource::BuildNode(FbxNode* fbx_node, int parent_node_index)
{
	FbxAMatrix& fbx_local_transform = fbx_node->EvaluateLocalTransform();

	Node node;
	node.name = fbx_node->GetName();
	node.parent_index = parent_node_index;
	node.scale = FbxDouble4ToFloat3(fbx_local_transform.GetS());
	node.rotate = FbxDouble4ToFloat4(fbx_local_transform.GetQ());
	node.translate = FbxDouble4ToFloat3(fbx_local_transform.GetT());

	nodes.emplace_back(node);
}

// FBXノードを再帰的に辿ってメッシュデータを構築する
void ModelResource::BuildMeshes(ID3D11Device* device, FbxNode* fbx_node)
{
	FbxNodeAttribute* fbx_node_attribute = fbx_node->GetNodeAttribute();
	FbxNodeAttribute::EType fbx_node_attribute_type = FbxNodeAttribute::EType::eUnknown;

	if (fbx_node_attribute != nullptr)
	{
		fbx_node_attribute_type = fbx_node_attribute->GetAttributeType();
	}

	switch (fbx_node_attribute_type)
	{
	case FbxNodeAttribute::eMesh:
		BuildMesh(device, fbx_node, static_cast<FbxMesh*>(fbx_node_attribute));
		break;
	}

	// 再帰的に子ノードを処理する
	for (int i = 0; i < fbx_node->GetChildCount(); i++)
	{
		BuildMeshes(device, fbx_node->GetChild(i));
	}
}

// FBXメッシュからメッシュデータを構築する
void ModelResource::BuildMesh(ID3D11Device* device, FbxNode* fbx_node, FbxMesh* fbx_mesh)
{
	int fbx_control_points_count = fbx_mesh->GetControlPointsCount();

	//int fbxPolygonVertexCount = fbxMesh->GetPolygonVertexCount();
	//const int* fbxPolygonVertices = fbxMesh->GetPolygonVertices();

	int fbx_material_count = fbx_node->GetMaterialCount();
	int fbx_polygon_count = fbx_mesh->GetPolygonCount();

	meshes.emplace_back(Mesh());
	Mesh& mesh = meshes.back();
	mesh.subsets.resize(fbx_material_count > 0 ? fbx_material_count : 1);
	mesh.node_index = FindNodeIndex(fbx_node->GetName());

	// サブセットのマテリアル設定
	for (int fbx_material_index = 0; fbx_material_index < fbx_material_count; fbx_material_index++)
	{
		const FbxSurfaceMaterial* fbx_surface_material = fbx_node->GetMaterial(fbx_material_index);

		Subset& subset = mesh.subsets.at(fbx_material_index);
		int index = FindMaterialIndex(fbx_node->GetScene(), fbx_surface_material);
		subset.material = &materials.at(index);
		subset.material_index = index;
	}

	// サブセットの頂点インデックス範囲設定
	if (fbx_material_count > 0)
	{
		for (int fbx_polygon_index = 0; fbx_polygon_index < fbx_polygon_count; fbx_polygon_index++)
		{
			int fbx_material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(fbx_polygon_index);
			mesh.subsets.at(fbx_material_index).index_count += 3;
		}

		int offset = 0;
		for (Subset& subset : mesh.subsets)
		{
			subset.start_index = offset;
			offset += subset.index_count;

			subset.index_count = 0;
		}
	}

	// 頂点影響力データ
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
	// 頂点影響力データを抽出する
	// 影響力の合計が1以上になった時の処理と影響力が4つ以上あるときの処理
	std::vector<BoneInfluence> bone_influences;
	{
		bone_influences.resize(fbx_control_points_count);

		// スキニングに必要な情報を取得する
		int fbx_deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int fbx_deformer_index = 0; fbx_deformer_index < fbx_deformer_count; fbx_deformer_index++)
		{
			FbxSkin* fbx_skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(fbx_deformer_index, FbxDeformer::eSkin));

			int fbx_cluster_count = fbx_skin->GetClusterCount();
			for (int fbx_cluster_index = 0; fbx_cluster_index < fbx_cluster_count; fbx_cluster_index++)
			{
				FbxCluster* fbx_cluster = fbx_skin->GetCluster(fbx_cluster_index);

				// 頂点影響力データを抽出する
				{
					int fbx_cluster_control_point_indices_count = fbx_cluster->GetControlPointIndicesCount();
					const int* fbx_control_point_indices = fbx_cluster->GetControlPointIndices();
					const double* fbx_control_point_weights = fbx_cluster->GetControlPointWeights();

					for (int i = 0; i < fbx_cluster_control_point_indices_count; i++)
					{
						BoneInfluence& bone_influence = bone_influences.at(fbx_control_point_indices[i]);

						bone_influence.Add(fbx_cluster_index, static_cast<float>(fbx_control_point_weights[i]));
					}
				}

				// ボーン変換行列用の逆行列の計算をする
				{
					// メッシュ空間からワールド空間への変換行列
					FbxAMatrix fbx_mesh_space_transform;
					fbx_cluster->GetTransformMatrix(fbx_mesh_space_transform);

					// ボーン空間からワールド空間への変換行列
					FbxAMatrix fbx_bone_space_transform;
					fbx_cluster->GetTransformLinkMatrix(fbx_bone_space_transform);

					// ボーン逆行列を計算する
					FbxAMatrix fbx_inverse_transform = fbx_bone_space_transform.Inverse() * fbx_mesh_space_transform;

					DirectX::XMFLOAT4X4 inverse_transforms = FbxAMatrixToFloat4x4(fbx_inverse_transform);
					mesh.inverse_transforms.emplace_back(inverse_transforms);

					int nodeIndex = FindNodeIndex(fbx_cluster->GetLink()->GetName());
					mesh.node_indices.emplace_back(nodeIndex);
				}
			}
		}
	}

	// ジオメトリ行列
	FbxAMatrix fbx_geometric_transform(
		fbx_node->GetGeometricTranslation(FbxNode::eSourcePivot),
		fbx_node->GetGeometricRotation(FbxNode::eSourcePivot),
		fbx_node->GetGeometricScaling(FbxNode::eSourcePivot)
	);
	DirectX::XMFLOAT4X4 geometric_transform = FbxAMatrixToFloat4x4(fbx_geometric_transform);
	DirectX::XMMATRIX GM = DirectX::XMLoadFloat4x4(&geometric_transform);

	// UVセット名
	FbxStringList fbx_UV_set_names;
	fbx_mesh->GetUVSetNames(fbx_UV_set_names);

	// 頂点データ
	mesh.vertices.resize(static_cast<size_t>(fbx_polygon_count) * 3);
	mesh.indices.resize(static_cast<size_t>(fbx_polygon_count) * 3);

	int vertex_count = 0;
	const FbxVector4* fbx_control_points = fbx_mesh->GetControlPoints();
	for (int fbx_polygon_index = 0; fbx_polygon_index < fbx_polygon_count; fbx_polygon_index++)
	{
		// ポリゴンに適用されているマテリアルインデックスを取得する
		int fbx_material_index = 0;
		if (fbx_material_count > 0)
		{
			fbx_material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(fbx_polygon_index);
		}

		Subset& subset = mesh.subsets.at(fbx_material_index);
		const int index_offset = subset.start_index + subset.index_count;

		for (int fbx_vertex_index = 0; fbx_vertex_index < 3; fbx_vertex_index++)
		{
			Vertex vertex;

			int fbx_control_point_index = fbx_mesh->GetPolygonVertex(fbx_polygon_index, fbx_vertex_index);
			// Position
			{
				DirectX::XMFLOAT3 position = FbxDouble4ToFloat3(fbx_control_points[fbx_control_point_index]);
				DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&position);
				V = DirectX::XMVector3TransformCoord(V, GM);
				DirectX::XMStoreFloat3(&vertex.position, V);

				//AABB用の位置を保持
				if (mesh.position_max_value.x < position.x) { mesh.position_max_value.x = position.x; }
				if (mesh.position_max_value.y < position.y) { mesh.position_max_value.y = position.y; }
				if (mesh.position_max_value.z < position.z) { mesh.position_max_value.z = position.z; }

				if (mesh.position_min_value.x > position.x) { mesh.position_min_value.x = position.x; }
				if (mesh.position_min_value.y > position.y) { mesh.position_min_value.y = position.y; }
				if (mesh.position_min_value.z > position.z) { mesh.position_min_value.z = position.z; }
			}

			// Weight
			{
				BoneInfluence& bone_influence = bone_influences.at(fbx_control_point_index);
				vertex.bone_index.x = bone_influence.indices[0];
				vertex.bone_index.y = bone_influence.indices[1];
				vertex.bone_index.z = bone_influence.indices[2];
				vertex.bone_index.w = bone_influence.indices[3];
				vertex.bone_weight.x = bone_influence.weights[0];
				vertex.bone_weight.y = bone_influence.weights[1];
				vertex.bone_weight.z = bone_influence.weights[2];
				vertex.bone_weight.w = 1.0f - (vertex.bone_weight.x + vertex.bone_weight.y + vertex.bone_weight.z);
				//vertex.boneWeight.w = boneInfluence.weights[3];
			}

			// Normal
			if (fbx_mesh->GetElementNormalCount() > 0)
			{
				FbxVector4 fbx_normal;
				fbx_mesh->GetPolygonVertexNormal(fbx_polygon_index, fbx_vertex_index, fbx_normal);

				DirectX::XMFLOAT3 normal = FbxDouble4ToFloat3(fbx_normal);
				DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&normal);
				V = DirectX::XMVector3TransformCoord(V, GM);
				V = DirectX::XMVector3Normalize(V);
				DirectX::XMStoreFloat3(&vertex.normal, V);

				vertex.normal = FbxDouble4ToFloat3(fbx_normal);
			}
			else
			{
				vertex.normal = DirectX::XMFLOAT3(0, 0, 0);
			}

			// Texcoord
			if (fbx_mesh->GetElementUVCount() > 0)
			{
				bool fbx_unmapped_UV;
				FbxVector2 fbxUV;
				fbx_mesh->GetPolygonVertexUV(fbx_polygon_index, fbx_vertex_index, fbx_UV_set_names[0], fbxUV, fbx_unmapped_UV);
				fbxUV[1] = 1.0 - fbxUV[1];
				vertex.texcoord = FbxDouble2ToFloat2(fbxUV);
			}
			else
			{
				vertex.texcoord = DirectX::XMFLOAT2(0, 0);
			}
			size_t vc = static_cast<size_t>(index_offset) + static_cast<size_t>(fbx_vertex_index);
			mesh.indices.at(vc) = vertex_count;
			mesh.vertices.at(vertex_count) = vertex;
			vertex_count++;
		}

		subset.index_count += 3;
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC buffer_desc{};
		D3D11_SUBRESOURCE_DATA subresource_data{};

		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = mesh.vertices.data();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;

		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}

	// インデックスバッファ
	{
		D3D11_BUFFER_DESC buffer_desc{};
		D3D11_SUBRESOURCE_DATA subresource_data{};

		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(u_int) * mesh.indices.size());
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = mesh.indices.data();
		subresource_data.SysMemPitch = 0; // インデックスバッファには使用しない。
		subresource_data.SysMemSlicePitch = 0; // インデックスバッファには使用しない。
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HResultTrace(hr));
	}
}

// FBXシーン内のFBXマテリアルからマテリアルデータを構築する
void ModelResource::BuildMaterials(ID3D11Device* device, const char* dirname, FbxScene* fbxScene)
{
	int fbx_material_count = fbxScene->GetMaterialCount();

	if (fbx_material_count > 0)
	{
		for (int fbx_material_index = 0; fbx_material_index < fbx_material_count; fbx_material_index++)
		{
			FbxSurfaceMaterial* fbx_surface_material = fbxScene->GetMaterial(fbx_material_index);

			BuildMaterial(device, dirname, fbx_surface_material);
		}
	}
	else
	{
		Material material;
		material.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		materials.emplace_back(material);
	}
}

// FBXマテリアルからマテリアルデータを構築する
void ModelResource::BuildMaterial(ID3D11Device* device, const char* dirname, FbxSurfaceMaterial* fbxSurfaceMaterial)
{
	bool ret = false;

	Material material;

	// ディフューズカラー
	FbxProperty fbx_diffuse_property = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxProperty fbx_diffuse_factor_property = fbxSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	if (fbx_diffuse_property.IsValid() && fbx_diffuse_factor_property.IsValid())
	{
		FbxDouble fbx_factor = fbx_diffuse_factor_property.Get<FbxDouble>();
		FbxDouble3 fbx_color = fbx_diffuse_property.Get<FbxDouble3>();

		material.color.x = static_cast<float>(fbx_color[0] * fbx_factor);
		material.color.y = static_cast<float>(fbx_color[1] * fbx_factor);
		material.color.z = static_cast<float>(fbx_color[2] * fbx_factor);
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
	// ディフューズテクスチャ
	if (fbx_diffuse_property.IsValid())
	{
		int fbx_texture_count = fbx_diffuse_property.GetSrcObjectCount<FbxFileTexture>();
		if (fbx_texture_count > 0)
		{
			FbxFileTexture* fbx_texture = fbx_diffuse_property.GetSrcObject<FbxFileTexture>();

			// 相対パスの解決
			char filename[256];
			::_makepath_s(filename, 256, nullptr, dirname, fbx_texture->GetRelativeFileName(), nullptr);

			// マルチバイト文字からワイド文字へ変換
			size_t length;
			wchar_t wfilename[256];
			::mbstowcs_s(&length, wfilename, 256, filename, _TRUNCATE);

			// テクスチャ読み込み
			D3D11_TEXTURE2D_DESC texture2d_desc;
			LoadTextureFromFile(device, wfilename, material.shader_resource_view.GetAddressOf(), &texture2d_desc);
			if (material.shader_resource_view == nullptr)
			{
				MakeDummyTexture(device, material.shader_resource_view.GetAddressOf());
			}
			else
			{
				material.texture_filename = fbx_texture->GetRelativeFileName();
			}
		}
	}
	//

	materials.emplace_back(material);
}

// アニメーションデータを構築
void ModelResource::BuildAnimations(FbxScene* fbxScene)
{
	// すべてのアニメーション名を取得
	FbxArray<FbxString*> fbx_anim_stack_names;
	fbxScene->FillAnimStackNameArray(fbx_anim_stack_names);

	int fbx_animation_count = fbx_anim_stack_names.Size();
	for (int fbx_animation_index = 0; fbx_animation_index < fbx_animation_count; fbx_animation_index++)
	{
		animations.emplace_back(Animation());
		Animation& animation = animations.back();

		// アニメーションデータのサンプリング設定
		FbxTime::EMode fbx_time_mode = fbxScene->GetGlobalSettings().GetTimeMode();
		FbxTime fbx_frame_time;
		fbx_frame_time.SetTime(0, 0, 0, 1, 0, fbx_time_mode);

		float sampling_rate = static_cast<float>(fbx_frame_time.GetFrameRate(fbx_time_mode));
		float sampling_time = 1.0f / sampling_rate;

		FbxString* fbx_anim_stack_name = fbx_anim_stack_names.GetAt(fbx_animation_index);
		FbxAnimStack* fbx_anim_stack = fbxScene->FindMember<FbxAnimStack>(fbx_anim_stack_name->Buffer());

		// 再生するアニメーションを指定する。
		fbxScene->SetCurrentAnimationStack(fbx_anim_stack);

		// アニメーションの再生開始時間と再生終了時間を取得する
		FbxTakeInfo* fbx_take_info = fbxScene->GetTakeInfo(fbx_anim_stack_name->Buffer());
		FbxTime fbx_start_time = fbx_take_info->mLocalTimeSpan.GetStart();
		FbxTime fbx_end_time = fbx_take_info->mLocalTimeSpan.GetStop();

		// 抽出するデータは60フレーム基準でサンプリングする
		FbxTime fbx_sampling_step;
		fbx_sampling_step.SetTime(0, 0, 1, 0, 0, fbx_time_mode);
		fbx_sampling_step = fbx_sampling_step.Get() * sampling_time; //データが失われると警告が出ているがstatic_castすると本当に失うので放置

		//int start_frame =fbx_start_time.Get() / fbx_sampling_step.Get(); //データが失われると警告が出ているがstatic_castすると本当に失うので放置
		//int end_frame = fbx_end_time.Get() / fbx_sampling_step.Get(); //データが失われると警告が出ているがstatic_castすると本当に失うので放置
		int frame_count = (fbx_end_time.Get() - fbx_start_time.Get()) / fbx_sampling_step.Get(); //データが失われると警告が出ているがstatic_castすると本当に失うので放置

		// アニメーションの対象となるノードを列挙する
		std::vector<FbxNode*> fbx_nodes;
		FbxNode* fbx_root_node = fbxScene->GetRootNode();
		for (Node& node : nodes)
		{
			FbxNode* fbx_node = fbx_root_node->FindChild(node.name.c_str());
			fbx_nodes.emplace_back(fbx_node);
		}

		// アニメーションデータを抽出する
		animation.seconds_length = frame_count * sampling_time;
		animation.keyframes.resize(static_cast<size_t>(frame_count) + 1);

		float seconds = 0.0f;
		Keyframe* keyframe = animation.keyframes.data();
		size_t fbx_node_count = fbx_nodes.size();
		FbxTime fbx_current_time = fbx_start_time;
		for (FbxTime fbx_current_time = fbx_start_time; fbx_current_time < fbx_end_time; fbx_current_time += fbx_sampling_step, keyframe++)
		{
			// キーフレーム毎の姿勢データを取り出す。
			keyframe->seconds = seconds;
			keyframe->node_keys.resize(fbx_node_count);
			for (size_t fbx_node_index = 0; fbx_node_index < fbx_node_count; fbx_node_index++)
			{
				NodeKeyData& key_data = keyframe->node_keys.at(fbx_node_index);
				FbxNode* fbx_node = fbx_nodes.at(fbx_node_index);
				if (fbx_node == nullptr)
				{
					// アニメーション対象のノードがなかったのでダミーデータを設定
					Node& node = nodes.at(fbx_node_index);
					key_data.scale = node.scale;
					key_data.rotate = node.rotate;
					key_data.translate = node.translate;
				}
				else if (fbx_node_index == root_motion_node_index)
				{
					// ルートモーションは無視する
					Node& node = nodes.at(fbx_node_index);
					key_data.scale = DirectX::XMFLOAT3(1, 1, 1);
					key_data.rotate = DirectX::XMFLOAT4(0, 0, 0, 1);
					key_data.translate = DirectX::XMFLOAT3(0, 0, 0);
				}
				else
				{
					// 指定時間のローカル行列からスケール値、回転値、移動値を取り出す。
					const FbxAMatrix& fbx_local_transform = fbx_node->EvaluateLocalTransform(fbx_current_time);

					key_data.scale = FbxDouble4ToFloat3(fbx_local_transform.GetS());
					key_data.rotate = FbxDouble4ToFloat4(fbx_local_transform.GetQ());
					key_data.translate = FbxDouble4ToFloat3(fbx_local_transform.GetT());
				}
			}
			seconds += sampling_time;
		}
	}

	// 後始末
	for (int i = 0; i < fbx_animation_count; i++)
	{
		delete fbx_anim_stack_names[i];
	}
}

// ノードインデックスを取得する
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

// マテリアルインデックスを取得する
int ModelResource::FindMaterialIndex(FbxScene* fbxScene, const FbxSurfaceMaterial* fbxSurfaceMaterial)
{
	int fbx_material_count = fbxScene->GetMaterialCount();

	for (int i = 0; i < fbx_material_count; i++)
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

	//	 シリアライズ
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
			LOG(" Error : model deserialize failed : %s ModelResource.cpp\n", result);
			return;
		}
	}
}

// デシリアライズ
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
			LOG(" Error : model deserialize failed : %s ModelResource.cpp\n", filename);
			return;
		}
	}
}

void ModelResource::Load(ID3D11Device* device, const char* filename)
{
	// ディレクトリパス取得
	char drive[32], dir[256], dirname[256];
	::_splitpath_s(filename, drive, sizeof(drive), dir, sizeof(dir), nullptr, 0, nullptr, 0);
	::_makepath_s(dirname, sizeof(dirname), drive, dir, nullptr, nullptr);

	// デシリアライズ
	Deserialize(filename);

	BuildSerializedModel(device, dirname);
}